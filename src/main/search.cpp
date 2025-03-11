#include "search.hpp"
#include "evaluate.hpp"
#include "generate.hpp"
#include "uci.hpp"

#include <limits.h>
#include <iostream>

SearchResult minimax(const Position& pos, int depth, int alpha, int beta)
{
	SearchResult result = { .score  = -1000000 };

	if (depth == 0)
	{
		/* we have reached our search depth, so evaluate the position.       */
		result.score = evaluate(pos);
		return result;
	}
	std::vector<Move>	moves;

	moves.reserve(EXPECTED_MAX_MOVES); // MAX_MOVES / 4 should be plenty for most use cases
	generate_legal_moves(&pos, moves); // should change to void

	for (Move& move : moves)
	{
		Position copy = pos;
		/* do a move, the current player in `copy` is then the opponent, */
		/* and so when we call minimax we get the score of the opponent. */
		do_move(&copy, move);
		/* minimax is called recursively. this call returns the score of */
		/* the opponent, so we must negate it to get our score.          */
		int score = -minimax(copy, depth - 1, -beta, -alpha).score;
		if (depth == 6)
			std::cout << "Move: " << move_to_string(move) << " Score: " << score << std::endl;
		if (score >= beta)
		{
			result.move = move;
			result.score = beta;
			return result;
		}
		/* update the best move if we found a better one.                */
		if (score > result.score) {
			result.move = move;
			result.score = score;
			alpha = score;
		}
	}
	
	return result;
}

Move search(const SearchInfo *info) {
	return minimax(*info->pos, 6).move;
}
