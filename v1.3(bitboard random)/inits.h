#ifndef INITS
#define INITS

#include "defines.h"
#include "magic.h"

// Load engine inits
void start_inits();

// New game inits
void board_inits();

// Inits pawns
void init_pawns();

// Inits knights
void init_knights();

// Inits bishops
void init_bishops();

// Inits rooks
void init_rooks();

// Inits queens
void init_queens();

// Inits kings
void init_kings();

// Inits whole boards
void init_board();

// Inits each king possible move from a square
void init_king_moves();

// Inits each knight possible move from a square
void init_knight_moves();

// Inits each pawn possible move from a square
void init_pawn_moves();

#endif
