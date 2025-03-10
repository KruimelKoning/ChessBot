#include "parse.hpp"
#include "types.hpp"

int parse_file(char c) {
	switch (c) {
	case 'a':
		return FILE_A;

	case 'b':
		return FILE_B;

	case 'c':
		return FILE_C;

	case 'd':
		return FILE_D;

	case 'e':
		return FILE_E;

	case 'f':
		return FILE_F;

	case 'g':
		return FILE_G;

	case 'h':
		return FILE_H;

	default:
		return NO_FILE;
	}
}

int parse_rank(char c) {
	switch (c) {
	case '1':
		return RANK_1;

	case '2':
		return RANK_2;

	case '3':
		return RANK_3;

	case '4':
		return RANK_4;

	case '5':
		return RANK_5;

	case '6':
		return RANK_6;

	case '7':
		return RANK_7;

	case '8':
		return RANK_8;

	default:
		return NO_RANK;
	}
}

int parse_square(const char *string) {
	int file = parse_file(string[0]);

	if (file != NO_FILE) {
		int rank = parse_rank(string[1]);

		if (rank != NO_RANK) {
			return SQUARE(file, rank);
		}
	}
	
	return NO_SQUARE;
}

int parse_color(char c) {
	switch (c) {
	case 'w':
		return WHITE;

	case 'b':
		return BLACK;

	default:
		return NO_COLOR;
	}
}

int parse_type(char c) {
	switch (c) {
	case 'p':
		return PAWN;

	case 'n':
		return KNIGHT;

	case 'b':
		return BISHOP;

	case 'r':
		return ROOK;

	case 'q':
		return QUEEN;

	case 'k':
		return KING;

	default:
		return NO_TYPE;
	}
}

int parse_piece(char c) {
	switch (c) {
	case 'P':
		return PIECE(WHITE, PAWN);

	case 'N':
		return PIECE(WHITE, KNIGHT);

	case 'B':
		return PIECE(WHITE, BISHOP);

	case 'R':
		return PIECE(WHITE, ROOK);

	case 'Q':
		return PIECE(WHITE, QUEEN);

	case 'K':
		return PIECE(WHITE, KING);

	case 'p':
		return PIECE(BLACK, PAWN);

	case 'n':
		return PIECE(BLACK, KNIGHT);

	case 'b':
		return PIECE(BLACK, BISHOP);

	case 'r':
		return PIECE(BLACK, ROOK);

	case 'q':
		return PIECE(BLACK, QUEEN);

	case 'k':
		return PIECE(BLACK, KING);

	default:
		return NO_PIECE;
	}
}
