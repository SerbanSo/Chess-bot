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

int board[2][9][9];

int king_moved[2];
int left_rook_moved[2];
int right_rook_moved[2];

int en_passant_available[2][9];

// King's position on the board
position king_position[2];

int main()
{
	srand(time(NULL)); //for the random move generation
	std::ios::sync_with_stdio(false); //to speed input
	setbuf(stdin, NULL);
	setbuf(stdout, NULL);
	signal(SIGINT, SIG_IGN); //to prevent the engine crashing after the first move

	char input_buffer[100];

	bool game_over = 0;
	bool force_mode = 0;

	while ( game_over == 0 )
	{
		cin >> input_buffer;

		if ( !strcmp(input_buffer, "xboard") )
		{
			init_board();
		}
		else if ( !strcmp(input_buffer, "new") )
		{
			init_board();
			turn = 0;
			force_mode = 0;
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
			make_random_move();

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
			if ( !is_move_valid(input_buffer, turn) )
			{
				std::cout << "Illegal move: " << input_buffer << "\n";
				continue;
			}

			make_move(input_buffer, turn);

			// Update the turn
			turn = 1 - turn;

			if( !force_mode )
			{
				make_random_move();

				// If the bot made a move update the turn
				turn = 1 - turn;
			}
		}
	}

	return 0;
}
