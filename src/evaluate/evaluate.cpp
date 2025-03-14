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
		if (pos.board[square] != NO_PIECE && pos.board[square] != KING)
		{
			pieces.piece[COLOR(pos.board[square])][TYPE(pos.board[square])]++;
			count++;
		}
	}
	pieces.all = count;
	return pieces;
}

bool	is_end_game(const Position& pos)
{
	Pieces pieceCount = piece_count(pos);

	if (pieceCount.piece[WHITE][QUEEN] == 0 && pieceCount.piece[BLACK][QUEEN] == 0 &&
		pieceCount.piece[WHITE][ROOK] == 0 && pieceCount.piece[BLACK][ROOK] == 0 &&
		pieceCount.piece[WHITE][BISHOP] + pieceCount.piece[WHITE][KNIGHT] <= 1 &&
		pieceCount.piece[BLACK][BISHOP] + pieceCount.piece[BLACK][KNIGHT] <= 1)
	{
		return true;
	}

	if (pieceCount.piece[WHITE][QUEEN] + pieceCount.piece[WHITE][ROOK] == 0 &&
		pieceCount.piece[BLACK][QUEEN] + pieceCount.piece[BLACK][ROOK] == 0 &&
		(pieceCount.piece[WHITE][PAWN] > 0 || pieceCount.piece[BLACK][PAWN] > 0))
	{
		return true;
	}
	if (pieceCount.all <= 8)
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
