#include "NaiveAlgo.h"

void NaiveAlgo::addOne()
{
}

NaiveAlgo::NaiveAlgo(int player_number, int num_of_rows, int num_of_cols): player_number(player_number),
                                                                           numOfRows(num_of_rows),
                                                                           numOfCols(num_of_cols)
{
	shadow_board = new tileMarks*[numOfRows];
	board = new char*[numOfRows];
	for (int i = 0; i < numOfRows; i++)
	{
		shadow_board[i] = new tileMarks[numOfCols];
		board[i] = new char[numOfCols];
		for (int j = 0; j < numOfCols; j++)
		{
			shadow_board[i][j] = tileMarks::Attack;
		}
	}
	nextAttack = make_pair(0, 0);
}

NaiveAlgo::~NaiveAlgo()
{
	for (int i = 0; i < numOfRows; i++)
	{
		delete[] board[i];
		delete[] shadow_board[i];
	}
	delete[] board;
	delete[] shadow_board;
}
