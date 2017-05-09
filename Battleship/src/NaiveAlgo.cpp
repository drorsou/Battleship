#include "NaiveAlgo.h"

void NaiveAlgo::addOne()
{
	nextAttack.first += ((nextAttack.second + 1) / numOfCols) % numOfRows; // adds one if col = numOfCols - 1
	nextAttack.second = (nextAttack.second + 1) % numOfCols;
}

NaiveAlgo::NaiveAlgo(){	
	nextAttack = make_pair(0, 0);
}

std::pair<int, int> NaiveAlgo::attack()
{
	while(shadow_board[nextAttack.first][nextAttack.second] != Attack)
	{
		addOne();
	}
	auto res = nextAttack;
	res.first++;
	res.second++;
	addOne();
	return res;
}

void NaiveAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
	row--;
	col--;
	shadow_board[row][col] = Attacked;
}

IBattleshipGameAlgo* GetAlgorithm()
{
	return new NaiveAlgo();
}

