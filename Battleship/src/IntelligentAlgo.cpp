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
}

bool IntelligentAlgo::init(const std::string& path)
{
	return true;
}

std::pair<int, int> IntelligentAlgo::attack()
{
	std::pair<int, int> res;
	if(!possibleAttacks.empty())
	{
		res = possibleAttacks.top();
		possibleAttacks.pop();
		return res;
	}	
	while(nextAttack.first < numOfRows - 1 && nextAttack.second < numOfCols - 1) // not suppose to happen
	{
		if (shadow_board[nextAttack.first][nextAttack.second] == Attack)
		{
			res = nextAttack;
			addOne();
			return res;
		}
		// If marked Don't Attack or Attacked, keep looking for a target
		addOne();
	}
	/*
	 *  We get here only if there are no more targets (and the last target is 10,10)
	 *  We should get here only once - and win in this turn 
	 *  (otherwise we have passed all the possible tiles and we haven't sank all of the enemy ships)
	*/
	return nextAttack;
}

void IntelligentAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
	if(result == AttackResult::Miss)
	{
		board[row][col] = MISS_SYM;		
	}
	else
	{
		if (!checkColor(row, col))
		{
			if(row > 0 && shadow_board[row - 1][col] == tileMarks::Attack)
			{
				possibleAttacks.emplace(row - 1, col);
			}
			if(col > 0 && shadow_board[row][col - 1] == tileMarks::Attack)
			{
				possibleAttacks.emplace(row, col - 1);
			}
			if (row < numOfRows - 1 && shadow_board[row + 1][col] == tileMarks::Attack)
			{
				possibleAttacks.emplace(row + 1, col);
			}
			if (col < numOfCols && shadow_board[row][col + 1] == tileMarks::Attack)
			{
				possibleAttacks.emplace(row, col + 1);
			}
		}
	}
	shadow_board[row][col] = tileMarks::Attacked;
}

