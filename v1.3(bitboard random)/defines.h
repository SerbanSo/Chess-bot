#ifndef DEFINES
#define DEFINES

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <csignal>
#include <string>
#include <algorithm>

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

typedef struct{
	int x;
	int y;
} position;

extern int engine;
extern int opponent;

extern int king_moved[2];
extern int left_rook_moved[2];
extern int right_rook_moved[2];

extern int en_passant_available[2][9];

extern position king_position[2];

//----------------------------------------
//Starting defines for bitboard

#define A1 0
#define B1 1
#define C1 2
#define D1 3
#define E1 4
#define F1 5
#define G1 6
#define H1 7

#define A2 8
#define B2 9
#define C2 10
#define D2 11
#define E2 12
#define F2 13
#define G2 14
#define H2 15

#define A3 16
#define B3 17
#define C3 18
#define D3 19
#define E3 20
#define F3 21
#define G3 22
#define H3 23

#define A4 24
#define B4 25
#define C4 26
#define D4 27
#define E4 28
#define F4 29
#define G4 30
#define H4 31

#define A5 32
#define B5 33
#define C5 34
#define D5 35
#define E5 36
#define F5 37
#define G5 38
#define H5 39

#define A6 40
#define B6 41
#define C6 42
#define D6 43
#define E6 44
#define F6 45
#define G6 46
#define H6 47

#define A7 48
#define B7 49
#define C7 50
#define D7 51
#define E7 52
#define F7 53
#define G7 54
#define H7 55

#define A8 56
#define B8 57
#define C8 58
#define D8 59
#define E8 60
#define F8 61
#define G8 62
#define H8 63

// Structure for bitboard representation
typedef struct{
	unsigned long long white_board; 
	unsigned long long black_board;

	unsigned long long white_pawns;
	unsigned long long black_pawns;

	unsigned long long white_knights;
	unsigned long long black_knights;

	unsigned long long white_bishops;
	unsigned long long black_bishops;

	unsigned long long white_rooks;
	unsigned long long black_rooks;

	unsigned long long white_queen;
	unsigned long long black_queen;

	unsigned long long white_king;
	unsigned long long black_king;
} Play_board;

typedef struct {
	unsigned long long board[2];
	unsigned long long pawns[2];
	unsigned long long knights[2];
	unsigned long long bishops[2];
	unsigned long long rooks[2];
	unsigned long long queen[2];
	unsigned long long king[2];
} Bitboard;

typedef struct {
	bool moved_left_rook;
	bool moved_right_rook;
	bool moved_king;
	int king_pos;
	int left_rook_pos;
	int right_rook_pos;
} Castling;

// The bitboard use in-game
extern Bitboard board;
extern Castling castling[2];
extern unsigned long long king_moves[64];
extern unsigned long long knight_moves[64];
extern unsigned long long rook_moves[64];
extern unsigned long long bishop_moves[64];
extern unsigned long long queen_moves[64];
extern unsigned long long pawn_moves[2][64];
extern unsigned long long pawn_attacks[2][64];
extern int king_pos[2];
extern int en_passant[2];
extern unsigned long long notAFile;
extern unsigned long long notHFile;
// Structure for magic pieces
typedef struct {
	unsigned long long mask;
	unsigned long long magic;
	int shift;
	int offset;
} Magic;

extern Magic rook_magic[64];
extern Magic bishop_magic[64];
extern unsigned long long rook_table[102400];
extern unsigned long long bishop_table[5248];

// FOR TESTING PURPOSE

void print_bit(unsigned long long a);
extern std::ofstream bit_board_file;

//----------------------------------------

#endif
