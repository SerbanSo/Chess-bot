#include <iostream>
#include "defines.h"

using namespace std;

unsigned long long rook_moves[64];

int rook_direction[4] = {1, 2, 3, 4};
int rook_shift[4] = {1, -1, 8, -8};

void init_rook_moves()
{
	// Line A1-H1
	rook_moves[A1] = (1ull << A2) | (1ull << A3) | (1ull << A4) | (1ull << A5) | (1ull << A6) | (1ull << A7)
				   | (1ull << B1) | (1ull << C1) | (1ull << D1) | (1ull << E1) | (1ull << F1) | (1ull << G1);
	rook_moves[B1] = (1ull << B2) | (1ull << B3) | (1ull << B4) | (1ull << B5) | (1ull << B6) | (1ull << B7)
				   | (1ull << C1) | (1ull << D1) | (1ull << E1) | (1ull << F1) | (1ull << G1);
	rook_moves[C1] = (1ull << C2) | (1ull << C3) | (1ull << C4) | (1ull << C5) | (1ull << C6) | (1ull << C7)
				   | (1ull << B1) | (1ull << D1) | (1ull << E1) | (1ull << F1) | (1ull << G1);
	rook_moves[D1] = (1ull << D2) | (1ull << D3) | (1ull << D4) | (1ull << D5) | (1ull << D6) | (1ull << D7)
				   | (1ull << B1) | (1ull << C1) | (1ull << E1) | (1ull << F1) | (1ull << G1);
	rook_moves[E1] = (1ull << E2) | (1ull << E3) | (1ull << E4) | (1ull << E5) | (1ull << E6) | (1ull << E7)
				   | (1ull << B1) | (1ull << C1) | (1ull << D1) | (1ull << F1) | (1ull << G1);
	rook_moves[F1] = (1ull << F2) | (1ull << F3) | (1ull << F4) | (1ull << F5) | (1ull << F6) | (1ull << F7)
				   | (1ull << B1) | (1ull << C1) | (1ull << D1) | (1ull << E1) | (1ull << G1);
	rook_moves[G1] = (1ull << G2) | (1ull << G3) | (1ull << G4) | (1ull << G5) | (1ull << G6) | (1ull << G7)
				   | (1ull << B1) | (1ull << C1) | (1ull << D1) | (1ull << E1) | (1ull << F1);
	rook_moves[H1] = (1ull << H2) | (1ull << H3) | (1ull << H4) | (1ull << H5) | (1ull << H6) | (1ull << H7)
				   | (1ull << B1) | (1ull << C1) | (1ull << D1) | (1ull << E1) | (1ull << F1) | (1ull << G1);

	// Line A2-H2
	rook_moves[A2] = (1ull << A3) | (1ull << A4) | (1ull << A5) | (1ull << A6) | (1ull << A7)
				   | (1ull << B2) | (1ull << C2) | (1ull << D2) | (1ull << E2) | (1ull << F2) | (1ull << G2);
	rook_moves[B2] = (1ull << B3) | (1ull << B4) | (1ull << B5) | (1ull << B6) | (1ull << B7)
				   | (1ull << C2) | (1ull << D2) | (1ull << E2) | (1ull << F2) | (1ull << G2);
	rook_moves[C2] = (1ull << C3) | (1ull << C4) | (1ull << C5) | (1ull << C6) | (1ull << C7) 
				   | (1ull << B2) | (1ull << D2) | (1ull << E2) | (1ull << F2) | (1ull << G2);
	rook_moves[D2] = (1ull << D3) | (1ull << D4) | (1ull << D5) | (1ull << D6) | (1ull << D7)
				   | (1ull << B2) | (1ull << C2) | (1ull << E2) | (1ull << F2) | (1ull << G2);
	rook_moves[E2] = (1ull << E3) | (1ull << E4) | (1ull << E5) | (1ull << E6) | (1ull << E7)
				   | (1ull << B2) | (1ull << C2) | (1ull << D2) | (1ull << F2) | (1ull << G2);
	rook_moves[F2] = (1ull << F3) | (1ull << F4) | (1ull << F5) | (1ull << F6) | (1ull << F7)
				   | (1ull << B2) | (1ull << C2) | (1ull << D2) | (1ull << E2) | (1ull << G2);
	rook_moves[G2] = (1ull << G3) | (1ull << G4) | (1ull << G5) | (1ull << G6) | (1ull << G7)
				   | (1ull << B2) | (1ull << C2) | (1ull << D2) | (1ull << E2) | (1ull << F2);
	rook_moves[H2] = (1ull << H3) | (1ull << H4) | (1ull << H5) | (1ull << H6) | (1ull << H7)
				   | (1ull << B2) | (1ull << C2) | (1ull << D2) | (1ull << E2) | (1ull << F2) | (1ull << G2);

	// Line A3-H3
	rook_moves[A3] = (1ull << A2) | (1ull << A4) | (1ull << A5) | (1ull << A6) | (1ull << A7)
				   | (1ull << B3) | (1ull << C3) | (1ull << D3) | (1ull << E3) | (1ull << F3) | (1ull << G3);
	rook_moves[B3] = (1ull << B2) | (1ull << B4) | (1ull << B5) | (1ull << B6) | (1ull << B7)
				   | (1ull << C3) | (1ull << D3) | (1ull << E3) | (1ull << F3) | (1ull << G3);
	rook_moves[C3] = (1ull << C2) | (1ull << C4) | (1ull << C5) | (1ull << C6) | (1ull << C7)
				   | (1ull << B3) | (1ull << D3) | (1ull << E3) | (1ull << F3) | (1ull << G3);
	rook_moves[D3] = (1ull << D2) | (1ull << D4) | (1ull << D5) | (1ull << D6) | (1ull << D7)
				   | (1ull << B3) | (1ull << C3) | (1ull << E3) | (1ull << F3) | (1ull << G3);
	rook_moves[E3] = (1ull << E2) | (1ull << E4) | (1ull << E5) | (1ull << E6) | (1ull << E7)
				   | (1ull << B3) | (1ull << C3) | (1ull << D3) | (1ull << F3) | (1ull << G3);
	rook_moves[F3] = (1ull << F2) | (1ull << F4) | (1ull << F5) | (1ull << F6) | (1ull << F7)
				   | (1ull << B3) | (1ull << C3) | (1ull << D3) | (1ull << E3) | (1ull << G3);
	rook_moves[G3] = (1ull << G2) | (1ull << G4) | (1ull << G5) | (1ull << G6) | (1ull << G7)
				   | (1ull << B3) | (1ull << C3) | (1ull << D3) | (1ull << E3) | (1ull << F3);
	rook_moves[H3] = (1ull << H2) | (1ull << H4) | (1ull << H5) | (1ull << H6) | (1ull << H7)
				   | (1ull << B3) | (1ull << C3) | (1ull << D3) | (1ull << E3) | (1ull << F3) | (1ull << G3);

	// Line A4-H4
	rook_moves[A4] = (1ull << A2) | (1ull << A3) | (1ull << A5) | (1ull << A6) | (1ull << A7)
				   | (1ull << B4) | (1ull << C4) | (1ull << D4) | (1ull << E4) | (1ull << F4) | (1ull << G4);
	rook_moves[B4] = (1ull << B2) | (1ull << B3) | (1ull << B5) | (1ull << B6) | (1ull << B7)
				   | (1ull << C4) | (1ull << D4) | (1ull << E4) | (1ull << F4) | (1ull << G4);
	rook_moves[C4] = (1ull << C2) | (1ull << C3) | (1ull << C5) | (1ull << C6) | (1ull << C7)
				   | (1ull << B4) | (1ull << D4) | (1ull << E4) | (1ull << F4) | (1ull << G4);
	rook_moves[D4] = (1ull << D2) | (1ull << D3) | (1ull << D5) | (1ull << D6) | (1ull << D7)
				   | (1ull << B4) | (1ull << C4) | (1ull << E4) | (1ull << F4) | (1ull << G4);
	rook_moves[E4] = (1ull << E2) | (1ull << E3) | (1ull << E5) | (1ull << E6) | (1ull << E7)
				   | (1ull << B4) | (1ull << C4) | (1ull << D4) | (1ull << F4) | (1ull << G4);
	rook_moves[F4] = (1ull << F2) | (1ull << F3) | (1ull << F5) | (1ull << F6) | (1ull << F7)
				   | (1ull << B4) | (1ull << C4) | (1ull << D4) | (1ull << E4) | (1ull << G4);
	rook_moves[G4] = (1ull << G2) | (1ull << G3) | (1ull << G5) | (1ull << G6) | (1ull << G7)
				   | (1ull << B4) | (1ull << C4) | (1ull << D4) | (1ull << E4) | (1ull << F4);
	rook_moves[H4] = (1ull << H2) | (1ull << H3) | (1ull << H5) | (1ull << H6) | (1ull << H7)
				   | (1ull << B4) | (1ull << C4) | (1ull << D4) | (1ull << E4) | (1ull << F4) | (1ull << G4);

	// Line A5-H5
	rook_moves[A5] = (1ull << A2) | (1ull << A3) | (1ull << A4) | (1ull << A6) | (1ull << A7)
				   | (1ull << B5) | (1ull << C5) | (1ull << D5) | (1ull << E5) | (1ull << F5) | (1ull << G5);
	rook_moves[B5] = (1ull << B2) | (1ull << B3) | (1ull << B4) | (1ull << B6) | (1ull << B7)
				   | (1ull << C5) | (1ull << D5) | (1ull << E5) | (1ull << F5) | (1ull << G5);
	rook_moves[C5] = (1ull << C2) | (1ull << C3) | (1ull << C4) | (1ull << C6) | (1ull << C7)
				   | (1ull << B5) | (1ull << D5) | (1ull << E5) | (1ull << F5) | (1ull << G5);
	rook_moves[D5] = (1ull << D2) | (1ull << D3) | (1ull << D4) | (1ull << D6) | (1ull << D7)
				   | (1ull << B5) | (1ull << C5) | (1ull << E5) | (1ull << F5) | (1ull << G5);
	rook_moves[E5] = (1ull << E2) | (1ull << E3) | (1ull << E4) | (1ull << E6) | (1ull << E7)
				   | (1ull << B5) | (1ull << C5) | (1ull << D5) | (1ull << F5) | (1ull << G5);
	rook_moves[F5] = (1ull << F2) | (1ull << F3) | (1ull << F4) | (1ull << F6) | (1ull << F7)
				   | (1ull << B5) | (1ull << C5) | (1ull << D5) | (1ull << E5) | (1ull << G5);
	rook_moves[G5] = (1ull << G2) | (1ull << G3) | (1ull << G4) | (1ull << G6) | (1ull << G7)
				   | (1ull << B5) | (1ull << C5) | (1ull << D5) | (1ull << E5) | (1ull << F5);
	rook_moves[H5] = (1ull << H2) | (1ull << H3) | (1ull << H4) | (1ull << H6) | (1ull << H7)
				   | (1ull << B5) | (1ull << C5) | (1ull << D5) | (1ull << E5) | (1ull << F5) | (1ull << G5);

	// Line A6-H6
	rook_moves[A6] = (1ull << A2) | (1ull << A3) | (1ull << A4) | (1ull << A5) | (1ull << A7)
				   | (1ull << B6) | (1ull << C6) | (1ull << D6) | (1ull << E6) | (1ull << F6) | (1ull << G6);
	rook_moves[B6] = (1ull << B2) | (1ull << B3) | (1ull << B4) | (1ull << B5) | (1ull << B7)
				   | (1ull << C6) | (1ull << D6) | (1ull << E6) | (1ull << F6) | (1ull << G6);
	rook_moves[C6] = (1ull << C2) | (1ull << C3) | (1ull << C4) | (1ull << C5) | (1ull << C7)
				   | (1ull << B6) | (1ull << D6) | (1ull << E6) | (1ull << F6) | (1ull << G6);
	rook_moves[D6] = (1ull << D2) | (1ull << D3) | (1ull << D4) | (1ull << D5) | (1ull << D7)
				   | (1ull << B6) | (1ull << C6) | (1ull << E6) | (1ull << F6) | (1ull << G6);
	rook_moves[E6] = (1ull << E2) | (1ull << E3) | (1ull << E4) | (1ull << E5) | (1ull << E7)
				   | (1ull << B6) | (1ull << C6) | (1ull << D6) | (1ull << F6) | (1ull << G6);
	rook_moves[F6] = (1ull << F2) | (1ull << F3) | (1ull << F4) | (1ull << F5) | (1ull << F7)
				   | (1ull << B6) | (1ull << C6) | (1ull << D6) | (1ull << E6) | (1ull << G6);
	rook_moves[G6] = (1ull << G2) | (1ull << G3) | (1ull << G4) | (1ull << G5) | (1ull << G7)
				   | (1ull << B6) | (1ull << C6) | (1ull << D6) | (1ull << E6) | (1ull << F6);
	rook_moves[H6] = (1ull << H2) | (1ull << H3) | (1ull << H4) | (1ull << H5) | (1ull << H7)
				   | (1ull << B6) | (1ull << C6) | (1ull << D6) | (1ull << E6) | (1ull << F6) | (1ull << G6);

	// Line A7-H7
	rook_moves[A7] = (1ull << A2) | (1ull << A3) | (1ull << A4) | (1ull << A5) | (1ull << A6)
				   | (1ull << B7) | (1ull << C7) | (1ull << D7) | (1ull << E7) | (1ull << F7) | (1ull << G7);
	rook_moves[B7] = (1ull << B2) | (1ull << B3) | (1ull << B4) | (1ull << B5) | (1ull << B6)
				   | (1ull << C7) | (1ull << D7) | (1ull << E7) | (1ull << F7) | (1ull << G7);
	rook_moves[C7] = (1ull << C2) | (1ull << C3) | (1ull << C4) | (1ull << C5) | (1ull << C6)
				   | (1ull << B7) | (1ull << D7) | (1ull << E7) | (1ull << F7) | (1ull << G7);
	rook_moves[D7] = (1ull << D2) | (1ull << D3) | (1ull << D4) | (1ull << D5) | (1ull << D6)
				   | (1ull << B7) | (1ull << C7) | (1ull << E7) | (1ull << F7) | (1ull << G7);
	rook_moves[E7] = (1ull << E2) | (1ull << E3) | (1ull << E4) | (1ull << E5) | (1ull << E6)
				   | (1ull << B7) | (1ull << C7) | (1ull << D7) | (1ull << F7) | (1ull << G7);
	rook_moves[F7] = (1ull << F2) | (1ull << F3) | (1ull << F4) | (1ull << F5) | (1ull << F6)
				   | (1ull << B7) | (1ull << C7) | (1ull << D7) | (1ull << E7) | (1ull << G7);
	rook_moves[G7] = (1ull << G2) | (1ull << G3) | (1ull << G4) | (1ull << G5) | (1ull << G6)
				   | (1ull << B7) | (1ull << C7) | (1ull << D7) | (1ull << E7) | (1ull << F7);
	rook_moves[H7] = (1ull << H2) | (1ull << H3) | (1ull << H4) | (1ull << H5) | (1ull << H6)
				   | (1ull << B7) | (1ull << C7) | (1ull << D7) | (1ull << E7) | (1ull << F7) | (1ull << G7);

	// Line A8-H8
	rook_moves[A8] = (1ull << A2) | (1ull << A3) | (1ull << A4) | (1ull << A5) | (1ull << A6) | (1ull << A7) 
				   | (1ull << B8) | (1ull << C8) | (1ull << D8) | (1ull << E8) | (1ull << F8) | (1ull << G8);
	rook_moves[B8] = (1ull << B2) | (1ull << B3) | (1ull << B4) | (1ull << B5) | (1ull << B6) | (1ull << B7) 
				   | (1ull << C8) | (1ull << D8) | (1ull << E8) | (1ull << F8) | (1ull << G8);
	rook_moves[C8] = (1ull << C2) | (1ull << C3) | (1ull << C4) | (1ull << C5) | (1ull << C6) | (1ull << C7) 
				   | (1ull << B8) | (1ull << D8) | (1ull << E8) | (1ull << F8) | (1ull << G8);
	rook_moves[D8] = (1ull << D2) | (1ull << D3) | (1ull << D4) | (1ull << D5) | (1ull << D6) | (1ull << D7) 
				   | (1ull << B8) | (1ull << C8) | (1ull << E8) | (1ull << F8) | (1ull << G8);
	rook_moves[E8] = (1ull << E2) | (1ull << E3) | (1ull << E4) | (1ull << E5) | (1ull << E6) | (1ull << E7) 
				   | (1ull << B8) | (1ull << C8) | (1ull << D8) | (1ull << F8) | (1ull << G8);
	rook_moves[F8] = (1ull << F2) | (1ull << F3) | (1ull << F4) | (1ull << F5) | (1ull << F6) | (1ull << F7) 
				   | (1ull << B8) | (1ull << C8) | (1ull << D8) | (1ull << E8) | (1ull << G8);
	rook_moves[G8] = (1ull << G2) | (1ull << G3) | (1ull << G4) | (1ull << G5) | (1ull << G6) | (1ull << G7) 
				   | (1ull << B8) | (1ull << C8) | (1ull << D8) | (1ull << E8) | (1ull << F8);
	rook_moves[H8] = (1ull << H2) | (1ull << H3) | (1ull << H4) | (1ull << H5) | (1ull << H6) | (1ull << H7) 
				   | (1ull << B8) | (1ull << C8) | (1ull << D8) | (1ull << E8) | (1ull << F8) | (1ull << G8);

}

void print_bit(unsigned long long a)
{
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

void back_track() {

}

int main() {
	init_rook_moves();

	print_bit(rook_moves[C5]);

	return 0;
}