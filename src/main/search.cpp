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

// std::unordered_map<uint64_t, int>	visited;

SearchResult minimax(const Position& pos, int depth, int alpha = -1'000'000, int beta = 1'000'000)
{
	SearchResult result = { .score  = -1'000'000'000 };

	uint64_t	hashedPos = hash(pos); // hashedPos is the hashed position
	// if (visited[hashedPos] == true)
	// {
	// 	return result;
	// }
	// visited[hashedPos] = true;
	if (repetitionTable[hashedPos] == 2)
	{
		SearchResult draw = { .score = 0 };
		return draw;
	}
	if (depth == 0) // check for mate
	{
		/* we have reached our search depth, so evaluate the position.       */
		result.score = evaluate(pos);
		if (transpositionTable.size() >= 40'000)
			transpositionTable.clear();
		// print_position(&pos, stdout);
		// std::cout << "Evaluation: " << result.score << std::endl;
		return result;
	}
	std::vector<Move>	moves;

	moves.reserve(EXPECTED_MAX_MOVES); // MAX_MOVES / 4 should be plenty for most use cases
	generate_legal_moves(&pos, moves); // should change to void

	std::sort(moves.begin(), moves.end(), [pos](Move move1, Move move2)
	{
		return compareMoves(pos, move1, move2);
	});

	for (Move& move : moves)
	{
		Position copy = pos;
		/* do a move, the current player in `copy` is then the opponent, */
		/* and so when we call minimax we get the score of the opponent. */
		do_move(&copy, move);
		/* minimax is called recursively. this call returns the score of */
		/* the opponent, so we must negate it to get our score.          */
		int score = -minimax(copy, depth - 1, -beta, -alpha).score;
		// if (depth == 6)
		// 	std::cout << "Move: " << move_to_string(move) << " Score: " << score << std::endl;
		if (score >= beta)
		{
			result.move = move;
			result.score = score;
			return result;
		}
		/* update the best move if we found a better one.                */
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
	SearchResult	result = minimax(*info->pos, 6);
	repetitionTable[hash(*info->pos)]++;
	// visited.clear();
	return result.move;
}
