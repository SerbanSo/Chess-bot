#include "magic.h"

unsigned long long rook_attack(int index, unsigned long long bitboard) {
	bitboard &= rook_magic[index].mask;
	
	// Calculate position in table using *magic*
	int table_pos = ((bitboard * rook_magic[index].magic) >> (64 - rook_magic[index].shift)) + rook_magic[index].offset;

	return rook_table[table_pos];
}

unsigned long long bishop_attack(int index, unsigned long long bitboard) {
	bitboard &= bishop_magic[index].mask;

	// Calculate position in table using *magic*
	int table_pos = ((bitboard * bishop_magic[index].magic) >> (64 - bishop_magic[index].shift)) + bishop_magic[index].offset;

	return bishop_table[table_pos];
}

unsigned long long queen_attack(int index, unsigned long long bitboard) {
	return rook_attack(index, bitboard) | bishop_attack(index, bitboard);
}

void init_magic() {
	std::ifstream masks;
	std::ifstream magics;
	std::ifstream shifts;
	std::ifstream offsets;
	masks.open("Database/rook_masks_magic.txt");
	magics.open("Database/rook_magics_magic.txt");
	shifts.open("Database/rook_shifts_magic.txt");
	offsets.open("Database/rook_offsets_magic.txt");

	for(int i = 0; i < 64; i++) {
		masks >> std::hex >> rook_magic[i].mask;
		magics >> std::hex >> rook_magic[i].magic;
		shifts >> std::hex >> rook_magic[i].shift;
		offsets >> std::hex >> rook_magic[i].offset;
	}

	masks.close();
	magics.close();
	shifts.close();
	offsets.close();

	masks.open("Database/bishop_masks_magic.txt");
	magics.open("Database/bishop_magics_magic.txt");
	shifts.open("Database/bishop_shifts_magic.txt");
	offsets.open("Database/bishop_offsets_magic.txt");

	for(int i = 0; i < 64; i++) {
		masks >> std::hex >> bishop_magic[i].mask;
		magics >> std::hex >> bishop_magic[i].magic;
		shifts >> std::hex >> bishop_magic[i].shift;
		offsets >> std::hex >> bishop_magic[i].offset;
	}

	masks.close();
	magics.close();
	shifts.close();
	offsets.close();

	std::ifstream table;
	table.open("Database/rook_table_magic.txt");

	for(int i = 0; i < 102400; i++) {
		table >> std::hex >> rook_table[i];
	}

	table.close();

	table.open("Database/bishop_table_magic.txt");

	for(int i = 0; i < 5248; i++) {
		table >> std::hex >> bishop_table[i];
	}

	table.close();
}
