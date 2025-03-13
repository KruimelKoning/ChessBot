#include "transposition.hpp"
#include "generate.hpp"
#include "evaluate.hpp"
#include "heatmap.hpp"
#include "types.hpp"
#include "uci.hpp"

#include <iostream>

int index_heatmap(int square, int type, int colour, bool isEndGame)
{
	if (colour == WHITE)
		flip(square);
	return ultra_pesto_table[isEndGame][type][square];
}

void piece_mobility(const Position& pos, int& evaluation, int score[2])
{
	std::vector<Move> moves;
	moves.reserve(40);  // Reserve space for efficiency

	generate_legal_moves(&pos, moves);

	for (const Move& move : moves) {
		int piece = pos.board[move.from_square];
		if (TYPE(piece) != PAWN) {
			score[COLOR(piece)] += 10;
		}
	}

	evaluation = score[pos.side_to_move] - score[1 - pos.side_to_move];
}

int piece_count(const Position& pos)
{
	int count = 0;

	for (int square = 0; square < 64; square++) 
	{
		if (pos.board[square] != NO_PIECE && TYPE(pos.board[square]) != PAWN)
		{
			count++;
		}
	}
	return count;
}

bool	is_end_game(const Position& pos)
{
	return piece_count(pos) <= 8;
}

int evaluate(const Position& pos)
{
	uint64_t	hashedPosition = hash(pos);
	const auto&	value = transpositionTable.find(hashedPosition);
	int32_t		evaluation;

	if (value != transpositionTable.end())
		return value->second;

	int		score[2] = { 0, 0 };
	bool	isEndGame = is_end_game(pos);

	for (int square = 0; square < 64; square++)
	{
		int piece = pos.board[square];

		if (piece != NO_PIECE)
		{
			int type = TYPE(piece);
			int colour = COLOR(piece);

			score[colour] += (piece_value[isEndGame][type] + index_heatmap(square, type, colour, isEndGame));
		}
	}

	
	// piece_mobility(pos, evaluation, score);
	evaluation = score[pos.side_to_move] - score[1 - pos.side_to_move];
	transpositionTable.emplace_hint(transpositionTable.end() ,hashedPosition, evaluation);
	return evaluation;
}
