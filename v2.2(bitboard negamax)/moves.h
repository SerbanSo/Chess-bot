#ifndef MOVES
#define MOVES

#include "defines.h"
#include "magic.h" // Mb delete???

// Transform a move into an integer, ex. for a move "a1", "b3"
int parse_move(char *move);

// Determine if the command received from xboard is a move
bool is_move(char *move);

// Send the move to std::cout
void send_move(int start, int end);

// Check if the received move is valid
bool is_move_valid(int start, int end, int player);

// If the move is valid, update the matrices and remove any attacked piece
void make_move(unsigned long long start, unsigned long long end, int player);

// Check for check ;)
bool is_in_check(int player);

// Check if the move is a valid castling one
bool castling_valid_move(int start, int end, int player);

// Check if the move is valid for a pawn
bool pawn_valid_move(int start, int end, int player);

// Check if the move is valid for a rook
bool rook_valid_move(int start, int end, int player);

// Check if the move is valid for a knight
bool knight_valid_move(int start, int end, int player);

// Check if the move is valid for a bishop
bool bishop_valid_move(int start, int end, int player);

// Check if the move is valid for a queen
bool queen_valid_move(int start, int end, int player);

// Check if the move is valid for a king
bool king_valid_move(int start, int end, int player);

// Remove the piece from the given positions
void remove_piece(unsigned long long pos, int player);

// Update both boards
void update_boards();

#endif
