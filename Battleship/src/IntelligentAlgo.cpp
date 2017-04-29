#include "IntelligentAlgo.h"

void IntelligentAlgo::setBoard(int color, const char** board, int numRows, int numCols) {
	player_number = color;
	for (int row = 0; row < numRows; row++)
		for (int col = 0; col < numCols; col++) 
		{
			this->board[row][col] = board[row][col];
			if (board[row][col] != BLANK)
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
				if (row < numRows)
				{
					this->shadow_board[row + 1][col] = DontAttack;
				}
				if (col < numCols)
				{
					this->shadow_board[row][col + 1] = DontAttack;
				}				
			}			
		}
};