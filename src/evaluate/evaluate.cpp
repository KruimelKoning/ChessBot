#include "evaluate.hpp"
#include "types.hpp"
#include "heatmap.hpp"

int index_heatmap(int square, int type, int colour, bool isEndGame)
{
	if (colour == BLACK)
		flip(square);
	return ultra_pesto_table[isEndGame][type][square];
}

static const int piece_value[6] = { 100, 300, 300, 500, 900, 1000000 };

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

			score[colour] += (piece_value[type] + index_heatmap(square, type, colour, isEndGame));
		}
	}

	return score[pos.side_to_move] - score[1 - pos.side_to_move];
}
