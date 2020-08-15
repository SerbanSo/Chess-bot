#include "defines.h"
#include "inits.h"
#include "moves.h"

using std::cin;
using std::cout;

// Default case
int engine = BLACK;
int opponent = WHITE;

// 0 - white needs to move
// 1 - black needs to move
int turn = 0;

int king_moved[2];
int left_rook_moved[2];
int right_rook_moved[2];

int en_passant_available[2][9];

// King's position on the board
position king_position[2];

// ---------------------------------
// Starting bitboard implementation

Bitboard board;
Castling castling[2];
unsigned long long king_moves[64];
unsigned long long knight_moves[64];
unsigned long long rook_moves[64];
unsigned long long bishop_moves[64];
unsigned long long queen_moves[64];
unsigned long long pawn_moves[2][64];
unsigned long long pawn_attacks[2][64];
Magic rook_magic[64];
Magic bishop_magic[64];
unsigned long long rook_table[102400];
unsigned long long bishop_table[5248];
int en_passant[2];
int king_pos[2];
unsigned long long notAFile;
unsigned long long notHFile;

std::ofstream bit_board_file;
void print_bit_board_to_file()
{
	std::string buffer_white[8], buffer_black[8];

	bit_board_file.open("bit_board.txt", std::fstream::out);

	unsigned long long j = 1;
	for(int i = 0; i < 64; i++)
	{
		buffer_white[i/8] += (((board.pawns[0] & j) == 0) ? "0 " : "1 ");
		buffer_black[i/8] += (((board.pawns[1] & j) == 0) ? "0 " : "1 ");

		// New line
		if((i+1) % 8 == 0)
		{
			buffer_white[i/8] += "\n";
			buffer_black[i/8] += "\n";
		}

		j = j << 1;
	}

	bit_board_file << "WHITE\n";
	for(int i = 7; i >= 0; i--)
		bit_board_file << buffer_white[i];

	bit_board_file << "BLACK\n";
	for(int i = 7; i >= 0; i--)
		bit_board_file << buffer_black[i];

	bit_board_file.close();
}

// ---------------------------------
int main()
{
	srand(time(NULL));
	std::ios::sync_with_stdio(false);
	setbuf(stdin, NULL);
	setbuf(stdout, NULL);
	signal(SIGINT, SIG_IGN);

	std::ofstream input_from_xboard;
	input_from_xboard.open("input_from_xboard.txt", std::fstream::out);

	char input_buffer[100];

	bool game_over = 0;
	bool force_mode = 0;

	start_inits();

	while ( game_over == 0 )
	{
		cin >> input_buffer;

		input_from_xboard << input_buffer << "\n";
		input_from_xboard.flush();

		if (!strcmp(input_buffer, "xboard"))
		{
			board_inits();
		}
		else if (!strcmp(input_buffer, "new"))
		{
			board_inits();
			turn = 0;
			force_mode = 0;
		}
		else if (!strcmp(input_buffer, "force"))
		{
			// Enable force mode
			force_mode = 1;
		}
		else if (!strcmp(input_buffer, "go"))
		{
			// Update the engine color
			if(turn == 0)
			{
				engine = WHITE;
				opponent = BLACK;
			}
			else
			{
				engine = BLACK;
				opponent = WHITE;
			}

			// Make the first move
			make_random_move();

			// Update turn
			turn = 1 - turn;

			// Disable force mode
			force_mode = 0;
		}
		else if (!strcmp(input_buffer, "white"))
		{
			engine = WHITE;
			opponent = BLACK;
		}
		else if (!strcmp(input_buffer, "black"))
		{
			engine = BLACK;
			opponent = WHITE;
		}
		else if (!strcmp(input_buffer, "quit"))
		{
			break;
		}
		else if (!strcmp(input_buffer, "resign"))
		{
			break;
		}
		else if (is_move(input_buffer))
		{
			// if ( !bit_is_move_valid(input_buffer, turn) )
			// {
			// 	std::cout << "Illegal move: " << input_buffer << "\n";
			// 	continue;
			// }

			// make_move(input_buffer, turn);
			// print_board_to_file();

			make_move(input_buffer, turn);

			// Update the turn
			turn = 1 - turn;

			if(!force_mode)
			{
				make_random_move();

				// If the bot made a move update the turn
				turn = 1 - turn;
			}
		}
	}

	input_from_xboard.close();

	return 0;
}

void print_bit(unsigned long long a)
{
	std::string buffer[8];

	// bit_board_file.open("bit_board.txt", std::fstream::out);
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
	// bit_board_file.close();
}

// TEST MAIN
/*
int main()
{
	start_inits();
	// print_bit_board_to_file();
	print_bit(board.board[0] | board.board[1]);

	std::cout << "White\n";
	char tmp1[] = "a2a3";
	std::cout << " " << is_move_valid(C4, D3, BLACK);
	make_move(tmp1, WHITE);
	print_bit(board.board[0] | board.board[1]);

	std::cout << "Black\n";
	char tmp2[] = "g8h6";
	std::cout << " " << is_move_valid(C4, D3, BLACK);
	make_move(tmp2, BLACK);
	print_bit(board.board[0] | board.board[1]);

	std::cout << "White\n";
	char tmp3[] = "b2b3";
	std::cout << " " << is_move_valid(C4, D3, BLACK);
	make_move(tmp3, WHITE);
	print_bit(board.board[0] | board.board[1]);

	std::cout << "Black\n";
	char tmp4[] = "g7g6";
	std::cout << " " << is_move_valid(C4, D3, BLACK);
	make_move(tmp4, BLACK);
	print_bit(board.board[0] | board.board[1]);

	std::cout << "White\n";
	char tmp5[] = "c2c3";
	std::cout << castling[WHITE].moved_king << " " << is_move_valid(E1, C1, WHITE);
	make_move(tmp5, WHITE);
	print_bit(board.board[0] | board.board[1]);

	std::cout << "Black\n";
	char tmp6[] = "f8g7";
	std::cout << castling[WHITE].moved_king << " " << is_move_valid(E1, C1, BLACK);
	make_move(tmp6, BLACK);
	print_bit(board.board[0] | board.board[1]);

	std::cout << "White\n";
	char tmp7[] = "d2d3";
	std::cout << castling[WHITE].moved_king << " " << is_move_valid(E1, G1, WHITE);
	make_move(tmp7, WHITE);
	print_bit(board.board[0] | board.board[1]);

	std::cout << "Black\n";
	char tmp8[] = "e8g8";
	std::cout << castling[WHITE].moved_king << " " << is_move_valid(E1, C1, BLACK);
	make_move(tmp8, BLACK);
	print_bit(board.board[0] | board.board[1]);

	// std::cout << "White\n";
	// char tmp9[] = "e2e3";
	// std::cout << castling[WHITE].moved_king << " " << is_move_valid(E1, C1, WHITE);
	// make_move(tmp9, WHITE);
	// print_bit(board.board[0] | board.board[1]);

	// std::cout << "Black\n";
	// char tmp10[] = "d8d7";
	// std::cout << castling[WHITE].moved_king << " " << is_move_valid(E1, C1, BLACK);
	// make_move(tmp10, BLACK);
	// print_bit(board.board[0] | board.board[1]);

	// std::cout << "White\n";
	// char tmp11[] = "f2f3";
	// std::cout << castling[WHITE].moved_king << " " << is_move_valid(E1, C1, WHITE);
	// make_move(tmp11, WHITE);
	// print_bit(board.board[0] | board.board[1]);

	// std::cout << "Black\n";
	// char tmp12[] = "e8c8";
	// std::cout << castling[WHITE].moved_king << " " << is_move_valid(E8, C8, BLACK);
	// make_move(tmp12, BLACK);
	// print_bit(board.board[0] | board.board[1]);

	// std::cout << "White\n";
	// char tmp1[] = "g1h3";
	// std::cout << " " << is_move_valid(C4, D3, BLACK);
	// make_move(tmp1, WHITE);
	// print_bit(board.board[0] | board.board[1]);

	// std::cout << "Black\n";
	// char tmp2[] = "a7a6";
	// std::cout << " " << is_move_valid(C4, D3, BLACK);
	// make_move(tmp2, BLACK);
	// print_bit(board.board[0] | board.board[1]);

	// std::cout << "White\n";
	// char tmp3[] = "e2e3";
	// std::cout << " " << is_move_valid(C4, D3, BLACK);
	// make_move(tmp3, WHITE);
	// print_bit(board.board[0] | board.board[1]);

	// std::cout << "Black\n";
	// char tmp4[] = "a6a5";
	// std::cout << " " << is_move_valid(C4, D3, BLACK);
	// make_move(tmp4, BLACK);
	// print_bit(board.board[0] | board.board[1]);

	// std::cout << "White\n";
	// char tmp5[] = "f1e2";
	// std::cout << castling[WHITE].moved_king << " " << is_move_valid(E1, C1, WHITE);
	// make_move(tmp5, WHITE);
	// print_bit(board.board[0] | board.board[1]);

	// std::cout << "Black\n";
	// char tmp6[] = "b7b6";
	// std::cout << castling[WHITE].moved_king << " " << is_move_valid(E1, C1, BLACK);
	// make_move(tmp6, BLACK);
	// print_bit(board.board[0] | board.board[1]);

	// std::cout << "White\n";
	// char tmp7[] = "e1g1";
	// std::cout << castling[WHITE].moved_king << " " << is_move_valid(E1, G1, WHITE);
	// make_move(tmp7, WHITE);
	// print_bit(board.board[0] | board.board[1]);

	// // std::cout << "Black\n";
	// // char tmp8[] = "b6b5";
	// // std::cout << castling[WHITE].moved_king << " " << is_move_valid(E1, C1, BLACK);
	// // make_move(tmp8, BLACK);
	// // print_bit(board.board[0] | board.board[1]);

	// // std::cout << "White\n";
	// // char tmp9[] = "d1e2";
	// // std::cout << castling[WHITE].moved_king << " " << is_move_valid(E1, C1, WHITE);
	// // make_move(tmp9, WHITE);
	// // print_bit(board.board[0] | board.board[1]);

	// // std::cout << "Black\n";
	// // char tmp10[] = "c7c6";
	// // std::cout << castling[WHITE].moved_king << " " << is_move_valid(E1, C1, BLACK);
	// // make_move(tmp10, BLACK);
	// // print_bit(board.board[0] | board.board[1]);

	// std::cout << "White\n";
	// char tmp11[] = "c1e1";
	// std::cout << castling[WHITE].moved_king << " " << is_move_valid(E1, C1, WHITE);
	// make_move(tmp11, WHITE);
	// print_bit(board.board[0] | board.board[1]);

	// print_bit(board.king[WHITE]);
	// print_bit(board.rooks[WHITE]);

	return 0;
}
*/