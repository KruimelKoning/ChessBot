#pragma once

#include "position.hpp"

/* the evaluation function tries to determine the value of the current       */
/* position for the current player. the greater the value, the better the    */
/* position is for the current player. our basic implementation just sums    */
/* the value of all pieces on the board. knights are worth more than pawns,  */
/* rooks are worth more than knights, and so on. the evaluation function is  */
/* very important, because all moves the engine makes are based on the value */
/* assigned to them by this function. you will find some common improvements */
/* listed below, but you are strongly encouraged to come up with your own    */
/* ideas. if you need some inspiration, play a game of chess and take a look */
/* at one of the positions. who's winning? and, why? this function returns   */
/* a positive value when the current player is winning, and a negative value */
/* when the opponent is winning.                                             */
/*                                                                           */
/* POSSIBLE IMPROVEMENT: piece square tables                                 */
/* improve the evaluation to also take into account the position of pieces   */
/* on the board. for example, knights can reach more squares from the center */
/* of the board, and pawns are more valuable when they are close to          */
/* promoting. you can also vary this depending on the phase of the game. for */
/* example, at the start of the game, the king wants to castle and hide in a */
/* corner behind the other pieces, but when there are few other pieces left  */
/* the king should become more active.                                       */
/*                                                                           */
/* POSSIBLE IMPROVEMENT: common patterns                                     */
/* pawns like to be connected in chains so they can defend each other. rooks */
/* like to be placed on open files, and bishops like to be placed on open    */
/* diagonals. other common patterns include pins, forks, and batteries. add  */
/* some logic to your evaluation function to detect these patterns and       */
/* change the value of the position accordingly.                             */
/*                                                                           */
/* POSSIBLE IMPROVEMENT: mobility score                                      */
/* the mobility score is a measure of how many moves a player can make. if a */
/* player has many possible moves, they probably have a strong position.     */
/*                                                                           */
/* https://www.chessprogramming.org/Evaluation                               */
/* https://www.chessprogramming.org/Piece-Square_Tables                      */
/* https://www.chessprogramming.org/Mobility                                 */
int evaluate(const Position& pos);
