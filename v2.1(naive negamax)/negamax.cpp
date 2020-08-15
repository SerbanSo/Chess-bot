#include "negamax.h"

int piece_value(int piece)
{
    if ( piece == PAWN )
    {
        return 100;
    }
    else if ( piece == KNIGHT )
    {
        return 320;
    }
    else if ( piece == BISHOP )
    {
        return 330;
    }
    else if ( piece == ROOK )
    {
        return 500;
    }
    else if ( piece == QUEEN )
    {
        return 900;
    }
    else if ( piece == KING )
    {
        return 100000;
    }

    return 0;
}

//returns the evaluation score in centipawns, from the current player's perspective
int board_evaluation(int board[2][9][9], position pieces_positions[2][16], int pieces_number[2], int king_moved[2], int left_rook_moved[2], int right_rook_moved[2], int en_passant_available[2][9], position king_position[2], int include_table, int player)
{
    int material_eval = 0;
    //int mobility_eval = 0;

    for ( int i = 0; i < pieces_number[player]; i++ )
    {
        material_eval += piece_value(board[player][pieces_positions[player][i].x][pieces_positions[player][i].y]);
        material_eval += include_table & piece_value_table[player][board[player][pieces_positions[player][i].x][pieces_positions[player][i].y]][pieces_positions[player][i].x][pieces_positions[player][i].y];
    }
    //mobility_eval += generate_moves(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, NULL, player);

    for ( int i = 0; i < pieces_number[1 - player]; i++ )
    {
        material_eval -= piece_value(board[1 - player][pieces_positions[1 - player][i].x][pieces_positions[1 - player][i].y]);
        material_eval -= include_table & piece_value_table[1 - player][board[1 - player][pieces_positions[1 - player][i].x][pieces_positions[1 - player][i].y]][pieces_positions[1 - player][i].x][pieces_positions[1 - player][i].y];
    }
    //mobility_eval -= generate_moves(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, NULL, 1 - player);

    return material_eval;
}

void copy_game_state(int board[2][9][9], position pieces_positions[2][16], int pieces_number[2], int king_moved[2], int left_rook_moved[2], int right_rook_moved[2], int en_passant_available[2][9], position king_position[2],
                     int board_copy[2][9][9], position pieces_positions_copy[2][16], int pieces_number_copy[2], int king_moved_copy[2], int left_rook_moved_copy[2], int right_rook_moved_copy[2], int en_passant_available_copy[2][9], position king_position_copy[2])
{
    memcpy(board_copy, board, 162 * sizeof(int));
    memcpy(pieces_positions_copy, pieces_positions, 32 * sizeof(position));
    memcpy(pieces_number_copy, pieces_number, 2 * sizeof(int));
    memcpy(king_moved_copy, king_moved, 2 * sizeof(int));
    memcpy(left_rook_moved_copy, left_rook_moved, 2 * sizeof(int));
    memcpy(right_rook_moved_copy, right_rook_moved, 2 * sizeof(int));
    memcpy(en_passant_available_copy, en_passant_available, 18 * sizeof(int));
    memcpy(king_position_copy, king_position, 2 * sizeof(position));
}

bool threefold_repetition(int player)
{
    int repetitions = 0;

    for ( int i = (player == BLACK); i < move_number - 1; i += 2 )
    {
        if ( !strcmp(past_moves[i], past_moves[move_number - 2]) && !strcmp(past_moves[i + 1], past_moves[move_number - 1]) )
        {
            repetitions++;

            if ( repetitions >= 2 )
            {
                return true;
            }
        }
    }

    return false;
}

void find_best_move(int board[2][9][9], position pieces_positions[2][16], int pieces_number[2], int king_moved[2], int left_rook_moved[2], int right_rook_moved[2], int en_passant_available[2][9], position king_position[2], int player, int depth)
{
    char best_move[5];
    best_move[0] = 0;
    char second_best_move[5];
    second_best_move[0] = 0;

    int eval = negamax(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, second_best_move, best_move, player, depth);

    // forced checkmate sequence detected
    if ( eval == MINUS_INF )
    {
        if ( second_best_move[0] == 0 ) // there are no legal moves
        {
            std::cout << "resign\n";
        }
        else // the best move is null but the opponent might not go down the checkmate variation so we make the second best move
        {
            // Updates the engine's matrices
            make_move(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, second_best_move, engine);

            //fprintf(stderr, "%s\n", second_best_move);
            // Send the move to xboard
            std::cout << "move " << second_best_move << "\n";

            past_moves[move_number][0] = second_best_move[0];
            past_moves[move_number][1] = second_best_move[1];
            past_moves[move_number][2] = second_best_move[2];
            past_moves[move_number][3] = second_best_move[3];
            past_moves[move_number][4] = second_best_move[4];
            move_number++;
        }

        return;
    }

    if ( threefold_repetition(player) )
    {
        if ( eval >= -200 && board_evaluation(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, 0, player) >= -200 ) // tries to avoid threefold repetition by making the second best move
        {
            // Updates the engine's matrices
            make_move(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, second_best_move, engine);

            //fprintf(stderr, "%s\n", second_best_move);
            // Send the move to xboard
            std::cout << "move " << second_best_move << "\n";

            past_moves[move_number][0] = second_best_move[0];
            past_moves[move_number][1] = second_best_move[1];
            past_moves[move_number][2] = second_best_move[2];
            past_moves[move_number][3] = second_best_move[3];
            past_moves[move_number][4] = second_best_move[4];
            move_number++;

            return;
        }
    }
    //fprintf(stderr, "%d %d %s %s\n", eval, board_evaluation(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, 0, player), best_move, second_best_move);

    // Updates the engine's matrices
    make_move(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, best_move, engine);

    // Send the move to xboard
    std::cout << "move " << best_move << "\n";

    past_moves[move_number][0] = best_move[0];
    past_moves[move_number][1] = best_move[1];
    past_moves[move_number][2] = best_move[2];
    past_moves[move_number][3] = best_move[3];
    past_moves[move_number][4] = best_move[4];
    move_number++;
}

int negamax(int board[2][9][9], position pieces_positions[2][16], int pieces_number[2], int king_moved[2], int left_rook_moved[2], int right_rook_moved[2], int en_passant_available[2][9], position king_position[2], char second_best_move[5], char best_move[5], int player, int depth)
{
    if ( depth == 0 )//game_over(board) || depth == 0 )
    {
        return board_evaluation(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, 0xffffffff, player);
    }

    double max_eval = MINUS_INF;

    int board_copy[2][9][9];
    position pieces_positions_copy[2][16];
    int pieces_number_copy[2];
    char best_move_copy[5];
    char second_best_move_copy[5];

    int king_moved_copy[2];
    int left_rook_moved_copy[2];
    int right_rook_moved_copy[2];
    int en_passant_available_copy[2][9];
    position king_position_copy[2];

    char valid_moves[64][5];
    int valid_moves_number;

    valid_moves_number = generate_moves(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, valid_moves, player);

    for ( int i = 0; i < valid_moves_number; i++ )
    {
        copy_game_state(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, board_copy, pieces_positions_copy, pieces_number_copy, king_moved_copy, left_rook_moved_copy, right_rook_moved_copy, en_passant_available_copy, king_position_copy);

        make_move(board_copy, pieces_positions_copy, pieces_number_copy, king_moved_copy, left_rook_moved_copy, right_rook_moved_copy, en_passant_available_copy, king_position_copy, valid_moves[i], player);

        int eval = -1 * negamax(board_copy, pieces_positions_copy, pieces_number_copy, king_moved_copy, left_rook_moved_copy, right_rook_moved_copy, en_passant_available_copy, king_position_copy, second_best_move_copy, best_move_copy, 1 - player, depth - 1);

        //fprintf(stderr, "%s %d %s %lf\n", ( player == engine ) ? "engine" : "opponent", depth, valid_moves[i], eval);

        if ( eval > max_eval )
        {
            max_eval = eval;

            second_best_move[0] = best_move[0];
            second_best_move[1] = best_move[1];
            second_best_move[2] = best_move[2];
            second_best_move[3] = best_move[3];
            second_best_move[4] = best_move[4];

            best_move[0] = valid_moves[i][0];
            best_move[1] = valid_moves[i][1];
            best_move[2] = valid_moves[i][2];
            best_move[3] = valid_moves[i][3];
            best_move[4] = valid_moves[i][4];
        }
        else if ( eval == max_eval )
        {
            second_best_move[0] = valid_moves[i][0];
            second_best_move[1] = valid_moves[i][1];
            second_best_move[2] = valid_moves[i][2];
            second_best_move[3] = valid_moves[i][3];
            second_best_move[4] = valid_moves[i][4];
        }
    }

    //fprintf(stderr, "%d %s %d\n", depth, best_move, max_eval);

    return max_eval;
}

int generate_moves(int board[2][9][9], position pieces_positions[2][16], int pieces_number[2], int king_moved[2], int left_rook_moved[2], int right_rook_moved[2], int en_passant_available[2][9], position king_position[2],  char valid_moves[64][5], int player)
{
    int moves_number = 0;

    for ( int i = 0; i < pieces_number[player]; i++ )
    {
        //fprintf(stderr, "%d %d\n", pieces_positions[player][i].x, pieces_positions[player][i].y);

        if ( board[player][pieces_positions[player][i].x][pieces_positions[player][i].y] == PAWN )
        {
            generate_moves_pawn(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, pieces_positions[player][i], valid_moves, &moves_number, player);
        }
        else if ( board[player][pieces_positions[player][i].x][pieces_positions[player][i].y] == KNIGHT )
        {
            generate_moves_knight(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, pieces_positions[player][i], valid_moves, &moves_number, player);
        }
        else if ( board[player][pieces_positions[player][i].x][pieces_positions[player][i].y] == BISHOP )
        {
            generate_moves_bishop(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, pieces_positions[player][i], valid_moves, &moves_number, player);
        }
        else if ( board[player][pieces_positions[player][i].x][pieces_positions[player][i].y] == ROOK )
        {
            generate_moves_rook(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, pieces_positions[player][i], valid_moves, &moves_number, player);
        }
        else if ( board[player][pieces_positions[player][i].x][pieces_positions[player][i].y] == QUEEN )
        {
            generate_moves_queen(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, pieces_positions[player][i], valid_moves, &moves_number, player);
        }
        else if ( board[player][pieces_positions[player][i].x][pieces_positions[player][i].y] == KING )
        {
            generate_moves_king(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, pieces_positions[player][i], valid_moves, &moves_number, player);
        }
    }

    return moves_number;
}

void add_valid_move(char move[5], char valid_moves[64][5], int *moves_number)
{
    if ( valid_moves != NULL )
    {
        valid_moves[(*moves_number)][0] = move[0];
        valid_moves[(*moves_number)][1] = move[1];
        valid_moves[(*moves_number)][2] = move[2];
        valid_moves[(*moves_number)][3] = move[3];
        valid_moves[(*moves_number)][4] = move[4];
    }

    (*moves_number)++;
}

void generate_moves_pawn(int board[2][9][9], position pieces_positions[2][16], int pieces_number[2], int king_moved[2], int left_rook_moved[2], int right_rook_moved[2], int en_passant_available[2][9], position king_position[2], position start, char valid_moves[64][5], int *moves_number, int player)
{
    char move[5];
    move[0] = 'a' + start.x - 1;
    move[1] = '0' + start.y;
    move[4] = 0;

    int direction = ( player == WHITE ) ? 1 : -1;
	int base_line = ( player == WHITE ) ? 1 : 8;
	int opposite_line = 9 - base_line;

    //avanseaza 1 linie
    move[2] = move[0];
    move[3] = move[1] + direction;
    if ( is_move_valid(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, move, player) )
    {
        if ( start.y == opposite_line - direction ) //promotion
        {
            move[4] = 'n';
            add_valid_move(move, valid_moves, moves_number);

            move[4] = 'b';
            add_valid_move(move, valid_moves, moves_number);

            move[4] = 'r';
            add_valid_move(move, valid_moves, moves_number);

            move[4] = 'q';
            add_valid_move(move, valid_moves, moves_number);
        }
        else
        {
            add_valid_move(move, valid_moves, moves_number);
        }
    }

    //avanseaza 2 linii
    if ( start.y == ( base_line + direction ) )
    {
        move[2] = move[0];
        move[3] = move[1] + 2 * direction;
        if ( is_move_valid(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, move, player) )
        {
            add_valid_move(move, valid_moves, moves_number);
        }
    }

    //captureaza la stanga
    move[2] = move[0] - 1;
    move[3] = move[1] + direction;
    if ( is_move_valid(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, move, player) )
    {
        add_valid_move(move, valid_moves, moves_number);
    }

    //captureaza la dreapta
    move[2] = move[0] + 1;
    move[3] = move[1] + direction;
    if ( is_move_valid(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, move, player) )
    {
        add_valid_move(move, valid_moves, moves_number);
    }
}

void generate_moves_knight(int board[2][9][9], position pieces_positions[2][16], int pieces_number[2], int king_moved[2], int left_rook_moved[2], int right_rook_moved[2], int en_passant_available[2][9], position king_position[2], position start, char valid_moves[64][5], int *moves_number, int player)
{
    char move[5];
    move[0] = 'a' + start.x - 1;
    move[1] = '0' + start.y;
    move[4] = 0;

    move[2] = move[0] + 1;
    move[3] = move[1] + 2;
    if ( is_move_valid(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, move, player) )
    {
        add_valid_move(move, valid_moves, moves_number);
    }

    move[2] = move[0] + 2;
    move[3] = move[1] + 1;
    if ( is_move_valid(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, move, player) )
    {
        add_valid_move(move, valid_moves, moves_number);
    }

    move[2] = move[0] + 2;
    move[3] = move[1] - 1;
    if ( is_move_valid(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, move, player) )
    {
        add_valid_move(move, valid_moves, moves_number);
    }

    move[2] = move[0] + 1;
    move[3] = move[1] - 2;
    if ( is_move_valid(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, move, player) )
    {
        add_valid_move(move, valid_moves, moves_number);
    }

    move[2] = move[0] - 1;
    move[3] = move[1] - 2;
    if ( is_move_valid(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, move, player) )
    {
        add_valid_move(move, valid_moves, moves_number);
    }

    move[2] = move[0] - 2;
    move[3] = move[1] - 1;
    if ( is_move_valid(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, move, player) )
    {
        add_valid_move(move, valid_moves, moves_number);
    }

    move[2] = move[0] - 2;
    move[3] = move[1] + 1;
    if ( is_move_valid(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, move, player) )
    {
        add_valid_move(move, valid_moves, moves_number);
    }

    move[2] = move[0] - 1;
    move[3] = move[1] + 2;
    if ( is_move_valid(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, move, player) )
    {
        add_valid_move(move, valid_moves, moves_number);
    }
}

void generate_moves_bishop(int board[2][9][9], position pieces_positions[2][16], int pieces_number[2], int king_moved[2], int left_rook_moved[2], int right_rook_moved[2], int en_passant_available[2][9], position king_position[2], position start, char valid_moves[64][5], int *moves_number, int player)
{
    char move[5];
    move[0] = 'a' + start.x - 1;
    move[1] = '0' + start.y;
    move[4] = 0;

    for ( int i = -1, j = 1; start.x + i >= A && start.y + j <= 8; i--, j++ ) // in stanga sus
    {
        move[2] = move[0] + i;
        move[3] = move[1] + j;

        if ( is_move_valid(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, move, player) )
        {
            add_valid_move(move, valid_moves, moves_number);
        }
    }

    for ( int i = 1, j = 1; start.x + i <= H && start.y + j <= 8; i++, j++ ) // in dreapta sus
    {
        move[2] = move[0] + i;
        move[3] = move[1] + j;

        if ( is_move_valid(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, move, player) )
        {
            add_valid_move(move, valid_moves, moves_number);
        }
    }

    for ( int i = 1, j = -1; start.x + i <= H && start.y + j >= 1; i++, j-- ) // in dreapta jos
    {
        move[2] = move[0] + i;
        move[3] = move[1] + j;

        if ( is_move_valid(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, move, player) )
        {
            add_valid_move(move, valid_moves, moves_number);
        }
    }

    for ( int i = -1, j = -1; start.x + i >= A && start.y + j >= 1; i--, j-- ) // in stanga jos
    {
        move[2] = move[0] + i;
        move[3] = move[1] + j;

        if ( is_move_valid(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, move, player) )
        {
            add_valid_move(move, valid_moves, moves_number);
        }
    }
}

void generate_moves_rook(int board[2][9][9], position pieces_positions[2][16], int pieces_number[2], int king_moved[2], int left_rook_moved[2], int right_rook_moved[2], int en_passant_available[2][9], position king_position[2], position start, char valid_moves[64][5], int *moves_number, int player)
{
    char move[5];
    move[0] = 'a' + start.x - 1;
    move[1] = '0' + start.y;
    move[4] = 0;

    for ( int i = 1; start.y + i <= 8; i++ ) // in sus
    {
        move[2] = move[0];
        move[3] = move[1] + i;

        if ( is_move_valid(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, move, player) )
        {
            add_valid_move(move, valid_moves, moves_number);
        }
    }

    for ( int i = -1; start.y + i >= 1; i-- ) // in jos
    {
        move[2] = move[0];
        move[3] = move[1] + i;

        if ( is_move_valid(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, move, player) )
        {
            add_valid_move(move, valid_moves, moves_number);
        }
    }

    for ( int i = -1; start.x + i >= A; i-- ) // in stanga
    {
        move[2] = move[0] + i;
        move[3] = move[1];

        if ( is_move_valid(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, move, player) )
        {
            add_valid_move(move, valid_moves, moves_number);
        }
    }

    for ( int i = 1; start.x + i <= H; i++ ) // in dreapta
    {
        move[2] = move[0] + i;
        move[3] = move[1];

        if ( is_move_valid(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, move, player) )
        {
            add_valid_move(move, valid_moves, moves_number);
        }
    }
}

void generate_moves_queen(int board[2][9][9], position pieces_positions[2][16], int pieces_number[2], int king_moved[2], int left_rook_moved[2], int right_rook_moved[2], int en_passant_available[2][9], position king_position[2], position start, char valid_moves[64][5], int *moves_number, int player)
{
    generate_moves_bishop(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, start, valid_moves, moves_number, player);
    generate_moves_rook(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, start, valid_moves, moves_number, player);
}

void generate_moves_king(int board[2][9][9], position pieces_positions[2][16], int pieces_number[2], int king_moved[2], int left_rook_moved[2], int right_rook_moved[2], int en_passant_available[2][9], position king_position[2], position start, char valid_moves[64][5], int *moves_number, int player)
{
    char move[5];
    move[0] = 'a' + start.x - 1;
    move[1] = '0' + start.y;
    move[4] = 0;

    move[2] = move[0];
    move[3] = move[1] + 1;
    if ( is_move_valid(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, move, player) )
    {
        add_valid_move(move, valid_moves, moves_number);
    }

    move[2] = move[0] + 1;
    move[3] = move[1] + 1;
    if ( is_move_valid(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, move, player) )
    {
        add_valid_move(move, valid_moves, moves_number);
    }

    move[2] = move[0] + 1;
    move[3] = move[1];
    if ( is_move_valid(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, move, player) )
    {
        add_valid_move(move, valid_moves, moves_number);
    }

    move[2] = move[0] + 1;
    move[3] = move[1] - 1;
    if ( is_move_valid(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, move, player) )
    {
        add_valid_move(move, valid_moves, moves_number);
    }

    move[2] = move[0];
    move[3] = move[1] - 1;
    if ( is_move_valid(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, move, player) )
    {
        add_valid_move(move, valid_moves, moves_number);
    }

    move[2] = move[0] - 1;
    move[3] = move[1] - 1;
    if ( is_move_valid(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, move, player) )
    {
        add_valid_move(move, valid_moves, moves_number);
    }

    move[2] = move[0] - 1;
    move[3] = move[1];
    if ( is_move_valid(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, move, player) )
    {
        add_valid_move(move, valid_moves, moves_number);
    }

    move[2] = move[0] - 1;
    move[3] = move[1] + 1;
    if ( is_move_valid(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, move, player) )
    {
        add_valid_move(move, valid_moves, moves_number);
    }

    int base_line = ( player == WHITE ) ? 1 : 8;
    // castling
    if ( start.x == E && start.y == base_line && king_moved[player] == 0 )
    {
        move[2] = move[0] - 2;
        move[3] = move[1];
        if ( is_move_valid(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, move, player) )
        {
            add_valid_move(move, valid_moves, moves_number);
        }

        move[2] = move[0] + 2;
        move[3] = move[1];
        if ( is_move_valid(board, pieces_positions, pieces_number, king_moved, left_rook_moved, right_rook_moved, en_passant_available, king_position, move, player) )
        {
            add_valid_move(move, valid_moves, moves_number);
        }
    }
}
