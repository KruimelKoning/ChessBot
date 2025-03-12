#include "position.hpp"
#include "parse.hpp"
#include "types.hpp"

void print_position(const Position *pos, FILE *stream) {
	char castling_rights_buffer[] = { '-', '\0', '\0', '\0', '\0' };
	char en_passant_square_buffer[] = { '-', '\0', '\0' };
	char *castling_rights = castling_rights_buffer;
	char *en_passant_square = en_passant_square_buffer;
	int file;
	int rank;

	/* print piece placement.                                                */
	fprintf(stream, "  a b c d e f g h  \n");

	for (rank = 7; rank >= 0; rank--) {
		fprintf(stream, "%c", '1' + rank);

		for (file = 0; file < 8; file++) {
			int piece = pos->board[SQUARE(file, rank)];

			if (piece == NO_PIECE) {
				fprintf(stream, " -");
			} else {
				fprintf(stream, " %c", "PpNnBbRrQqKk"[piece]);
			}
		}

		fprintf(stream, " %c\n", '1' + rank);
	}

	fprintf(stream, "  a b c d e f g h  \n");

	/* stringify castling rights.                                            */
	if (pos->castling_rights[WHITE] & KING_SIDE) {
		*castling_rights++ = 'K';
	}

	if (pos->castling_rights[WHITE] & QUEEN_SIDE) {
		*castling_rights++ = 'Q';
	}

	if (pos->castling_rights[BLACK] & KING_SIDE) {
		*castling_rights++ = 'k';
	}

	if (pos->castling_rights[BLACK] & QUEEN_SIDE) {
		*castling_rights++ = 'q';
	}

	/* stringify en passant square.                                          */
	if (pos->en_passant_square != NO_SQUARE) {
		*en_passant_square++ = "abcdefgh"[FILE(pos->en_passant_square)];
		*en_passant_square++ = '1' + RANK(pos->en_passant_square);
	}

	/* print other information.                                              */
	fprintf(stream, "side to move: %c\n", "wb"[pos->side_to_move]);
	fprintf(stream, "castling rights: %s\n", castling_rights_buffer);
	fprintf(stream, "en passant square: %s\n", en_passant_square_buffer);
}

#include <iostream>

void	find_king_square(Position& pos)
{
	for (int i = 0; i < 64; i++)
	{
		if (TYPE(pos.board[i]) == KING)
		{
			pos.king_pos[COLOR(pos.board[i])] = i;
		}
	}
}

int parse_position(Position *pos, const char *fen) {
	int file;
	int rank;
	int square;
	int index;

	/* initialize an empty board.                                            */
	for (square = 0; square < 64; square++) {
		pos->board[square] = NO_PIECE;
	}

	/* parse piece placement.                                                */
	for (file = 0, rank = 7; file < 8 || rank > 0; fen++) {
		int piece = parse_piece(*fen);

		if (piece != NO_PIECE) {
			if (file >= 8) {
				return FAILURE;
			}

			pos->board[SQUARE(file, rank)] = piece;
			file++;
		} else if (*fen >= '1' && *fen <= '8') {
			file += *fen - '0';

			if (file > 8) {
				return FAILURE;
			}
		} else if (*fen == '/') {
			if (file != 8) {
				return FAILURE;
			}

			file = 0;
			rank--;
		} else {
			return FAILURE;
		}
	}

	if (*fen++ != ' ') {
		return FAILURE;
	}

	/* parse side to move.                                                   */
	pos->side_to_move = parse_color(*fen++);

	if (pos->side_to_move == NO_COLOR) {
		return FAILURE;
	}

	if (*fen++ != ' ') {
		return FAILURE;
	}

	/* parse castling rights.                                                */
	pos->castling_rights[WHITE] = 0;
	pos->castling_rights[BLACK] = 0;

	if (*fen == '-') {
		fen++;
	} else {
		const char *start = fen;

		if (*fen == 'K') {
			pos->castling_rights[WHITE] |= KING_SIDE;
			fen++;
		}

		if (*fen == 'Q') {
			pos->castling_rights[WHITE] |= QUEEN_SIDE;
			fen++;
		}

		if (*fen == 'k') {
			pos->castling_rights[BLACK] |= KING_SIDE;
			fen++;
		}

		if (*fen == 'q') {
			pos->castling_rights[BLACK] |= QUEEN_SIDE;
			fen++;
		}

		if (fen == start) {
			return FAILURE;
		}
	}

	if (*fen++ != ' ') {
		return FAILURE;
	}

	/* parse en passant square.                                              */
	pos->en_passant_square = parse_square(fen);

	if (*fen == '-') {
		fen += 1;
	} else if (pos->en_passant_square != NO_SQUARE) {
		fen += 2;
	} else {
		return FAILURE;
	}

	/* ignore halfmove clock and fullmove counter.                           */
	for (index = 0; index < 2; index++) {
		if (*fen++ != ' ') {
			return FAILURE;
		}

		if (*fen < '0' || *fen > '9') {
			return FAILURE;
		}

		while (*fen >= '0' && *fen <= '9') {
			fen++;
		}
	}

	/* make sure we reached the end of the string.                           */
	if (*fen) {
		return FAILURE;
	}
	find_king_square(*pos);
	return SUCCESS;
}
