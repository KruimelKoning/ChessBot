#pragma once

#include <vector>
#include <algorithm>

#include "position.hpp"
#include "move.hpp"

/* information passed to the search function.                                */
struct SearchInfo {
	/* a pointer to the position.                                            */
	const Position *pos;

	/* time in milliseconds for both players.                                */
	int time[2];

	/* increment in milliseconds for both players.                           */
	int increment[2];
};

/* the return type of `minimax`                                              */
struct SearchResult {
	/* the best move found for the position passed to `minimax`.             */
	Move move;

	/* the score of the position passed to `minimax`.                        */
	int score;
};

/* in essence, `minimax` is just another evaluation function. it looks some  */
/* number of moves into the future and returns the value of the best         */
/* position we can reach, assuming our opponent is trying to give us a worse */
/* position. the number of number of moves we look into the future is called */
/* the search depth. at depth 1, we have full control over the position, so  */
/* the value of this position is the maximum of the values of all positions  */
/* after one move. at depth 2, we also consider that our opponent will get   */
/* to make a move, and they will always pick the move that is worst for us,  */
/* so the value of a position after we make a move is the minimum of the     */
/* values of all positions after our opponent also moves. and the final      */
/* value of this position is the maximum of those values. this recursive     */
/* alternating minimizing and maximizing is where minimax gets its name      */
/* from. this function returns both the best move and the value of the       */
/* position.                                                                 */
/*                                                                           */
/* POSSIBLE IMPROVEMENT: alpha-beta pruning                                  */
/* our naive minimax function wastes a lot of time calculating moves for one */
/* player, after we have already found a move that is so bad for the other   */
/* player that they would never want to enter this position anyways. for     */
/* example, if we are trying to determine the value of a queen move, and we  */
/* find the move results in a position where the opponent can forcibly       */
/* capture the queen, there is no point in calculating the rest of the       */
/* possible outcomes after we move the queen. once we find that the opponent */
/* can force us into a position that is worse than one we already have for a */
/* different move, we can stop calculating this branch of the search tree.   */
/* alpha-beta pruning accomplishes this by keeping track of two values,      */
/* alpha, and beta. alpha is the minimum score that we can surely reach no   */
/* no matter what our opponent does, and beta is the maximum score that our  */
/* opponent can surely reach no matter what we do. when a move is found with */
/* a score that falls outside of these bounds, we can stop searching this    */
/* position, because one of the players is sure to play something else for   */
/* one of the moves leading up to this position.                             */
/*                                                                           */
/* POSSIBLE IMPROVEMENT: move ordering                                       */
/* with alpha-beta pruning implemented, there are suddenly big performance   */
/* improvements to be had by searching first moves that are likely to be     */
/* good. why? remember, alpha-beta pruning works by stopping the search of a */
/* position when a move is found that is so bad for one player that they     */
/* will surely play something else for one of the moves leading up to this   */
/* position. so, we want to try these moves that are likely to cause cutoffs */
/* as soon as possible, so that we might be able to avoid searching the rest */
/* of the moves. the move ordering key is a sort of mini evaluation function */
/* for moves instead of positions. captures, promotions, and checks, are     */
/* often ordered before quiet moves. you can also use results from previous  */
/* searches to order moves, if some move was good before, it is likely still */
/* pretty good even if the position is slightly different.                   */
/*                                                                           */
/* POSSIBLE IMPROVEMENT: transposition table                                 */
/* in chess, transposition is the act of transitioning from one position     */
/* into another position. most positions can be transposed into in multiple  */
/* ways. for example, 1. e4 e5 2. Nc3 gives the same position as             */
/* 1. Nc3 e5 2. e4. as there are two ways to reach this position, our naive  */
/* minimax implementation would end up evaluating it twice. implement a hash */
/* table to quickly check if a position has been seen before, and if so,     */
/* just take the result from a previous search. special care must be taken   */
/* when this is combined with alpha-beta pruning, as the values for alpha    */
/* and beta may be different from what they were when the position was first */
/* searched. also note that your program has limited memory, so you must     */
/* develop a strategy to replace old entries with ones that are more likely  */
/* to be useful when you run out of space. see the wiki for more info.       */
/*                                                                           */
/* POSSIBLE IMPROVEMENT: quiescence search                                   */
/* our naive minimax implementation searches every move to the same depth.   */
/* in practice, it is often better to spend some extra time after the main   */
/* search depth has been reached to check if there are any moves that could  */
/* have a big impact on the evaluation, and might result in an inaccurate    */
/* evaluation if left unresolved. for example if, at the end of our search   */
/* depth, we reach a position where one player could capture a piece and the */
/* capturing piece could not be recaptured, then the other player is very    */
/* likely to lose that piece, and it would be inaccurate to evaluate the     */
/* position with the piece still on the board. quiescence search extends the */
/* search after the depth has reached zero, but only considers impactful     */
/* moves such as captures. when there are no such moves left, the normal     */
/* evaluation function is called. quiescence search works best when you have */
/* a special move generator that only generates impactful moves and doesn't  */
/* waste any time generating quiet moves.                                    */
/*                                                                           */
/* https://www.chessprogramming.org/Minimax                                  */
/* https://www.chessprogramming.org/Alpha-Beta                               */
/* https://www.chessprogramming.org/Move_Ordering                            */
/* https://www.chessprogramming.org/Transposition_Table                      */
/* https://www.chessprogramming.org/Quiescence_Search                        */
// SearchResult minimax(const Position& pos, int depth, int alpha = 0, int beta = 0);

/* the search function sets up the search parameters and calls `minimax` to  */
/* starts searching. our basic implementation always starts a search at a    */
/* fixed depth of 4.                                                         */
/*                                                                           */
/* POSSIBLE IMPROVEMENT: time management                                     */
/* instead of always searching to a fixed depth, come up with a better       */
/* strategy to effectively use your time. for example, when you are low on   */
/* time you might want to search to a shallower depth.                       */
/*                                                                           */
/* POSSIBLE IMPROVEMENT: iterative deepening                                 */
/* even if you know how much time you want to spend calculating your next    */
/* move, how do you decide what depth to search at so that the search        */
/* completes within the given time? the time it takes to search to a given   */
/* depth depends on the number of legal moves in the position, the speed of  */
/* the system the engine is running on, and other things. instead of trying  */
/* to guess a good depth, you can start with some low depth, such as 2, and  */
/* incrementally search higher depths when there is still time left. this    */
/* might seem like it wastes a lot of time searching lower depths only to    */
/* discard the result after searching a higher depth, but in practice this   */
/* time is insignificant, because a search at even one depth higher takes an */
/* order of magnitude longer to complete than all lower depths before it.    */
/*                                                                           */
/* POSSIBLE IMPROVEMENT: opening book                                        */
/* a deterministic chess engine will always output the same move when given  */
/* the same position. so instead of wasting time calculating the best move   */
/* in the starting position, you can instead precalculate the best move and  */
/* return it immediately. you can also precalculate moves for common         */
/* openings (for example, what to do after 1. e4 e5?), and endgames.         */
/*                                                                           */
/* https://www.chessprogramming.org/Search                                   */
/* https://www.chessprogramming.org/Time_Management                          */
/* https://www.chessprogramming.org/Iterative_Deepening                      */
/* https://www.chessprogramming.org/Opening_Book                             */
Move search(const SearchInfo& info);
