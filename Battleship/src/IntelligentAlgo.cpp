#include "IntelligentAlgo.h"


void IntelligentAlgo::addTwo()
{
	if (nextAttack.second >= numOfCols - 2)
	{
		if (nextAttack.first < numOfRows - 1)
			numberOfRuns++;
		nextAttack.first = (nextAttack.first + 1) % numOfRows;
	}
	nextAttack.second = (nextAttack.second + 2) % numOfCols;
}

void IntelligentAlgo::markLeft(int row, int col)
{
	if (col > 0)
		shadow_board[row][col - 1] = DontAttack;
}

void IntelligentAlgo::markRight(int row, int col)
{
	if (col < numOfCols)
		shadow_board[row][col + 1] = DontAttack;
}

void IntelligentAlgo::markUp(int row, int col)
{
	if (row > 0)
		shadow_board[row - 1][col] = DontAttack;
}

void IntelligentAlgo::markDown(int row, int col)
{
	if (row < numOfRows)
		shadow_board[row + 1][col] = DontAttack;
}

void IntelligentAlgo::addAttackLeft(int row, int col)
{
	if (col > 0 && shadow_board[row][col - 1] == Attack)
		possibleAttacks.emplace(row, col - 1, Left);
}

void IntelligentAlgo::addAttackRight(int row, int col)
{
	if (col < numOfCols - 1 && shadow_board[row][col + 1] == Attack)
		possibleAttacks.emplace(row, col + 1, Right);
}

void IntelligentAlgo::addAttackUp(int row, int col)
{
	if (row > 0 && shadow_board[row - 1][col] == Attack)
		possibleAttacks.emplace(row - 1, col, Up);
}

void IntelligentAlgo::addAttackDown(int row, int col)
{
	if (row < numOfRows - 1 && shadow_board[row + 1][col] == Attack)
		possibleAttacks.emplace(row + 1, col, Down);
}

void IntelligentAlgo::markSink(int row, int col, direction dir)
{
	switch (dir)
	{
	case Up:
		markUp(row, col);
		markLeft(row, col);
		markRight(row, col);
		break;
	case Down:
		markDown(row, col);
		markLeft(row, col);
		markRight(row, col);
		break;
	case Right:
		markDown(row, col);
		markUp(row, col);
		markRight(row, col);
		break;
	case Left:
		markDown(row, col);
		markLeft(row, col);
		markUp(row, col);
		break;
	case None:
		markUp(row, col);
		markDown(row, col);
		markLeft(row, col);
		markRight(row, col);
		break;
	}
}

void IntelligentAlgo::markHit(int row, int col, direction dir)
{
	switch(dir)
	{
	case Up:
		markLeft(row, col);
		markRight(row, col);
		markLeft(row - 1, col);
		markRight(row - 1, col);
		break;
	case Down:
		markLeft(row, col);
		markRight(row, col);
		markLeft(row + 1, col);
		markRight(row + 1, col);
		break;
	case Left:
		markLeft(row, col);
		markRight(row, col);
		markLeft(row, col + 1);
		markRight(row, col + 1);
		break;
	case Right:
		markLeft(row, col);
		markRight(row, col);
		markLeft(row, col - 1);
		markRight(row, col - 1);
		break;
	case None:
		break;
	}
}

void IntelligentAlgo::addAttacks(int row, int col, direction dir)
{
	switch (dir)
	{
	case Up:
		addAttackUp(row, col);
		break;
	case Down:
		addAttackDown(row, col);
		break;
	case Left:
		addAttackLeft(row, col);
		break;
	case Right:
		addAttackRight(row, col);
		break;
	case None:
		addAttackUp(row, col);
		addAttackDown(row, col);
		addAttackLeft(row, col);
		addAttackRight(row, col);
		break;
	}
}

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
	std::tuple<int,int,direction> temp;
	if(!possibleAttacks.empty())
	{
		do
		{
			temp = possibleAttacks.top();
			possibleAttacks.pop();
		} while (shadow_board[std::get<0>(temp)][std::get<1>(temp)] != Attack);
		lastFired = temp;
		return make_pair(std::get<0>(temp), std::get<1>(temp));
	}	
	while(numberOfRuns < 2) // not suppose to happen
	{
		if (shadow_board[nextAttack.first][nextAttack.second] == Attack)
		{
			std::pair<int,int> res = nextAttack;			
			addTwo();
			lastFired = make_tuple(res.first, res.second, None);
			return res;
		}
		// If marked Don't Attack or Attacked, keep looking for a target
		addTwo();
	}
	/*
	 *  We get here only if there are no more targets (and the last target is 10,10)
	 *  We should get here only once - and win in this turn 
	 *  (otherwise we have passed all the possible tiles and we haven't sank all of the enemy ships)
	*/
	lastFired = make_tuple(nextAttack.first, nextAttack.second, None);
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
		if(player == player_number)
		{
			if (result == AttackResult::Sink)
			{
				markSink(row, col, std::get<2>(lastFired));
			}
			else
			{
				markHit(row, col, std::get<2>(lastFired));
				addAttacks(row, col, std::get<2>(lastFired));
			}
		}
		else if (board[row][col] == BLANK)
		{
			addAttacks(row, col, None);
		}
		board[row][col] = HIT_SYM;
	}
	shadow_board[row][col] = tileMarks::Attacked;
}

