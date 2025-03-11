#include "transposition.hpp"
#include "types.hpp"
#include "position.hpp"

static uint64_t randTable[12][64];
static uint64_t miscTable[6];

void	initRandTable()
{
	std::mt19937_64 randNbr(123);

	for (int i = 0; i < PIECE_COUNT; i++)
	{
		for (int i2 = 0; i2 < BOARD_SIZE; i2++)
		{
			randTable[i][i2] = randNbr();
		}
	}
	for (int i = 0; i < 6; i++)
	{
		miscTable[i] = randNbr();
	}
}

uint64_t	hash(const Position& pos)
{
	uint64_t	result = 0;

	for (int i = 0; i < BOARD_SIZE; i++)
	{
		result ^= randTable[pos.board[i] + 1][i];
	}
	if (pos.side_to_move == BLACK)
		result ^= miscTable[0];

	if (pos.castling_rights[0] & KING_SIDE)
		result ^= miscTable[1];

	if (pos.castling_rights[0] & QUEEN_SIDE)
		result ^= miscTable[2];

	if (pos.castling_rights[1] & KING_SIDE)
		result ^= miscTable[3];

	if (pos.castling_rights[1] & QUEEN_SIDE)
		result ^= miscTable[4];

	if (pos.en_passant_square != NO_SQUARE)
		result ^= std::rotl<uint64_t>(pos.en_passant_square, miscTable[5]);
	
	return result;
}