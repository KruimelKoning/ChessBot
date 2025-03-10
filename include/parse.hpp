#pragma once

/* parse a file. valid files are 'a', 'b', 'c', 'd', 'e', 'f', 'g', and 'h'. */
/* returns the file on success, `NO_FILE` on failure.                        */
int parse_file(char c);

/* parse a rank. valid ranks are '1', '2', '3', '4', '5', '6', '7', and '8'. */
/* returns the rank on success, `NO_RANK` on failure.                        */
int parse_rank(char c);

/* parse a square. valid squares are a file followed by a rank. trailing     */
/* characters are ignored. returns the square on success, `NO_SQUARE` on     */
/* failure.                                                                  */
int parse_square(const char *string);

/* parse a color. valid colors are 'w' and 'b'. returns the color on         */
/* success, `NO_COLOR` on failure.                                           */
int parse_color(char c);

/* parse a type. valid types are 'p', 'n', 'b', 'r', 'q', and 'k'. returns   */
/* the type on success, `NO_TYPE` on failure.                                */
int parse_type(char c);

/* parse a piece. valid pieces are 'P', 'p', 'N', 'n', 'B', 'b', 'R', 'r',   */
/* 'Q', 'q', 'K', and 'k'. uppercase letters are for white pieces, lowercase */
/* letters are for black pieces. returns the piece on success, `NO_PIECE` on */
/* failure.                                                                  */
int parse_piece(char c);
