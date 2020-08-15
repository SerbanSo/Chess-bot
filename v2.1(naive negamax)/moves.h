#ifndef MOVES
#define MOVES

#include "defines.h"

// Returns true if the (x,y) square is attacked by at least one enemy piece
bool is_attacked(int board[2][9][9], position pieces_positions[2][16], int pieces_number[2], int king_moved[2], int left_rook_moved[2], int right_rook_moved[2], int en_passant_available[2][9], position king_position[2], int x, int y, int player);

// Determine if the command received from xboard is a move
bool is_move(char *move);

// Returns true if the king is in check after making the move
bool is_in_check_after_move(int board[2][9][9], position pieces_positions[2][16], int pieces_number[2], int king_moved[2], int left_rook_moved[2], int right_rook_moved[2], int en_passant_available[2][9], position king_position[2], int startX, int startY, int endX, int endY, int player);

// Determine if the move is legal or illegal
bool is_move_valid(int board[2][9][9], position pieces_positions[2][16], int pieces_number[2], int king_moved[2], int left_rook_moved[2], int right_rook_moved[2], int en_passant_available[2][9], position king_position[2], char *move, int player);

// Updates the piece vectors, required for generating moves efficiently
void change_position(position pieces_positions[2][16], int pieces_number[2], int player, int startX, int startY, int endX, int endY);

// Updates the piece vectors, required for generating moves efficiently
void delete_piece(position pieces_positions[2][16], int pieces_number[2], int player, int startX, int startY);

// Update the engine matrices after a move
void make_move(int board[2][9][9], position pieces_positions[2][16], int pieces_number[2], int king_moved[2], int left_rook_moved[2], int right_rook_moved[2], int en_passant_available[2][9], position king_position[2], char *move, int player);

// Generate and send a random move
void make_random_move();

// Determine if the move is valid for a Pawn piece
bool pawn_valid_move(int board[2][9][9], int king_moved[2], int left_rook_moved[2], int right_rook_moved[2], int en_passant_available[2][9], position king_position[2], int startX, int startY, int endX, int endY, int player);

// Determine if the move is valid for a Rook piece
bool rook_valid_move(int board[2][9][9], int startX, int startY, int endX, int endY);

// Determine if the move is valid for a Knight piece
bool knight_valid_move(int board[2][9][9], int startX, int startY, int endX, int endY);

// Determine if the move is valid for a Bishop piece
bool bishop_valid_move(int board[2][9][9], int startX, int startY, int endX, int endY);

// Determine if the move is valid for a Queen piece
bool queen_valid_move(int board[2][9][9], int startX, int startY, int endX, int endY);

// Determine if the move is valid for a King piece
bool king_valid_move(int board[2][9][9], position pieces_positions[2][16], int pieces_number[2], int king_moved[2], int left_rook_moved[2], int right_rook_moved[2], int en_passant_available[2][9], position king_position[2], int startX, int startY, int endX, int endY, int player);

#endif
