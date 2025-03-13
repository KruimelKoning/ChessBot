#include "search.hpp"
#include "tools.hpp"
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

SearchResult minimax(const Position& pos, int depth, int alpha = -1000000, int beta = 1000000)
{
	SearchResult result = { .score  = -1000000000 };

	if (depth == 0)
	{
		/* we have reached our search depth, so evaluate the position.       */
		result.score = evaluate(pos);
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

std::unordered_map<uint64_t, int>	visited;

SearchResult	miniMax(const Position& pos, int depth, int alpha = -1'000'000, int beta = 1'000'000)
{
	uint64_t		hashedPosition = hash(pos);
	const auto&		value = transpositionTable.find(hashedPosition);
	SearchResult	bestMove{ .score = -100'000'000 };

	if (value != transpositionTable.end())
	{
		return { value->second.bestMove, value->second.score };
	}

	if (depth == 0)
	{
		SearchResult result{};
		result.score = evaluate(pos);
		if (transpositionTable.size() >= 40'000)
			transpositionTable.clear();
		return result;
	}
	std::vector<Move>			moves;

	moves.reserve(EXPECTED_MAX_MOVES);
	generate_legal_moves(&pos, moves);

	if (moves.size() == 0)
		return {Move{}, -1'000'000 + depth};

	std::sort(moves.begin(), moves.end(), [pos](Move move1, Move move2)
	{
		return compareMoves(pos, move1, move2);
	});

	for (const Move& move : moves)
	{
		Position newPos = pos;

		if (timesUp() == true)
		{
			return bestMove;
		}
		do_move(&newPos, move);
		int score = -miniMax(newPos, depth - 1, -beta, -alpha).score;
		if (score >= beta)
		{
			transpositionTable[hashedPosition] = { depth, score, move };
			return { move, score };
		}
		if (score > bestMove.score)
		{
			bestMove = { move, score };
			alpha = std::max(alpha, score);
		}
	}
	transpositionTable[hashedPosition] = { depth, bestMove.score, bestMove.move };
	return bestMove;
}

Move	search(const SearchInfo& info)
{
	// static int firstVisit;

	// // will shit the bed when using a fen string
	// if (firstVisit == 0)
	// {
	// 	firstVisit = 1;
	// 	if (info.pos->side_to_move == WHITE)
	// 		return { 12, 28, NO_PIECE };
	// 	return { 52, 36, NO_PIECE};
	// }

	SearchResult	currentMove;
	SearchResult	bestMove;

	std::cout << "hallo??\n";
	// timesUp(info.time[info.pos->side_to_move] / 32);
	timesUp(10000);
	for (int depth = 1; timesUp() == false; depth++)
	{
		// std::cout << depth << std::endl;
		// visited.clear();
		currentMove = miniMax(*info.pos, depth);
		if (timesUp() == false)
			bestMove = currentMove;
		// std::cout << "bestmove: " << bestMove.score << std::endl;
	}
	return bestMove.move;
}
