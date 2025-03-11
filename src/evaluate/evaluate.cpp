#include "transposition.hpp"
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

int evaluate(const Position& pos)
{
	uint64_t	hashedPosition = hash(pos);
	const auto&	value = transpositionTable.find(hashedPosition);
	if (value != transpositionTable.end())
		return value->second;

	int		score[2] = { 0, 0 };
	int		square;
	bool	isEndGame = piece_count(pos) <= 8;

	for (square = 0; square < 64; square++)
	{
		int piece = pos.board[square];

		if (piece != NO_PIECE)
		{
			int type = TYPE(piece);
			int colour = COLOR(piece);

			// std::cout << "Piece: " << type << " pos: " << square << " score: " << piece_value[isEndGame][type] + index_heatmap(square, type, colour, isEndGame) << std::endl;
			score[colour] += (piece_value[isEndGame][type] + index_heatmap(square, type, colour, isEndGame));
		}
	}
	// print_position(&pos, stdout);
	// std::cout << "Evaluation: " << score[pos.side_to_move] - score[1 - pos.side_to_move] << std::endl;

	int32_t evaluation = score[pos.side_to_move] - score[1 - pos.side_to_move];
	transpositionTable.emplace(hashedPosition, evaluation);
	return evaluation;
}
