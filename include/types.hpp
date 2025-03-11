#pragma once

#define SUCCESS 0
#define FAILURE -1

#define NO_COLOR -1
#define NO_TYPE -1
#define NO_PIECE -1
#define NO_FILE -1
#define NO_RANK -1
#define NO_SQUARE -1

#define BOARD_SIZE 64

#define FILE_A 0
#define FILE_B 1
#define FILE_C 2
#define FILE_D 3
#define FILE_E 4
#define FILE_F 5
#define FILE_G 6
#define FILE_H 7

#define RANK_1 0
#define RANK_2 1
#define RANK_3 2
#define RANK_4 3
#define RANK_5 4
#define RANK_6 5
#define RANK_7 6
#define RANK_8 7

#define WHITE 0
#define BLACK 1

#define PAWN 0
#define KNIGHT 1
#define BISHOP 2
#define ROOK 3
#define QUEEN 4
#define KING 5
#define PIECE_COUNT 12

#define KING_SIDE 1
#define QUEEN_SIDE 2

/* returns the square with the given file and rank.                          */
#define SQUARE(file, rank) ((rank) * 8 + (file))

/* returns the file of the given square.                                     */
#define FILE(square) ((square) % 8)

/* returns the rank of the given square.                                     */
#define RANK(square) ((square) / 8)

/* returns the piece with the given color and type.                          */
#define PIECE(color, type) ((type) * 2 + (color))

/* returns the color of the given piece.                                     */
#define COLOR(piece) ((piece) % 2)

/* returns the type of the given piece.                                      */
#define TYPE(piece) ((piece) / 2)

/* returns the rank from the perspective of the given color.                 */
#define RELATIVE(rank, color) ((color) == WHITE ? (rank) : 7 - (rank))
