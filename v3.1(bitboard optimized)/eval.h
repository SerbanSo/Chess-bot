#ifndef EVAL
#define EVAL

#include "defines.h"
#include "negamax.h"
using namespace std;

// Total player evaluation
int player_eval(int player);

// Pieces value for middle game
int piece_value_middle(int piece);

// Pieces value for end game
int piece_value_end(int piece);

// Bonus or penalty for pieces mobility for middle game
int mobility_middle(vector<int> piece_moves);

// Bonus or penalty for pieces mobility for end game
int mobility_end(vector<int> piece_moves);

// Bonus or penalty for each pieces on each square for middle game
int piece_square_bonus_middle(int player);

// Bonus or penalty for each pieces on each square for end game
int piece_square_bonus_end(int player);

// King safety bonus points
int king_safety(int player);

#endif
