#include "moves.h"

int parse_move(char *move) {
	return (move[0] - 'a') + (move[1] - '1') * 8;
}

bool is_move(char *move) {
	// Example for the move received from xboard:
	// "a1a2\n"

	if('a' <= move[0] && move[0] <= 'h')
		if('1' <= move[1] && move[1] <= '8')
			if('a' <= move[2] && move[2] <= 'h')
				if('1' <= move[3] && move[3] <= '8')
					return true;

	return false;
}

void send_move(int start, int end) {
	char a = 'a' + start % 8;
	char b = '1' + start / 8;
	char c = 'a' + end % 8;
	char d = '1' + end / 8;

	std::cout << "move " << a << b << c << d << "\n";
}

bool is_move_valid(int start, int end, int player) {
	if(pawn_valid_move(start, end, player))	{
		return true;
	}
	else if(knight_valid_move(start, end, player)) {
		return true;
	}
	else if(king_valid_move(start, end, player)) {
		return true;
	}
	else if(rook_valid_move(start, end, player)) {
		return true;
	}
	else if(bishop_valid_move(start, end, player)) {
		return true;
	}
	else if(queen_valid_move(start, end, player)) {
		return true;
	} else if(castling_valid_move(start, end, player)) {
		return true;
	}

	return false;
}

void make_move(char *move, int player) {
	// Position in matrix
	int start = parse_move(move);
	int end = parse_move(move+2);

	// Position on the board
	unsigned long long start_pos = (1ull << start);
	unsigned long long end_pos = (1ull << end);

	// Reset en_passant
	en_passant[player] = -1;

	if(pawn_valid_move(start, end, player)) {
		// Move
		board.pawns[player] ^= start_pos;
		board.pawns[player] |= end_pos;

		// Promote the pawn to queen
		if(end >= A8 && end <= H8 && player == WHITE) {
			board.pawns[player] ^= end_pos;
			board.queen[player] |= end_pos;
		}
		if(end >= A1 && end <= H1 && player == BLACK) {
			board.pawns[player] ^= end_pos;
			board.queen[player] |= end_pos;
		}

		// Check for en-passant
		if(en_passant[1-player] != -1) {
			if(en_passant[1-player] + 1 == start) {
				if(player == BLACK) {
					if(en_passant[1-player] - 8 == end)
						remove_piece((1ULL << en_passant[1-player]), 1-player);
				} else {
					if(en_passant[1-player] + 8 == end)
						remove_piece((1ULL << en_passant[1-player]), 1-player);
				}
			} else if(en_passant[1-player] - 1 == start) {
				if(player == BLACK) {
					if(en_passant[1-player] - 8 == end)
						remove_piece((1ULL << en_passant[1-player]), 1-player);
				} else {
					if(en_passant[1-player] + 8 == end)
						remove_piece((1ULL << en_passant[1-player]), 1-player);
				}
			}
		}

		// Activate en_passant
		if(start >= A2 && start <= H2 && end >= A4 && end <= H4) {
			en_passant[player] = end;
		}
		if(start >= A7 && start <= H7 && end >= A5 && end <= H5) {
			en_passant[player] = end;
		}
	}
	else if(knight_valid_move(start, end, player)) {
		// Move
		board.knights[player] ^= start_pos;
		board.knights[player] |= end_pos;
	}
	else if(king_valid_move(start, end, player)) {
		// Move
		board.king[player] ^= start_pos;
		board.king[player] |= end_pos;
		king_pos[player] = end;

		// Deactivate castling
		castling[player].moved_king = true;
	}
	else if(rook_valid_move(start, end, player)) {
		// Move
		board.rooks[player] ^= start_pos;
		board.rooks[player] |= end_pos;

		// Deactivate castling
		if(start == A1 || start == A8) {
			// Left rook
			castling[player].moved_left_rook = true;
		} else if(start == H1 || start == H8) {
			// Right rook
			castling[player].moved_right_rook = true;
		}
	}
	else if(bishop_valid_move(start, end, player)) {
		// Move
		board.bishops[player] ^= start_pos;
		board.bishops[player] |= end_pos;
	}
	else if(queen_valid_move(start, end, player)) {
		// Move
		board.queen[player] ^= start_pos;
		board.queen[player] |= end_pos;
	}
	else if(castling_valid_move(start, end, player)) {
		// Move king
		board.king[player] ^= start_pos;
		board.king[player] |= end_pos;

		if(start > end) {
			// Big castling
			// Move rook
			board.rooks[player] ^= (1ULL << castling[player].left_rook_pos);
			board.rooks[player] |= (1ULL << (start-1));
		} else {
			// Small castling
			// Move rook
			board.rooks[player] ^= (1ULL << castling[player].right_rook_pos);
			board.rooks[player] |= (1ULL << (start+1));
		}

		// Deactivate castling
		castling[player].moved_king = true;
	}

	// Remove opponents piece
	if(board.board[1-player] & end_pos)
		remove_piece(end_pos, 1-player);

	// Update the "global" boards for each color
	update_boards();
}

bool is_chess(int player) {
	unsigned long long king_board_pos;
	unsigned long long full_board = board.board[0] | board.board[1];

	king_board_pos = (1ULL << king_pos[player]);

	// Knight attacks king
	if((board.knights[1-player] & knight_moves[king_pos[player]]) != 0)
		return true;

	// Rook attacks king
	if(((rook_attack(king_pos[player], full_board)) & board.rooks[1-player]) != 0)
		return true;

	// Bishop attacks king
	if(((bishop_attack(king_pos[player], full_board)) & board.bishops[1-player]) != 0)
		return true;

	// Queen attacks king
	if(((queen_attack(king_pos[player], full_board)) & board.queen[1-player]) != 0)
		return true;	

	// if((king_board_pos & pawn_attacks[1-player][king_pos[player]]) != 0)
	// 	return true;

	if(player == WHITE) {
		// Pawn attacks king
		// Shift all pawns by 7 or 9
		if(((board.pawns[1-player] >> 7) & king_board_pos & notHFile) != 0)
			return true;
		else if(((board.pawns[1-player] >> 9) & king_board_pos & notAFile) != 0)
			return true;
	}

	if(player == BLACK) {
		// Pawn attacks king
		// Shift all pawns by 7 or 9
		if(((board.pawns[1-player] << 7) & king_board_pos & notHFile) != 0)
			return true;
		else if(((board.pawns[1-player] << 9) & king_board_pos & notAFile) != 0)
			return true;
	}

	return false;
}

void make_random_move() {
	// Move buffer
	char move[4];

	// Limit for random moves
	int limit = 1000000;

	// While the random move isn't valid
	while (limit--) {
		move[0] = 'a' + rand() % 8;
		move[1] = '1' + rand() % 8;
		move[2] = 'a' + rand() % 8;
		move[3] = '1' + rand() % 8;

		int start = parse_move(move);
		int end = parse_move(move+2);

		if(is_move_valid(start, end, engine)) {
			// Store the current game state
			Bitboard undo_board = board;
			int undo_king_pos[2] = {king_pos[0], king_pos[1]};

			// Make the move
			make_move(move, engine);

			// If it chess after move, undo the move
			if(is_chess(engine)) {
				board = undo_board;
				king_pos[0] = undo_king_pos[0];
				king_pos[1] = undo_king_pos[1];
				continue;
			}

			// Debug
			bit_board_file << "WHITE\n";
			// print_bit(bit_board.white_board);

			bit_board_file << "BLACK\n";
			// print_bit(bit_board.black_board);

			// Send the move to std::out
			send_move(start, end);
			return;
		}
	}

	std::cout << "resign" << "\n";
	system("exit");
}

bool castling_valid_move(int start, int end, int player) {
	// The way castling can happen:
	//
	// 0 0 0 0 0 0 0 0 //black's base_line
	// 0 0 0 0 0 0 0 0
	// 0 0 0 0 0 0 0 0
	// 0 0 0 0 0 0 0 0
	// 0 0 0 0 0 0 0 0
	// 0 0 0 0 0 0 0 0
	// 0 0 0 0 0 0 0 0
	// X 0 0 0 1 0 0 X //white's base_line
	//
	unsigned long long start_pos = (1ull << start);
	unsigned long long end_pos = (1ull << end);

	// The king was moved or the king isn't on start position
	if(castling[player].moved_king || castling[player].king_pos != start)
		return false;

	// If castling with the left rook
	if(castling[player].left_rook_pos + 2 == end) {
		if(castling[player].moved_left_rook || ((1ULL << castling[player].left_rook_pos) & board.rooks[player]) == 0)
			return false;

		int tmp = king_pos[player];
		bool valid_condition = true;

		if(is_chess(player))
			return false;

		// For each square between king and rook
		for(int i = start-1; i >= start-2; i--) {
			king_pos[player]--;

			// If it is a colission or is in check
			if((((board.board[0] | board.board[1]) & (1ULL << king_pos[player])) != 0) || is_chess(player)) {
				valid_condition = false;
				break;
			}
		}
		king_pos[player] = tmp;

		// B1 or B8 not clear
		if(((board.board[0] | board.board[1]) & (1ULL << start-3)) != 0)
			return false;


		if(valid_condition)
			return true;

		std::cout << "Here\n";
		return false;
	}

	// If castling with the right rook
	if(castling[player].right_rook_pos - 1 == end) {
		if(castling[player].moved_right_rook || ((1ULL << castling[player].right_rook_pos) & board.rooks[player]) == 0)
			return false;

		if(is_chess(player))
			return false;

		int tmp = king_pos[player];
		bool valid_condition = true;
		// For each square between king and rook
		for(int i = start+1; i <= start+2; i++) {
			king_pos[player]++;

			// If it is a colission or is in check
			if((((board.board[0] | board.board[1]) & (1ULL << king_pos[player])) != 0) || is_chess(player)) {
				valid_condition = false;
				break;
			}
		}
		king_pos[player] = tmp;

		if(valid_condition)
			return true;
		return false;
	}

	return false;
}

bool pawn_valid_move(int start, int end, int player) {
	// The way the pawn can move:
	//
	// 0 0 0 0 0 0 0 0 //black's base_line
	// 0 0 0 0 0 0 0 0
	// 0 0 0 0 0 0 0 0
	// 0 0 0 0 0 0 0 0
	// x x x x x x x x
	// x x 1 1 x 1 x x
	// 1 1 0 0 1 0 1 1
	// 0 0 0 0 0 0 0 0 //white's base_line
	//
	// Shift by 7 for right attack
	// Shift by 9 for left attack
	// Shift by 8 for forward move
	// Shift my 16 for double forward move
	// TO DO en passant and promote

	unsigned long long start_pos = (1ull << start);
	unsigned long long end_pos = (1ull << end);
	unsigned long long posible_moves;
	unsigned long long pawn;

	pawn = board.pawns[player] & start_pos;

	// Not a pawn move
	if(pawn == 0)
		return false;

	// Invalid move, the pawn arrives on a friendly piece
	if((board.board[player] & end_pos) != 0)
		return false;

	// Forward move, check collision
	if((pawn_moves[player][start] & end_pos) != 0)
		if((pawn_moves[player][start] & (board.board[1-player] | board.board[player])) != 0)
			return false;
		else
			return true;

	// Check attack
	if((pawn_attacks[player][start] & end_pos) != 0)
		if((end_pos & board.board[1-player]) != 0)
			return true;

	// Check en-passant
	/*
	if(en_passant[1-player] != -1) {
		if(en_passant[1-player] + 1 == start) {
			if(player == BLACK) {
				if(en_passant[1-player] - 8 == end)
					return true;
			} else {
				if(en_passant[1-player] + 8 == end)
					return true;
			}
		} else if(en_passant[1-player] - 1 == start) {
			if(player == BLACK) {
				if(en_passant[1-player] - 8 == end)
					return true;
			} else {
				if(en_passant[1-player] + 8 == end)
					return true;
			}
		}
	}
	*/

	if(en_passant[1-player] != -1) {
		if(((1ULL << (en_passant[1-player] + 1)) & notAFile) == start_pos) {
			if(player == BLACK) {
				if(en_passant[1-player] - 8 == end)
					return true;
			} else {
				if(en_passant[1-player] + 8 == end)
					return true;
			}
		} else if(((1ULL << (en_passant[1-player] - 1)) & notHFile) == start_pos) {
			if(player == BLACK) {
				if(en_passant[1-player] - 8 == end)
					return true;
			} else {
				if(en_passant[1-player] + 8 == end)
					return true;
			}
		}
	}

	return false;
}

bool rook_valid_move(int start, int end, int player) {
	// The way the rook can move:
	//
	// 0 0 0 0 x 0 0 0 //black's base_line
	// 0 0 0 0 x 0 0 0
	// 0 0 0 0 x 0 0 0
	// 0 0 0 0 x 0 0 0
	// x x x x 1 x x x
	// 0 0 0 0 x 0 0 0
	// 0 0 0 0 x 0 0 0
	// 0 0 0 0 x 0 0 0 //white's base_line

	unsigned long long start_pos = (1ull << start);
	unsigned long long end_pos = (1ull << end);
	unsigned long long rook;

	// Not a move for rook
	if((board.rooks[player] & start_pos) == 0)
		return false;

	// Invalid move, the rook arrives on a friendly piece
	if((board.board[player] & end_pos) != 0)
		return false;

	// Retrive the attack table for start position from database
	rook = rook_attack(start, board.board[0] | board.board[1]);

	// Return if the rook can reach the end position
	return (rook & end_pos) != 0 ? true : false;
}

bool knight_valid_move(int start, int end, int player) {
	// The way a knight can move:
	//
	// 0 0 0 0 0 0 0 0 //black's base_line
	// 0 0 0 0 0 0 0 0
	// 0 0 0 x 0 x 0 0
	// 0 0 x 0 0 0 x 0
	// 0 0 0 0 1 0 0 0
	// 0 0 x 0 0 0 x 0
	// 0 0 0 x 0 x 0 0
	// 0 0 0 0 0 0 0 0 //white's base_line
	unsigned long long start_pos = (1ull << start);
	unsigned long long end_pos = (1ull << end);

	// Not a knight move
	if((board.knights[player] & start_pos) == 0)
		return false;

	// Invalid move, the knight arrives on a friendly piece
	if((board.board[player] & end_pos) != 0)
		return false;

	// Return if the knight can reach the end position
	return (knight_moves[start] & end_pos) != 0 ? true : false;
}

bool bishop_valid_move(int start, int end, int player) {
	// The way a bishop can move:
	//
	// x 0 0 0 0 0 0 0 //black's base_line
	// 0 x 0 0 0 0 0 x
	// 0 0 x 0 0 0 x 0
	// 0 0 0 x 0 x 0 0
	// 0 0 0 0 1 0 0 0
	// 0 0 0 x 0 x 0 0
	// 0 0 x 0 0 0 x 0
	// 0 x 0 0 0 0 0 0 //white's base_line

	unsigned long long start_pos = (1ull << start);
	unsigned long long end_pos = (1ull << end);
	unsigned long long bishop;

	// Not a move for bishop
	if((board.bishops[player] & start_pos) == 0)
		return false;

	// Invalid move, the bishop arrives on a friendly piece
	if((board.board[player] & end_pos) != 0)
		return false;

	// Retrive the attack table for start position from database
	bishop = bishop_attack(start, board.board[0] | board.board[1]);

	// Return if the bishop can reach the end position
	return (bishop & end_pos) != 0 ? true : false;
}

bool queen_valid_move(int start, int end, int player) {
	// The way a queen can move:
	//
	// x 0 0 0 x 0 0 0 //black's base_line
	// 0 x 0 0 x 0 0 x
	// 0 0 x 0 x 0 x 0
	// 0 0 0 x x x 0 0
	// x x x x 1 x x x
	// 0 0 0 x x x 0 0
	// 0 0 x 0 x 0 x 0
	// 0 x 0 0 x 0 0 0 //white's base_line

	unsigned long long start_pos = (1ull << start);
	unsigned long long end_pos = (1ull << end);
	unsigned long long queen;

	// Not a move for queen
	if((board.queen[player] & start_pos) == 0)
		return false;

	// Invalid move, the queen arrives on a friendly piece
	if((board.board[player] & end_pos) != 0)
		return false;

	// Retrive the attack table for start position from database
	queen = queen_attack(start, board.board[0] | board.board[1]);

	// Return if the queen can reach the end position
	return (queen & end_pos) != 0 ? true : false;
}

bool king_valid_move(int start, int end, int player) {
	// The way the king can move:
	//
	// 0 0 0 0 0 0 0 0 //black's base_line
	// 0 0 0 0 0 0 0 0
	// 0 0 0 0 0 0 0 0
	// 0 0 0 x x x 0 0
	// 0 0 0 x 1 x 0 0
	// 0 0 0 x x x 0 0
	// 0 0 0 0 0 0 0 0
	// 0 0 0 0 0 0 0 0 //white's base_line
	//
	// TO DO castling

	unsigned long long start_pos = (1ull << start);
	unsigned long long end_pos = (1ull << end);

	// Not a king move
	if((board.king[player] & start_pos) == 0)
		return false;

	// Invalid move, the king arrives on a friendly piece
	if((board.board[player] & end_pos) != 0)
		return false;

	// Invalid move, destination is near the opposite king
	if((king_moves[end] & board.king[1-player]) != 0)
		return false;

	// Return if the king can reach the end position
	return (king_moves[start] & end_pos) != 0 ? true : false;
}

void remove_piece(unsigned long long pos, int player) {
	if(board.pawns[player] & pos) {
		board.pawns[player] ^= pos;
	}
	else if(board.rooks[player] & pos) {
		board.rooks[player] ^= pos;
	}
	else if(board.knights[player] & pos) {
		board.knights[player] ^= pos;
	}
	else if(board.bishops[player] & pos) {
		board.bishops[player] ^= pos;
	}
	else if(board.queen[player] & pos) {
		board.queen[player] ^= pos;
	}
}

void update_boards() {
	// White board
	board.board[0] = board.pawns[0];
	board.board[0] |= board.knights[0];
	board.board[0] |= board.bishops[0];
	board.board[0] |= board.rooks[0];
	board.board[0] |= board.queen[0];
	board.board[0] |= board.king[0];

	// Black board
	board.board[1] = board.pawns[1];
	board.board[1] |= board.knights[1];
	board.board[1] |= board.bishops[1];
	board.board[1] |= board.rooks[1];
	board.board[1] |= board.queen[1];
	board.board[1] |= board.king[1];
}