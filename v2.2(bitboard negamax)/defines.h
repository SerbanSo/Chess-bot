#ifndef DEFINES
#define DEFINES

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <string>
#include <algorithm>

// Square value defines
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

// For game representation
typedef struct {
	unsigned long long board[2];
	unsigned long long pawns[2];
	unsigned long long knights[2];
	unsigned long long bishops[2];
	unsigned long long rooks[2];
	unsigned long long queens[2];
	unsigned long long king[2];
	int score[2];
} Bitboard;

// Variables used for checking castling rights
typedef struct {
	bool moved_left_rook;
	bool moved_right_rook;
	bool moved_king;
	int king_pos; //useless
	int left_rook_pos; //useless
	int right_rook_pos; //useless
} Castling;

// The state of the game at a given moment in time
typedef struct {
    Bitboard board;
    Castling castling[2];
    int en_passant[2];
} GameState;

// Structure for a move
typedef struct {
	unsigned long long start;
	unsigned long long end;
} Move;

// Structure for magic pieces
typedef struct {
	unsigned long long mask;
	unsigned long long magic;
	int shift;
	int offset;
} Magic;

#define MINUS_INF -2000000

extern int engine;
extern int opponent;

// The bitboard used in-game
extern Bitboard board;

// Piece value for each square, used in evaluation
extern int piece_value_table[2][7][9][9];

// For special moves
extern Castling castling[2];
extern int en_passant[2];
extern unsigned long long notAFile;
extern unsigned long long notHFile;
extern unsigned long long rank8File;
extern unsigned long long rank1File;
extern unsigned long long rank2File;
extern unsigned long long rank4File;
extern unsigned long long rank7File;
extern unsigned long long rank5File;

// For move generation
extern unsigned long long king_moves[64];
extern unsigned long long knight_moves[64];
extern unsigned long long rook_moves[64];
extern unsigned long long bishop_moves[64];
extern unsigned long long queen_moves[64];
extern unsigned long long pawn_moves[2][64];
extern unsigned long long pawn_attacks[2][64];
extern Magic rook_magic[64];
extern Magic bishop_magic[64];
extern unsigned long long rook_table[102400];
extern unsigned long long bishop_table[5248];

// For threefold repetition detection
// TODO initialize, update in make_move
extern GameState past_game_states[512];
extern int move_number;

// Maximum depth
extern int max_depth;

// FOR TESTING PURPOSES
void print_bit(unsigned long long a);
extern std::ofstream bit_board_file;

#endif
