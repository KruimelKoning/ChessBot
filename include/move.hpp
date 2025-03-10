#pragma once

#include "position.hpp"

/* this struct represents a chess move.                                      */
/*                                                                           */
/* POSSIBLE IMPROVEMENT: compact encoding                                    */
/* this struct currently has three integers. a more efficient implementation */
/* might use just a single integer, using bitwise operations to encode and   */
/* decode the move.                                                          */
/*                                                                           */
/* POSSIBLE IMPROVEMENT: extended move structure                             */
/* when making (or sorting) moves, you may need some additional information  */
/* such as the piece that is moving, or the piece that is being captured.    */
/* while these can easily be looked up from the position, the move generator */
/* already did that, and it would be faster not to have to look it up again. */
/* the move generator could encode this extra information in the move        */
/* instead, saving other parts of the code from having to redo the lookups.  */
/*                                                                           */
/* https://www.chessprogramming.org/Encoding_Moves                           */
struct Move {
	/* the square the piece is moving from.                                  */
	int from_square;

	/* the square the piece is moving to.                                    */
	int to_square;

	/* the type of piece that is being promoted to, if any.                  */
	int promotion_type;
};

/* create a move from the given parameters.                                  */
Move make_move(int from_square, int to_square, int promotion_type);

/* parse a move and store the result in `move`. valid moves are the from     */
/* square, followed by the to square, optionally followed by the promotion   */
/* type. examples: e2e4, b1c3, e7d8q, and e1g1. returns `SUCCESS` on         */
/* success, `FAILURE` on failure.                                            */
int parse_move(Move *move, const char *string);

/* make a move on the position. the move must be pseudo-legal for the given  */
/* position.                                                                 */
/*                                                                           */
/* POSSIBLE IMPROVEMENT: undo move                                           */
/* when searching for the best move in a position, you will often need to    */
/* make a move, examine the resulting position, and then go back to the      */
/* previous position. instead of making a copy of the whole position, it is  */
/* faster to go back to the previous position by undoing the move. the piece */
/* movement itself can easily be undone my moving the piece back from the to */
/* square to the from square. other information, such as the captured piece, */
/* castling rights, and en passant square still need to be copied. also,     */
/* don't forget to unpromote your pawns!                                     */
/*                                                                           */
/* https://www.chessprogramming.org/Make_Move                                */
void do_move(Position *pos, Move move);

/* check if a move is legal for the given position. the move must already be */
/* known to be pseudo-legal.                                                 */
/*                                                                           */
/* POSSIBLE IMPROVEMENT: legality test                                       */
/* for simplicity, to check if a move is legal, we generate all pseudo-legal */
/* moves for the opponent and check if any of them can capture the king.     */
/* this is inefficient, and duplicates work that will also be done by the    */
/* search function. one way to improve this is to instead look from the king */
/* to the opponent pieces. this way we don't have to generate all opponent   */
/* moves.                                                                    */
/*                                                                           */
/* https://www.chessprogramming.org/Legal_Move                               */
int is_legal(const Position *pos, Move move);
