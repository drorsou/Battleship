#include "BaseAlgo.h"

BaseAlgo::~BaseAlgo()
{
	for (int i = 0; i < numOfRows; i++)
	{
		delete[] board[i];
		delete[] shadow_board[i];
	}
	delete[] board;
	delete[] shadow_board;
}

void BaseAlgo::setBoard(int player, const char** board, int numRows, int numCols)
{
	this->numOfRows = numRows;
	this->numOfCols = numCols;
	this->player_number = player;
	for (int row = 0; row < numRows; row++)
		for (int col = 0; col < numCols; col++)
		{
			this->board[row][col] = board[row][col];
			if (board[row][col] != static_cast<char>(Ship::Symbol::Blank))
			{
				this->shadow_board[row][col] = DontAttack;
				if (row > 0)
				{
					this->shadow_board[row - 1][col] = DontAttack;
				}
				if (col > 0)
				{
					this->shadow_board[row][col - 1] = DontAttack;
				}
				if (row < numRows - 1)
				{
					this->shadow_board[row + 1][col] = DontAttack;
				}
				if (col < numCols - 1)
				{
					this->shadow_board[row][col + 1] = DontAttack;
				}
			}
		}
}

bool BaseAlgo::init(const std::string& path)
{
	return true;
}
