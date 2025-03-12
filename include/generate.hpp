#pragma once

#include "move.hpp"
#include "position.hpp"

#include <stddef.h>

#include <vector>
#include <algorithm>

/* maximum number of moves in any chess position. the actual maximum number  */
/* of moves seems to be 218, but just to be safe we round up to the nearest  */
/* power of two. the `moves` parameter of `generate_psuedo_legal_moves` and  */
/* `generate_legal_moves` should be an array of size `MAX_MOVES`.            */
#define MAX_MOVES 256
#define EXPECTED_MAX_MOVES (MAX_MOVES / 4)

/* generate all pseudo-legal moves and store them in `moves`, which must be  */
/* large enough to hold all pseudo-legal moves in the position. pseudo-legal */
/* moves include moves that leave the king in check, or castling through a   */
/* square that is controlled by an opponent piece. returns the number of     */
/* pseudo-legal moves generated.                                             */
/*                                                                           */
/* POSSIBLE IMPROVEMENT: staged or chunked move generation                   */
/* some moves such as captures and pawn promotions are likely to have a      */
/* bigger impact on the game than quiet moves. instead of wasting time       */
/* generating (and sorting) all moves, provide a way to generate important   */
/* moves separately. this is especially useful for alpha-beta pruning.       */
/*                                                                           */
/* https://www.chessprogramming.org/Move_Generation                          */
/* https://www.chessprogramming.org/Pseudo-Legal_Move                        */
size_t generate_pseudo_legal_moves(const Position *pos, std::vector<Move>& moves);

/* generate all legal moves and store them in `moves`, which must be large   */
/* enough to hold all legal moves in the position. returns the number of     */
/* legal moves generated.                                                    */
/*                                                                           */
/* POSSIBLE IMPROVEMENT: getting out of check                                */
/* in some positions, such as when in check, the number of legal moves is    */
/* much smaller than the number of pseudo-legal moves. instead of generating */
/* all pseudo-legal moves, write a generator specifically for moves that get */
/* out of check.                                                             */
/*                                                                           */
/* https://www.chessprogramming.org/Check                                    */
/* https://www.chessprogramming.org/Double_Check                             */
size_t generate_legal_moves(const Position *pos, std::vector<Move>& moves);


size_t generate_sliding_move(const Position *pos, std::vector<Move>& moves, int from_square, int file_offset, int rank_offset);
size_t generate_simple_move(const Position *pos, std::vector<Move>& moves, int from_square, int file_offset, int rank_offset);
size_t generate_pawn_capture(const Position *pos, std::vector<Move>& moves, int from_square, int file_offset, int rank_offset);