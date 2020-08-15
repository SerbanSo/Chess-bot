#ifndef DEFINES
#define DEFINES

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <csignal>

enum COLORS
{
	WHITE = 0,
	BLACK = 1
};

enum PIECES
{
	EMPTY = 0,
	PAWN = 1,
	KNIGHT = 2,
	BISHOP = 3,
	ROOK = 4,
	QUEEN = 5,
	KING = 6
};

enum LETTERS
{
	A = 1,
	B = 2,
	C = 3,
	D = 4,
	E = 5,
	F = 6,
	G = 7,
	H = 8
};

struct position
{
	int x;
	int y;
};

// Play board matrix
extern int board[2][9][9];

//the corresponding colors for the engine and its opponent
extern int engine;
extern int opponent;

extern int king_moved[2];
extern int left_rook_moved[2];
extern int right_rook_moved[2];

extern int en_passant_available[2][9];

extern position king_position[2];

#endif
