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

Pieces piece_count(const Position& pos)
{
	int count = 0;
	Pieces pieces;

	for (int square = 0; square < 64; square++) 
	{
		if (pos.board[square] != NO_PIECE && TYPE(pos.board[square]) == QUEEN)
		{
			int color = COLOR(pos.board[square]);
			pieces.queen[color]++;
			count++;
		}
		else if(pos.board[square] != NO_PIECE && TYPE(pos.board[square]) == ROOK)
		{
			int color = COLOR(pos.board[square]);
			pieces.rook[color]++;
			count++;
		}
		else if (pos.board[square] != NO_PIECE && TYPE(pos.board[square]) == BISHOP)
		{
			int color = COLOR(pos.board[square]);
			pieces.bishop[color]++;
			count++;
		}
		else if (pos.board[square] != NO_PIECE && TYPE(pos.board[square]) == KNIGHT)
		{
			int color = COLOR(pos.board[square]);
			pieces.knight[color]++;
			count++;
		}
		else if (pos.board[square] != NO_PIECE)
		{
			int color = COLOR(pos.board[square]);
			pieces.pawns[color]++;
		}
	}
	pieces.all = count;
	return pieces;
}

bool	is_end_game(const Position& pos)
{
	Pieces pieceCount = piece_count(pos);

	if (pieceCount.queen[WHITE] == 0 && pieceCount.queen[BLACK] == 0 &&
		pieceCount.rook[WHITE] == 0 && pieceCount.rook[BLACK] == 0 &&
		pieceCount.bishop[WHITE] + pieceCount.knight[WHITE] <= 1 &&
		pieceCount.bishop[BLACK] + pieceCount.knight[BLACK] <= 1)
	{
		return true;
	}

	if (pieceCount.queen[WHITE] + pieceCount.rook[WHITE] == 0 &&
		pieceCount.queen[BLACK] + pieceCount.rook[BLACK] == 0 &&
		(pieceCount.pawns[WHITE] > 0 || pieceCount.pawns[BLACK] > 0))
	{
		return true;
	}
	if (pieceCount.all <= 4)
		return (true);
	return false;
}

int evaluate(const Position& pos)
{
	uint64_t	hashedPosition = hash(pos);
	const auto&	value = transpositionTable.find(hashedPosition);

	if (value != transpositionTable.end())
		return value->second;

	int		score[2] = { 0, 0 };
	bool	isEndGame = is_end_game(pos);
	int32_t	evaluation;

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
	evaluation = score[pos.side_to_move] - score[1 - pos.side_to_move];
	transpositionTable.emplace_hint(transpositionTable.end() ,hashedPosition, evaluation);
	return evaluation;
}
