#ifndef NEGAMAX
#define NEGAMAX

#include "defines.h"
#include "moves.h"
#include <vector>

using namespace std;

// Returns the value for each piece
int piece_value(int piece);

// Returns the evaluation score in centipawns, from the current player's perspective
int board_evaluation(int player);

// Return piece value at the given position
int piece_evaluation(unsigned long long position, int player, vector<int> piece_moves, Bitboard oldboard);

// It offers a bonus for Chebyshev's distance from the piece to the king
int king_distance_bonus(int player, unsigned long long position);

// Returns the value of a piece calculated as:
// piece value + piece value table + x * number of possible moves
// where x is arbitrary choose for each piece type
int piece_evaluation(unsigned long long position, int player, vector<int> piece_moves, Bitboard oldboard);

// Returns the evaluation score in centipawns, for a player
int player_evaluation(int player);

// TO DO
bool identical_game_states(GameState g1, GameState g2);

// TO DO
bool threefold_repetition();

// Find the best move possible, and send it to xboard
// Resign if no good move is found
void find_best_move(int player, int depth);

// Negamax implementation
int negamax(int player, int depth, Move &best_move, Move &second_best_move);

// Compare function for moves vector sorting
bool moves_cmp_fct(const Move & a, const Move & b);

// Negamax with alpha-beta pruning
int alphanegamax(int player, int depth, int alpha, int beta, Move &best_move, Move &second_best_move);
int alphanegamax2(int player, int depth, int alpha, int beta, Move &best_move, Move &second_best_move);
int alphanegamax3(int player, int depth, int alpha, int beta, Move &best_move, Move &second_best_move, vector<int> old_piece_moves);

// TO DO
void add_valid_move(int *moves_number);

// Generate all possible moves numbers for given player
void gen_piece_moves(int player, vector<int> &piece_moves);

// Generate all possible moves for given player
int generate_moves(int player, vector<Move> &moves, vector<int> &piece_moves);

// Generate moves for players pawns
void generate_moves_pawn(int player, vector<Move> &moves);

// Generate moves for players knights
void generate_moves_knight(int player, vector<Move> &moves);

// Generate moves for players bishops
void generate_moves_bishop(int player, vector<Move> &moves);

// Generate moves for players rooks
void generate_moves_rook(int player, vector<Move> &moves);

// Generate moves for players queen(s)
void generate_moves_queen(int player, vector<Move> &moves);

// Generate moves for players king
void generate_moves_king(int player, vector<Move> &moves);


#endif
