#ifndef MAGIC
#define MAGIC

#include "defines.h"

// Return the bitboard coresponding to a rook on given positon and 
// given bitboard
unsigned long long rook_attack(int index, unsigned long long bitboard);

// Return the bitboard coresponding to a bishop on given positon and 
// given bitboard
unsigned long long bishop_attack(int index, unsigned long long bitboard);

// Return the bitboard coresponding to a queen on given positon and 
// given bitboard
unsigned long long queen_attack(int index, unsigned long long bitboard);

// Initialise rook and bishop Magic struct and tables
void init_magic();

#endif