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

bool	mirrorAble(const SearchInfo& info, Move& toMove)
{
	std::vector<Move> moves;
	moves.reserve(EXPECTED_MAX_MOVES);

	generate_legal_moves(info.pos, moves);

	for (Move& move : moves)
	{
		Position	copy = *info.pos;
		int i;

		do_move(&copy, move);
		print_position(&copy, stdout);
		for (i = 0; i < 64; i++)
		{
			int a = copy.board[i];
			int b = copy.board[i ^ 56];
			
			if (a == NO_PIECE && b == NO_PIECE)
			{
				continue;
			}
			if (a == NO_PIECE || b == NO_PIECE)
			{
				break;
			}
			if (TYPE(a) == TYPE(b) && COLOR(a) != COLOR(b))
						{
				continue;
			}
			break;
		}
		if (i == 64)
		{
			toMove = move;
			return true;
		}
	}
	return false;
}

Move search(const SearchInfo *info)
{
	if (info->pos->side_to_move == BLACK)
	{
		Move	mirror;

		if (mirrorAble(*info, mirror) == true)
		{
			return mirror;
		}
	}
	return minimax(*info->pos, 6).move;
}
