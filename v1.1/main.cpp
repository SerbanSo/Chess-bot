#include <iostream>
#include <string.h>
#include <fstream>
#include <cstdio>
// #include "define.h"

using namespace std;

int chessboard[9][9];

enum PIESE{
	PION = 1,
	TURN = 2,
	CAL = 3,
	NEBUN = 4,
	REGE = 5,
	REGINA = 6
};

enum ALF{
	A = 1,
	B = 2,
	C = 3,
	D = 4,
	E = 5,
	F = 6,
	G = 7,
	H = 8
};

void init_board()
{
	chessboard[1][A] = chessboard[1][H] = chessboard[8][A] = chessboard[8][H] = TURN;

	chessboard[1][B] = chessboard[1][G] = chessboard[8][B] = chessboard[8][G] = CAL;

	chessboard[1][C] = chessboard[1][F] = chessboard[8][C] = chessboard[8][F] = NEBUN;

	chessboard[1][D] = chessboard[8][D] = REGINA;

	chessboard[1][E] = chessboard[8][E] = REGE;

	for(int i = 1; i <= 8; i++)
	{
		chessboard[2][i] = chessboard[7][i] = PION;
	}

}

void print_board()
{
	ofstream log("log2.txt", ios::app);

	for(int i = 1; i <= 8; i++)
	{
		for(int j = 1; j <= 8; j++)
		{
			log << chessboard[i][j] << " ";
		}
		log << endl;
	}

	log << "\n\n";
	log.close();
}

int main()
{
	char buffer[100];

	ofstream log("log.txt", ios::app);

	log.close();

	init_board();

	print_board();

	// MOVE COMMAND: move 'e1e2'\n

	// while(1)
	// {
	// 	cin >> buffer;

	// 	log.open("log.txt", ios::app);

	// 	log << buffer << "\n";

	// 	cout << "move e7e5\n";

	// 	if(strcmp(buffer, "xboard") == 0){
	// 		log << buffer << "\n";
	// 	} else if(strcmp(buffer, "new") == 0){
	// 		log << buffer << "\n";
	// 	} else if(strcmp(buffer, "force") == 0){
	// 		log << buffer << "\n";
	// 		break;
	// 	} else if(strcmp(buffer, "go") == 0){
	// 		log << buffer << "\n";
	// 		break;
	// 	} else if(strcmp(buffer, "white") == 0){
	// 		log << buffer << "\n";
	// 		break;
	// 	} else if(strcmp(buffer, "black") == 0){
	// 		log << buffer << "\n";
	// 		break;
	// 	} else if(strcmp(buffer, "quit") == 0){
	// 		log << buffer << "\n";
	// 		break;
	// 	} else if(strcmp(buffer, "resign") == 0){
	// 		log << buffer << "\n";
	// 		break;
	// 	} else if(strcmp(buffer, "move") == 0){
	// 		log << buffer << "\n";
	// 		break;
	// 	}

	// 	log.close();
	// 	// fprintf(stdout, "%s\n", "d4d5");

	// }

	log.close();

	//
	while(1)
	{
		cin >> buffer;

		log.open("log.txt", ios::app);

		log << buffer << "\n";

		// log.close();

		if ( chessboard[7][E] )
		{
			cout << "move e7e5\n";
			chessboard[7][E] = 0;
			chessboard[5][E] = 1;
			log << "TRYING: move e7e5\n";
			print_board();
		}
		else if ( chessboard[7][D] )
		{
			cout << "move d7d5\n";
			chessboard[7][D] = 0;
			chessboard[5][D] = 1;
			log << "TRYING: move d7d5\n";
			print_board();
		}
		else if ( chessboard[7][A] )
		{
			cout << "move a7a5\n";
			chessboard[7][A] = 0;
			chessboard[5][A] = 1;
			log << "TRYING: move a7a5\n";
			print_board();
		}
		else if ( chessboard[7][B] )
		{
			cout << "move b7b5\n";
			chessboard[7][B] = 0;
			chessboard[5][B] = 1;
			log << "TRYING: move b7b5\n";
			print_board();
		}
		else
		{
			cout << "resign\n";
			log << "TRYING: resign\n";
		}

		if(strcmp(buffer, "xboard") == 0){
			log << buffer << "\n";
		} else if(strcmp(buffer, "new") == 0){
			log << buffer << "\n";
		} else if(strcmp(buffer, "force") == 0){
			log << buffer << "\n";
			break;
		} else if(strcmp(buffer, "go") == 0){
			log << buffer << "\n";
			break;
		} else if(strcmp(buffer, "white") == 0){
			log << buffer << "\n";
			break;
		} else if(strcmp(buffer, "black") == 0){
			log << buffer << "\n";
			break;
		} else if(strcmp(buffer, "quit") == 0){
			log << buffer << "\n";
			break;
		} else if(strcmp(buffer, "resign") == 0){
			log << buffer << "\n";
			break;
		} else if(strcmp(buffer, "move") == 0){
			log << buffer << "\n";
			break;
		}

		log.close();
		// fprintf(stdout, "%s\n", "d4d5");

	}
	//

	return 0;
}