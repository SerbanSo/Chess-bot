#include "inits.h"

//initializes the board at the start of a game
void init_board()
{
	// For the white player
	board[WHITE][A][1] = board[WHITE][H][1] = ROOK;
	board[WHITE][B][1] = board[WHITE][G][1] = KNIGHT;
	board[WHITE][C][1] = board[WHITE][F][1] = BISHOP;
	board[WHITE][D][1] = QUEEN;
	board[WHITE][E][1] = KING;
	for ( int i = A; i <= H; i++ )
	{
		board[WHITE][i][7] = board[WHITE][i][8] = EMPTY; //where black pieces will be
		board[WHITE][i][2] = PAWN;
		en_passant_available[WHITE][i] = 0;
	}

	king_position[WHITE].x = E;
	king_position[WHITE].y = 1;

	king_moved[WHITE] = 0;
	left_rook_moved[WHITE] = 0;
	right_rook_moved[WHITE] = 0;

	for ( int i = 0; i < 8; i++ )
	{
		// first line
		pieces_positions[WHITE][i].x = A + i;
		pieces_positions[WHITE][i].y = 1;

		// second line
		pieces_positions[WHITE][i + 8].x = A + i;
		pieces_positions[WHITE][i + 8].y = 2;
	}
	pieces_number[WHITE] = 16;

	//empty space between the players
	for ( int i = A; i <= H; i++ )
	{
		for ( int j = 3; j <= 6; j++ )
		{
			board[WHITE][i][j] = EMPTY;
			board[BLACK][i][j] = EMPTY;
		}
	}

	// For the black player
	board[BLACK][A][8] = board[BLACK][H][8] = ROOK;
	board[BLACK][B][8] = board[BLACK][G][8] = KNIGHT;
	board[BLACK][C][8] = board[BLACK][F][8] = BISHOP;
	board[BLACK][D][8] = QUEEN;
	board[BLACK][E][8] = KING;
	for ( int i = A; i <= H; i++ )
	{
		board[BLACK][i][1] = board[BLACK][i][2] = EMPTY; //where white pieces will be
		board[BLACK][i][7] = PAWN;
		en_passant_available[BLACK][i] = 0;
	}

	king_position[BLACK].x = E;
	king_position[BLACK].y = 8;

	king_moved[BLACK] = 0;
	left_rook_moved[BLACK] = 0;
	right_rook_moved[BLACK] = 0;

	for ( int i = 0; i < 8; i++ )
	{
		// seventh line
		pieces_positions[BLACK][i].x = A + i;
		pieces_positions[BLACK][i].y = 7;

		// eighth line
		pieces_positions[BLACK][i + 8].x = A + i;
		pieces_positions[BLACK][i + 8].y = 8;
	}
	pieces_number[BLACK] = 16;
}

void init_piece_value_table()
{
	// for white pieces
	// pawn
	for ( int i = 0; i < 8; i++ )
	{
		piece_value_table[WHITE][PAWN][A + i][8] = 0;
		piece_value_table[WHITE][PAWN][A + i][7] = 50;

		piece_value_table[WHITE][PAWN][A + i][1] = 0;
	}

	piece_value_table[WHITE][PAWN][A][6] = 10;
	piece_value_table[WHITE][PAWN][B][6] = 10;
	piece_value_table[WHITE][PAWN][C][6] = 20;
	piece_value_table[WHITE][PAWN][D][6] = 30;
	piece_value_table[WHITE][PAWN][E][6] = 30;
	piece_value_table[WHITE][PAWN][F][6] = 20;
	piece_value_table[WHITE][PAWN][G][6] = 10;
	piece_value_table[WHITE][PAWN][H][6] = 10;

	piece_value_table[WHITE][PAWN][A][5] = 5;
	piece_value_table[WHITE][PAWN][B][5] = 5;
	piece_value_table[WHITE][PAWN][C][5] = 10;
	piece_value_table[WHITE][PAWN][D][5] = 25;
	piece_value_table[WHITE][PAWN][E][5] = 25;
	piece_value_table[WHITE][PAWN][F][5] = 10;
	piece_value_table[WHITE][PAWN][G][5] = 5;
	piece_value_table[WHITE][PAWN][H][5] = 5;

	piece_value_table[WHITE][PAWN][A][4] = 0;
	piece_value_table[WHITE][PAWN][B][4] = 0;
	piece_value_table[WHITE][PAWN][C][4] = 0;
	piece_value_table[WHITE][PAWN][D][4] = 20;
	piece_value_table[WHITE][PAWN][E][4] = 20;
	piece_value_table[WHITE][PAWN][F][4] = 0;
	piece_value_table[WHITE][PAWN][G][4] = 0;
	piece_value_table[WHITE][PAWN][H][4] = 0;

	piece_value_table[WHITE][PAWN][A][3] = 5;
	piece_value_table[WHITE][PAWN][B][3] = -5;
	piece_value_table[WHITE][PAWN][C][3] = -10;
	piece_value_table[WHITE][PAWN][D][3] = 0;
	piece_value_table[WHITE][PAWN][E][3] = 0;
	piece_value_table[WHITE][PAWN][F][3] = -10;
	piece_value_table[WHITE][PAWN][G][3] = -5;
	piece_value_table[WHITE][PAWN][H][3] = 5;

	piece_value_table[WHITE][PAWN][A][2] = 5;
	piece_value_table[WHITE][PAWN][B][2] = 10;
	piece_value_table[WHITE][PAWN][C][2] = 10;
	piece_value_table[WHITE][PAWN][D][2] = -20;
	piece_value_table[WHITE][PAWN][E][2] = -20;
	piece_value_table[WHITE][PAWN][F][2] = 10;
	piece_value_table[WHITE][PAWN][G][2] = 10;
	piece_value_table[WHITE][PAWN][H][2] = 5;

	//knight
	piece_value_table[WHITE][KNIGHT][A][8] = -50;
	piece_value_table[WHITE][KNIGHT][B][8] = -40;
	piece_value_table[WHITE][KNIGHT][C][8] = -30;
	piece_value_table[WHITE][KNIGHT][D][8] = -30;
	piece_value_table[WHITE][KNIGHT][E][8] = -30;
	piece_value_table[WHITE][KNIGHT][F][8] = -30;
	piece_value_table[WHITE][KNIGHT][G][8] = -40;
	piece_value_table[WHITE][KNIGHT][H][8] = -50;

	piece_value_table[WHITE][KNIGHT][A][7] = -40;
	piece_value_table[WHITE][KNIGHT][B][7] = -20;
	piece_value_table[WHITE][KNIGHT][C][7] = 0;
	piece_value_table[WHITE][KNIGHT][D][7] = 0;
	piece_value_table[WHITE][KNIGHT][E][7] = 0;
	piece_value_table[WHITE][KNIGHT][F][7] = 0;
	piece_value_table[WHITE][KNIGHT][G][7] = -20;
	piece_value_table[WHITE][KNIGHT][H][7] = -40;

	piece_value_table[WHITE][KNIGHT][A][6] = -30;
	piece_value_table[WHITE][KNIGHT][B][6] = 0;
	piece_value_table[WHITE][KNIGHT][C][6] = 10;
	piece_value_table[WHITE][KNIGHT][D][6] = 15;
	piece_value_table[WHITE][KNIGHT][E][6] = 15;
	piece_value_table[WHITE][KNIGHT][F][6] = 10;
	piece_value_table[WHITE][KNIGHT][G][6] = 0;
	piece_value_table[WHITE][KNIGHT][H][6] = -30;

	piece_value_table[WHITE][KNIGHT][A][5] = -30;
	piece_value_table[WHITE][KNIGHT][B][5] = 5;
	piece_value_table[WHITE][KNIGHT][C][5] = 15;
	piece_value_table[WHITE][KNIGHT][D][5] = 20;
	piece_value_table[WHITE][KNIGHT][E][5] = 20;
	piece_value_table[WHITE][KNIGHT][F][5] = 15;
	piece_value_table[WHITE][KNIGHT][G][5] = 5;
	piece_value_table[WHITE][KNIGHT][H][5] = -30;

	piece_value_table[WHITE][KNIGHT][A][4] = -30;
	piece_value_table[WHITE][KNIGHT][B][4] = 0;
	piece_value_table[WHITE][KNIGHT][C][4] = 15;
	piece_value_table[WHITE][KNIGHT][D][4] = 20;
	piece_value_table[WHITE][KNIGHT][E][4] = 20;
	piece_value_table[WHITE][KNIGHT][F][4] = 15;
	piece_value_table[WHITE][KNIGHT][G][4] = 0;
	piece_value_table[WHITE][KNIGHT][H][4] = -30;

	piece_value_table[WHITE][KNIGHT][A][3] = -30;
	piece_value_table[WHITE][KNIGHT][B][3] = 5;
	piece_value_table[WHITE][KNIGHT][C][3] = 10;
	piece_value_table[WHITE][KNIGHT][D][3] = 15;
	piece_value_table[WHITE][KNIGHT][E][3] = 15;
	piece_value_table[WHITE][KNIGHT][F][3] = 10;
	piece_value_table[WHITE][KNIGHT][G][3] = 5;
	piece_value_table[WHITE][KNIGHT][H][3] = -30;

	piece_value_table[WHITE][KNIGHT][A][2] = -40;
	piece_value_table[WHITE][KNIGHT][B][2] = -20;
	piece_value_table[WHITE][KNIGHT][C][2] = 0;
	piece_value_table[WHITE][KNIGHT][D][2] = 5;
	piece_value_table[WHITE][KNIGHT][E][2] = 5;
	piece_value_table[WHITE][KNIGHT][F][2] = 0;
	piece_value_table[WHITE][KNIGHT][G][2] = -20;
	piece_value_table[WHITE][KNIGHT][H][2] = -40;

	piece_value_table[WHITE][KNIGHT][A][1] = -50;
	piece_value_table[WHITE][KNIGHT][B][1] = -40;
	piece_value_table[WHITE][KNIGHT][C][1] = -30;
	piece_value_table[WHITE][KNIGHT][D][1] = -30;
	piece_value_table[WHITE][KNIGHT][E][1] = -30;
	piece_value_table[WHITE][KNIGHT][F][1] = -30;
	piece_value_table[WHITE][KNIGHT][G][1] = -40;
	piece_value_table[WHITE][KNIGHT][H][1] = -50;

	//bishop
	piece_value_table[WHITE][BISHOP][A][8] = -20;
	piece_value_table[WHITE][BISHOP][B][8] = -10;
	piece_value_table[WHITE][BISHOP][C][8] = -10;
	piece_value_table[WHITE][BISHOP][D][8] = -10;
	piece_value_table[WHITE][BISHOP][E][8] = -10;
	piece_value_table[WHITE][BISHOP][F][8] = -10;
	piece_value_table[WHITE][BISHOP][G][8] = -10;
	piece_value_table[WHITE][BISHOP][H][8] = -20;

	piece_value_table[WHITE][BISHOP][A][7] = -10;
	piece_value_table[WHITE][BISHOP][B][7] = 0;
	piece_value_table[WHITE][BISHOP][C][7] = 0;
	piece_value_table[WHITE][BISHOP][D][7] = 0;
	piece_value_table[WHITE][BISHOP][E][7] = 0;
	piece_value_table[WHITE][BISHOP][F][7] = 0;
	piece_value_table[WHITE][BISHOP][G][7] = 0;
	piece_value_table[WHITE][BISHOP][H][7] = -10;

	piece_value_table[WHITE][BISHOP][A][6] = -10;
	piece_value_table[WHITE][BISHOP][B][6] = 0;
	piece_value_table[WHITE][BISHOP][C][6] = 5;
	piece_value_table[WHITE][BISHOP][D][6] = 10;
	piece_value_table[WHITE][BISHOP][E][6] = 10;
	piece_value_table[WHITE][BISHOP][F][6] = 5;
	piece_value_table[WHITE][BISHOP][G][6] = 0;
	piece_value_table[WHITE][BISHOP][H][6] = -10;

	piece_value_table[WHITE][BISHOP][A][5] = -10;
	piece_value_table[WHITE][BISHOP][B][5] = 5;
	piece_value_table[WHITE][BISHOP][C][5] = 5;
	piece_value_table[WHITE][BISHOP][D][5] = 10;
	piece_value_table[WHITE][BISHOP][E][5] = 10;
	piece_value_table[WHITE][BISHOP][F][5] = 5;
	piece_value_table[WHITE][BISHOP][G][5] = 5;
	piece_value_table[WHITE][BISHOP][H][5] = -10;

	piece_value_table[WHITE][BISHOP][A][4] = -10;
	piece_value_table[WHITE][BISHOP][B][4] = 0;
	piece_value_table[WHITE][BISHOP][C][4] = 10;
	piece_value_table[WHITE][BISHOP][D][4] = 10;
	piece_value_table[WHITE][BISHOP][E][4] = 10;
	piece_value_table[WHITE][BISHOP][F][4] = 10;
	piece_value_table[WHITE][BISHOP][G][4] = 0;
	piece_value_table[WHITE][BISHOP][H][4] = -10;

	piece_value_table[WHITE][BISHOP][A][3] = -10;
	piece_value_table[WHITE][BISHOP][B][3] = 10;
	piece_value_table[WHITE][BISHOP][C][3] = 10;
	piece_value_table[WHITE][BISHOP][D][3] = 10;
	piece_value_table[WHITE][BISHOP][E][3] = 10;
	piece_value_table[WHITE][BISHOP][F][3] = 10;
	piece_value_table[WHITE][BISHOP][G][3] = 10;
	piece_value_table[WHITE][BISHOP][H][3] = -10;

	piece_value_table[WHITE][BISHOP][A][2] = -10;
	piece_value_table[WHITE][BISHOP][B][2] = 5;
	piece_value_table[WHITE][BISHOP][C][2] = 0;
	piece_value_table[WHITE][BISHOP][D][2] = 0;
	piece_value_table[WHITE][BISHOP][E][2] = 0;
	piece_value_table[WHITE][BISHOP][F][2] = 0;
	piece_value_table[WHITE][BISHOP][G][2] = 5;
	piece_value_table[WHITE][BISHOP][H][2] = -10;

	piece_value_table[WHITE][BISHOP][A][1] = -20;
	piece_value_table[WHITE][BISHOP][B][1] = -10;
	piece_value_table[WHITE][BISHOP][C][1] = -10;
	piece_value_table[WHITE][BISHOP][D][1] = -10;
	piece_value_table[WHITE][BISHOP][E][1] = -10;
	piece_value_table[WHITE][BISHOP][F][1] = -10;
	piece_value_table[WHITE][BISHOP][G][1] = -10;
	piece_value_table[WHITE][BISHOP][H][1] = -20;

	//rook
	for ( int i = 0; i < 8 ; i++ )
	{
		piece_value_table[WHITE][ROOK][A + i][8] = 0;
	}

	piece_value_table[WHITE][ROOK][A][7] = 5;
	piece_value_table[WHITE][ROOK][B][7] = 10;
	piece_value_table[WHITE][ROOK][C][7] = 10;
	piece_value_table[WHITE][ROOK][D][7] = 10;
	piece_value_table[WHITE][ROOK][E][7] = 10;
	piece_value_table[WHITE][ROOK][F][7] = 10;
	piece_value_table[WHITE][ROOK][G][7] = 10;
	piece_value_table[WHITE][ROOK][H][7] = 5;

	for ( int i = 6; i >= 2 ; i-- )
	{
		piece_value_table[WHITE][ROOK][A][i] = -5;
		piece_value_table[WHITE][ROOK][B][i] = 0;
		piece_value_table[WHITE][ROOK][C][i] = 0;
		piece_value_table[WHITE][ROOK][D][i] = 0;
		piece_value_table[WHITE][ROOK][E][i] = 0;
		piece_value_table[WHITE][ROOK][F][i] = 0;
		piece_value_table[WHITE][ROOK][G][i] = 0;
		piece_value_table[WHITE][ROOK][H][i] = -5;
	}

	piece_value_table[WHITE][ROOK][A][1] = 0;
	piece_value_table[WHITE][ROOK][B][1] = 0;
	piece_value_table[WHITE][ROOK][C][1] = 0;
	piece_value_table[WHITE][ROOK][D][1] = 5;
	piece_value_table[WHITE][ROOK][E][1] = 5;
	piece_value_table[WHITE][ROOK][F][1] = 0;
	piece_value_table[WHITE][ROOK][G][1] = 0;
	piece_value_table[WHITE][ROOK][H][1] = 0;

	//queen
	for ( int i = A; i <= H; i++ )
	{
		for ( int j = 1; j <= 8; j++ )
		{
			piece_value_table[WHITE][QUEEN][i][j] = 0;
		}
	}

	//king
	for ( int i = A; i <= H; i++ )
	{
		for ( int j = 1; j <= 8; j++ )
		{
			piece_value_table[WHITE][KING][i][j] = 0;
		}
	}

	// for black pieces the values are mirrored
	for ( int i = A; i <= H; i++ )
	{
		for ( int j = 1; j <= 8; j++ )
		{
			piece_value_table[BLACK][PAWN][i][j] = piece_value_table[WHITE][PAWN][i][9 - j];
			piece_value_table[BLACK][KNIGHT][i][j] = piece_value_table[WHITE][KNIGHT][i][9 - j];
			piece_value_table[BLACK][BISHOP][i][j] = piece_value_table[WHITE][BISHOP][i][9 - j];
			piece_value_table[BLACK][ROOK][i][j] = piece_value_table[WHITE][ROOK][i][9 - j];
			piece_value_table[BLACK][QUEEN][i][j] = piece_value_table[WHITE][QUEEN][i][9 - j];
			piece_value_table[BLACK][KING][i][j] = piece_value_table[WHITE][KING][i][9 - j];
		}
	}
}
