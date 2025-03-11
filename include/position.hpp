#pragma once

#include <stdio.h>
#include <stdint.h>

/* this struct represents the placement of pieces on a chess board, as well  */
/* as any additional information such as side to move, castling rights, and  */
/* possibly an en passant square. we use a square centric approach to store  */
/* the placement of pieces because it is easy to implement, and being able   */
/* to quickly look up what piece is on any given square is useful for move   */
/* generation.                                                               */
/*                                                                           */
/* POSSIBLE IMPROVEMENT: bitboards                                           */
/* bitboards provide a way to store the placement of pieces on a chess board */
/* using 64 bit integers where every bit represents a square on the board.   */
/* you might have a bitboard for each piece type and color, and then use     */
/* bitwise operations for move generation. this is more efficient than a     */
/* square centric approach, but it is also more complicated to implement.    */
/*                                                                           */
/* POSSIBLE IMPROVEMENT: draw detection                                      */
/* to keep the code simple we do not detect draws. to implement draw         */
/* detection you need to store the number of reversible moves to keep track  */
/* of the fifty move rule, as well as a history of board states to detect    */
/* threefold repetition.                                                     */
/*                                                                           */
/* https://www.chessprogramming.org/Board_Representation                     */
/* https://www.chessprogramming.org/Bitboards                                */
struct Position {
	/* pieces indexed by square. `NO_PIECE` is used for empty squares.       */
	int	board[64] = {-1};

	/* color of the current side to move, must be `WHITE` or `BLACK`.        */
	int side_to_move;

	/* castling rights indexed by piece color.                               */
	int castling_rights[2];

	/* en passant square, may be `NO_SQUARE`.                                */
	int en_passant_square;
};

/* print out information about the position. useful for debugging.           */
void print_position(const Position *pos, FILE *stream);

/* parse a FEN string and store the result in `pos`. returns `SUCCESS` on    */
/* success, `FAILURE` on failure.                                            */
/*                                                                           */
/* https://www.chessprogramming.org/Forsyth-Edwards_Notation                 */
int parse_position(Position *pos, const char *fen);
