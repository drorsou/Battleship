#include "BaseAlgo.h"

void BaseAlgo::createBoards()
{
	this->board = new char*[this->numOfRows];
	this->shadow_board = new tileMarks*[this->numOfRows];
	for (int row = 0; row < this->numOfRows; row++)
	{
		this->board[row] = new char[this->numOfCols];
		this->shadow_board[row] = new tileMarks[this->numOfCols];
		for (int col = 0; col < this->numOfCols; col++)
			this->shadow_board[row][col] = tileMarks::Attack;
	}
}

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
	createBoards();
	for (int row = 0; row < numRows; row++)
		for (int col = 0; col < numCols; col++)
		{
			this->board[row][col] = board[row][col];
			if (board[row][col] != static_cast<char>(Ship::Symbol::Blank))
			{
				this->shadow_board[row][col] = tileMarks::DontAttack;
				if (row > 0)
				{
					this->shadow_board[row - 1][col] = tileMarks::DontAttack;
				}
				if (col > 0)
				{
					this->shadow_board[row][col - 1] = tileMarks::DontAttack;
				}
				if (row < numRows - 1)
				{
					this->shadow_board[row + 1][col] = tileMarks::DontAttack;
				}
				if (col < numCols - 1)
				{
					this->shadow_board[row][col + 1] = tileMarks::DontAttack;
				}
			}			
		}
}

bool BaseAlgo::init(const std::string& path)
{
	return true;
}
