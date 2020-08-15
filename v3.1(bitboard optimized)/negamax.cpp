#include "negamax.h"
#include "eval.h"

// Used in moves_cmp_fct()
int gplayer;

int piece_value(int piece)
{
    if ( piece == PAWN )
    {
        // return 100;
        return 128;
    }
    else if ( piece == KNIGHT )
    {
        // return 320;
        return 781;
    }
    else if ( piece == BISHOP )
    {
        // return 330;
        return 825;
    }
    else if ( piece == ROOK )
    {
        // return 500;
        return 1276;
    }
    else if ( piece == QUEEN )
    {
        // return 900;
        return 2538;
    }
    else if ( piece == KING )
    {
        // return 20000;
        return 100000;
    }

    return 0;
}

int board_evaluation(int player) {
    int material_eval = 0;

    material_eval += piece_value(PAWN) * __builtin_popcountll(board.pawns[player]);
    material_eval += piece_value(KNIGHT) * __builtin_popcountll(board.knights[player]);
    material_eval += piece_value(ROOK) * __builtin_popcountll(board.rooks[player]);
    material_eval += piece_value(BISHOP) * __builtin_popcountll(board.bishops[player]);
    material_eval += piece_value(QUEEN) * __builtin_popcountll(board.queens[player]);
    material_eval += piece_value(KING) * __builtin_popcountll(board.king[player]);

    material_eval -= piece_value(PAWN) * __builtin_popcountll(board.pawns[1-player]);
    material_eval -= piece_value(KNIGHT) * __builtin_popcountll(board.knights[1-player]);
    material_eval -= piece_value(ROOK) * __builtin_popcountll(board.rooks[1-player]);
    material_eval -= piece_value(BISHOP) * __builtin_popcountll(board.bishops[1-player]);
    material_eval -= piece_value(QUEEN) * __builtin_popcountll(board.queens[1-player]);
    material_eval -= piece_value(KING) * __builtin_popcountll(board.king[1-player]);

    return material_eval;
}

int king_distance_bonus(int player, unsigned long long position) {
    int pos = ffsll(position) - 1;
    int king_pos = ffsll(board.king[player]) - 1;
    int x1 = pos/8;
    int y1 = pos%8;
    int x2 = king_pos/8;
    int y2 = king_pos%8;

    switch(max(abs(x1-x2), abs(y1-y2))) {
        case 1:
            return 100;
        case 2:
            return 50;
        case 3:
            return 25;
        case 4:
            return 10;
        default:
            return 0;
    }
}

int piece_evaluation(unsigned long long position, int player, vector<int> piece_moves, Bitboard oldboard) {
    int pos = ffsll(position)-1;

    int row = pos%8;
    int col = pos/8;

    int sum = 0;

    if(game_time == MIDDLEGAME) {
        if(oldboard.pawns[player] & position) {
            sum += piece_value(PAWN); // piece value
            sum += pawn_piece_per_square_bonus_mg[player][row][col]; // bonus for current square
            sum += 5 * __builtin_popcountll(pawn_attacks[player][pos] & board.board[1-player]); // how much the piece can attack
            // sum += 10 * piece_moves[PAWN]; // useless?!?!?, 

            // bonus if it is a passed pawn
            // 450
            if(player == WHITE) {
                switch(row) {
                    case 0:
                        if(!(((~notAFile | ~notBFile) & board.pawns[1-player]) >> (pos+2))) {
                            sum += 100;
                        }
                        break;
                    case 1:
                        if(!(((~notAFile | ~notBFile | ~notCFile) & board.pawns[1-player]) >> (pos+2))) {
                            sum += 100;
                        }
                        break;
                    case 2:
                        if(!(((~notBFile | ~notCFile | ~notDFile) & board.pawns[1-player]) >> (pos+2))) {
                            sum += 100;
                        }
                        break;
                    case 3:
                        if(!(((~notCFile | ~notDFile | ~notEFile) & board.pawns[1-player]) >> (pos+2))) {
                            sum += 100;
                        }
                        break;
                    case 4:
                        if(!(((~notDFile | ~notEFile | ~notFFile) & board.pawns[1-player]) >> (pos+2))) {
                            sum += 100;
                        }
                        break;
                    case 5:
                        if(!(((~notEFile | ~notFFile | ~notGFile) & board.pawns[1-player]) >> (pos+2))) {
                            sum += 100;
                        }
                        break;
                    case 6:
                        if(!(((~notFFile | ~notGFile | ~notHFile) & board.pawns[1-player]) >> (pos+2))) {
                            sum += 100;
                        }
                        break;
                    case 7:
                        if(!(((~notGFile | ~notHFile) & board.pawns[1-player]) >> (pos+2))) {
                            sum += 100;
                        }
                        break;
                }
            } else {
                switch(row) {
                    case 0:
                        if(!(((~notAFile | ~notBFile) & board.pawns[1-player]) << (pos+2))) {
                            sum += 100;
                        }
                        break;
                    case 1:
                        if(!(((~notAFile | ~notBFile | ~notCFile) & board.pawns[1-player]) << (pos+2))) {
                            sum += 100;
                        }
                        break;
                    case 2:
                        if(!(((~notBFile | ~notCFile | ~notDFile) & board.pawns[1-player]) << (pos+2))) {
                            sum += 100;
                        }
                        break;
                    case 3:
                        if(!(((~notCFile | ~notDFile | ~notEFile) & board.pawns[1-player]) << (pos+2))) {
                            sum += 100;
                        }
                        break;
                    case 4:
                        if(!(((~notDFile | ~notEFile | ~notFFile) & board.pawns[1-player]) << (pos+2))) {
                            sum += 100;
                        }
                        break;
                    case 5:
                        if(!(((~notEFile | ~notFFile | ~notGFile) & board.pawns[1-player]) << (pos+2))) {
                            sum += 100;
                        }
                        break;
                    case 6:
                        if(!(((~notFFile | ~notGFile | ~notHFile) & board.pawns[1-player]) << (pos+2))) {
                            sum += 100;
                        }
                        break;
                    case 7:
                        if(!(((~notGFile | ~notHFile) & board.pawns[1-player]) << (pos+2))) {
                            sum += 100;
                        }
                        break;
                }
            }

            // return piece_value(PAWN) + pawn_piece_per_square_bonus_mg[player][row][col] + 5 * __builtin_popcountll(pawn_attacks[player][pos] & board.board[1-player]) + 10 * piece_moves[PAWN];
        } else if(oldboard.knights[player] & position) {
            sum += piece_value(KNIGHT);
            sum += knight_piece_per_square_bonus_mg[player][row][col];
            sum += 30 * __builtin_popcountll(knight_moves[pos] & board.board[1-player]);
            // sum += knight_mg_mobility[piece_moves[KNIGHT]]; // bonus for possible moves
            // return piece_value(KNIGHT) + knight_piece_per_square_bonus_mg[player][row][col] + 30 * __builtin_popcountll(knight_moves[pos] & board.board[1-player]) + knight_mg_mobility[piece_moves[KNIGHT]];
        } else if(oldboard.rooks[player] & position){
            sum += piece_value(ROOK);
            sum += rook_piece_per_square_bonus_mg[player][row][col];
            sum += 20 * __builtin_popcountll(rook_attack(pos, board.board[1-player]));
            // sum += rook_mg_mobility[piece_moves[ROOK]];
            // return piece_value(ROOK) + rook_piece_per_square_bonus_mg[player][row][col] + 20 * __builtin_popcountll(rook_attack(pos, board.board[1-player])) + rook_mg_mobility[piece_moves[ROOK]];
        } else if(oldboard.bishops[player] & position) {
            sum += piece_value(BISHOP);
            sum += bishop_piece_per_square_bonus_mg[player][row][col];
            sum += 20 * __builtin_popcountll(bishop_attack(pos, board.board[1-player]));
            sum += (__builtin_popcountll(board.bishops[player]) == 2 ? 100 : 0);
            // sum += bishop_mg_mobility[piece_moves[BISHOP]];
            // return piece_value(BISHOP) + bishop_piece_per_square_bonus_mg[player][prow][col] + 20 * __builtin_popcountll(bishop_attack(pos, board.board[1-player]))
            // + (__builtin_popcountll(board.bishops[player]) == 2 ? 100 : 0) + bishop_mg_mobility[piece_moves[BISHOP]];
        } else if(oldboard.queens[player] & position) {
            sum += piece_value(QUEEN);
            sum += queen_piece_per_square_bonus_mg[player][row][col];
            sum += 20 * __builtin_popcountll(queen_attack(pos, board.board[1-player]));
            // sum += queen_mg_mobility[piece_moves[QUEEN]];
            // return piece_value(QUEEN) + queen_piece_per_square_bonus_mg[player][row][col] + 20 * __builtin_popcountll(queen_attack(pos, board.board[1-player])) + queen_mg_mobility[piece_moves[QUEEN]];
        } else if(oldboard.king[player] & position) {
            sum += piece_value(KING);
            sum += king_piece_per_square_bonus_mg[player][row][col];
            sum += 10 * __builtin_popcountll(king_moves[pos] & board.board[1-player]);
            // return piece_value(KING) + king_piece_per_square_bonus_mg[player][row][col] + 10 * __builtin_popcountll(king_moves[pos] & board.board[1-player]);
        }
    } else {
        if(oldboard.pawns[player] & position) {
            sum += piece_value(PAWN);
            sum += pawn_piece_per_square_bonus_eg[player][row][col];
            sum += 5 * __builtin_popcountll(pawn_attacks[player][pos] & board.board[1-player]);
            // sum += 10 * piece_moves[PAWN];

            if(player == WHITE) {
                switch(row) {
                    case 0:
                        if(!(((~notAFile | ~notBFile) & board.pawns[1-player]) >> (pos+2))) {
                            sum += 100;
                        }
                        break;
                    case 1:
                        if(!(((~notAFile | ~notBFile | ~notCFile) & board.pawns[1-player]) >> (pos+2))) {
                            sum += 100;
                        }
                        break;
                    case 2:
                        if(!(((~notBFile | ~notCFile | ~notDFile) & board.pawns[1-player]) >> (pos+2))) {
                            sum += 100;
                        }
                        break;
                    case 3:
                        if(!(((~notCFile | ~notDFile | ~notEFile) & board.pawns[1-player]) >> (pos+2))) {
                            sum += 100;
                        }
                        break;
                    case 4:
                        if(!(((~notDFile | ~notEFile | ~notFFile) & board.pawns[1-player]) >> (pos+2))) {
                            sum += 100;
                        }
                        break;
                    case 5:
                        if(!(((~notEFile | ~notFFile | ~notGFile) & board.pawns[1-player]) >> (pos+2))) {
                            sum += 100;
                        }
                        break;
                    case 6:
                        if(!(((~notFFile | ~notGFile | ~notHFile) & board.pawns[1-player]) >> (pos+2))) {
                            sum += 100;
                        }
                        break;
                    case 7:
                        if(!(((~notGFile | ~notHFile) & board.pawns[1-player]) >> (pos+2))) {
                            sum += 100;
                        }
                        break;
                }
            } else {
                switch(row) {
                    case 0:
                        if(!(((~notAFile | ~notBFile) & board.pawns[1-player]) << (pos+2))) {
                            sum += 100;
                        }
                        break;
                    case 1:
                        if(!(((~notAFile | ~notBFile | ~notCFile) & board.pawns[1-player]) << (pos+2))) {
                            sum += 100;
                        }
                        break;
                    case 2:
                        if(!(((~notBFile | ~notCFile | ~notDFile) & board.pawns[1-player]) << (pos+2))) {
                            sum += 100;
                        }
                        break;
                    case 3:
                        if(!(((~notCFile | ~notDFile | ~notEFile) & board.pawns[1-player]) << (pos+2))) {
                            sum += 100;
                        }
                        break;
                    case 4:
                        if(!(((~notDFile | ~notEFile | ~notFFile) & board.pawns[1-player]) << (pos+2))) {
                            sum += 100;
                        }
                        break;
                    case 5:
                        if(!(((~notEFile | ~notFFile | ~notGFile) & board.pawns[1-player]) << (pos+2))) {
                            sum += 100;
                        }
                        break;
                    case 6:
                        if(!(((~notFFile | ~notGFile | ~notHFile) & board.pawns[1-player]) << (pos+2))) {
                            sum += 100;
                        }
                        break;
                    case 7:
                        if(!(((~notGFile | ~notHFile) & board.pawns[1-player]) << (pos+2))) {
                            sum += 100;
                        }
                        break;
                }
            }

            // return piece_value(PAWN) + pawn_piece_per_square_bonus_mg[player][row][col] + 5 * __builtin_popcountll(pawn_attacks[player][pos] & board.board[1-player]) + 10 * piece_moves[PAWN];
        } else if(oldboard.knights[player] & position) {
            sum += piece_value(KNIGHT);
            sum += knight_piece_per_square_bonus_eg[player][row][col];
            sum += 30 * __builtin_popcountll(knight_moves[pos] & board.board[1-player]);
            // sum += knight_eg_mobility[piece_moves[KNIGHT]];
            // return piece_value(KNIGHT) + knight_piece_per_square_bonus_mg[player][row][col] + 30 * __builtin_popcountll(knight_moves[pos] & board.board[1-player]) + knight_mg_mobility[piece_moves[KNIGHT]];
        } else if(oldboard.rooks[player] & position){
            sum += piece_value(ROOK);
            sum += rook_piece_per_square_bonus_eg[player][row][col];
            sum += 20 * __builtin_popcountll(rook_attack(pos, board.board[1-player]));
            // sum += rook_eg_mobility[piece_moves[ROOK]];
            // return piece_value(ROOK) + rook_piece_per_square_bonus_mg[player][row][col] + 20 * __builtin_popcountll(rook_attack(pos, board.board[1-player])) + rook_mg_mobility[piece_moves[ROOK]];
        } else if(oldboard.bishops[player] & position) {
            sum += piece_value(BISHOP);
            sum += bishop_piece_per_square_bonus_eg[player][row][col];
            sum += 20 * __builtin_popcountll(bishop_attack(pos, board.board[1-player]));
            sum += (__builtin_popcountll(board.bishops[player]) == 2 ? 100 : 0);
            // sum += bishop_eg_mobility[piece_moves[BISHOP]];
            // return piece_value(BISHOP) + bishop_piece_per_square_bonus_mg[player][prow][col] + 20 * __builtin_popcountll(bishop_attack(pos, board.board[1-player]))
            // + (__builtin_popcountll(board.bishops[player]) == 2 ? 100 : 0) + bishop_mg_mobility[piece_moves[BISHOP]];
        } else if(oldboard.queens[player] & position) {
            sum += piece_value(QUEEN);
            sum += queen_piece_per_square_bonus_eg[player][row][col];
            sum += 20 * __builtin_popcountll(queen_attack(pos, board.board[1-player]));
            // sum += queen_eg_mobility[piece_moves[QUEEN]];
            // return piece_value(QUEEN) + queen_piece_per_square_bonus_mg[player][row][col] + 20 * __builtin_popcountll(queen_attack(pos, board.board[1-player])) + queen_mg_mobility[piece_moves[QUEEN]];
        } else if(oldboard.king[player] & position) {
            sum += piece_value(KING);
            sum += king_piece_per_square_bonus_eg[player][row][col];
            sum += 10 * __builtin_popcountll(king_moves[pos] & board.board[1-player]);
            // return piece_value(KING) + king_piece_per_square_bonus_mg[player][row][col] + 10 * __builtin_popcountll(king_moves[pos] & board.board[1-player]);
        }
    }


    return sum;
}

int player_evaluation(int player) {
    int material_eval = 0;

    unsigned long long tmp_piece = board.pawns[player];
    while(tmp_piece != 0) {
        material_eval += piece_value(PAWN);

        tmp_piece &= tmp_piece-1;
    }

    tmp_piece = board.knights[player];
    while(tmp_piece != 0) {
        material_eval += piece_value(KNIGHT);

        tmp_piece &= tmp_piece-1;
    }

    tmp_piece = board.rooks[player];
    while(tmp_piece != 0) {
        material_eval += piece_value(ROOK);

        tmp_piece &= tmp_piece-1;
    }

    tmp_piece = board.bishops[player];
    while(tmp_piece != 0) {
        material_eval += piece_value(BISHOP);

        tmp_piece &= tmp_piece-1;
    }

    tmp_piece = board.queens[player];
    while(tmp_piece != 0) {
        material_eval += piece_value(QUEEN);

        tmp_piece &= tmp_piece-1;
    }

    tmp_piece = board.king[player];
    while(tmp_piece != 0) {
        material_eval += piece_value(KING);

        tmp_piece &= tmp_piece-1;
    }

    return material_eval;
}

bool identical_game_states(GameState g1, GameState g2)
{
    //compare board data
    if ( g1.board.board[WHITE] != g2.board.board[WHITE] || g1.board.board[BLACK] != g2.board.board[BLACK] )
    {
        return false;
    }
    if ( g1.board.pawns[WHITE] != g2.board.pawns[WHITE] || g1.board.pawns[BLACK] != g2.board.pawns[BLACK] )
    {
        return false;
    }
    if ( g1.board.knights[WHITE] != g2.board.knights[WHITE] || g1.board.knights[BLACK] != g2.board.knights[BLACK] )
    {
        return false;
    }
    if ( g1.board.bishops[WHITE] != g2.board.bishops[WHITE] || g1.board.bishops[BLACK] != g2.board.bishops[BLACK] )
    {
        return false;
    }
    if ( g1.board.rooks[WHITE] != g2.board.rooks[WHITE] || g1.board.rooks[BLACK] != g2.board.rooks[BLACK] )
    {
        return false;
    }
    if ( g1.board.queens[WHITE] != g2.board.queens[WHITE] || g1.board.queens[BLACK] != g2.board.queens[BLACK] )
    {
        return false;
    }

    //compare castling data
    if ( g1.castling[WHITE].moved_king != g2.castling[WHITE].moved_king || g1.castling[BLACK].moved_king != g2.castling[BLACK].moved_king )
    {
        return false;
    }
    if ( g1.castling[WHITE].moved_left_rook != g2.castling[WHITE].moved_left_rook || g1.castling[BLACK].moved_left_rook != g2.castling[BLACK].moved_left_rook )
    {
        return false;
    }
    if ( g1.castling[WHITE].moved_king != g2.castling[WHITE].moved_king || g1.castling[BLACK].moved_king != g2.castling[BLACK].moved_king )
    {
        return false;
    }

    //comopare en_passant data
    if ( g1.en_passant[WHITE] != g2.en_passant[WHITE] || g1.en_passant[BLACK] != g2.en_passant[BLACK] )
    {
        return false;
    }

    return true;
}

bool threefold_repetition()
{
    int repetitions = 0;

    GameState game_state_after_move;
    game_state_after_move.board = board;
    game_state_after_move.en_passant[0] = en_passant[0];
    game_state_after_move.en_passant[1] = en_passant[1];
    game_state_after_move.castling[0] = castling[0];
    game_state_after_move.castling[1] = castling[1];
    //make_move

    // verifica daca starea la care se ajunge dupa mutarea curenta a mai aparut de minim 2 ori
    for ( int i = 0; i < move_number; i++ )
    {
        if ( identical_game_states(game_state_after_move, past_game_states[i]) )
        {
            repetitions++;

            if ( repetitions == 1 )
            {
                return true;
            }
        }
    }

    //undo move

    return false;
}

void find_best_move(int player, int depth)
{
    Move best_move, second_best_move;
    best_move.start = 0;
    best_move.end = 0;


    int piece_number = __builtin_popcountll(board.queens[WHITE] | board.queens[BLACK] | board.knights[WHITE] | board.knights[BLACK]
            | board.rooks[WHITE] | board.rooks[BLACK] | board.bishops[WHITE] | board.bishops[BLACK]);

    // depth = rand() % 4 + 4;
    if(piece_number >= 8) {
        depth = 6;
    } else if(piece_number >= 4) {
        depth = 7;
        game_time = ENDGAME;
    } else if (piece_number >= 2) {
        depth = 8;
    } else {
        depth = 9;
    }

    if(board.queens[WHITE] | board.queens[BLACK]) {
        depth = 6;
    }

    vector<int> old_piece_moves(7, 0);
    // int eval = negamax(player, depth, best_move, second_best_move);
    // int eval = alphanegamax3(player, depth, MINUS_INF, -MINUS_INF, best_move, second_best_move, old_piece_moves);
    int eval = alphanegamax(player, depth, MINUS_INF, -MINUS_INF, best_move, second_best_move);
 

    if(!is_move_valid(ffsll(best_move.start)-1, ffsll(best_move.end)-1, player)) {
        // print_bit(board.board[player]);
        cout << "HEHEHE\n";

        make_random_move();
        return;

        // std::cout << "resign\n";
    }

    // if(eval == MINUS_INF) {
    //     if(second_best_move.start == 0 && second_best_move.end == 0) {
    //         std::cout << "resign\n";
    //     } else {
    //         cout << "HEHEHE\n";
    //         make_move(second_best_move.start, second_best_move.end, player);
    //         int start_move = ffsll(second_best_move.start)-1;
    //         int end_move = ffsll(second_best_move.end)-1;

    //         char move_xboard[4];
    //         move_xboard[0] = 'a' + start_move % 8;
    //         move_xboard[1] = '1' + start_move / 8;
    //         move_xboard[2] = 'a' + end_move % 8;
    //         move_xboard[3] = '1' + end_move / 8;

    //         // Send the move to xboard
    //         std::cout << "move " << move_xboard << "\n";
    //     }

    //     return;
    // }

    // if(threefold_repetition(player)) {
    //     std::cout << "repetitie ba\n";
    //     std::cout << "resign\n";
    //     return;
    // }

    if(best_move.start == best_move.end) {
        std::cout << "resign\n";
        return;
    }

    // make_random_move();

    // Updates the engine's matrices
    make_move(best_move.start, best_move.end, engine);

    // print_bit(board.king[engine]);

    int start_move = ffsll(best_move.start)-1;
    int end_move = ffsll(best_move.end)-1;

    char move_xboard[4];
    move_xboard[0] = 'a' + start_move % 8;
    move_xboard[1] = '1' + start_move / 8;
    move_xboard[2] = 'a' + end_move % 8;
    move_xboard[3] = '1' + end_move / 8;

    // // Send the move to xboard
    std::cout << "move " << move_xboard << "\n";
}

int negamax(int player, int depth, Move &best_move, Move &second_best_move) {
    if ( depth == 0 ) {
        //TODO detectie checkmate vs stalemate, ambele ar da aceeasi evaluare
        // return board_evaluation(player);
        // return board.score[player] - board.score[1-player];
        return board_evaluation(player);
        // return 1;
        // TODO quiescence search
    }

    int eval = MINUS_INF;
    int max_eval = MINUS_INF;

    int valid_moves_number;
    vector<Move> moves;
    vector<int> piece_moves(7, 0);
    valid_moves_number = generate_moves(player, moves, piece_moves);
    std::random_shuffle(moves.begin(), moves.end());

    GameState undo;
    undo.board = board;
    undo.castling[0] = castling[0];
    undo.castling[1] = castling[1];
    undo.en_passant[0] = en_passant[0];
    undo.en_passant[1] = en_passant[1];

    Move tmp_best, tmp_second;

    for (int i = 0; i < valid_moves_number; i++) {
        make_move(moves[i].start, moves[i].end, player);

        if(!is_in_check(player)) {
            // Update score
            // board_evaluation(moves[i].start, moves[i].end, player, piece_moves);

            eval = -1 * negamax(1-player, depth-1, tmp_best, tmp_second);
        } else {
            eval = MINUS_INF;
        }

        // Undo the state
        board = undo.board;
        castling[0] = undo.castling[0];
        castling[1] = undo.castling[1];
        en_passant[0] = undo.en_passant[0];
        en_passant[1] = undo.en_passant[1];

        // if ( /a fost calculata deja functia pt starea de dupa mutare )
        // {
        //     eval = 1337; //ia valoarea din hash map
        // }
        // else
        // {
        //     eval = -1 * negamax(1 - player, depth - 1);
        // }

        if ( eval > max_eval ) {
            second_best_move = best_move;
            best_move = moves[i];
            max_eval = eval;
        } else if ( eval == max_eval ) {
            second_best_move = moves[i];
        }
    }

    //eventual retine undeva mutarea cea mai buna de pe fiecare nivel pentru a putea afisa principal variation, might help with debugging

    return max_eval;
}

bool moves_cmp_fct(const Move & a, const Move & b) {
    return ((a.end & board.board[1-gplayer]) > (b.end & board.board[1-gplayer]));
}

int alphanegamax(int player, int depth, int alpha, int beta, Move &best_move, Move &second_best_move) {
    if ( depth == 0 ) {
        return board.score[player] - board.score[1-player];
    }

    int eval = MINUS_INF;
    int max_eval = MINUS_INF;
    gplayer = player;

    int valid_moves_number;
    vector<Move> moves;
    // If used a heuristic that takes into account the possible
    // number moves for each type of piece
    // (uncomment the lines from generate_moves)
    vector<int> piece_moves(7, 0);
    valid_moves_number = generate_moves(player, moves, piece_moves);

    // Stalemate
    if(valid_moves_number == 0 && !is_in_check(player)) {
        cout << "STALE MATE\n";
        return -MINUS_INF;
    }

    // A bit of random to spice things up
    std::random_shuffle(moves.begin(), moves.end());

    // Sort by the possibility of capturing the pieces
    std::sort(moves.begin(), moves.end(), moves_cmp_fct);

    // Stores the current state of the game
    GameState undo;
    undo.board = board;
    undo.castling[0] = castling[0];
    undo.castling[1] = castling[1];
    undo.en_passant[0] = en_passant[0];
    undo.en_passant[1] = en_passant[1];

    Move tmp_best, tmp_second;

    for (int i = 0; i < valid_moves_number; i++) {

        make_move(moves[i].start, moves[i].end, player);

        GameState tmp;
        tmp.board = board;
        tmp.castling[0] = castling[0];
        tmp.castling[1] = castling[1];
        tmp.en_passant[0] = en_passant[0];
        tmp.en_passant[1] = en_passant[1];

        // Ignore the move if it result in check for the king
        if(!is_in_check(player)) {
            // Update score

            // Decrease the opponent's score if he has lost a piece
            board.score[1-player] -= piece_evaluation(moves[i].end, 1-player, piece_moves, undo.board);
            if(moves[i].end & undo.board.board[1-player])
                board.score[1-player] -= king_distance_bonus(1-player, moves[i].end);

            // Decrease the score of the moved piece from the starting position
            board.score[player] -= piece_evaluation(moves[i].start, player, piece_moves, undo.board);
            board.score[player] -= king_distance_bonus(player, moves[i].start);
            // Add the score of the piece on the end position (help with promote + maybe other things :))
            board.score[player] += piece_evaluation(moves[i].end, player, piece_moves, board);
            board.score[player] += king_distance_bonus(player, moves[i].end);

            // Alphanegamax recursive call
            eval = -1 * alphanegamax(1-player, depth-1, -beta, -alpha, tmp_best, tmp_second);
        }
        else
            eval = MINUS_INF;

        // Undo the state
        board = undo.board;
        castling[0] = undo.castling[0];
        castling[1] = undo.castling[1];
        en_passant[0] = undo.en_passant[0];
        en_passant[1] = undo.en_passant[1];

        // Choose the move with maximum score
        if ( eval > max_eval ) {
            max_eval = eval;
        }

        if(max_eval > alpha) {
            second_best_move = best_move;
            best_move = moves[i];
            alpha = max_eval;
        }

        if(alpha >= beta) {
            break;
        }
    }

    return alpha;
}

int alphanegamax3(int player, int depth, int alpha, int beta, Move &best_move, Move &second_best_move, vector<int> old_piece_moves) {
    if ( depth == 0 ) {
        return board.score[player] - board.score[1-player];
    }

    int eval = MINUS_INF;
    int max_eval = MINUS_INF;
    gplayer = player;

    int valid_moves_number;
    vector<Move> moves;
    // If used a heuristic that takes into account the possible
    // number moves for each type of piece
    // (uncomment the lines from generate_moves)
    vector<int> piece_moves(7, 0);
    valid_moves_number = generate_moves(player, moves, piece_moves);

    // A bit of random to spice things up
    std::random_shuffle(moves.begin(), moves.end());

    // Sort by the possibility of capturing the pieces
    std::sort(moves.begin(), moves.end(), moves_cmp_fct);

    // Stores the current state of the game
    GameState undo;
    undo.board = board;
    undo.castling[0] = castling[0];
    undo.castling[1] = castling[1];
    undo.en_passant[0] = en_passant[0];
    undo.en_passant[1] = en_passant[1];

    Move tmp_best, tmp_second;

    for (int i = 0; i < valid_moves_number; i++) {

        make_move(moves[i].start, moves[i].end, player);

        GameState tmp;
        tmp.board = board;
        tmp.castling[0] = castling[0];
        tmp.castling[1] = castling[1];
        tmp.en_passant[0] = en_passant[0];
        tmp.en_passant[1] = en_passant[1];

        // Ignore the move if it results in a threefold repetition
        if(threefold_repetition()) {
            // Undo the state
            board = undo.board;
            castling[0] = undo.castling[0];
            castling[1] = undo.castling[1];
            en_passant[0] = undo.en_passant[0];
            en_passant[1] = undo.en_passant[1];
            continue;
        }

        // Ignore the move if it result in check for the king
        if(!is_in_check(player)) {
            // Update score

            // Decrease the opponent's score if he has lost a piece
            board.score[1-player] -= piece_evaluation(moves[i].end, 1-player, old_piece_moves, undo.board);
            if(moves[i].end & undo.board.board[1-player])
                board.score[1-player] -= king_distance_bonus(1-player, moves[i].end);

            // Decrease the score of the moved piece from the starting position
            board.score[player] -= piece_evaluation(moves[i].start, player, old_piece_moves, undo.board);
            board.score[player] -= king_distance_bonus(player, moves[i].start);

            // Add the score of the piece on the end position (help with promote + maybe other things :))
            board.score[player] += piece_evaluation(moves[i].end, player, piece_moves, board);
            board.score[player] += king_distance_bonus(player, moves[i].end);

            // Alphanegamax recursive call
            eval = -1 * alphanegamax3(1-player, depth-1, -beta, -alpha, tmp_best, tmp_second, piece_moves);
        }
        else
            eval = MINUS_INF;

        // Undo the state
        board = undo.board;
        castling[0] = undo.castling[0];
        castling[1] = undo.castling[1];
        en_passant[0] = undo.en_passant[0];
        en_passant[1] = undo.en_passant[1];

        // Choose the move with maximum score
        if ( eval > max_eval ) {
            max_eval = eval;
        }

        if(max_eval > alpha) {
            second_best_move = best_move;
            best_move = moves[i];
            alpha = max_eval;
        }

        if(alpha >= beta) {
            break;
        }
    }

    return alpha;
}

int alphanegamax2(int player, int depth, int alpha, int beta, Move &best_move, Move &second_best_move) {
    if ( depth == 0 ) {
        return player_eval(player) - player_eval(1-player);
    }

    int eval = MINUS_INF;
    int max_eval = MINUS_INF;
    gplayer = player;

    int valid_moves_number;
    vector<Move> moves;
    // If used a heuristic that takes into account the possible
    // number moves for each type of piece
    // (uncomment the lines from generate_moves)
    vector<int> piece_moves(7, 0);
    valid_moves_number = generate_moves(player, moves, piece_moves);

    // A bit of random to spice things up
    std::random_shuffle(moves.begin(), moves.end());

    // Sort by the possibility of capturing the pieces
    std::sort(moves.begin(), moves.end(), moves_cmp_fct);

    // Stores the current state of the game
    GameState undo;
    undo.board = board;
    undo.castling[0] = castling[0];
    undo.castling[1] = castling[1];
    undo.en_passant[0] = en_passant[0];
    undo.en_passant[1] = en_passant[1];

    Move tmp_best, tmp_second;

    for (int i = 0; i < valid_moves_number; i++) {

        make_move(moves[i].start, moves[i].end, player);

        GameState tmp;
        tmp.board = board;
        tmp.castling[0] = castling[0];
        tmp.castling[1] = castling[1];
        tmp.en_passant[0] = en_passant[0];
        tmp.en_passant[1] = en_passant[1];

        // Ignore the move if it results in a threefold repetition
        if(identical_game_states(tmp, forbidden_move)) {
            // Undo the state
            board = undo.board;
            castling[0] = undo.castling[0];
            castling[1] = undo.castling[1];
            en_passant[0] = undo.en_passant[0];
            en_passant[1] = undo.en_passant[1];
            continue;
        }

        // Ignore the move if it result in check for the king
        if(!is_in_check(player)) {
            // Alphanegamax recursive call
            eval = -1 * alphanegamax2(1-player, depth-1, -beta, -alpha, tmp_best, tmp_second);
        }
        else
            eval = MINUS_INF;

        // Undo the state
        board = undo.board;
        castling[0] = undo.castling[0];
        castling[1] = undo.castling[1];
        en_passant[0] = undo.en_passant[0];
        en_passant[1] = undo.en_passant[1];

        // Choose the move with maximum score
        if ( eval > max_eval ) {
            max_eval = eval;
        }

        if(max_eval > alpha) {
            second_best_move = best_move;
            best_move = moves[i];
            alpha = max_eval;
        }

        if(alpha >= beta) {
            break;
        }
    }

    return alpha;
}

void gen_piece_moves(int player, vector<int> &piece_moves) {
    int last = 0;
    vector<Move> moves;
    generate_moves_pawn(player, moves);
    piece_moves[PAWN] = moves.size()-last;
    last = moves.size();

    generate_moves_knight(player, moves);
    piece_moves[KNIGHT] = moves.size()-last;
    last = moves.size();

    generate_moves_bishop(player, moves);
    piece_moves[BISHOP] = moves.size()-last;
    last = moves.size();

    generate_moves_rook(player, moves);
    piece_moves[ROOK] = moves.size()-last;
    last = moves.size();

    generate_moves_queen(player, moves);
    piece_moves[QUEEN] = moves.size()-last;
    last = moves.size();

    generate_moves_king(player, moves);
    piece_moves[KING] = moves.size()-last;   
}

int generate_moves(int player, vector<Move> &moves, vector<int> &piece_moves) {
    // int last = 0;
    generate_moves_pawn(player, moves);
    // piece_moves[PAWN] = moves.size()-last;
    // last = moves.size();

    generate_moves_knight(player, moves);
    // piece_moves[KNIGHT] = moves.size()-last;
    // last = moves.size();

    generate_moves_bishop(player, moves);
    // piece_moves[BISHOP] = moves.size()-last;
    // last = moves.size();

    generate_moves_rook(player, moves);
    // piece_moves[ROOK] = moves.size()-last;
    // last = moves.size();

    generate_moves_queen(player, moves);
    // piece_moves[QUEEN] = moves.size()-last;
    // last = moves.size();

    generate_moves_king(player, moves);
    // piece_moves[KING] = moves.size()-last;


    return moves.size();
}

void generate_moves_pawn(int player, vector<Move> &moves) {
    unsigned long long tmp_pawns = board.pawns[player];
    unsigned long long full_board = board.board[0] | board.board[1];
    Move move;

    while(tmp_pawns != 0) {
        unsigned long long pawn = tmp_pawns & -tmp_pawns;
        int pos = ffsll(pawn)-1;
        unsigned long long pawn_move = pawn_moves[player][pos];

        // Generate forward moves
        if(player == WHITE) {
            unsigned long long tmp_move = pawn_move & -pawn_move;

            if(!(tmp_move & full_board)) {
                move.start = pawn;
                move.end = tmp_move;
                moves.push_back(move);
            }

            pawn_move &= pawn_move-1;

            if(pawn_move != 0) {
                tmp_move = pawn_move & -pawn_move;

                if(!(pawn_moves[player][pos] & full_board)) {
                    move.start = pawn;
                    move.end = tmp_move;
                    moves.push_back(move);
                }
            }
        }

        if(player == BLACK) {
            unsigned long long tmp_move = pawn_move & -pawn_move;

            if(!(pawn_moves[player][pos] & full_board)) {
                move.start = pawn;
                move.end = tmp_move;
                moves.push_back(move);
            }

            pawn_move &= pawn_move-1;

            if(pawn_move != 0) {
                tmp_move = pawn_move & -pawn_move;

                if(!(tmp_move & full_board)) {
                    move.start = pawn;
                    move.end = tmp_move;
                    moves.push_back(move);
                }
            }
        }

        unsigned long long pawn_attack = pawn_attacks[player][pos];

        // Generate attacks
        while(pawn_attack != 0) {
            unsigned long long tmp_move = pawn_attack & -pawn_attack;

            if(tmp_move & board.board[1-player]) {
                move.start = pawn;
                move.end = tmp_move;
                moves.push_back(move);
            }

            pawn_attack &= pawn_attack-1;
        }

        tmp_pawns &= tmp_pawns-1;
    }

    tmp_pawns = board.pawns[player];

    // Generate en-passant
    if(en_passant[1-player] != -1) {

        if(player == WHITE) {
            if((1ULL << (en_passant[1-player] + 1)) & tmp_pawns) {
                if(!(((tmp_pawns << 7) & notHFile) & full_board)) {
                    move.start = 1ULL << (en_passant[1-player] + 1);
                    move.end = (1ULL << (en_passant[1-player] + 1)) << 7;
                    moves.push_back(move);
                }
            }

            if((1ULL << (en_passant[1-player] - 1)) & tmp_pawns) {
                if(!(((tmp_pawns << 9) & notAFile) & full_board)) {
                    move.start = 1ULL << (en_passant[1-player] - 1);
                    move.end = (1ULL << (en_passant[1-player] - 1)) << 9;
                    moves.push_back(move);
                }
            }
        }

        if(player == BLACK) {
            if((1ULL << (en_passant[1-player] + 1)) & tmp_pawns) {
                if(!(((tmp_pawns >> 9) & notHFile) & full_board)) {
                    move.start = 1ULL << (en_passant[1-player] + 1);
                    move.end = (1ULL << (en_passant[1-player] + 1)) >> 9;
                    moves.push_back(move);
                }
            }

            if((1ULL << (en_passant[1-player] - 1)) & tmp_pawns) {
                if(!(((tmp_pawns >> 7) & notAFile) & full_board)) {
                    move.start = 1ULL << (en_passant[1-player] - 1);
                    move.end = (1ULL << (en_passant[1-player] - 1)) >> 7;
                    moves.push_back(move);
                }
            }
        }
    }
}

void generate_moves_knight(int player, vector<Move> &moves) {
    unsigned long long tmp_knights = board.knights[player];
    Move move;

    while(tmp_knights != 0) {
        unsigned long long knight = tmp_knights & -tmp_knights;
        int pos = ffsll(knight)-1;
        unsigned long long knight_move = knight_moves[pos];

        while(knight_move != 0) {
            unsigned long long tmp_move = knight_move & -knight_move;

            if(!(tmp_move & board.board[player])) {
                move.start = knight;
                move.end = tmp_move;
                moves.push_back(move);
            }

            knight_move &= knight_move-1;
        }

        tmp_knights &= tmp_knights-1;
    }
}

void generate_moves_bishop(int player, vector<Move> &moves) {
    unsigned long long tmp_bishops = board.bishops[player];
    Move move;

    while(tmp_bishops != 0) {
        unsigned long long bishop = tmp_bishops & -tmp_bishops;
        int pos = ffsll(bishop)-1;
        unsigned long long bishop_move = bishop_attack(pos, board.board[0] | board.board[1]);

        while(bishop_move != 0) {
            unsigned long long tmp_move = bishop_move & -bishop_move;

            if(!(tmp_move & board.board[player])) {
                move.start = bishop;
                move.end = tmp_move;
                moves.push_back(move);
            }

            bishop_move &= bishop_move-1;
        }

        tmp_bishops &= tmp_bishops-1;
    }
}

void generate_moves_rook(int player, vector<Move> &moves) {
    unsigned long long tmp_rooks = board.rooks[player];
    Move move;

    while(tmp_rooks != 0) {
        unsigned long long rook = tmp_rooks & -tmp_rooks;
        int pos = ffsll(rook)-1;
        unsigned long long rook_move = rook_attack(pos, board.board[0] | board.board[1]);

        while(rook_move != 0) {
            unsigned long long tmp_move = rook_move & -rook_move;

            if(!(tmp_move & board.board[player])) {
                move.start = rook;
                move.end = tmp_move;
                moves.push_back(move);
            }

            rook_move &= rook_move-1;
        }

        tmp_rooks &= tmp_rooks-1;
    }
}

void generate_moves_queen(int player, vector<Move> &moves) {
    unsigned long long tmp_queens = board.queens[player];
    Move move;

    while(tmp_queens != 0) {
        unsigned long long queen = tmp_queens & -tmp_queens;
        int pos = ffsll(queen)-1;
        unsigned long long queen_move = queen_attack(pos, board.board[0] | board.board[1]);

        while(queen_move != 0) {
            unsigned long long tmp_move = queen_move & -queen_move;

            if(!(tmp_move & board.board[player])) {
                move.start = queen;
                move.end = tmp_move;
                moves.push_back(move);
            }

            queen_move &= queen_move-1;
        }

        tmp_queens &= tmp_queens-1;
    }
}

void generate_moves_king(int player, vector<Move> &moves) {
    unsigned long long tmp_king = board.king[player];
    Move move;

    while(tmp_king != 0) {
        unsigned long long king = tmp_king & -tmp_king;
        int pos = ffsll(king)-1;
        unsigned long long king_move = king_moves[pos];

        // Normal moves
        while(king_move != 0) {
            unsigned long long tmp_move = king_move & -king_move;

            if(!(tmp_move & board.board[player]) && !(tmp_move & king_moves[ffsll(board.king[1-player])-1])) {
                move.start = king;
                move.end = tmp_move;
                moves.push_back(move);
            }

            king_move &= king_move-1;
        }

        // Castling
        if(castling_valid_move(pos, pos-2, player)) {
            move.start = king;
            move.end = (1ULL << (pos-2));
            moves.push_back(move);
        }
        if(castling_valid_move(pos, pos+2, player)) {
            move.start = king;
            move.end = (1ULL << (pos+2));
            moves.push_back(move);
        }

        tmp_king &= tmp_king-1;
    }
}
