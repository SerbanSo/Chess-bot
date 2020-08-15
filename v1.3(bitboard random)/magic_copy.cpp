// include "common.h"

#include <cstdint>
//#ifndef COMMON_H
//#define COMMON_H

typedef uint64_t U64;

static const int kSquares = 64;

static const char kRookMagics[] = "rook_magics.magic";
static const char kRookMasks[] = "rook_masks.magic";
static const char kRookShifts[] = "rook_shifts.magic";
static const char kRookOffsets[] = "rook_offsets.magic";
static const char kRookAttackTable[] = "rook_attack_table.magic";

static const char kBishopMagics[] = "bishop_magics.magic";
static const char kBishopMasks[] = "bishop_masks.magic";
static const char kBishopShifts[] = "bishop_shifts.magic";
static const char kBishopOffsets[] = "bishop_offsets.magic";
static const char kBishopAttackTable[] = "bishop_attack_table.magic";

// #endif

// common.h

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;


inline unsigned Indx(unsigned row, unsigned col) { return row * 8 + col; }
inline unsigned Row(unsigned index) { return index / 8; }
inline unsigned Col(unsigned index) { return index % 8; }

enum D
{
    NORTH,
    SOUTH,
    EAST,
    WEST,
    NORTH_EAST,
    NORTH_WEST,
    SOUTH_EAST,
    SOUTH_WEST
};

// Index of the next square along this direction. Returns -1 if 
// next index is outside the board.
int nextIndex(int index, D direction) {
	int row = Row(index);
	int col = Col(index);

	switch(direction) {
		case NORTH:
			row++;
			break;
		case SOUTH:
			row--;
			break;
		case EAST:
			col++;
			break;
		case WEST:
			col--;
			break;
		case NORTH_EAST:
			row++;
			col++;
			break;
		case SOUTH_EAST:
			row--;
			col++;
			break;
		case SOUTH_WEST:
			row--;
			col--;
			break;
		case NORTH_WEST:
			row++;
			col--;
			break;
	}

	return (row > 7 || row < 0 || col > 7 || col < 0) ? -1 : Indx(row, col);
}

// Number of squares from given square to the edge of the board
// along this direction.
int edgeDistance(int index, D direction) {
	int row = Row(index);
	int col = Col(index);

	int d = -1;
	switch (direction) {
    case NORTH:
    	d = 7 - row;
       	break;
    case SOUTH:
    	d = row;
    	break;
    case EAST:
    	d = 7 - col;
    	break;
    case WEST:
    	d = col;
    	break;
    case NORTH_EAST:
    	d = min(7 - row, 7 - col);
    	break;
    case SOUTH_EAST:
    	d = min(row, 7 - col);
    	break;
    case SOUTH_WEST:
    	d = min(row, col);
    	break;
    case NORTH_WEST:
    	d = min(7 - row, col);
    	break;
	}
	return d;
}

// Masks all the bits from the given index, and along the given direction to 1,
// excluding the square given by the index and the edge of the board along
// given direction.
unsigned long long maskBits(D direction, int index) {
  unsigned long long bitboard = 0ULL;
  int next_index = index;

	while ((next_index = nextIndex(index, direction)) >= 0 &&
			nextIndex(next_index, direction) >= 0) {
		bitboard |= (1ULL << next_index);
  }

  	return bitboard;
}

// Generate all piece occupancies along a rank, file or diagonal, in the
// given direction, with index as the reference point. The square given
// by the index and the edge of the board in the given direction are not
// covered. For example, direction = NORTH_WEST, index = 29 (marked by X)
// will generate all combinations of occupancies for squares marked by #
// (there are 8 possible occupancies):
// 8 | 0 0 0 0 0 0 0 0
// 7 | 0 0 # 0 0 0 0 0
// 6 | 0 0 0 # 0 0 0 0
// 5 | 0 0 0 0 # 0 0 0
// 4 | 0 0 0 0 0 X 0 0
// 3 | 0 0 0 0 0 0 0 0
// 2 | 0 0 0 0 0 0 0 0
// 1 | 0 0 0 0 0 0 0 0
// -------------------
//   | A B C D E F G H
void PrintBitBoard(const U64 bitboard);
void generateOccupancies(D direction, int index, std::vector<unsigned long long>* bbv) {
  	// Number of squares in this direction excluding current square and
  	// edge of the board.
  	int num_squares = edgeDistance(index, direction) - 1;
  	if (num_squares <= 0) {
	    return;
  	}

  	// Number of possible piece occupancies in these squares along
  	// the given direction.
  	unsigned num_occupancies = (1U << num_squares);

  	// Create bitboard for each occupancy with the index next to given
  	// index as starting point, along the given direction.
	for (unsigned occupancy = 0U; occupancy < num_occupancies; ++occupancy) {
		U64 bitboard = 0ULL;
    	int next_index = index;
    	for (unsigned bit_mask = 1U; bit_mask <= occupancy; bit_mask <<= 1) {
    		next_index = nextIndex(next_index, direction);

    		printf("-------\n");
    		PrintBitBoard(!!(occupancy & bit_mask));
    		printf("\n");
    		printf("-------\n");
    		bitboard |= (U64(!!(occupancy & bit_mask)) << next_index);
    }
    	// PrintBitBoard(bitboard);
    	// printf("\n");

    	bbv->push_back(bitboard);
  }
}

// My fct
unsigned long long generateOccupancy(D direction, int index) {
	unsigned long long bitboard = 0;

	int num_squares = edgeDistance(index, direction) - 1;

	if(num_squares <= 0) {
		return bitboard;
	}

	unsigned int num_occupancies = (1u < num_squares);

	for(int occupancy = 0; occupancy < num_occupancies; occupancy++) {
		int next_index = index;
		for(int bit_mask = 1; bit_mask <= occupancy; bit_mask <<= 1) {
			next_index = nextIndex(next_index, direction);

			bitboard |= (U64(!!(occupancy & bit_mask)) << next_index);
		}
	}

    PrintBitBoard(bitboard);
    printf("\n");

    return bitboard;
}

void PrintBitBoard(const U64 bitboard) {
	for (int row = 7; row >= 0; --row) {
		for (int col = 0; col <= 7; ++col) {
			if (bitboard & (1ULL << ((row * 8) + col))) {
        		std::cout << "1 ";
      		} else {
        		std::cout << "0 ";
      		}
    	}
    	std::cout << std::endl;
  	}
}

// Generate all occupancies for
// Make four call:
// 1. For rook: NORTH, EAST, SOUTH, WEST
// 2. For bishop: NORTH_EAST, SOUTH_EAST, SOUTH_WEST, NORTH_WEST
void combineOccupancies(D direction, int index, vector<unsigned long long> &occupancies) {
	vector<unsigned long long> direction_occupancies;

	// Put in direction_occupancies all possible occupancy positions
	generateOccupancies(direction, index, &direction_occupancies);

	// If direction_occupancies is empty there are no posible occupancy
	// in that direction
	if(direction_occupancies.empty())
		return;

	// // If the occupancies vector is null 
	// // all occupancies are found in direction_occupancies
	// if(occupancies.empty()) {
	// 	// occupancies.insert(occupancies.end(), bbv.begin(), bbv.end());
	// 	occupancies.swap(direction_occupancies);
	// 	return;
	// }

	vector<unsigned long long> tmp;

	// Add all occupancies combined with the current found occupancies
	for(int i = 0; i < direction_occupancies.size(); i++) {
		for(int j = 0; j < occupancies.size(); j++) {
			tmp.push_back(direction_occupancies[i] | occupancies[j]);
		}
	}

	occupancies.swap(tmp);
}

int main() {

	std::vector<unsigned long long> bbv;

	generateOccupancies(NORTH_WEST, 29, &bbv);
	// PrintBitBoard(bbv[2]);

	// generateOccupancy(NORTH_WEST, 29);

	return 0;
}