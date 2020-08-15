#ifndef MOVES
#define MOVES

#include "defines.h"

// Returns true if the (x,y) square is attacked by at least one enemy piece
bool is_attacked(int x, int y, int player);

// Determine if the command received from xboard is a move
bool is_move(char *move);

// Returns true if the king is in check after making the move
bool is_in_check_after_move(int startX, int startY, int endX, int endY, int player);

// Determine if the move is legal or illegal
bool is_move_valid(char *move, int player);

// Update the engine matrices after a move
void make_move(char *move, int player);

// Generate and send a random move
void make_random_move();

// Determine if the move is valid for a Pawn piece
bool pawn_valid_move(int startX, int startY, int endX, int endY, int player);

// Determine if the move is valid for a Rook piece
bool rook_valid_move(int startX, int startY, int endX, int endY);

// Determine if the move is valid for a Knight piece
bool knight_valid_move(int startX, int startY, int endX, int endY);

// Determine if the move is valid for a Bishop piece
bool bishop_valid_move(int startX, int startY, int endX, int endY);

// Determine if the move is valid for a Queen piece
bool queen_valid_move(int startX, int startY, int endX, int endY);

// Determine if the move is valid for a King piece
bool king_valid_move(int startX, int startY, int endX, int endY, int player);

#endif
