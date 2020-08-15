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
    // impartiri si modulo puteri 2 poti fi facute cu bit hacking
	char a = 'a' + start % 8; //start & 0x00000111 (ia ultimii 3 biti)
	char b = '1' + start / 8; //start & 0x11111000 (ia primii 5 biti)
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
	}
    else if(castling_valid_move(start, end, player)) {
		return true;
	}

	return false;
}

void make_move(unsigned long long start, unsigned long long end, int player) {
	en_passant[player] = -1;

	if(board.pawns[player] & start) {
		// Move
		board.pawns[player] ^= start;
		board.pawns[player] |= end;

		// Promote the pawn to queen
		if((end & rank1File) || (end & rank8File)) {
			board.pawns[player] ^= end;
			board.queens[player] |= end;
		}

		// Check for en-passant
		if(en_passant[1-player] != -1) {
			if(en_passant[1-player] + 1 == ffsll(start)-1) {
				if(player == BLACK) {
					if(en_passant[1-player] - 8 == ffsll(end)-1)
						remove_piece((1ULL << en_passant[1-player]), 1-player);
				} else {
					if(en_passant[1-player] + 8 == ffsll(end)-1)
						remove_piece((1ULL << en_passant[1-player]), 1-player);
				}
			} else if(en_passant[1-player] - 1 == ffsll(start)-1) {
				if(player == BLACK) {
					if(en_passant[1-player] - 8 == ffsll(end)-1)
						remove_piece((1ULL << en_passant[1-player]), 1-player);
				} else {
					if(en_passant[1-player] + 8 == ffsll(end)-1)
						remove_piece((1ULL << en_passant[1-player]), 1-player);
				}
			}
		}

		// Activate en_passant
		if((start & rank2File) && (end & rank4File)) {
			en_passant[player] = ffsll(end)-1;
		}
		if((start & rank7File) && (end & rank5File)) {
			en_passant[player] = ffsll(end)-1;
		}
	} else if(board.knights[player] & start) {
		// Move
		board.knights[player] ^= start;
		board.knights[player] |= end;
	} else if(board.rooks[player] & start) {
		// Move
		board.rooks[player] ^= start;
		board.rooks[player] |= end;

		// Deactivate castling
		if(ffsll(start)-1 == A1 || ffsll(start)-1 == A8) {
			// Left rook
			castling[player].moved_left_rook = true;
		} else if(ffsll(start)-1 == H1 || ffsll(start)-1 == H8) {
			// Right rook
			castling[player].moved_right_rook = true;
		}
	} else if(board.bishops[player] & start) {
		// Move
		board.bishops[player] ^= start;
		board.bishops[player] |= end;
	} else if(board.queens[player] & start) {
		// Move
		board.queens[player] ^= start;
		board.queens[player] |= end;
	} else if(board.king[player] & start) {

		if(!castling[player].moved_king)
			if(castling_valid_move(ffsll(start)-1, ffsll(end)-1, player)) {
				if(start > end) {
					// Big castling
					// Move rook
					board.rooks[player] ^= (1ULL << castling[player].left_rook_pos);
					board.rooks[player] |= (1ULL << (ffsll(start)-1-1));
				} else {
					// Small castling
					// Move rook
					board.rooks[player] ^= (1ULL << castling[player].right_rook_pos);
					board.rooks[player] |= (1ULL << (ffsll(start)-1+1));
				}
			}
		// Move
		board.king[player] ^= start;
		board.king[player] |= end;

		// Deactivate castling
		castling[player].moved_king = true;
	}

	// Remove opposite piece
	remove_piece(end, 1-player);

	update_boards();
}

bool is_in_check(int player) {
	unsigned long long full_board = board.board[0] | board.board[1];
	int pos = ffsll(board.king[player])-1;

	// Knight attacks king
	if((board.knights[1-player] & knight_moves[pos]))
		return true;

	// Rook attacks king
	if((board.rooks[1-player] & rook_attack(pos, full_board)) != 0)
		return true;

	// Bishop attacks king
	if((board.bishops[1-player] & bishop_attack(pos, full_board)) != 0)
		return true;

	// Queen attacks king
	if((board.queens[1-player] & queen_attack(pos, full_board)) != 0)
		return true;

	if(player == WHITE) {
		// Pawn attacks king
		// Shift all pawns by 7 or 9
		if(((board.pawns[1-player] >> 7) & board.king[player] & notAFile) != 0)
			return true;
		else if(((board.pawns[1-player] >> 9) & board.king[player] & notHFile) != 0)
			return true;
	}

	if(player == BLACK) {
		// Pawn attacks king
		// Shift all pawns by 7 or 9
		if(((board.pawns[1-player] << 7) & board.king[player] & notHFile) != 0)
			return true;
		else if(((board.pawns[1-player] << 9) & board.king[player] & notAFile) != 0)
			return true;
	}

	return false;
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
	unsigned long long full_board = board.board[0] | board.board[1];

	// The king was moved or the king isn't on start position
	if(castling[player].moved_king || castling[player].king_pos != start)
		return false;

	// If castling with the left rook
	if(castling[player].left_rook_pos + 2 == end) {
		if(castling[player].moved_left_rook || ((1ULL << castling[player].left_rook_pos) & board.rooks[player]) == 0)
			return false;

		int tmp = ffsll(board.king[player])-1;
		int tmp2 = tmp;
		bool valid_condition = true;

		if(is_in_check(player))
			return false;

		// First square
		tmp--;
		board.king[player] = (1ULL << tmp);
		if((full_board & board.king[player]) || is_in_check(player)) {
			board.king[player] = (1ULL << tmp2);
			return false;
		}

		// Second square
		tmp--;
		board.king[player] = (1ULL << tmp);
		if((full_board & board.king[player]) || is_in_check(player)) {
			board.king[player] = (1ULL << tmp2);
			return false;
		}

		tmp = tmp2;
		board.king[player] = (1ULL << tmp);

		// B1 or B8 not clear
		if((full_board & (1ULL << (start-3))) != 0)
			return false;

		return true;
	}

	// If castling with the right rook
	if(castling[player].right_rook_pos - 1 == end) {
		if(castling[player].moved_right_rook || ((1ULL << castling[player].right_rook_pos) & board.rooks[player]) == 0)
			return false;

		if(is_in_check(player))
			return false;

		int tmp = ffsll(board.king[player])-1;
		int tmp2 = tmp;

		tmp++;
		board.king[player] = (1ULL << tmp);
		if((full_board & board.king[player]) || is_in_check(player)) {
			board.king[player] = (1ULL << tmp2);
			return false;
		}

		tmp++;
		board.king[player] = (1ULL << tmp);
		if((full_board & board.king[player]) || is_in_check(player)) {
			board.king[player] = (1ULL << tmp2);
			return false;
		}

		tmp = tmp2;
		board.king[player] = (1ULL << tmp);

		return true;
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
		if(abs(end-start) == 8)
			if((end_pos & (board.board[1-player] | board.board[player])) != 0)
				return false;
			else 
				return true;
		else			
			if((pawn_moves[player][start] & (board.board[1-player] | board.board[player])) != 0)
				return false;
			else
				return true;

	// Check attack
	if((pawn_attacks[player][start] & end_pos) != 0)
		if((end_pos & board.board[1-player]) != 0)
			return true;

	// En-passant check
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
	if((board.queens[player] & start_pos) == 0)
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
	else if(board.queens[player] & pos) {
		board.queens[player] ^= pos;
	}
}

void update_boards() {
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
