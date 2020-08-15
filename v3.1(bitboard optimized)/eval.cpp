#include "eval.h"

int player_eval(int player) {
	int sum = 0;

	if(game_time == MIDDLEGAME) {
		// Piece value 
	    sum += piece_value_middle(PAWN) * __builtin_popcountll(board.pawns[player]);
	    sum += piece_value_middle(KNIGHT) * __builtin_popcountll(board.knights[player]);
	    sum += piece_value_middle(ROOK) * __builtin_popcountll(board.rooks[player]);
	    sum += piece_value_middle(BISHOP) * __builtin_popcountll(board.bishops[player]);
	    sum += piece_value_middle(QUEEN) * __builtin_popcountll(board.queens[player]);
	    sum += piece_value_middle(KING) * __builtin_popcountll(board.king[player]);


	    // Mobility value
	    vector<int> piece_moves(7, 0);
	    gen_piece_moves(player, piece_moves);
	    sum += mobility_middle(piece_moves);


	    // Piece per square value
	    sum += piece_square_bonus_middle(player);
	}

	if(game_time == ENDGAME) {
		// Piece value 
	    sum += piece_value_end(PAWN) * __builtin_popcountll(board.pawns[player]);
	    sum += piece_value_end(KNIGHT) * __builtin_popcountll(board.knights[player]);
	    sum += piece_value_end(ROOK) * __builtin_popcountll(board.rooks[player]);
	    sum += piece_value_end(BISHOP) * __builtin_popcountll(board.bishops[player]);
	    sum += piece_value_end(QUEEN) * __builtin_popcountll(board.queens[player]);
	    sum += piece_value_end(KING) * __builtin_popcountll(board.king[player]);


	    // Mobility value
	    vector<int> piece_moves(7, 0);
	    gen_piece_moves(player, piece_moves);
	    sum += mobility_end(piece_moves);


	    // Piece per square value
	    sum += piece_square_bonus_end(player);
	}

	sum += king_safety(player);

    return sum;
}

int piece_value_middle(int piece)
{
    if ( piece == PAWN )
    {
        return 128;
    }
    else if ( piece == KNIGHT )
    {
        return 781;
    }
    else if ( piece == BISHOP )
    {
        return 825;
    }
    else if ( piece == ROOK )
    {
        return 1276;
    }
    else if ( piece == QUEEN )
    {
        return 2538;
    }
    else if ( piece == KING )
    {
        return 100000;
    }

    return 0;
}

int piece_value_end(int piece)
{
    if ( piece == PAWN )
    {
        return 213;
    }
    else if ( piece == KNIGHT )
    {
        return 854;
    }
    else if ( piece == BISHOP )
    {
        return 915;
    }
    else if ( piece == ROOK )
    {
        return 1380;
    }
    else if ( piece == QUEEN )
    {
        return 2682;
    }
    else if ( piece == KING )
    {
        return 100000;
    }

    return 0;
}

// Piece mobility 
// int knight_mg_mobility[] = {-15, -5, -1, 2, 5, 7, 9, 11, 13};
int knight_mg_mobility[] = {-62, -53, -12, -4, 3, 13, 22, 28, 33};
// int knight_eg_mobility[] = {-30, -10, -2, 4, 10, 14, 18, 22, 26};
int knight_eg_mobility[] = {-81, -56, -30, -14, 8, 15, 23, 27, 33};

// int bishop_mg_mobility[] = {-25, -11, -6, -1, 3, 6, 9, 12, 14, 17, 19, 21, 23, 25};
int bishop_mg_mobility[] = {-48, -20, 16, 26, 38, 51, 55, 63, 63, 68, 81, 81, 91, 98};
// int bishop_eg_mobility[] = {-50, -22, -11, -2, 6, 12, 18, 24, 29, 34, 38, 42, 46, 50};
int bishop_eg_mobility[] = {-59, -23, -3, 13, 24, 42, 54, 57, 65, 73, 78, 86, 88, 97};

// int rook_mg_mobility[] = {-10, -4, -2, 0, 2, 3, 4, 5, 6, 8, 8, 9, 10, 11, 12};
int rook_mg_mobility[] = {-60, -20, 2, 3, 3, 11, 22, 31, 40, 40, 41, 48, 57, 57, 62};

// int rook_eg_mobility[] = {-50, -22, -11, -2, 6, 12, 18, 24, 29, 34, 38, 42, 46, 50, 54};
int rook_eg_mobility[] = {-78, -17, 23, 39, 70, 99, 103, 121, 134, 139, 158, 164, 168, 169, 172};

// int queen_mg_mobility[] = {-10, -6, -5, -4, -2, -2, -1, 0, 1, 2, 2, 3, 3, 4, 4, 5, 6, 6, 6,
// 7, 7, 8, 8, 9, 9, 10, 10, 10};
int queen_mg_mobility[] = {-34, -15, -10, -10, 20, 23, 23, 35, 38, 53, 64, 65, 65, 66, 67, 67, 72, 72, 77, 79, 93, 108, 108, 108, 110, 114, 114, 116};

// int queen_eg_mobility[] = {-50, -30, -22, -16, -10, -6, -2, 2, 6, 10, 13, 16, 19, 22, 24,
// 27, 30, 32, 34, 37, 39, 41, 43, 45, 47, 50, 51, 53};
int queen_eg_mobility[] = {-36, -21, -1, 22, 41, 56, 59, 75, 78, 96, 96, 100, 121, 127, 131, 133, 136, 141, 147, 150, 151, 168, 168, 171, 182, 182, 192, 219};

int mobility_middle(vector<int> piece_moves) {
	int sum = 0;

	sum += knight_mg_mobility[piece_moves[KNIGHT]];
	sum += bishop_mg_mobility[piece_moves[BISHOP]];
	sum += rook_mg_mobility[piece_moves[ROOK]];
	sum += queen_mg_mobility[piece_moves[QUEEN]];

	return sum;
}

int mobility_end(vector<int> piece_moves) {
	int sum = 0;

	sum += knight_eg_mobility[piece_moves[KNIGHT]];
	sum += bishop_eg_mobility[piece_moves[BISHOP]];
	sum += rook_eg_mobility[piece_moves[ROOK]];
	sum += queen_eg_mobility[piece_moves[QUEEN]];

	return sum;
}

// Piece per square bonus https://hxim.github.io/Stockfish-Evaluation-Guide/

int knight_piece_per_square_bonus_mg[2][64][64] = {
	{
		{-175, -92, -74, -73, -73, -74, -92, -175},
		{-77, -41, -27, -15, -15, -27, -41, -77},
		{-61, -17, 6, 12, 12, 6, -17, -61},
		{-35, 8, 40, 49, 49, 40, 8, -35},
		{-34, 13, 44, 51, 51, 44, 13, -34},
		{-9, 22, 58, 53, 53, 58, 22, -9},
		{-67, -27, 4, 37, 37, 4, -27, -67},
		{-201, -83, -56, -26, -26, -56, -83, -201}
	},
	{
		{-201, -83, -56, -26, -26, -56, -83, -201},
		{-67, -27, 4, 37, 37, 4, -27, -67},
		{-9, 22, 58, 53, 53, 58, 22, -9},
		{-34, 13, 44, 51, 51, 44, 13, -34},
		{-35, 8, 40, 49, 49, 40, 8, -35},
		{-61, -17, 6, 12, 12, 6, -17, -61},
		{-77, -41, -27, -15, -15, -27, -41, -77},
		{-175, -92, -74, -73, -73, -74, -92, -175},
	}
};

int knight_piece_per_square_bonus_eg[2][64][64] = {
	{
		{-96, -65, -49, -21, -21, -49, -65, -96},
		{-67, -54, -18, 8, 8, -18, -54, -67},
		{-40, -27, -8, 29, 29, -8, -27, -40},
		{-35, -2, 13, 28, 28, 13, -2, -35},
		{-45, -16, 9, 39, 39, 9, -16, -45},
		{-51, -44, -16, 17, 17, -16, -44, -51},
		{-69, -50, -51, 12, 12, -51, -50, -69},
		{-100, -88, -56, -17, -17, -56, -88, -100}
	},
	{
		{-100, -88, -56, -17, -17, -56, -88, -100},
		{-69, -50, -51, 12, 12, -51, -50, -69},
		{-51, -44, -16, 17, 17, -16, -44, -51},
		{-45, -16, 9, 39, 39, 9, -16, -45},
		{-35, -2, 13, 28, 28, 13, -2, -35},
		{-40, -27, -8, 29, 29, -8, -27, -40},
		{-67, -54, -18, 8, 8, -18, -54, -67},
		{-96, -65, -49, -21, -21, -49, -65, -96}
	}
};

int bishop_piece_per_square_bonus_mg[2][64][64] = {
	{
		{-53, -5, -8, -23, -23, -8, -5, -53},
		{-15, 8, 19, 4, 4, 19, 8, -15},
		{-7, 21, -5, 17, 17, -5, 21, -7},
		{-5, 11, 25, 39, 39, 25, 11, -5},
		{-12, 29, 22, 31, 31, 22, 29, -12},
		{-16, 6, 1, 11, 11, 1, 6, -16},
		{-17, -14, 5, 0, 0, 5, -14, -17},
		{-48, 1, -14, -23, -23, -14, 1, -48}
	},
	{
		{-48, 1, -14, -23, -23, -14, 1, -48},
		{-17, -14, 5, 0, 0, 5, -14, -17},
		{-16, 6, 1, 11, 11, 1, 6, -16},
		{-12, 29, 22, 31, 31, 22, 29, -12},
		{-5, 11, 25, 39, 39, 25, 11, -5},
		{-7, 21, -5, 17, 17, -5, 21, -7},
		{-15, 8, 19, 4, 4, 19, 8, -15},
		{-53, -5, -8, -23, -23, -8, -5, -53}
	}
};

int bishop_piece_per_square_bonus_eg[2][64][64] = {
	{
		{-57, -30, -37, -12, -12, -37, -30, -57},
		{-37, -13, -17, 1, 1, -17, -13, -37},
		{-16, -1, -2, 10, 10, -2, -1, -16},
		{-20, -6, 0, 17, 17, 0, -6, -20},
		{-17, -1, -14, 15, 15, -14, -1, -17},
		{-30, 6, 4, 6, 6, 4, 6, -30},
		{-31, -20, -1, 1, 1, -1, -20, -31},
		{-46, -42, -37, -24, -24, -37, -42, -46}
	},
	{
		{-46, -42, -37, -24, -24, -37, -42, -46},
		{-31, -20, -1, 1, 1, -1, -20, -31},
		{-30, 6, 4, 6, 6, 4, 6, -30},
		{-17, -1, -14, 15, 15, -14, -1, -17},
		{-20, -6, 0, 17, 17, 0, -6, -20},
		{-16, -1, -2, 10, 10, -2, -1, -16},
		{-37, -13, -17, 1, 1, -17, -13, -37},
		{-57, -30, -37, -12, -12, -37, -30, -57}
	}
};

int rook_piece_per_square_bonus_mg[2][64][64] = {
	{
		{-31, -20, -14, -5, -5, -14, -20, -31},
		{-21, -13, -8, 6, 6, -8, -13, -21},
		{-25, -11, -1, 3, 3, -1, -11, -25},
		{-13, -5, -4, -6, -6, -4, -5, -13},
		{-27, -15, -4, 3, 3, -4, -15, -27},
		{-22, -2, 6, 12, 12, 6, -2, -22},
		{-2, 12, 16, 18, 18, 16, 12, -2},
		{-17, -19, -1, 9, 9, -1, -19, -17}
	},
	{
		{-17, -19, -1, 9, 9, -1, -19, -17},
		{-2, 12, 16, 18, 18, 16, 12, -2},
		{-22, -2, 6, 12, 12, 6, -2, -22},
		{-27, -15, -4, 3, 3, -4, -15, -27},
		{-13, -5, -4, -6, -6, -4, -5, -13},
		{-25, -11, -1, 3, 3, -1, -11, -25},
		{-21, -13, -8, 6, 6, -8, -13, -21},
		{-31, -20, -14, -5, -5, -14, -20, -31}
	}	
};

int rook_piece_per_square_bonus_eg[2][64][64] = {
	{
		{-9, -13, -10, -9, -9, -10, -13, -9},
		{-12, -9, -1, -2, -2, -1, -9, -12},
		{6, -8, -2, -6, -6, -2, -8, 6},
		{-6, 1, -9, 7, 7, -9, 1, -6},
		{-5, 8, 7, -6, -6, 7, 8, -5},
		{6, 1, -7, 10, 10, -7, 1, 6},
		{4, 5, 20, -5, -5, 20, 5, 4},
		{18, 0, 19, 13, 13, 19, 0, 18}
	},
	{
		{18, 0, 19, 13, 13, 19, 0, 18},
		{4, 5, 20, -5, -5, 20, 5, 4},
		{6, 1, -7, 10, 10, -7, 1, 6},
		{-5, 8, 7, -6, -6, 7, 8, -5},
		{-6, 1, -9, 7, 7, -9, 1, -6},
		{6, -8, -2, -6, -6, -2, -8, -6},
		{-12, -9, -1, -2, -2, -1, -9, -12},
		{-9, -13, -10, -9, -9, -10, -13, -9},
	}
};

int queen_piece_per_square_bonus_mg[2][64][64] = {
	{
		{3, -5, -5, 4, 4, -5, -5, 3},
		{-3, 5, 8, 12, 12, 8, 5, -3},
		{-3, 6, 13, 7, 7, 13, 6, -3},
		{4, 5, 9, 8, 8, 9, 5, 4},
		{0, 14, 12, 5, 5, 12, 14, 0},
		{-4, 10, 6, 8, 8, 6, 10, -4},
		{-5, 6, 10, 8, 8, 10, 6, -5},
		{-2, -2, 1, -2, -2, 1, -2, -2}
	},
	{
		{-2, -2, 1, -2, -2, 1, -2, -2},
		{-5, 6, 10, 8, 8, 10, 6, -5},
		{-4, 10, 6, 8, 8, 6, 10, -4},
		{0, 14, 12, 5, 5, 12, 14, 0},
		{4, 5, 9, 8, 8, 9, 5, 4},
		{-3, 6, 13, 7, 7, 13, 6, -3},
		{-3, 5, 8, 12, 12, 8, 5, -3},
		{3, -5, -5, 4, 4, -5, -5, 3}
	}
};

int queen_piece_per_square_bonus_eg[2][64][64] = {
	{
		{-69, -57, 47, -26, -26, 47, -57, -69},
		{-55, -31, -22, -4, -4, -22, -31, -55},
		{-39, -18, -9, 3, 3, -9, -18, -39},
		{-23, -3, 13, 24, 24, 13, -3, -23},
		{-29, -6, 9, 21, 21, 9, -6, -29},
		{-38, -18, -12, 1, 1, -12, -18, -38},
		{-50, -27, -24, -8, -8, -24, -27, -50},
		{-75, -52, -43, -36, -36, -43, -52, -75}
	},
	{
		{-75, -52, -43, -36, -36, -43, -52, -75},
		{-50, -27, -24, -8, -8, -24, -27, -50},
		{-38, -18, -12, 1, 1, -12, -18, -38},
		{-29, -6, 9, 21, 21, 9, -6, -29},
		{-23, -3, 13, 24, 24, 13, -3, -23},
		{-39, -18, -9, 3, 3, -9, -18, -39},
		{-55, -31, -22, -4, -4, -22, -31, -55},
		{-69, -57, 47, -26, -26, 47, -57, -69}
	}
};

int king_piece_per_square_bonus_mg[2][64][64] = {
	{
		{271, 327, 271, 198, 198, 271, 327, 271},
		{278, 303, 234, 179, 179, 234, 303, 278},
		{195, 258, 169, 120, 120, 169, 258, 195},
		{164, 190, 138, 98, 98, 138, 190, 164},
		{154, 179, 105, 70, 70, 105, 179, 154},
		{123, 145, 81, 31, 31, 81, 145, 123},
		{88, 120, 65, 33, 33, 65, 120, 88},
		{59, 89, 45, -1, -1, 45, 89, 59}
	},
	{
		{59, 89, 45, -1, -1, 45, 89, 59},
		{88, 120, 65, 33, 33, 65, 120, 88},
		{123, 145, 81, 31, 31, 81, 145, 123},
		{154, 179, 105, 70, 70, 105, 179, 154},
		{164, 190, 138, 98, 98, 138, 190, 164},
		{195, 258, 169, 120, 120, 169, 258, 195},
		{278, 303, 234, 179, 179, 234, 303, 278},
		{271, 327, 271, 198, 198, 271, 327, 271},
	}
};

int king_piece_per_square_bonus_eg[2][64][64] = {
	{
		{1, 45, 85, 76, 76, 85, 45, 1},
		{53, 100, 133, 135, 135, 133, 100, 53},
		{88, 130, 169, 175, 175, 169, 130, 88},
		{103, 156, 172, 172, 172, 172, 156, 103},
		{96, 166, 199, 199, 199, 199, 166, 96},
		{92, 172, 184, 191, 191, 184, 172, 92},
		{47, 121, 116, 131, 131, 116, 121, 47},
		{11, 59, 73, 78, 78, 73, 59, 11}
	},
	{
		{11, 59, 73, 78, 78, 73, 59, 11},
		{47, 121, 116, 131, 131, 116, 121, 47},
		{92, 172, 184, 191, 191, 184, 172, 92},
		{96, 166, 199, 199, 199, 199, 166, 96},
		{103, 156, 172, 172, 172, 172, 156, 103},
		{88, 130, 169, 175, 175, 169, 130, 88},
		{53, 100, 133, 135, 135, 133, 100, 53},
		{1, 45, 85, 76, 76, 85, 45, 1},
	}
};

int pawn_piece_per_square_bonus_mg[2][64][64] = {
	{
		{0, 0, 0, 0, 0, 0, 0, 0},
		{3, 3, 10, 19, 16, 19, 7, -5},
		{-9, -15, 11, 15, 32, 22, 5, -22},
		{-8, -23, 6, 20, 40, 17, 4, -12},
		{13, 0, -13, 1, 11, -2, -13, 5},
		{-5, -12, -7, 22, -8, -5, -15, -18},
		{-7, 7, -3, -13, 5, -16, 10, -8},
		{0, 0, 0, 0, 0, 0, 0, 0}
	},
	{
		{0, 0, 0, 0, 0, 0, 0, 0},
		{-7, 7, -3, -13, 5, -16, 10, -8},
		{-5, -12, -7, 22, -8, -5, -15, -18},
		{13, 0, -13, 1, 11, -2, -13, 5},
		{-8, -23, 6, 20, 40, 17, 4, -12},
		{-9, -15, 11, 15, 32, 22, 5, -22},
		{3, 3, 10, 19, 16, 19, 7, -5},
		{0, 0, 0, 0, 0, 0, 0, 0}
	}
};

int pawn_piece_per_square_bonus_eg[2][64][64] = {
	{
		{0, 0, 0, 0, 0, 0, 0, 0},
		{-10, -6, 10, 0, 14, 7, -5, -19},
		{-10, -10, -10, 4, 4, 3, -6, -4},
		{6, -2, -8, -4, -13, -12, -10, -9},
		{9, 4, 3, -12, -12, -6, 13, 8},
		{28, 20, 21, 28, 30, 7, 6, 13},
		{0, -11, 12, 21, 25, 19, 4, 7},
		{0, 0, 0, 0, 0, 0, 0, 0}
	},
	{
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, -11, 12, 21, 25, 19, 4, 7},
		{28, 20, 21, 28, 30, 7, 6, 13},
		{9, 4, 3, -12, -12, -6, 13, 8},
		{6, -2, -8, -4, -13, -12, -10, -9},
		{-10, -10, -10, 4, 4, 3, -6, -4},
		{-10, -6, 10, 0, 14, 7, -5, -19},
		{0, 0, 0, 0, 0, 0, 0, 0}
	}
};

int piece_square_bonus_middle(int player) {
	int sum = 0;

	unsigned long long tmp_knights = board.knights[player];
	unsigned long long tmp_bishops = board.bishops[player];
    unsigned long long tmp_rooks = board.rooks[player];
    unsigned long long tmp_queens = board.queens[player];
    unsigned long long tmp_king = board.king[player];
    unsigned long long tmp_pawns = board.pawns[player];

    while(tmp_knights != 0) {
        unsigned long long knights = tmp_knights & -tmp_knights;
        int pos = ffsll(knights)-1;
        sum += knight_piece_per_square_bonus_mg[player][pos/8][pos%8];

        tmp_knights &= tmp_knights-1;
    }

    while(tmp_bishops != 0) {
        unsigned long long bishops = tmp_bishops & -tmp_bishops;
        int pos = ffsll(bishops)-1;
        sum += bishop_piece_per_square_bonus_mg[player][pos/8][pos%8];

        tmp_bishops &= tmp_bishops-1;
    }

    while(tmp_rooks != 0) {
        unsigned long long rooks = tmp_rooks & -tmp_rooks;
        int pos = ffsll(rooks)-1;
        sum += rook_piece_per_square_bonus_mg[player][pos/8][pos%8];

        tmp_rooks &= tmp_rooks-1;
    }

    while(tmp_queens != 0) {
        unsigned long long queens = tmp_queens & -tmp_queens;
        int pos = ffsll(queens)-1;
        sum += queen_piece_per_square_bonus_mg[player][pos/8][pos%8];

        tmp_queens &= tmp_queens-1;
    }

    while(tmp_king != 0) {
        unsigned long long king = tmp_king & -tmp_king;
        int pos = ffsll(king)-1;
        sum += king_piece_per_square_bonus_mg[player][pos/8][pos%8];

        tmp_king &= tmp_king-1;
    }

    while(tmp_pawns != 0) {
        unsigned long long pawns = tmp_pawns & -tmp_pawns;
        int pos = ffsll(pawns)-1;
        sum += pawn_piece_per_square_bonus_mg[player][pos/8][pos%8];

        tmp_pawns &= tmp_pawns-1;
    }

    return sum;
}

int piece_square_bonus_end(int player) {
	int sum = 0;

	unsigned long long tmp_knights = board.knights[player];
	unsigned long long tmp_bishops = board.bishops[player];
    unsigned long long tmp_rooks = board.rooks[player];
    unsigned long long tmp_queens = board.queens[player];
    unsigned long long tmp_king = board.king[player];
    unsigned long long tmp_pawns = board.pawns[player];

    while(tmp_knights != 0) {
        unsigned long long knights = tmp_knights & -tmp_knights;
        int pos = ffsll(knights)-1;
        sum += knight_piece_per_square_bonus_eg[player][pos/8][pos%8];

        tmp_knights &= tmp_knights-1;
    }

    while(tmp_bishops != 0) {
        unsigned long long bishops = tmp_bishops & -tmp_bishops;
        int pos = ffsll(bishops)-1;
        sum += bishop_piece_per_square_bonus_eg[player][pos/8][pos%8];

        tmp_bishops &= tmp_bishops-1;
    }

    while(tmp_rooks != 0) {
        unsigned long long rooks = tmp_rooks & -tmp_rooks;
        int pos = ffsll(rooks)-1;
        sum += rook_piece_per_square_bonus_eg[player][pos/8][pos%8];

        tmp_rooks &= tmp_rooks-1;
    }

    while(tmp_queens != 0) {
        unsigned long long queens = tmp_queens & -tmp_queens;
        int pos = ffsll(queens)-1;
        sum += queen_piece_per_square_bonus_eg[player][pos/8][pos%8];

        tmp_queens &= tmp_queens-1;
    }

    while(tmp_king != 0) {
        unsigned long long king = tmp_king & -tmp_king;
        int pos = ffsll(king)-1;
        sum += king_piece_per_square_bonus_eg[player][pos/8][pos%8];

        tmp_king &= tmp_king-1;
    }

    while(tmp_pawns != 0) {
        unsigned long long pawns = tmp_pawns & -tmp_pawns;
        int pos = ffsll(pawns)-1;
        sum += pawn_piece_per_square_bonus_eg[player][pos/8][pos%8];

        tmp_pawns &= tmp_pawns-1;
    }

    return sum;
}

int king_safety(int player) {
	int sum = 0;

	unsigned long long tmp_knights = board.knights[player];
	unsigned long long tmp_bishops = board.bishops[player];
    unsigned long long tmp_rooks = board.rooks[player];
    unsigned long long tmp_queens = board.queens[player];
    unsigned long long tmp_king = board.king[player];
    unsigned long long tmp_pawns = board.pawns[player];

    int king_pos;

    while(tmp_king != 0) {
        unsigned long long king = tmp_king & -tmp_king;
        king_pos = ffsll(king)-1;

        tmp_king &= tmp_king-1;
        break;
    }

    while(tmp_knights != 0) {
        unsigned long long knights = tmp_knights & -tmp_knights;
        int pos = ffsll(knights)-1;

        int x1 = pos/8;
    	int y1 = pos%8;
    	int x2 = king_pos/8;
    	int y2 = king_pos%8;

    	switch(max(abs(x1-x2), abs(y1-y2))) {
	        case 1:
            	sum += 100;
        	case 2:
	            sum += 50;
        	case 3:
	            sum += 25;
        	case 4:
	            sum += 10;
        	default:
	            sum += 0;
	    }

        tmp_knights &= tmp_knights-1;
    }

    while(tmp_bishops != 0) {
        unsigned long long bishops = tmp_bishops & -tmp_bishops;
        int pos = ffsll(bishops)-1;

        int x1 = pos/8;
    	int y1 = pos%8;
    	int x2 = king_pos/8;
    	int y2 = king_pos%8;

    	switch(max(abs(x1-x2), abs(y1-y2))) {
	        case 1:
            	sum += 100;
        	case 2:
	            sum += 50;
        	case 3:
	            sum += 25;
        	case 4:
	            sum += 10;
        	default:
	            sum += 0;
	    }

        tmp_bishops &= tmp_bishops-1;
    }

    while(tmp_rooks != 0) {
        unsigned long long rooks = tmp_rooks & -tmp_rooks;
        int pos = ffsll(rooks)-1;
        
        int x1 = pos/8;
    	int y1 = pos%8;
    	int x2 = king_pos/8;
    	int y2 = king_pos%8;

    	switch(max(abs(x1-x2), abs(y1-y2))) {
	        case 1:
            	sum += 100;
        	case 2:
	            sum += 50;
        	case 3:
	            sum += 25;
        	case 4:
	            sum += 10;
        	default:
	            sum += 0;
	    }


        tmp_rooks &= tmp_rooks-1;
    }

    while(tmp_queens != 0) {
        unsigned long long queens = tmp_queens & -tmp_queens;
        int pos = ffsll(queens)-1;
        
        int x1 = pos/8;
    	int y1 = pos%8;
    	int x2 = king_pos/8;
    	int y2 = king_pos%8;

    	switch(max(abs(x1-x2), abs(y1-y2))) {
	        case 1:
            	sum += 100;
        	case 2:
	            sum += 50;
        	case 3:
	            sum += 25;
        	case 4:
	            sum += 10;
        	default:
	            sum += 0;
	    }


        tmp_queens &= tmp_queens-1;
    }

    while(tmp_pawns != 0) {
        unsigned long long pawns = tmp_pawns & -tmp_pawns;
        int pos = ffsll(pawns)-1;
        
        int x1 = pos/8;
    	int y1 = pos%8;
    	int x2 = king_pos/8;
    	int y2 = king_pos%8;

    	switch(max(abs(x1-x2), abs(y1-y2))) {
	        case 1:
            	sum += 100;
        	case 2:
	            sum += 50;
        	case 3:
	            sum += 25;
        	case 4:
	            sum += 10;
        	default:
	            sum += 0;
	    }


        tmp_pawns &= tmp_pawns-1;
    }

    return sum;
}