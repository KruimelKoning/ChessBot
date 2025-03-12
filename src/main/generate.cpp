#include "generate.hpp"
#include "types.hpp"

/* add the file and rank offset to the square. returns `NO_SQUARE` if the    */
/* resulting square is off the board.                                        */
static int add_offset(int square, int file_offset, int rank_offset) {
	int file = FILE(square) + file_offset;
	int rank = RANK(square) + rank_offset;

	if (file >= 0 && file < 8 && rank >= 0 && rank < 8) {
		return SQUARE(file, rank);
	} else {
		return NO_SQUARE;
	}
}

/* generate a pawn move, taking into account promotions. returns the number  */
/* of moves generated.                                                       */
size_t generate_pawn_move(const Position *pos, std::vector<Move>& moves, int from_square, int to_square) {
	size_t count = 0;

	if (RANK(to_square) == RELATIVE(RANK_8, pos->side_to_move)) {
		moves.push_back({ from_square, to_square, ROOK   });
		moves.push_back({ from_square, to_square, QUEEN  });
		moves.push_back({ from_square, to_square, BISHOP });
		moves.push_back({ from_square, to_square, KNIGHT });
	} else {
		moves.push_back({ from_square, to_square, NO_TYPE });
	}

	return count;
}

/* generate a pawn capture, taking into account promotions. this function    */
/* makes sure that the destination square is on the board, and that it       */
/* contains an opponent piece or is the en passant square. returns the       */
/* number of moves generated.                                                */
size_t generate_pawn_capture(const Position *pos, std::vector<Move>& moves, int from_square, int file_offset, int rank_offset) {
	int to_square = add_offset(from_square, file_offset, rank_offset);

	if (to_square != NO_SQUARE) {
		int piece = pos->board[to_square];
		int capture = piece != NO_PIECE && COLOR(piece) != pos->side_to_move;

		if (capture || to_square == pos->en_passant_square) {
			return generate_pawn_move(pos, moves, from_square, to_square);
		}
	}

	return 0;
}

/* generate a simple, non-sliding move. this function makes sure that the    */
/* destination square is on the board, and that it is empty or contains an   */
/* opponent piece. returns the number of moves generated.                    */
size_t generate_simple_move(const Position *pos, std::vector<Move>& moves, int from_square, int file_offset, int rank_offset) {
	size_t count = 0;
	int to_square = add_offset(from_square, file_offset, rank_offset);

	if (to_square != NO_SQUARE) {
		int piece = pos->board[to_square];

		if ((piece == NO_PIECE || COLOR(piece) != pos->side_to_move)) {
			moves.push_back({ from_square, to_square, NO_TYPE });
		}
	}

	return count;
}

/* generate a sliding move. this function keeps adding the offset in a loop  */
/* until the destination square runs off the board or into another piece.    */
/* returns the number of moves generated.                                    */
size_t generate_sliding_move(const Position *pos, std::vector<Move>& moves, int from_square, int file_offset, int rank_offset) {
	size_t count = 0;
	int to_square = add_offset(from_square, file_offset, rank_offset);

	while (to_square != NO_SQUARE) {
		int piece = pos->board[to_square];

		if ((piece == NO_PIECE || COLOR(piece) != pos->side_to_move)) {
			moves.push_back({ from_square, to_square, NO_TYPE });
		}

		if (piece != NO_PIECE) {
			break;
		}

		to_square = add_offset(to_square, file_offset, rank_offset);
	}

	return count;
}

size_t generate_pseudo_legal_moves(const Position *pos, std::vector<Move>& moves) {
	size_t count = 0;
	int forward = pos->side_to_move == WHITE ? 1 : -1;
	int square;

	for (square = 0; square < 64; square++) {
		int piece = pos->board[square];

		/* skip this square if it is empty or contains an opponent piece.    */
		if (piece == NO_PIECE || COLOR(piece) != pos->side_to_move) {
			continue;
		}

		switch (TYPE(piece)) {
			int up;
			int up_up;

		case PAWN:
			up = add_offset(square, 0, forward);
			up_up = add_offset(square, 0, forward * 2);

			/* pawn push.                                                    */
			if (up != NO_SQUARE && pos->board[up] == NO_PIECE) {
				count += generate_pawn_move(pos, moves, square, up);

				/* double pawn push.                                         */
				if (up_up != NO_SQUARE && pos->board[up_up] == NO_PIECE) {
					if (RANK(square) == RELATIVE(RANK_2, COLOR(piece))) {
						count += generate_pawn_move(pos, moves, square, up_up);
					}
				}
			}

			/* pawn captures.                                                */
			count += generate_pawn_capture(pos, moves, square, -1, forward);
			count += generate_pawn_capture(pos, moves, square, 1, forward);

			break;
		case KNIGHT:
			/* knight moves.                                                 */
			count += generate_simple_move(pos, moves, square, -1, -2);
			count += generate_simple_move(pos, moves, square, 1, -2);
			count += generate_simple_move(pos, moves, square, -2, -1);
			count += generate_simple_move(pos, moves, square, 2, -1);
			count += generate_simple_move(pos, moves, square, -2, 1);
			count += generate_simple_move(pos, moves, square, 2, 1);
			count += generate_simple_move(pos, moves, square, -1, 2);
			count += generate_simple_move(pos, moves, square, 1, 2);

			break;
		case BISHOP:
		case ROOK:
		case QUEEN:
			/* bishop and queen moves.                                       */
			if (TYPE(piece) != ROOK) {
				count += generate_sliding_move(pos, moves, square, -1, -1);
				count += generate_sliding_move(pos, moves, square, 1, -1);
				count += generate_sliding_move(pos, moves, square, -1, 1);
				count += generate_sliding_move(pos, moves, square, 1, 1);
			}

			/* rook and queen moves.                                         */
			if (TYPE(piece) != BISHOP) {
				count += generate_sliding_move(pos, moves, square, 0, -1);
				count += generate_sliding_move(pos, moves, square, -1, 0);
				count += generate_sliding_move(pos, moves, square, 1, 0);
				count += generate_sliding_move(pos, moves, square, 0, 1);
			}

			break;
		case KING:
			/* simple king moves.                                            */
			count += generate_simple_move(pos, moves, square, -1, -1);
			count += generate_simple_move(pos, moves, square, 0, -1);
			count += generate_simple_move(pos, moves, square, 1, -1);
			count += generate_simple_move(pos, moves, square, -1, 0);
			count += generate_simple_move(pos, moves, square, 1, 0);
			count += generate_simple_move(pos, moves, square, -1, 1);
			count += generate_simple_move(pos, moves, square, 0, 1);
			count += generate_simple_move(pos, moves, square, 1, 1);

			/* king side castling.                                           */
			if (pos->castling_rights[pos->side_to_move] & KING_SIDE) {
				int f1 = SQUARE(FILE_F, RELATIVE(RANK_1, pos->side_to_move));
				int g1 = SQUARE(FILE_G, RELATIVE(RANK_1, pos->side_to_move));
				int f1_empty = pos->board[f1] == NO_PIECE;
				int g1_empty = pos->board[g1] == NO_PIECE;

				if (f1_empty && g1_empty) {
					moves.push_back({ square, g1, NO_TYPE });
				}
			}

			/* queen side castling.                                          */
			if (pos->castling_rights[pos->side_to_move] & QUEEN_SIDE) {
				int b1 = SQUARE(FILE_B, RELATIVE(RANK_1, pos->side_to_move));
				int c1 = SQUARE(FILE_C, RELATIVE(RANK_1, pos->side_to_move));
				int d1 = SQUARE(FILE_D, RELATIVE(RANK_1, pos->side_to_move));
				int b1_empty = pos->board[b1] == NO_PIECE;
				int c1_empty = pos->board[c1] == NO_PIECE;
				int d1_empty = pos->board[d1] == NO_PIECE;

				if (b1_empty && c1_empty && d1_empty) {
					moves.push_back({ square, c1, NO_TYPE });
				}
			}
			break;
		}
	}
	return count;
}

size_t	generate_legal_moves(const Position *pos, std::vector<Move>& moves)
{
	generate_pseudo_legal_moves(pos, moves);
	std::erase_if(moves, [pos](const Move& move)
	{
		return !is_legal(pos, move);
	});
	return 0; //TODO: change later
}


// size_t generate_legal_moves(const Position *pos, std::vector<Move>& moves) {
// 	size_t pseudo_legal_count = generate_pseudo_legal_moves(pos, moves);
// 	size_t index;
// 	size_t count = 0;

// 	for (index = 0; index < pseudo_legal_count; index++) {
// 		if (is_legal(pos, moves[index])) {
// 			moves[count++] = moves[index];
// 		}
// 	}

// 	return count;
// }