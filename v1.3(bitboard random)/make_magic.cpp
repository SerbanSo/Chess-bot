#include "defines.h"

#include <vector>
using namespace std;

inline unsigned Indx(unsigned row, unsigned col) { return row * 8 + col; }
inline unsigned Row(unsigned index) { return index / 8; }
inline unsigned Col(unsigned index) { return index % 8; }

enum Directions
{
	NORTH,
	EAST,
	SOUTH,
	WEST,
	NORTH_EAST,
	SOUTH_EAST,
	SOUTH_WEST,
	NORTH_WEST
};

// Determine next index for a given direction
int next_index(int index, Directions direction) {
	int row = Row(index);
	int col = Col(index);

	switch(direction) {
		case NORTH:				// For Rook
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
		case NORTH_EAST:		// For Bishop
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

	if(row > 7 || row < 0 || col > 7 || col < 0)
		return -1;

	return 8 * row + col;
}

// Determine distance to the edge in given direction
int edge_distance(int index, Directions direction) {
	int row = Row(index);
	int col = Col(index);

	int distance = -1;
	switch (direction) {
    case NORTH:				// For Rook
    	distance = 7 - row;
       	break;
    case SOUTH:
    	distance = row;
    	break;
    case EAST:
    	distance = 7 - col;
    	break;
    case WEST:
    	distance = col;
    	break;
    case NORTH_EAST:		// For bishop
    	distance = min(7 - row, 7 - col);
    	break;
    case SOUTH_EAST:
    	distance = min(row, 7 - col);
    	break;
    case SOUTH_WEST:
    	distance = min(row, col);
    	break;
    case NORTH_WEST:
    	distance = min(7 - row, col);
    	break;
	}

	// Remove the edge from total distance
	return distance - 1;
}

// Make a mask that contains all
// Example: For a rook on position C5, given direction SOUTH
// 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0
// 0 0 X 0 0 0 0 0
// 0 0 1 0 0 0 0 0
// 0 0 1 0 0 0 0 0
// 0 0 1 0 0 0 0 0
// 0 0 0 0 0 0 0 0
//
int direction_mask(int index, Directions direction) {
	unsigned long long direction_bitboard = 0;
	int nextIndex = index;

	// Add to bitboard each bit in given direction
	while((nextIndex = next_index(nextIndex, direction)) >= 0) {
		// Don't add the edge in bitboard
		if(next_index(nextIndex, direction) < 0)
			break;

		// Add current position in bitboard
		direction_bitboard |= (1ULL << nextIndex);
	}

	return direction_bitboard;
}

// Generate all possible occupancies for a given direction
// MUST REMAKE
void generate_direction_occupancies(int index, Directions direction, vector<unsigned long long> &direction_occupancies) {
	int distance = edge_distance(index, direction);

	// If there is no possible move in the given direction
	if(distance <= 0)
		return;

	// Total number of occupancies
	// 2^distance bitboards
	unsigned occupancies_number = 1 << distance;

	// Generate all possible occupancies
	// TO CHANGE
	for(unsigned i = 0; i < occupancies_number; i++) {
		unsigned long long bitboard = 0;
		int nextIndex = index;

		for(unsigned j = 1; j <= i; j<<=1) {
			nextIndex = next_index(nextIndex, direction);

			bitboard |= ((unsigned long long)(!!(i & j)) << nextIndex);
		}

		direction_occupancies.push_back(bitboard);
	}
}

// Combine all occupancies for a given index
// Must be called four time, for each direction
// MUST REMAKE
void combine_occupancies(int index, Directions direction, vector<unsigned long long> &occupancies) {
	vector<unsigned long long> direction_occupancies;

	generate_direction_occupancies(index, direction, direction_occupancies);

	// No occupancy in given direction
	if(direction_occupancies.empty()) {
		return;
	}

	// No occupancies
	if(occupancies.empty()) {
		// occupancies.swap(direction_occupancies);
		occupancies.insert(occupancies.end(), direction_occupancies.begin(), direction_occupancies.end());
		return;
	}

	vector<unsigned long long> tmp;
	// Combine all current occupancies
	for(unsigned long long i = 0; i < direction_occupancies.size(); i++) {
		for(unsigned long long j = 0; j < occupancies.size(); j++) {
			tmp.push_back(direction_occupancies[i] | occupancies[j]);
		}
	}

	occupancies.swap(tmp);
}

// Generate attack bitboard in given direction
unsigned long long generate_direction_attack(int index, Directions direction, unsigned long long occupancy) {
	unsigned long long bitboard = 0;
	int nextIndex = index;

	while((nextIndex = next_index(nextIndex, direction)) != -1) {
		bitboard |= (1ULL << nextIndex);

		// If the current position is occupied
		if(occupancy & (1ULL << nextIndex)) {
			break;
		}
	}

	return bitboard;
}


// ----------------REMOVE THIS-------------------
// Returns a unsigned 64 bit random number.
unsigned long long U64Rand() {
  return ((unsigned long long)(0xFFFF & rand()) << 48) |
      ((unsigned long long)(0xFFFF & rand()) << 32) |
      ((unsigned long long)(0xFFFF & rand()) << 16) |
      (unsigned long long)(0xFFFF & rand());
}

// Bias the random number to contain more 0 bits.
unsigned long long ZeroBitBiasedRandom() {
  return U64Rand() & U64Rand() & U64Rand();
}
// ----------------------------------------------

unsigned long long generate_magic(int index, int type, int shift, vector<unsigned long long> &attack_table) {
	vector<unsigned long long> occupancies;

	// Rook
	if(type == 1) {
		combine_occupancies(index, NORTH, occupancies);
		combine_occupancies(index, EAST, occupancies);
		combine_occupancies(index, SOUTH, occupancies);
		combine_occupancies(index, WEST, occupancies);
	}

	// Bishop
	if(type == 2) {
		combine_occupancies(index, NORTH_EAST, occupancies);
		combine_occupancies(index, SOUTH_EAST, occupancies);
		combine_occupancies(index, SOUTH_WEST, occupancies);
		combine_occupancies(index, NORTH_WEST, occupancies);
	}

	vector<unsigned long long> attacks;

	for(int i = 0; i < occupancies.size(); i++) {
		unsigned long long attack = 0;

		// Rook
		if(type == 1) {
			attack |= generate_direction_attack(index, NORTH, occupancies[i]);
			attack |= generate_direction_attack(index, EAST, occupancies[i]);
			attack |= generate_direction_attack(index, SOUTH, occupancies[i]);
			attack |= generate_direction_attack(index, WEST, occupancies[i]);
		}

		// Bishop
		if(type == 2) {
			attack |= generate_direction_attack(index, NORTH_EAST, occupancies[i]);
			attack |= generate_direction_attack(index, SOUTH_EAST, occupancies[i]);
			attack |= generate_direction_attack(index, SOUTH_WEST, occupancies[i]);
			attack |= generate_direction_attack(index, NORTH_WEST, occupancies[i]);
		}

		attacks.push_back(attack);
	}


	// ??????
  	// No bishop or rook attack can cover all squares of the board.
  	static const unsigned long long kInvalidAttack = ~0ULL;

  	// Trial and error approach to generate magics.
  	while(1) {
  		vector<unsigned long long> table(1u << shift, kInvalidAttack);
  		unsigned long long candidate_magic = ZeroBitBiasedRandom();
  		bool collision = false;

  		for(int k = 0; k < occupancies.size(); k++) {
  			unsigned long long occupancy = occupancies.at(k);
  			unsigned long long attack = attacks.at(k);
  			int offset = (occupancy * candidate_magic) >> (64 - shift);

  			if(table.at(offset) == kInvalidAttack || table.at(offset) == attack) {
  				table.at(offset) = attack;
  			} else {
  				collision = true;
  				break;
  			}
  		}

  		if(!collision) {
  			attack_table.swap(table);
  			return candidate_magic;
  			break;
  		}
  	}
}

void magit_bits(int type, int shifts[], unsigned long long magics[], int offsets[], vector<unsigned long long> &attack_table) {
	for(int i = 0; i < 64; i++) {
		vector<unsigned long long> tmp_attack_table;

		magics[i] = generate_magic(i, type, shifts[i], tmp_attack_table);
		offsets[i] = attack_table.size();
		attack_table.insert(attack_table.end(), tmp_attack_table.begin(), tmp_attack_table.end());
	}
}

void print_bit(unsigned long long a) {
	std::string buffer[8];

	// bit_board_file << "\n";
	std::cout << "\n";

	unsigned long long j = 1;
	for(int i = 0; i < 64; i++)
	{
		buffer[i/8] += (((a & j) == 0) ? "0 " : "1 ");

		// new line
		if((i+1) % 8 == 0)
		{
			buffer[i/8] += "\n";
		}

		j = j << 1;
	}

	for(int i = 7; i >= 0; i--)
		// bit_board_file << buffer[i];
		std::cout << buffer[i];

	// bit_board_file << "\n";
	std::cout << "\n";

}

void write_to_file(string name, int v[]){
	ofstream file;
	file.open(name);

	for(int i = 0; i < 64; i++) {
		file << hex << v[i] << "\n";
	}
}

int main() {

	vector<unsigned long long> occupancies;

	// combine_occupancies(C5, NORTH, occupancies);
	// combine_occupancies(C5, EAST, occupancies);
	// combine_occupancies(C5, SOUTH, occupancies);
	// combine_occupancies(C5, WEST, occupancies);

	// for(int i = 0; i < occupancies.size(); i++) {
	// 	print_bit(occupancies[i]);
	// }

	// unsigned long long tmp_board = (1ULL << F5);
	// print_bit(tmp_board);
	// print_bit(generate_direction_attack(C5, EAST, tmp_board));

	// vector<unsigned long long> tmp_attack_table;

	// cout << generate_magic(A1, 1, 12, tmp_attack_table) << "\n";

	int rook_shifts[64] = {
		12, 11, 11, 11, 11, 11, 11, 12,
    	11, 10, 10, 10, 10, 10, 10, 11,
    	11, 10, 10, 10, 10, 10, 10, 11,
    	11, 10, 10, 10, 10, 10, 10, 11,
    	11, 10, 10, 10, 10, 10, 10, 11,
    	11, 10, 10, 10, 10, 10, 10, 11,
    	11, 10, 10, 10, 10, 10, 10, 11,
    	12, 11, 11, 11, 11, 11, 11, 12,
  	};

  	// write_to_file("rook_shifts.magic", rook_shifts);

  	int bishop_shifts[64] = {
	    6, 5, 5, 5, 5, 5, 5, 6,
	    5, 5, 5, 5, 5, 5, 5, 5,
	    5, 5, 7, 7, 7, 7, 5, 5,
	  	5, 5, 7, 9, 9, 7, 5, 5,
    	5, 5, 7, 9, 9, 7, 5, 5,
    	5, 5, 7, 7, 7, 7, 5, 5,
    	5, 5, 5, 5, 5, 5, 5, 5,
    	6, 5, 5, 5, 5, 5, 5, 6,
  	};

  	unsigned long long rook_masks[64], bishop_masks[64];

  	for(int i = 0; i < 64; i++) {
    	rook_masks[i] = direction_mask(i, NORTH);
    	rook_masks[i] |= direction_mask(i, EAST);
    	rook_masks[i] |= direction_mask(i, SOUTH);
    	rook_masks[i] |= direction_mask(i, WEST);

    	bishop_masks[i] = direction_mask(i, NORTH_EAST);
    	bishop_masks[i] |= direction_mask(i, SOUTH_EAST);
    	bishop_masks[i] |= direction_mask(i, SOUTH_WEST);
    	bishop_masks[i] |= direction_mask(i, NORTH_WEST);
  	}

	unsigned long long rook_magics[64], bishop_magics[64];
	int rook_offsets[64], bishop_offsets[64];
	vector<unsigned long long> rook_attack_table, bishop_attack_table;

	magit_bits(1, rook_shifts, rook_magics, rook_offsets, rook_attack_table);
	magit_bits(2, bishop_shifts, bishop_magics, bishop_offsets, bishop_attack_table);

	return 0;
}
