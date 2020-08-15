#include "inits.h"

void start_inits() {
	board_inits();

	init_king_moves();
	init_knight_moves();
	init_pawn_moves();

	init_magic();

	notAFile = (1ULL << A1) | (1ULL << A2) | (1ULL << A3) | (1ULL << A4)
				| (1ULL << A5) | (1ULL << A6) | (1ULL << A7) | (1ULL << A8);
	notAFile = ~notAFile;

	notHFile = (1ULL << H1) | (1ULL << H2) | (1ULL << H3) | (1ULL << H4)
				| (1ULL << H5) | (1ULL << H6) | (1ULL << H7) | (1ULL << H8);
	notHFile = ~notHFile;

	rank1File = (1ULL << A1) | (1ULL << B1) | (1ULL << C1) | (1ULL << D1)
				| (1ULL << E1) | (1ULL << F1) | (1ULL << H1) | (1ULL << G1);

	rank8File = (1ULL << A8) | (1ULL << B8) | (1ULL << C8) | (1ULL << D8)
				| (1ULL << E8) | (1ULL << F8) | (1ULL << H8) | (1ULL << G8);

	rank2File = (1ULL << A2) | (1ULL << B2) | (1ULL << C2) | (1ULL << D2)
				| (1ULL << E2) | (1ULL << F2) | (1ULL << H2) | (1ULL << G2);

	rank4File = (1ULL << A4) | (1ULL << B4) | (1ULL << C4) | (1ULL << D4)
				| (1ULL << E4) | (1ULL << F4) | (1ULL << H4) | (1ULL << G4);

	rank7File = (1ULL << A7) | (1ULL << B7) | (1ULL << C7) | (1ULL << D7)
				| (1ULL << E7) | (1ULL << F7) | (1ULL << H7) | (1ULL << G7);

	rank5File = (1ULL << A5) | (1ULL << B5) | (1ULL << C5) | (1ULL << D5)
				| (1ULL << E5) | (1ULL << F5) | (1ULL << H5) | (1ULL << G5);
}

void board_inits() {
	init_pawns();
	init_knights();
	init_bishops();
	init_rooks();
	init_queens();
	init_kings();
	init_board();

	// En-passant inits
	en_passant[0] = -1;
	en_passant[1] = -1;

	// Castling inits
	castling[0].moved_left_rook = false;
	castling[0].moved_right_rook = false;
	castling[0].moved_king = false;
	castling[0].king_pos = E1;
	castling[0].left_rook_pos = A1;
	castling[0].right_rook_pos = H1;

	castling[1].moved_left_rook = false;
	castling[1].moved_right_rook = false;
	castling[1].moved_king = false;
	castling[1].king_pos = E8;
	castling[1].left_rook_pos = A8;
	castling[1].right_rook_pos = H8;

	// Score for each player init
	board.score[0] = 8*piece_value(PAWN) + 2*piece_value(KNIGHT) + 2*piece_value(ROOK)
				+ 2*piece_value(BISHOP) + piece_value(QUEEN) + piece_value(KING);

	board.score[1] = 8*piece_value(PAWN) + 2*piece_value(KNIGHT) + 2*piece_value(ROOK)
				+ 2*piece_value(BISHOP) + piece_value(QUEEN) + piece_value(KING);

	// Move number and maximum depth inits
	move_number = 0;
	max_depth = 6;
}

void init_pawns() {
	// White pawns
	board.pawns[0] = (1ull << A2) | (1ull << B2) | (1ull << C2) | (1ull << D2) | (1ull << E2) | (1ull << F2) | (1ull << G2) | (1ull << H2);

	// Black pawns
	board.pawns[1] = (1ull << A7) | (1ull << B7) | (1ull << C7) | (1ull << D7) | (1ull << E7) | (1ull << F7) | (1ull << G7) | (1ull << H7);
}

void init_knights() {
	// White knights
	board.knights[0] = (1ull << B1) | (1ull << G1);

	// Black knights
	board.knights[1] = (1ull << B8) | (1ull << G8);
}

void init_bishops() {
	// White bishops
	board.bishops[0] = (1ull << C1) | (1ull << F1);

	// Black bishops
	board.bishops[1] = (1ull << C8) | (1ull << F8);
}

void init_rooks() {
	// White rooks
	board.rooks[0] = (1ull << A1) | (1ull << H1);

	// Black rooks
	board.rooks[1] = (1ull << A8) | (1ull << H8);
}

void init_queens() {
	// White queen
	board.queens[0] = (1ull << D1);

	// Black queen
	board.queens[1] = (1ull << D8);
}

void init_kings() {
	// White king
	board.king[0] = (1ull << E1);

	// Black king
	board.king[1] = (1ull << E8);
}

void init_board() {
	// White board
	board.board[0] = board.pawns[0];
	board.board[0] |= board.knights[0];
	board.board[0] |= board.bishops[0];
	board.board[0] |= board.rooks[0];
	board.board[0] |= board.queens[0];
	board.board[0] |= board.king[0];

	// Black board
	board.board[1] = board.pawns[1];
	board.board[1] |= board.knights[1];
	board.board[1] |= board.bishops[1];
	board.board[1] |= board.rooks[1];
	board.board[1] |= board.queens[1];
	board.board[1] |= board.king[1];
}

void init_king_moves()
{
	// Line A1-H1
	king_moves[A1] = (1ull << A2) | (1ull << B2) | (1ull << B1);
	king_moves[B1] = (1ull << A1) | (1ull << A2) | (1ull << B2) | (1ull << C2) | (1ull << C1);
	king_moves[C1] = (1ull << B1) | (1ull << B2) | (1ull << C2) | (1ull << D2) | (1ull << D1);
	king_moves[D1] = (1ull << C1) | (1ull << C2) | (1ull << D2) | (1ull << E2) | (1ull << E1);
	king_moves[E1] = (1ull << D1) | (1ull << D2) | (1ull << E2) | (1ull << F2) | (1ull << F1);
	king_moves[F1] = (1ull << E1) | (1ull << E2) | (1ull << F2) | (1ull << G2) | (1ull << G1);
	king_moves[G1] = (1ull << F1) | (1ull << F2) | (1ull << G2) | (1ull << H2) | (1ull << H1);
	king_moves[H1] = (1ull << G1) | (1ull << G2) | (1ull << H2);

	// Line A2-H2
	king_moves[A2] = (1ull << A3) | (1ull << B3) | (1ull << B2) | (1ull << B1) | (1ull << A1);
	king_moves[B2] = (1ull << A2) | (1ull << A3) | (1ull << B3) | (1ull << C3) | (1ull << C2) | (1ull << C1) | (1ull << B1) | (1ull << A1);
	king_moves[C2] = (1ull << B2) | (1ull << B3) | (1ull << C3) | (1ull << D3) | (1ull << D2) | (1ull << D1) | (1ull << C1) | (1ull << B1);
	king_moves[D2] = (1ull << C2) | (1ull << C3) | (1ull << D3) | (1ull << E3) | (1ull << E2) | (1ull << E1) | (1ull << D1) | (1ull << C1);
	king_moves[E2] = (1ull << D2) | (1ull << D3) | (1ull << E3) | (1ull << F3) | (1ull << F2) | (1ull << F1) | (1ull << E1) | (1ull << D1);
	king_moves[F2] = (1ull << E2) | (1ull << E3) | (1ull << F3) | (1ull << G3) | (1ull << G2) | (1ull << G1) | (1ull << F1) | (1ull << E1);
	king_moves[G2] = (1ull << F2) | (1ull << F3) | (1ull << G3) | (1ull << H3) | (1ull << H2) | (1ull << H1) | (1ull << G1) | (1ull << F1);
	king_moves[H2] = (1ull << G2) | (1ull << G3) | (1ull << H3) | (1ull << H1) | (1ull << G1);

	// Line A3-H3
	king_moves[A3] = (1ull << A4) | (1ull << B4) | (1ull << B3) | (1ull << B2) | (1ull << A2);
	king_moves[B3] = (1ull << A3) | (1ull << A4) | (1ull << B4) | (1ull << C4) | (1ull << C3) | (1ull << C2) | (1ull << B2) | (1ull << A2);
	king_moves[C3] = (1ull << B3) | (1ull << B4) | (1ull << C4) | (1ull << D4) | (1ull << D3) | (1ull << D2) | (1ull << C2) | (1ull << B2);
	king_moves[D3] = (1ull << C3) | (1ull << C4) | (1ull << D4) | (1ull << E4) | (1ull << E3) | (1ull << E2) | (1ull << D2) | (1ull << C2);
	king_moves[E3] = (1ull << D3) | (1ull << D4) | (1ull << E4) | (1ull << F4) | (1ull << F3) | (1ull << F2) | (1ull << E2) | (1ull << D2);
	king_moves[F3] = (1ull << E3) | (1ull << E4) | (1ull << F4) | (1ull << G4) | (1ull << G3) | (1ull << G2) | (1ull << F2) | (1ull << E2);
	king_moves[G3] = (1ull << F3) | (1ull << F4) | (1ull << G4) | (1ull << H4) | (1ull << H3) | (1ull << H2) | (1ull << G2) | (1ull << F2);
	king_moves[H3] = (1ull << G3) | (1ull << G4) | (1ull << H4) | (1ull << H2) | (1ull << G2);

	// Line A4-H4
	king_moves[A4] = (1ull << A5) | (1ull << B5) | (1ull << B4) | (1ull << B3) | (1ull << A3);
	king_moves[B4] = (1ull << A4) | (1ull << A5) | (1ull << B5) | (1ull << C5) | (1ull << C4) | (1ull << C3) | (1ull << B3) | (1ull << A3);
	king_moves[C4] = (1ull << B4) | (1ull << B5) | (1ull << C5) | (1ull << D5) | (1ull << D4) | (1ull << D3) | (1ull << C3) | (1ull << B3);
	king_moves[D4] = (1ull << C4) | (1ull << C5) | (1ull << D5) | (1ull << E5) | (1ull << E4) | (1ull << E3) | (1ull << D3) | (1ull << C3);
	king_moves[E4] = (1ull << D4) | (1ull << D5) | (1ull << E5) | (1ull << F5) | (1ull << F4) | (1ull << F3) | (1ull << E3) | (1ull << D3);
	king_moves[F4] = (1ull << E4) | (1ull << E5) | (1ull << F5) | (1ull << G5) | (1ull << G4) | (1ull << G3) | (1ull << F3) | (1ull << E3);
	king_moves[G4] = (1ull << F4) | (1ull << F5) | (1ull << G5) | (1ull << H5) | (1ull << H4) | (1ull << H3) | (1ull << G3) | (1ull << F3);
	king_moves[H4] = (1ull << G4) | (1ull << G5) | (1ull << H5) | (1ull << H3) | (1ull << G3);

	// Line A5-H5
	king_moves[A5] = (1ull << A6) | (1ull << B6) | (1ull << B5) | (1ull << B4) | (1ull << A4);
	king_moves[B5] = (1ull << A5) | (1ull << A6) | (1ull << B6) | (1ull << C6) | (1ull << C5) | (1ull << C4) | (1ull << B4) | (1ull << A4);
	king_moves[C5] = (1ull << B5) | (1ull << B6) | (1ull << C6) | (1ull << D6) | (1ull << D5) | (1ull << D4) | (1ull << C4) | (1ull << B4);
	king_moves[D5] = (1ull << C5) | (1ull << C6) | (1ull << D6) | (1ull << E6) | (1ull << E5) | (1ull << E4) | (1ull << D4) | (1ull << C4);
	king_moves[E5] = (1ull << D5) | (1ull << D6) | (1ull << E6) | (1ull << F6) | (1ull << F5) | (1ull << F4) | (1ull << E4) | (1ull << D4);
	king_moves[F5] = (1ull << E5) | (1ull << E6) | (1ull << F6) | (1ull << G6) | (1ull << G5) | (1ull << G4) | (1ull << F4) | (1ull << E4);
	king_moves[G5] = (1ull << F5) | (1ull << F6) | (1ull << G6) | (1ull << H6) | (1ull << H5) | (1ull << H4) | (1ull << G4) | (1ull << F4);
	king_moves[H5] = (1ull << G5) | (1ull << G6) | (1ull << H6) | (1ull << H4) | (1ull << G4);

	// Line A6-H6
	king_moves[A6] = (1ull << A7) | (1ull << B7) | (1ull << B6) | (1ull << B5) | (1ull << A5);
	king_moves[B6] = (1ull << A6) | (1ull << A7) | (1ull << B7) | (1ull << C7) | (1ull << C6) | (1ull << C5) | (1ull << B5) | (1ull << A5);
	king_moves[C6] = (1ull << B6) | (1ull << B7) | (1ull << C7) | (1ull << D7) | (1ull << D6) | (1ull << D5) | (1ull << C5) | (1ull << B5);
	king_moves[D6] = (1ull << C6) | (1ull << C7) | (1ull << D7) | (1ull << E7) | (1ull << E6) | (1ull << E5) | (1ull << D5) | (1ull << C5);
	king_moves[E6] = (1ull << D6) | (1ull << D7) | (1ull << E7) | (1ull << F7) | (1ull << F6) | (1ull << F5) | (1ull << E5) | (1ull << D5);
	king_moves[F6] = (1ull << E6) | (1ull << E7) | (1ull << F7) | (1ull << G7) | (1ull << G6) | (1ull << G5) | (1ull << F5) | (1ull << E5);
	king_moves[G6] = (1ull << F6) | (1ull << F7) | (1ull << G7) | (1ull << H7) | (1ull << H6) | (1ull << H5) | (1ull << G5) | (1ull << F5);
	king_moves[H6] = (1ull << G6) | (1ull << G7) | (1ull << H7) | (1ull << H5) | (1ull << G5);

	// Line A7-H7
	king_moves[A7] = (1ull << A8) | (1ull << B8) | (1ull << B7) | (1ull << B6) | (1ull << A6);
	king_moves[B7] = (1ull << A7) | (1ull << A8) | (1ull << B8) | (1ull << C8) | (1ull << C7) | (1ull << C6) | (1ull << B6) | (1ull << A6);
	king_moves[C7] = (1ull << B7) | (1ull << B8) | (1ull << C8) | (1ull << D8) | (1ull << D7) | (1ull << D6) | (1ull << C6) | (1ull << B6);
	king_moves[D7] = (1ull << C7) | (1ull << C8) | (1ull << D8) | (1ull << E8) | (1ull << E7) | (1ull << E6) | (1ull << D6) | (1ull << C6);
	king_moves[E7] = (1ull << D7) | (1ull << D8) | (1ull << E8) | (1ull << F8) | (1ull << F7) | (1ull << F6) | (1ull << E6) | (1ull << D6);
	king_moves[F7] = (1ull << E7) | (1ull << E8) | (1ull << F8) | (1ull << G8) | (1ull << G7) | (1ull << G6) | (1ull << F6) | (1ull << E6);
	king_moves[G7] = (1ull << F7) | (1ull << F8) | (1ull << G8) | (1ull << H8) | (1ull << H7) | (1ull << H6) | (1ull << G6) | (1ull << F6);
	king_moves[H7] = (1ull << G7) | (1ull << G8) | (1ull << H8) | (1ull << H6) | (1ull << G6);

	// Line A8-H8
	king_moves[A8] = (1ull << B8) | (1ull << B7) | (1ull << A7);
	king_moves[B8] = (1ull << A8) | (1ull << C8) | (1ull << C7) | (1ull << B7) | (1ull << A7);
	king_moves[C8] = (1ull << B8) | (1ull << D8) | (1ull << D7) | (1ull << C7) | (1ull << B7);
	king_moves[D8] = (1ull << C8) | (1ull << E8) | (1ull << E7) | (1ull << D7) | (1ull << C7);
	king_moves[E8] = (1ull << D8) | (1ull << F8) | (1ull << F7) | (1ull << E7) | (1ull << D7);
	king_moves[F8] = (1ull << E8) | (1ull << G8) | (1ull << G7) | (1ull << F7) | (1ull << E7);
	king_moves[G8] = (1ull << F8) | (1ull << H8) | (1ull << H7) | (1ull << G7) | (1ull << F7);
	king_moves[H8] = (1ull << G8) | (1ull << H7) | (1ull << G7);
}

void init_knight_moves()
{
	// Line A1-H1
	knight_moves[A1] = (1ull << B3) | (1ull << C2);
	knight_moves[B1] = (1ull << A3) | (1ull << C3) | (1ull << D2);
	knight_moves[C1] = (1ull << A2) | (1ull << B3) | (1ull << D3) | (1ull << E2);
	knight_moves[D1] = (1ull << B2) | (1ull << C3) | (1ull << E3) | (1ull << F2);
	knight_moves[E1] = (1ull << C2) | (1ull << D3) | (1ull << F3) | (1ull << G2);
	knight_moves[F1] = (1ull << D2) | (1ull << E3) | (1ull << G3) | (1ull << H2);
	knight_moves[G1] = (1ull << E2) | (1ull << F3) | (1ull << H3);
	knight_moves[H1] = (1ull << F2) | (1ull << G3);

	// Line A2-H2
	knight_moves[A2] = (1ull << B4) | (1ull << C3) | (1ull << C1);
	knight_moves[B2] = (1ull << A4) | (1ull << C4) | (1ull << D3) | (1ull << D1);
	knight_moves[C2] = (1ull << A3) | (1ull << B4) | (1ull << D4) | (1ull << E3) | (1ull << E1) | (1ull << A1);
	knight_moves[D2] = (1ull << B3) | (1ull << C4) | (1ull << E4) | (1ull << F3) | (1ull << F1) | (1ull << B1);
	knight_moves[E2] = (1ull << C3) | (1ull << D4) | (1ull << F4) | (1ull << G3) | (1ull << G1) | (1ull << C1);
	knight_moves[F2] = (1ull << D3) | (1ull << E4) | (1ull << G4) | (1ull << H3) | (1ull << H1) | (1ull << D1);
	knight_moves[G2] = (1ull << E3) | (1ull << F4) | (1ull << H4) | (1ull << E1);
	knight_moves[H2] = (1ull << F3) | (1ull << G4) | (1ull << F1);

	// Line A3-H3
	knight_moves[A3] = (1ull << B5) | (1ull << C4) | (1ull << C2) | (1ull << B1);
	knight_moves[B3] = (1ull << A5) | (1ull << C5) | (1ull << D4) | (1ull << D2) | (1ull << C1) | (1ull << A1);
	knight_moves[C3] = (1ull << A4) | (1ull << B5) | (1ull << D5) | (1ull << E4) | (1ull << E2) | (1ull << D1) | (1ull << B1) | (1ull << A2);
	knight_moves[D3] = (1ull << B4) | (1ull << C5) | (1ull << E5) | (1ull << F4) | (1ull << F2) | (1ull << E1) | (1ull << C1) | (1ull << B2);
	knight_moves[E3] = (1ull << C4) | (1ull << D5) | (1ull << F5) | (1ull << G4) | (1ull << G2) | (1ull << F1) | (1ull << D1) | (1ull << C2);
	knight_moves[F3] = (1ull << D4) | (1ull << E5) | (1ull << G5) | (1ull << H4) | (1ull << H2) | (1ull << G1) | (1ull << E1) | (1ull << D2);
	knight_moves[G3] = (1ull << E4) | (1ull << F5) | (1ull << H5) | (1ull << H1) | (1ull << F1) | (1ull << E2);
	knight_moves[H3] = (1ull << F4) | (1ull << G5) | (1ull << G1) | (1ull << F2);

	// Line A4-H4
	knight_moves[A4] = (1ull << B6) | (1ull << C5) | (1ull << C3) | (1ull << B2);
	knight_moves[B4] = (1ull << A6) | (1ull << C6) | (1ull << D5) | (1ull << D3) | (1ull << C2) | (1ull << A2);
	knight_moves[C4] = (1ull << A5) | (1ull << B6) | (1ull << D6) | (1ull << E5) | (1ull << E3) | (1ull << D2) | (1ull << B2) | (1ull << A3);
	knight_moves[D4] = (1ull << B5) | (1ull << C6) | (1ull << E6) | (1ull << F5) | (1ull << F3) | (1ull << E2) | (1ull << C2) | (1ull << B3);
	knight_moves[E4] = (1ull << C5) | (1ull << D6) | (1ull << F6) | (1ull << G5) | (1ull << G3) | (1ull << F2) | (1ull << D2) | (1ull << C3);
	knight_moves[F4] = (1ull << D5) | (1ull << E6) | (1ull << G6) | (1ull << H5) | (1ull << H3) | (1ull << G2) | (1ull << E2) | (1ull << D3);
	knight_moves[G4] = (1ull << E5) | (1ull << F6) | (1ull << H6) | (1ull << H2) | (1ull << F2) | (1ull << E3);
	knight_moves[H4] = (1ull << F5) | (1ull << G6) | (1ull << G2) | (1ull << F3);

	// Line A5-H5
	knight_moves[A5] = (1ull << B7) | (1ull << C6) | (1ull << C4) | (1ull << B3);
	knight_moves[B5] = (1ull << A7) | (1ull << C7) | (1ull << D6) | (1ull << D4) | (1ull << C3) | (1ull << A3);
	knight_moves[C5] = (1ull << A6) | (1ull << B7) | (1ull << D7) | (1ull << E6) | (1ull << E4) | (1ull << D3) | (1ull << B3) | (1ull << A4);
	knight_moves[D5] = (1ull << B6) | (1ull << C7) | (1ull << E7) | (1ull << F6) | (1ull << F4) | (1ull << E3) | (1ull << C3) | (1ull << B4);
	knight_moves[E5] = (1ull << C6) | (1ull << D7) | (1ull << F7) | (1ull << G6) | (1ull << G4) | (1ull << F3) | (1ull << D3) | (1ull << C4);
	knight_moves[F5] = (1ull << D6) | (1ull << E7) | (1ull << G7) | (1ull << H6) | (1ull << H4) | (1ull << G3) | (1ull << E3) | (1ull << D4);
	knight_moves[G5] = (1ull << E6) | (1ull << F7) | (1ull << H7) | (1ull << H3) | (1ull << F3) | (1ull << E4);
	knight_moves[H5] = (1ull << F6) | (1ull << G7) | (1ull << G3) | (1ull << F4);

	// Line A6-H6
	knight_moves[A6] = (1ull << B8) | (1ull << C7) | (1ull << C5) | (1ull << B4);
	knight_moves[B6] = (1ull << A8) | (1ull << C8) | (1ull << D7) | (1ull << D5) | (1ull << C4) | (1ull << A4);
	knight_moves[C6] = (1ull << A7) | (1ull << B8) | (1ull << D8) | (1ull << E7) | (1ull << E5) | (1ull << D4) | (1ull << B4) | (1ull << A5);
	knight_moves[D6] = (1ull << B7) | (1ull << C8) | (1ull << E8) | (1ull << F7) | (1ull << F5) | (1ull << E4) | (1ull << C4) | (1ull << B5);
	knight_moves[E6] = (1ull << C7) | (1ull << D8) | (1ull << F8) | (1ull << G7) | (1ull << G5) | (1ull << F4) | (1ull << D4) | (1ull << C5);
	knight_moves[F6] = (1ull << D7) | (1ull << E8) | (1ull << G8) | (1ull << H7) | (1ull << H5) | (1ull << G4) | (1ull << E4) | (1ull << D5);
	knight_moves[G6] = (1ull << E7) | (1ull << F8) | (1ull << H8) | (1ull << H4) | (1ull << F4) | (1ull << E5);
	knight_moves[H6] = (1ull << F7) | (1ull << G8) | (1ull << G4) | (1ull << F5);

	// Line A7-H7
	knight_moves[A7] = (1ull << C8) | (1ull << C6) | (1ull << B5);
	knight_moves[B7] = (1ull << D8) | (1ull << D6) | (1ull << C5) | (1ull << A5);
	knight_moves[C7] = (1ull << A8) | (1ull << E8) | (1ull << E6) | (1ull << D5) | (1ull << B5) | (1ull << A6);
	knight_moves[D7] = (1ull << B8) | (1ull << F8) | (1ull << F6) | (1ull << E5) | (1ull << C5) | (1ull << B6);
	knight_moves[E7] = (1ull << C8) | (1ull << G8) | (1ull << G6) | (1ull << F5) | (1ull << D5) | (1ull << C6);
	knight_moves[F7] = (1ull << D8) | (1ull << H8) | (1ull << H6) | (1ull << G5) | (1ull << E5) | (1ull << D6);
	knight_moves[G7] = (1ull << E8) | (1ull << H5) | (1ull << F5) | (1ull << E6);
	knight_moves[H7] = (1ull << F8) | (1ull << G5) | (1ull << F6);

	// Line A8-H8
	knight_moves[A8] = (1ull << C7) | (1ull << B6);
	knight_moves[B8] = (1ull << D7) | (1ull << C6) | (1ull << A6);
	knight_moves[C8] = (1ull << E7) | (1ull << D6) | (1ull << B6) | (1ull << A7);
	knight_moves[D8] = (1ull << F7) | (1ull << E6) | (1ull << C6) | (1ull << B7);
	knight_moves[E8] = (1ull << G7) | (1ull << F6) | (1ull << D6) | (1ull << C7);
	knight_moves[F8] = (1ull << H7) | (1ull << G6) | (1ull << E6) | (1ull << D7);
	knight_moves[G8] = (1ull << H6) | (1ull << F6) | (1ull << E7);
	knight_moves[H8] = (1ull << G6) | (1ull << F7);
}

void init_pawn_moves() {
	// For white pawns
	// Can't be on A1-H1 line
	// Can't move from A8-H8 line

	// Line A2-H2
	pawn_moves[WHITE][A2] = (1ULL << A3) | (1ULL << A4);
	pawn_moves[WHITE][B2] = (1ULL << B3) | (1ULL << B4);
	pawn_moves[WHITE][C2] = (1ULL << C3) | (1ULL << C4);
	pawn_moves[WHITE][D2] = (1ULL << D3) | (1ULL << D4);
	pawn_moves[WHITE][E2] = (1ULL << E3) | (1ULL << E4);
	pawn_moves[WHITE][F2] = (1ULL << F3) | (1ULL << F4);
	pawn_moves[WHITE][G2] = (1ULL << G3) | (1ULL << G4);
	pawn_moves[WHITE][H2] = (1ULL << H3) | (1ULL << H4);
	pawn_attacks[WHITE][A2] = (1ULL << B3);
	pawn_attacks[WHITE][B2] = (1ULL << A3) | (1ULL << C3);
	pawn_attacks[WHITE][C2] = (1ULL << B3) | (1ULL << D3);
	pawn_attacks[WHITE][D2] = (1ULL << C3) | (1ULL << E3);
	pawn_attacks[WHITE][E2] = (1ULL << D3) | (1ULL << F3);
	pawn_attacks[WHITE][F2] = (1ULL << E3) | (1ULL << G3);
	pawn_attacks[WHITE][G2] = (1ULL << F3) | (1ULL << H3);
	pawn_attacks[WHITE][H2] = (1ULL << G3);

	// Line A3-H3
	pawn_moves[WHITE][A3] = (1ULL << A4);
	pawn_moves[WHITE][B3] = (1ULL << B4);
	pawn_moves[WHITE][C3] = (1ULL << C4);
	pawn_moves[WHITE][D3] = (1ULL << D4);
	pawn_moves[WHITE][E3] = (1ULL << E4);
	pawn_moves[WHITE][F3] = (1ULL << F4);
	pawn_moves[WHITE][G3] = (1ULL << G4);
	pawn_moves[WHITE][H3] = (1ULL << H4);
	pawn_attacks[WHITE][A3] = (1ULL << B4);
	pawn_attacks[WHITE][B3] = (1ULL << A4) | (1ULL << C4);
	pawn_attacks[WHITE][C3] = (1ULL << B4) | (1ULL << D4);
	pawn_attacks[WHITE][D3] = (1ULL << C4) | (1ULL << E4);
	pawn_attacks[WHITE][E3] = (1ULL << D4) | (1ULL << F4);
	pawn_attacks[WHITE][F3] = (1ULL << E4) | (1ULL << G4);
	pawn_attacks[WHITE][G3] = (1ULL << F4) | (1ULL << H4);
	pawn_attacks[WHITE][H3] = (1ULL << G4);

	// Line A4-H4
	pawn_moves[WHITE][A4] = (1ULL << A5);
	pawn_moves[WHITE][B4] = (1ULL << B5);
	pawn_moves[WHITE][C4] = (1ULL << C5);
	pawn_moves[WHITE][D4] = (1ULL << D5);
	pawn_moves[WHITE][E4] = (1ULL << E5);
	pawn_moves[WHITE][F4] = (1ULL << F5);
	pawn_moves[WHITE][G4] = (1ULL << G5);
	pawn_moves[WHITE][H4] = (1ULL << H5);
	pawn_attacks[WHITE][A4] = (1ULL << B5);
	pawn_attacks[WHITE][B4] = (1ULL << A5) | (1ULL << C5);
	pawn_attacks[WHITE][C4] = (1ULL << B5) | (1ULL << D5);
	pawn_attacks[WHITE][D4] = (1ULL << C5) | (1ULL << E5);
	pawn_attacks[WHITE][E4] = (1ULL << D5) | (1ULL << F5);
	pawn_attacks[WHITE][F4] = (1ULL << E5) | (1ULL << G5);
	pawn_attacks[WHITE][G4] = (1ULL << F5) | (1ULL << H5);
	pawn_attacks[WHITE][H4] = (1ULL << G5);

	// Line A5-H5
	pawn_moves[WHITE][A5] = (1ULL << A6);
	pawn_moves[WHITE][B5] = (1ULL << B6);
	pawn_moves[WHITE][C5] = (1ULL << C6);
	pawn_moves[WHITE][D5] = (1ULL << D6);
	pawn_moves[WHITE][E5] = (1ULL << E6);
	pawn_moves[WHITE][F5] = (1ULL << F6);
	pawn_moves[WHITE][G5] = (1ULL << G6);
	pawn_moves[WHITE][H5] = (1ULL << H6);
	pawn_attacks[WHITE][A5] = (1ULL << B6);
	pawn_attacks[WHITE][B5] = (1ULL << A6) | (1ULL << C6);
	pawn_attacks[WHITE][C5] = (1ULL << B6) | (1ULL << D6);
	pawn_attacks[WHITE][D5] = (1ULL << C6) | (1ULL << E6);
	pawn_attacks[WHITE][E5] = (1ULL << D6) | (1ULL << F6);
	pawn_attacks[WHITE][F5] = (1ULL << E6) | (1ULL << G6);
	pawn_attacks[WHITE][G5] = (1ULL << F6) | (1ULL << H6);
	pawn_attacks[WHITE][H5] = (1ULL << G6);

	// Line A6-H6
	pawn_moves[WHITE][A6] = (1ULL << A7);
	pawn_moves[WHITE][B6] = (1ULL << B7);
	pawn_moves[WHITE][C6] = (1ULL << C7);
	pawn_moves[WHITE][D6] = (1ULL << D7);
	pawn_moves[WHITE][E6] = (1ULL << E7);
	pawn_moves[WHITE][F6] = (1ULL << F7);
	pawn_moves[WHITE][G6] = (1ULL << G7);
	pawn_moves[WHITE][H6] = (1ULL << H7);
	pawn_attacks[WHITE][A6] = (1ULL << B7);
	pawn_attacks[WHITE][B6] = (1ULL << A7) | (1ULL << C7);
	pawn_attacks[WHITE][C6] = (1ULL << B7) | (1ULL << D7);
	pawn_attacks[WHITE][D6] = (1ULL << C7) | (1ULL << E7);
	pawn_attacks[WHITE][E6] = (1ULL << D7) | (1ULL << F7);
	pawn_attacks[WHITE][F6] = (1ULL << E7) | (1ULL << G7);
	pawn_attacks[WHITE][G6] = (1ULL << F7) | (1ULL << H7);
	pawn_attacks[WHITE][H6] = (1ULL << G7);

	// Line A7-H7
	pawn_moves[WHITE][A7] = (1ULL << A8);
	pawn_moves[WHITE][B7] = (1ULL << B8);
	pawn_moves[WHITE][C7] = (1ULL << C8);
	pawn_moves[WHITE][D7] = (1ULL << D8);
	pawn_moves[WHITE][E7] = (1ULL << E8);
	pawn_moves[WHITE][F7] = (1ULL << F8);
	pawn_moves[WHITE][G7] = (1ULL << G8);
	pawn_moves[WHITE][H7] = (1ULL << H8);
	pawn_attacks[WHITE][A7] = (1ULL << B8);
	pawn_attacks[WHITE][B7] = (1ULL << A8) | (1ULL << C8);
	pawn_attacks[WHITE][C7] = (1ULL << B8) | (1ULL << D8);
	pawn_attacks[WHITE][D7] = (1ULL << C8) | (1ULL << E8);
	pawn_attacks[WHITE][E7] = (1ULL << D8) | (1ULL << F8);
	pawn_attacks[WHITE][F7] = (1ULL << E8) | (1ULL << G8);
	pawn_attacks[WHITE][G7] = (1ULL << F8) | (1ULL << H8);
	pawn_attacks[WHITE][H7] = (1ULL << G8);



	// For black pawns
	// Can't be on A8-H8 line
	// Can't move from A1-H1 line

	// Line A7-H7
	pawn_moves[BLACK][A7] = (1ULL << A6) | (1ULL << A5);
	pawn_moves[BLACK][B7] = (1ULL << B6) | (1ULL << B5);
	pawn_moves[BLACK][C7] = (1ULL << C6) | (1ULL << C5);
	pawn_moves[BLACK][D7] = (1ULL << D6) | (1ULL << D5);
	pawn_moves[BLACK][E7] = (1ULL << E6) | (1ULL << E5);
	pawn_moves[BLACK][F7] = (1ULL << F6) | (1ULL << F5);
	pawn_moves[BLACK][G7] = (1ULL << G6) | (1ULL << G5);
	pawn_moves[BLACK][H7] = (1ULL << H6) | (1ULL << H5);
	pawn_attacks[BLACK][A7] = (1ULL << B6);
	pawn_attacks[BLACK][B7] = (1ULL << A6) | (1ULL << C6);
	pawn_attacks[BLACK][C7] = (1ULL << B6) | (1ULL << D6);
	pawn_attacks[BLACK][D7] = (1ULL << C6) | (1ULL << E6);
	pawn_attacks[BLACK][E7] = (1ULL << D6) | (1ULL << F6);
	pawn_attacks[BLACK][F7] = (1ULL << E6) | (1ULL << G6);
	pawn_attacks[BLACK][G7] = (1ULL << F6) | (1ULL << H6);
	pawn_attacks[BLACK][H7] = (1ULL << G6);

	// Line A6-H6
	pawn_moves[BLACK][A6] = (1ULL << A5);
	pawn_moves[BLACK][B6] = (1ULL << B5);
	pawn_moves[BLACK][C6] = (1ULL << C5);
	pawn_moves[BLACK][D6] = (1ULL << D5);
	pawn_moves[BLACK][E6] = (1ULL << E5);
	pawn_moves[BLACK][F6] = (1ULL << F5);
	pawn_moves[BLACK][G6] = (1ULL << G5);
	pawn_moves[BLACK][H6] = (1ULL << H5);
	pawn_attacks[BLACK][A6] = (1ULL << B5);
	pawn_attacks[BLACK][B6] = (1ULL << A5) | (1ULL << C5);
	pawn_attacks[BLACK][C6] = (1ULL << B5) | (1ULL << D5);
	pawn_attacks[BLACK][D6] = (1ULL << C5) | (1ULL << E5);
	pawn_attacks[BLACK][E6] = (1ULL << D5) | (1ULL << F5);
	pawn_attacks[BLACK][F6] = (1ULL << E5) | (1ULL << G5);
	pawn_attacks[BLACK][G6] = (1ULL << F5) | (1ULL << H5);
	pawn_attacks[BLACK][H6] = (1ULL << G5);

	// Line A5-H5
	pawn_moves[BLACK][A5] = (1ULL << A4);
	pawn_moves[BLACK][B5] = (1ULL << B4);
	pawn_moves[BLACK][C5] = (1ULL << C4);
	pawn_moves[BLACK][D5] = (1ULL << D4);
	pawn_moves[BLACK][E5] = (1ULL << E4);
	pawn_moves[BLACK][F5] = (1ULL << F4);
	pawn_moves[BLACK][G5] = (1ULL << G4);
	pawn_moves[BLACK][H5] = (1ULL << H4);
	pawn_attacks[BLACK][A5] = (1ULL << B4);
	pawn_attacks[BLACK][B5] = (1ULL << A4) | (1ULL << C4);
	pawn_attacks[BLACK][C5] = (1ULL << B4) | (1ULL << D4);
	pawn_attacks[BLACK][D5] = (1ULL << C4) | (1ULL << E4);
	pawn_attacks[BLACK][E5] = (1ULL << D4) | (1ULL << F4);
	pawn_attacks[BLACK][F5] = (1ULL << E4) | (1ULL << G4);
	pawn_attacks[BLACK][G5] = (1ULL << F4) | (1ULL << H4);
	pawn_attacks[BLACK][H5] = (1ULL << G4);

	// Line A4-H4
	pawn_moves[BLACK][A4] = (1ULL << A3);
	pawn_moves[BLACK][B4] = (1ULL << B3);
	pawn_moves[BLACK][C4] = (1ULL << C3);
	pawn_moves[BLACK][D4] = (1ULL << D3);
	pawn_moves[BLACK][E4] = (1ULL << E3);
	pawn_moves[BLACK][F4] = (1ULL << F3);
	pawn_moves[BLACK][G4] = (1ULL << G3);
	pawn_moves[BLACK][H4] = (1ULL << H3);
	pawn_attacks[BLACK][A4] = (1ULL << B3);
	pawn_attacks[BLACK][B4] = (1ULL << A3) | (1ULL << C3);
	pawn_attacks[BLACK][C4] = (1ULL << B3) | (1ULL << D3);
	pawn_attacks[BLACK][D4] = (1ULL << C3) | (1ULL << E3);
	pawn_attacks[BLACK][E4] = (1ULL << D3) | (1ULL << F3);
	pawn_attacks[BLACK][F4] = (1ULL << E3) | (1ULL << G3);
	pawn_attacks[BLACK][G4] = (1ULL << F3) | (1ULL << H3);
	pawn_attacks[BLACK][H4] = (1ULL << G3);

	// Line A3-H3
	pawn_moves[BLACK][A3] = (1ULL << A2);
	pawn_moves[BLACK][B3] = (1ULL << B2);
	pawn_moves[BLACK][C3] = (1ULL << C2);
	pawn_moves[BLACK][D3] = (1ULL << D2);
	pawn_moves[BLACK][E3] = (1ULL << E2);
	pawn_moves[BLACK][F3] = (1ULL << F2);
	pawn_moves[BLACK][G3] = (1ULL << G2);
	pawn_moves[BLACK][H3] = (1ULL << H2);
	pawn_attacks[BLACK][A3] = (1ULL << B2);
	pawn_attacks[BLACK][B3] = (1ULL << A2) | (1ULL << C2);
	pawn_attacks[BLACK][C3] = (1ULL << B2) | (1ULL << D2);
	pawn_attacks[BLACK][D3] = (1ULL << C2) | (1ULL << E2);
	pawn_attacks[BLACK][E3] = (1ULL << D2) | (1ULL << F2);
	pawn_attacks[BLACK][F3] = (1ULL << E2) | (1ULL << G2);
	pawn_attacks[BLACK][G3] = (1ULL << F2) | (1ULL << H2);
	pawn_attacks[BLACK][H3] = (1ULL << G2);

	// Line A2-H2
	pawn_moves[BLACK][A2] = (1ULL << A1);
	pawn_moves[BLACK][B2] = (1ULL << B1);
	pawn_moves[BLACK][C2] = (1ULL << C1);
	pawn_moves[BLACK][D2] = (1ULL << D1);
	pawn_moves[BLACK][E2] = (1ULL << E1);
	pawn_moves[BLACK][F2] = (1ULL << F1);
	pawn_moves[BLACK][G2] = (1ULL << G1);
	pawn_moves[BLACK][H2] = (1ULL << H1);
	pawn_attacks[BLACK][A2] = (1ULL << B1);
	pawn_attacks[BLACK][B2] = (1ULL << A1) | (1ULL << C1);
	pawn_attacks[BLACK][C2] = (1ULL << B1) | (1ULL << D1);
	pawn_attacks[BLACK][D2] = (1ULL << C1) | (1ULL << E1);
	pawn_attacks[BLACK][E2] = (1ULL << D1) | (1ULL << F1);
	pawn_attacks[BLACK][F2] = (1ULL << E1) | (1ULL << G1);
	pawn_attacks[BLACK][G2] = (1ULL << F1) | (1ULL << H1);
	pawn_attacks[BLACK][H2] = (1ULL << G1);

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
