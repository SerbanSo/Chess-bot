#include "defines.h"
#include "inits.h"
#include "moves.h"
#include "negamax.h"

using std::cin;
using std::cout;

// Default case
int engine = BLACK;
int opponent = WHITE;
int game_time = MIDDLEGAME;
// 0 - white needs to move
// 1 - black needs to move
int turn = 0;

// ---------------------------------
// Starting bitboard implementation

Bitboard board;
int piece_value_table[2][7][9][9];
char test_pos[64][2];
std::queue<GameState> lastStates;
GameState forbidden_move;

Castling castling[2];
int en_passant[2];
unsigned long long notAFile;
unsigned long long notBFile;
unsigned long long notCFile;
unsigned long long notDFile;
unsigned long long notEFile;
unsigned long long notFFile;
unsigned long long notGFile;
unsigned long long notHFile;
unsigned long long rank8File;
unsigned long long rank1File;
unsigned long long rank2File;
unsigned long long rank4File;
unsigned long long rank7File;
unsigned long long rank5File;

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

int max_depth = 4;

GameState past_game_states[512];
int move_number = 0;

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
	std::ios::sync_with_stdio(false);
	setbuf(stdin, NULL);
	setbuf(stdout, NULL);

	char input_buffer[100];

	bool game_over = 0;
	bool force_mode = 0;

	start_inits();

	while ( !game_over )
	{
		cin >> input_buffer;

		if ( is_move(input_buffer) )
		{
			int start = parse_move(input_buffer);
			int end = parse_move(input_buffer+2);

			// Check if the move made by the oppenent is valid, just in case
			if(!is_move_valid(start, end, turn))
			{
				cout << "Illegal move: " << input_buffer << "\n";
				continue;
			}

			make_move(1ULL << start, 1ULL << end, turn);


			// for draw-repetition
		    GameState tmp;
		    tmp.board = board;
		    tmp.en_passant[0] = en_passant[0];
		    tmp.en_passant[1] = en_passant[1];
		    tmp.castling[0] = castling[0];
		    tmp.castling[1] = castling[1];
		    past_game_states[move_number] = tmp;
		    move_number++;

			// Update the turn
			turn = 1 - turn;

			if(!force_mode)
			{
				find_best_move(turn, max_depth);
			    tmp.board = board;
			    tmp.en_passant[0] = en_passant[0];
			    tmp.en_passant[1] = en_passant[1];
			    tmp.castling[0] = castling[0];
			    tmp.castling[1] = castling[1];
			    past_game_states[move_number] = tmp;
			    move_number++;

				// If the bot made a move update the turn
				turn = 1 - turn;
			}
		}
		else if (!strcmp(input_buffer, "new"))
		{
			board_inits();
			turn = 0;
			force_mode = 0;
		}
		else if ( !strcmp(input_buffer, "protover") )
		{
			// to prevent the engine crashing after the first move
			// and to use the [row][column][row][column] move format
			cout << "feature sigint=0 san=0\n";
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
			find_best_move(turn, max_depth);

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
		else if (!strcmp(input_buffer, "xboard"))
		{
			board_inits();
		}
	}

	return 0;
}

void print_bit(unsigned long long a)
{
	std::string buffer[8];

	bit_board_file.open("bit_board.txt", std::fstream::out);
	bit_board_file << "\n";
	// std::cout << "\n";

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
		bit_board_file << buffer[i];
		// std::cout << buffer[i];

	bit_board_file << "\n";
	// std::cout << "\n";
	bit_board_file.close();
}
