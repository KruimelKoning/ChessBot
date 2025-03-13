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
	const auto&		value = visited.find(hashedPosition);
	SearchResult	bestMove{.score = -100'000'000};

	if (value != visited.end())
		return bestMove;
	visited.emplace(hashedPosition, 0);

	if (depth == 0)
	{
		SearchResult result{};
		result.score = evaluate(pos);
		if (transpositionTable.size() >= 40'000)
			transpositionTable.clear();
		return result;
	}
	std::vector<Move>			moves;
	std::vector<SearchResult>	searches;

	moves.reserve(EXPECTED_MAX_MOVES);
	searches.reserve(EXPECTED_MAX_MOVES);
	generate_legal_moves(&pos, moves);

	for (Move& move : moves)
	{
		Position newPos = pos;

		if (timesUp() == true)
		{
			return bestMove;
		}
		do_move(&newPos, move);
		searches.push_back({ move, evaluate(newPos) });
	}

	std::sort(searches.begin(), searches.end(), [](SearchResult& s1, SearchResult& s2)
	{
		return s1.score > s2.score;
	});

	for (SearchResult search : searches)
	{
		Position newPos = pos;

		if (timesUp() == true)
		{
			return bestMove;
		}
		do_move(&newPos, search.move);
		int score = -miniMax(newPos, depth - 1, -beta, -alpha).score;
		if (score >= beta)
		{
			return { search.move, score };
		}
		if (score > bestMove.score)
		{
			bestMove = { search.move, score };
			alpha = std::max(alpha, score);
		}
	}

	return bestMove;
}

Move	search(const SearchInfo& info)
{
	static int firstVisit;

	// will shit the bed when using a fen string
	if (firstVisit == 0)
	{
		firstVisit = 1;
		if (info.pos->side_to_move == WHITE)
			return { 12, 28, NO_PIECE };
		return { 52, 36, NO_PIECE};
	}

	SearchResult	currentMove;
	SearchResult	bestMove;

	timesUp(info.time[info.pos->side_to_move] / 32);
	for (int depth = 1; timesUp() == false; depth++)
	{
		// std::cout << depth << "\n";
		visited.clear();
		currentMove = miniMax(*info.pos, depth);
		if (timesUp() == false)
			bestMove = currentMove;
		else if (currentMove.score > bestMove.score)
			bestMove = currentMove;
	}
	transpositionTable.clear();
	return bestMove.move;
}
