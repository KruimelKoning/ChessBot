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

int	king_safety(const Position& pos)
{

}

void	piece_mobility(const Position& pos, int& evaluation)
{
	std::vector<Move>	moves;
	generate_legal_moves(&pos, moves);

	for (Move& move : moves)
	{
		if (TYPE(pos.board[move.from_square]) != PAWN)
		{
			evaluation += 10;
		}
	}
	if (moves.size() == 0)
	{
		if (isCheck(pos, true))
			evaluation = -100'000;
		else
			evaluation = 0;
	}
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
	piece_mobility(pos, evaluation);
	transpositionTable.emplace(hashedPosition, evaluation);
	return evaluation;
}
