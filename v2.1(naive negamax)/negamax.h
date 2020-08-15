#ifndef NEGAMAX
#define NEGAMAX

#include "defines.h"
#include "moves.h"

int piece_value(int piece);

int board_evaluation(int board[2][9][9], position pieces_positions[2][16], int pieces_number[2], int king_moved[2], int left_rook_moved[2], int right_rook_moved[2], int en_passant_available[2][9], position king_position[2], int include_table, int player);

void copy_game_state(int board[2][9][9], position pieces_positions[2][16], int pieces_number[2], int king_moved[2], int left_rook_moved[2], int right_rook_moved[2], int en_passant_available[2][9], position king_position[2],
                     int board_copy[2][9][9], position pieces_positions_copy[2][16], int pieces_number_copy[2], int king_moved_copy[2], int left_rook_moved_copy[2], int right_rook_moved_copy[2], int en_passant_available_copy[2][9], position king_position_copy[2]);

bool threefold_repetition(int player);

void find_best_move(int board[2][9][9], position pieces_positions[2][16], int pieces_number[2], int king_moved[2], int left_rook_moved[2], int right_rook_moved[2], int en_passant_available[2][9], position king_position[2], int player, int depth);

int negamax(int board[2][9][9], position pieces_positions[2][16], int pieces_number[2], int king_moved[2], int left_rook_moved[2], int right_rook_moved[2], int en_passant_available[2][9], position king_position[2], char second_best_move[5], char best_move[5], int player, int depth);

int generate_moves(int board[2][9][9], position pieces_positions[2][16], int pieces_number[2], int king_moved[2], int left_rook_moved[2], int right_rook_moved[2], int en_passant_available[2][9], position king_position[2], char valid_moves[64][5], int player);

void add_valid_move(char move[5], char valid_moves[64][5], int *moves_number);

void generate_moves_pawn(int board[2][9][9], position pieces_positions[2][16], int pieces_number[2], int king_moved[2], int left_rook_moved[2], int right_rook_moved[2], int en_passant_available[2][9], position king_position[2], position start, char valid_moves[64][5], int *moves_number, int player);

void generate_moves_knight(int board[2][9][9], position pieces_positions[2][16], int pieces_number[2], int king_moved[2], int left_rook_moved[2], int right_rook_moved[2], int en_passant_available[2][9], position king_position[2], position start, char valid_moves[64][5], int *moves_number, int player);

void generate_moves_bishop(int board[2][9][9], position pieces_positions[2][16], int pieces_number[2], int king_moved[2], int left_rook_moved[2], int right_rook_moved[2], int en_passant_available[2][9], position king_position[2], position start, char valid_moves[64][5], int *moves_number, int player);

void generate_moves_rook(int board[2][9][9], position pieces_positions[2][16], int pieces_number[2], int king_moved[2], int left_rook_moved[2], int right_rook_moved[2], int en_passant_available[2][9], position king_position[2], position start, char valid_moves[64][5], int *moves_number, int player);

void generate_moves_queen(int board[2][9][9], position pieces_positions[2][16], int pieces_number[2], int king_moved[2], int left_rook_moved[2], int right_rook_moved[2], int en_passant_available[2][9], position king_position[2], position start, char valid_moves[64][5], int *moves_number, int player);

void generate_moves_king(int board[2][9][9], position pieces_positions[2][16], int pieces_number[2], int king_moved[2], int left_rook_moved[2], int right_rook_moved[2], int en_passant_available[2][9], position king_position[2], position start, char valid_moves[64][5], int *moves_number, int player);

#endif
