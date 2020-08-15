#include "defines.h"
#include "inits.h"
#include "negamax.h"

int max_depth = 4;

// Default case
int engine = BLACK;
int opponent = WHITE;

// 0 - white needs to move
// 1 - black needs to move
int turn = 0;

int board[2][9][9];
int piece_value_table[2][7][9][9];

int king_moved[2];
int left_rook_moved[2];
int right_rook_moved[2];

int en_passant_available[2][9];

// King's position on the board
position king_position[2];

// coordinates of every player's pieces and their number
position pieces_positions[2][16];
int pieces_number[2];

// to detect threefold repetition
char past_moves[1024][5];
int move_number = 0;

int main()
{
	srand(time(NULL)); //for the random move generation
	std::ios::sync_with_stdio(false); //to speed input
	setbuf(stdin, NULL);
	setbuf(stdout, NULL);

	char input_buffer[100];

	bool game_over = 0;
	bool force_mode = 0;

	init_piece_value_table();

	while ( game_over == 0 )
	{
		std::cin >> input_buffer;

		if ( !strcmp(input_buffer, "xboard") )
		{
			init_board();
			max_depth = 4;
			move_number = 0;
		}
		else if ( !strcmp(input_buffer, "protover") )
		{
			// to prevent the engine crashing after the first move
			// and to use the [row][column][row][column] move format
			std::cout << "feature sigint=0 san=0\n";
		}
		else if ( !strcmp(input_buffer, "new") )
		{
			init_board();
			turn = 0;
			force_mode = 0;

			max_depth = 4;
			move_number = 0;
		}
		else if ( !strcmp(input_buffer, "force") )
		{
			// Enable force mode
			force_mode = 1;
		}
		else if ( !strcmp(input_buffer, "go") )
		{
			// Update the engine color
			if (turn == 0)
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
			find_best_move(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, engine, max_depth + 1);

			// Update turn
			turn = 1 - turn;

			// Disable force mode
			force_mode = 0;
		}
		else if ( !strcmp(input_buffer, "white") )
		{
			engine = WHITE;
			opponent = BLACK;
		}
		else if ( !strcmp(input_buffer, "black") )
		{
			engine = BLACK;
			opponent = WHITE;
		}
		else if ( !strcmp(input_buffer, "quit") )
		{
			break;
		}
		else if ( !strcmp(input_buffer, "resign") )
		{
			break;
		}
		else if ( is_move(input_buffer) )
		{
			if ( !is_move_valid(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, input_buffer, turn) )
			{
				std::cout << "Illegal move: " << input_buffer << "\n";
				continue;
			}

			make_move(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, input_buffer, turn);

			past_moves[move_number][0] = input_buffer[0];
            past_moves[move_number][1] = input_buffer[1];
            past_moves[move_number][2] = input_buffer[2];
            past_moves[move_number][3] = input_buffer[3];
            past_moves[move_number][4] = input_buffer[4];
            move_number++;

			// Update the turn
			turn = 1 - turn;

			if ( pieces_number[engine] + pieces_number[opponent] <= 6 )
			{
				max_depth = 6;
			}

			if ( !force_mode )
			{
				find_best_move(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, engine, max_depth);
				// If the bot made a move update the turn
				turn = 1 - turn;
			}
		}
	}

	return 0;
}
