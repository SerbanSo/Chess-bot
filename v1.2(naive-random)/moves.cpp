#include "moves.h"

//returns true if the (x,y) square is attacked by at least one enemy piece
bool is_attacked(int x, int y, int player)
{
	int i, j;
	bool attacked = false;
	int direction = ( 1 - player == WHITE ) ? 1 : -1;

 	//for each enemy piece, check if it attacks the (x, y) square
	for ( i = 8; i >= 1; i-- )
	{
		for ( j = A; j <= H; j++ )
		{
			if ( board[1 - player][j][i] == PAWN )
			{
				if ( y == i + direction )
				{
					if ( x == j + 1 || x == j - 1 )
					{
						attacked = true; //diagonal attack
					}
				}
			}
			else if ( board[1 - player][j][i] == KNIGHT )
			{
				attacked = knight_valid_move(j, i, x, y);
			}
			else if ( board[1 - player][j][i] == BISHOP )
			{
				attacked = bishop_valid_move(j, i, x, y);
			}
			else if ( board[1 - player][j][i] == ROOK )
			{
				attacked = rook_valid_move(j, i, x, y);
			}
			else if ( board[1 - player][j][i] == QUEEN )
			{
				attacked = queen_valid_move(j, i, x, y);
			}
			else if ( board[1 - player][j][i] == KING )
			{
				attacked = king_valid_move(j, i, x, y, 1 - player);
			}

			if ( attacked == true )
			{
				return true;
			}
		}
	}

	return false;
}

//returns true if the move is valid according to the xboard convention
//format "[start column][start row][end column][end row]"
bool is_move(char *move)
{
	if ( 'a' <= move[0] && move[0] <= 'h' )
	{
		if ( '1' <= move[1] && move[1] <= '8' )
		{
			if ( 'a' <= move[2] && move[2] <= 'h' )
			{
				if ( '1' <= move[3] && move[3] <= '8' )
				{
					return true;
				}
			}
		}
	}

	return false;
}

//returns true if the considered move leads to a check
bool is_in_check_after_move(int startX, int startY, int endX, int endY, int player)
{
	//simulate the move
	int swap_aux1 = board[player][startX][startY];
	board[player][startX][startY] = EMPTY;
	board[player][endX][endY] = swap_aux1;

	int swap_aux2 = board[1 - player][endX][endY];
	board[1 - player][endX][endY] = EMPTY;

	bool result_aux;

	//the moved piece is the king and it would be moved into check
	if ( swap_aux1 == KING )
	{
		result_aux = is_attacked(endX, endY, player);
	}
	else //the king is in check after moving the piece
	{
		result_aux = is_attacked(king_position[player].x, king_position[player].y, player);
	}

	//undo the move
	board[player][startX][startY] = swap_aux1;
	board[player][endX][endY] = EMPTY;

	board[1 - player][endX][endY] = swap_aux2;

	return result_aux;
}

//returns true if player can make the move stored in the move string
bool is_move_valid(char *move, int player)
{
	//starting square cannot be identical to end square
	if ( move[0] == move[2] && move[1] == move[3] )
	{
		return false;
	}

	int startX = move[0] - 'a' + 1;
	int startY = move[1] - '0';
	int endX = move[2] - 'a' + 1;
	int endY = move[3] - '0';

	int piece = board[player][startX][startY];

	if ( board[player][endX][endY] != EMPTY || piece == EMPTY || is_in_check_after_move(startX, startY, endX, endY, player) )
	{
		return false;
	}
	else if ( piece == PAWN )
	{
		return pawn_valid_move(startX, startY, endX, endY, player);
	}
	else if ( piece == KNIGHT)
	{
		return knight_valid_move(startX, startY, endX, endY);
	}
	else if ( piece == BISHOP )
	{
		return bishop_valid_move(startX, startY, endX, endY);
	}
	else if ( piece == ROOK )
	{
		return rook_valid_move(startX, startY, endX, endY);
	}
	else if ( piece == QUEEN)
	{
		return queen_valid_move(startX, startY, endX, endY);
	}
	else if ( piece == KING)
	{
		return king_valid_move(startX, startY, endX, endY, player);
	}

	return false;
}

//updates the board and auxiliary vectors
void make_move(char *move, int player)
{
	int startX = move[0] - 'a' + 1;
	int startY = move[1] - '0';
	int endX = move[2] - 'a' + 1;
	int endY = move[3] - '0';

	int direction = ( player == WHITE ) ? 1 : -1;
	int base_line = ( player == WHITE ) ? 1 : 8;
	int opposite_line = 9 - base_line;

	//resets en passant for all pawns
	for ( int i = A; i <= H; i++ )
	{
		en_passant_available[player][i] = 0;
	}

	if ( board[player][startX][startY] == KING )
	{
		king_position[player].x = endX;
		king_position[player].y = endY;

		if ( endY == base_line && king_moved[player] == 0 )
		{
			if ( endX == C ) //left castle
			{
				left_rook_moved[player] = 1;

				board[player][endX][endY] = KING;
				board[player][startX][startY] = EMPTY;
				board[player][endX + 1][endY] = ROOK;
				board[player][A][base_line] = EMPTY;

				return;
			}
			else if ( endX == G ) //right castle
			{
				right_rook_moved[player] = 1;

				board[player][endX][endY] = KING;
				board[player][startX][startY] = EMPTY;
				board[player][endX - 1][endY] = ROOK;
				board[player][H][base_line] = EMPTY;

				return;
			}
		}

		king_moved[player] = 1;
	}

	//a rook was moved
	if ( startY == base_line )
	{
		if ( startX == A )
		{
			left_rook_moved[player] = 1;
		}
		else if ( startX == H )
		{
			right_rook_moved[player] = 1;
		}
	}

	//a rook was captured
	if ( endY == opposite_line )
	{
		if ( endX == A )
		{
			left_rook_moved[1 - player] = 1;
		}
		else if ( endX == H )
		{
			right_rook_moved[1 - player] = 1;
		}
	}

	if ( board[player][startX][startY] == PAWN )
	{
		//pawn promotion
		if ( endY == opposite_line )
		{
			//randomizes the piece that the engine will promote to, delete to promote to queen by default
			if ( player == engine )
			{
				int r = rand() % 4;

				if ( r == 0 )
				{
					move[4] = 'n';
				}
				else if ( r == 1 )
				{
					move[4] = 'b';
				}
				else if ( r == 2 )
				{
					move[4] = 'r';
				}
				else
				{
					move[4] = 'q';
				}
			}

			if ( move[4] == 'n' )
			{
				board[player][startX][startY] = KNIGHT;
			}
			else if ( move[4] == 'b' )
			{
				board[player][startX][startY] = BISHOP;
			}
			else if ( move[4] == 'r' )
			{
				board[player][startX][startY] = ROOK;
			}
			else
			{
				board[player][startX][startY] = QUEEN;
			}
		}

		//can capture by en passant
		if ( ( startY == base_line + direction * 4 ) && ( endY - startY == direction ) && en_passant_available[1 - player][endX] == 1 )
		{
			board[player][endX][endY] = PAWN;
			board[player][startX][startY] = EMPTY;
			board[1 - player][endX][startY] = EMPTY;

			return;
		}

		//enables en passant
		if ( endY == startY + 2 * direction )
		{
			en_passant_available[player][endX] = 1;
		}
	}

	board[player][endX][endY] = board[player][startX][startY]; //moves
	board[player][startX][startY] = EMPTY; //leaves initial position
	board[1 - player][endX][endY] = EMPTY; //captures
}

//generates up to 1,000,000 random moves until it find a valid one or resigns
void make_random_move()
{
	int r1, r2, r3, r4;

	char move[5];

	int limit = 1000000;

	// While the random move isn't valid
	while ( limit-- )
	{
		r1 = rand() % 8;
		r2 = rand() % 8;

		if ( board[engine][1 + r1][1 + r2] != EMPTY )
		{
			r3 = rand() % 8;
			r4 = rand() % 8;

			move[0] = 'a' + r1;
			move[1] = '1' + r2;
			move[2] = 'a' + r3;
			move[3] = '1' + r4;

			if ( is_move_valid(move, engine) )
			{
				// Send the move to xboard
				std::cout << "move " << move[0] << move[1] << move[2] << move[3];

				// Updates the engine's matrices
				make_move(move, engine);

				// a pawn was promoted
				if ( move[4] == 'n' || move[4] == 'b' || move[4] == 'r' || move[4] == 'q' )
				{
					std::cout << move[4];

					//resets the promotion flag
					move[4] = 0;
				}

				std::cout << '\n';

				return;
			}
		}
	}

	std::cout << "resign" << "\n";
	system("exit");
}

bool pawn_valid_move(int startX, int startY, int endX, int endY, int player)
{
	// Determine the direction, either UP for the WHITE player, or DOWN for the BLACK player
	int direction = ( player == WHITE ) ? 1 : -1;
	int base_line = ( player == WHITE ) ? 1 : 8;

	// If the pawn advances normally
	if ( startX == endX ) //the pawn stays on the same column
	{
		//the pawn can't advance less than 1 square or more than 2 squares
		if ( endY - startY != direction && endY - startY != 2 * direction )
		{
			return false;
		}

		//an enemy piece is right in front of the pawn
		if ( endY - startY == direction && board[1 - player][endX][endY] != EMPTY )
		{
			return false;
		}

		//the pawn tries to advance 2 squares but either isn't on the correct row or there is a piece blocking its advance
		if ( endY - startY == 2 * direction && ( ( direction == 1 && startY != 2 ) || ( direction == -1 && startY != 7 ) || board[player][endX][startY + direction] != EMPTY || board[1 - player][endX][startY + direction] != EMPTY || board[1 - player][endX][endY] != EMPTY ) )
		{
			return false;
		}
	}
	else //en passant or capture
	{
		//en passant
		//the pawn is on the correct row (his 5th) and moves in the right direction
		if ( ( startY == base_line + direction * 4 ) && ( endY - startY == direction ) )
		{
			//the enemy pawn on the target column can be captured by en passant
			if ( en_passant_available[1 - player][endX] == 1 && ( ( endX == startX - 1 ) || ( endX == startX + 1 ) ) )
			{
				return true;
			}
		}

		//the pawn doesn't advance exactly one square
		if ( endY - startY != direction )
		{
			return false;
		}
		//the pawn doesn't move exactly one square to the left or right
		if ( endX != startX - 1 && endX != startX + 1 )
		{
			return false;
		}
		//there is no enemy piece to capture
		if ( board[1 - player][endX][endY] == EMPTY )
		{
			return false;
		}
	}

	return true;
}

bool knight_valid_move(int startX, int startY, int endX, int endY)
{
	// The way a knight can move:
	//
	// 0 0 0 0 0 0 0 0
	// 0 0 0 0 0 0 0 0
	// 0 0 0 x 0 x 0 0
	// 0 0 x 0 0 0 x 0
	// 0 0 0 0 2 0 0 0
	// 0 0 x 0 0 0 x 0
	// 0 0 0 x 0 x 0 0
	// 0 0 0 0 0 0 0 0

	// If the desired position is of the same row or column
	if ( (endX == startX) || (endY == startY) )
		return false;

	// If the desired position doesn't have the "L" shape
	if ( (startX - endX) * (startX - endX) + (startY - endY) * (startY - endY) != 5 )
		return false;
	return true;

	// ToDo coliziune + sah
}

bool bishop_valid_move(int startX, int startY, int endX, int endY)
{
	// The way a bishop can move:
	//
	// x 0 0 0 0 0 0 0
	// 0 x 0 0 0 0 0 x
	// 0 0 x 0 0 0 x 0
	// 0 0 0 x 0 x 0 0
	// 0 0 0 0 3 0 0 0
	// 0 0 0 x 0 x 0 0
	// 0 0 x 0 0 0 x 0
	// 0 x 0 0 0 0 0 x

	int directionX, directionY;

	// If the desired position is diagonal
	if (fabs(startX - endX) != fabs(startY - endY))
	{
		return false;
	}

	// Determine the direction on x and y axis
	directionX = ( startX < endX) ? 1 : -1;
	directionY = ( startY < endY) ? 1 : -1;

	// For each square, check for collision with either a friend or a foe
	for (int i = startX + directionX, j = startY + directionY; i != endX && j != endY; i += directionX, j += directionY)
	{
		if (board[WHITE][i][j] != EMPTY || board[BLACK][i][j] != EMPTY)
		{
			return false;
		}
	}

	return true;
}

bool rook_valid_move(int startX, int startY, int endX, int endY)
{
	// The way a rook can move:
	//
	// 0 0 0 0 x 0 0 0
	// 0 0 0 0 x 0 0 0
	// 0 0 0 0 x 0 0 0
	// 0 0 0 0 x 0 0 0
	// x x x x 4 x x x
	// 0 0 0 0 x 0 0 0
	// 0 0 0 0 x 0 0 0
	// 0 0 0 0 x 0 0 0

	int direction;

	// If the desired position is not on either the same row or column
	if ( endX != startX && endY != startY )
	{
		return false;
	}

	if ( endX == startX ) //vertical move
	{
		// For the direction: move left, direction = -1; move right, direction = 1
		direction = ( startY < endY ) ? 1 : -1;

		// For each square, check for collision with either a friend or a foe
		for ( int i = startY + direction; i != endY; i += direction )
		{
			if ( board[WHITE][startX][i] != EMPTY || board[BLACK][startX][i] != EMPTY )
			{
				return false;
			}
		}
	}
	else if ( endY == startY ) //horizontal move
	{
		// For the direction: move down, direction = -1; move up, direction = 1
		direction = ( startX < endX ) ? 1 : -1;

		// For each square, check for collision with either a friend or a foe
		for ( int i = startX + direction; i != endX; i += direction )
		{
			if ( board[WHITE][i][startY] != EMPTY || board[BLACK][i][startY] != EMPTY )
			{
				return false;
			}
		}
	}

	return true;
}

bool queen_valid_move(int startX, int startY, int endX, int endY)
{
	// The way the queen can move:
	//
	// x 0 0 0 x 0 0 0
	// 0 x 0 0 x 0 0 x
	// 0 0 x 0 x 0 x 0
	// 0 0 0 x x x 0 0
	// x x x x 5 x x x
	// 0 0 0 x x x 0 0
	// 0 0 x 0 x 0 x 0
	// 0 x 0 0 x 0 0 x

	// If the move is invalid both as a bishop and a rook
	if ( !(bishop_valid_move(startX, startY, endX, endY) || rook_valid_move(startX, startY, endX, endY)) )
	{
		return false;
	}

	return true;
}

bool king_valid_move(int startX, int startY, int endX, int endY, int player)
{
	// The way the king can move:
	//
	// 0 0 0 0 0 0 0 0 //black's base_line
	// 0 0 0 0 0 0 0 0
	// 0 0 0 0 0 0 0 0
	// 0 0 0 x x x 0 0
	// 0 0 0 x 6 x 0 0
	// 0 0 0 x x x 0 0
	// 0 0 0 0 0 0 0 0
	// 0 0 0 0 0 0 0 0 //white's base_line

	int base_line = ( player == WHITE ) ? 1 : 8;

	//the king is still in its original position
	if ( startX == E && startY == base_line && king_moved[player] == 0 )
	{
		//tries to castle left (queenside)
		if ( endX == C && endY == base_line && left_rook_moved[player] == 0 && board[player][2][base_line] == EMPTY && board[1 - player][2][base_line] == EMPTY && board[player][3][base_line] == EMPTY && board[1 - player][3][base_line] == EMPTY && board[player][4][base_line] == EMPTY && board[1 - player][4][base_line] == EMPTY )
		{
			return !(is_attacked(E, base_line, player) || is_attacked(D, base_line, player) || is_attacked(C, base_line, player));
		}

		//tries to castle right (kingside)
		if ( endX == G && endY == base_line && right_rook_moved[player] == 0 && board[player][6][base_line] == EMPTY && board[1 - player][6][base_line] == EMPTY && board[player][7][base_line] == EMPTY && board[1 - player][7][base_line] == EMPTY )
		{
			return !(is_attacked(E, base_line, player) || is_attacked(F, base_line, player) || is_attacked(G, base_line, player));
		}
	}

	//normal move
	if ( ( endX - startX ) * ( endX - startX ) + ( endY - startY ) * ( endY - startY ) > 2 )
	{
		return false;
	}

	return true;
}
