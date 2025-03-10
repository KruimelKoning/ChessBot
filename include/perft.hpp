#pragma once

/* PERFormance Testing is a way to test the performance and correctness of   */
/* the move generator. it works by counting the number of positions that can */
/* be reached after a number of moves, and comparing it to a known value.    */
/*                                                                           */
/* it is recommended to run perft every time you make changes to the move    */
/* generator.                                                                */
/*                                                                           */
/* https://www.chessprogramming.org/Perft                                    */
void perft_run(void);
