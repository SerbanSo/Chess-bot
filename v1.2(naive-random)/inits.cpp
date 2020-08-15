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

	//empty space between the players
	for ( int i = A; i <=H; i++ )
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
}
