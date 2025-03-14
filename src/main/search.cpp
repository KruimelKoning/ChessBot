#include "search.hpp"
#include "heatmap.hpp"
#include "types.hpp"
#include "evaluate.hpp"
#include "generate.hpp"
#include "transposition.hpp"
#include "uci.hpp"

#include <limits.h>
#include <iostream>

bool	compareMoves(const Position& pos, Move move1, Move move2)
{
	bool	isEndGame = is_end_game(pos);

	int take1Value  = pos.board[move1.to_square] == NO_PIECE ? 0 : piece_value[isEndGame][TYPE(pos.board[move1.to_square])];
	// int piece1Value = piece_value[isEndGame][TYPE(pos.board[move1.from_square])];

	int take2Value  = pos.board[move2.to_square] == NO_PIECE ? 0 : piece_value[isEndGame][TYPE(pos.board[move2.to_square])];
	// int piece2Value = piece_value[isEndGame][TYPE(pos.board[move2.from_square])];

	return take1Value > take2Value;
}

SearchResult minimax(const Position& pos, int depth, int alpha = -1'000'000, int beta = 1'000'000)
{
	SearchResult result = { .score  = -1'000'000'000 };

	uint64_t	hashedPos = hash(pos); // hashedPos is the hashed position
	if (repetitionTable[hashedPos] == 2)
	{
		SearchResult draw = { .score = 0 };
		return draw;
	}
	if (depth == 0)
	{
		result.score = evaluate(pos);
		if (transpositionTable.size() >= 40'000)
		{
			transpositionTable.clear();
		}
		return result;
	}
	std::vector<Move>	moves;

	moves.reserve(EXPECTED_MAX_MOVES); // MAX_MOVES / 4 should be plenty for most use cases
	generate_legal_moves(&pos, moves); // should change to void
	
	if (moves.empty())  // No legal moves
	{
		// std::cout << "Is check: " << (isCheck(pos) ? "true" : "false") << std::endl;
		// print_position(&pos, stdout);
		result.score = isCheck(pos) ? -999'999 + depth : 0;
		return result;
	}

	std::sort(moves.begin(), moves.end(), [pos](Move move1, Move move2)
	{
		return compareMoves(pos, move1, move2);
	});

	for (Move& move : moves)
	{
		Position copy = pos;

		do_move(&copy, move);
		// bool shouldExtend = (depth == 1 && isCheck(copy));
		// if (depth == 1)
		// 	std::cout << "shoudlExtend: " << (shouldExtend ? "true" : "false") << std::endl;
		int score = -minimax(copy, depth - 1, -beta, -alpha).score;
		if (score >= beta)
		{
			result.move = move;
			result.score = score;
			return result;
		}
		if (score > result.score) {
			result.move = move;
			result.score = score;
			alpha = std::max(alpha, score);
		}
	}

	return result;
}

// Move	minimax(const Position& pos, int depth, int alpha = , int beta)
// {

// }

Move search(const SearchInfo *info)
{
	static int	move_count;
	SearchResult	result;


	if (move_count < 2)
	{
		std::cout << "movecount = " << move_count << std::endl;
		result = minimax(*info->pos, 2);
		move_count++;
	}
	else
	{
		if (is_end_game(*info->pos))
		{
			// std::cout << "Its endgame time...\n";
			result = minimax(*info->pos, 8);
		}
		else
		{
			// std::cout << "Its not endgame time\n";
			result = minimax(*info->pos, 5);
		}
	}

	repetitionTable[hash(*info->pos)]++;
	// visited.clear();
	return result.move;
}
