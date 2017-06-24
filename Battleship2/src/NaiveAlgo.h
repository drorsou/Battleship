#pragma once
#include "IBattleshipGAmeAlgo.h"

class NaiveAlgo : public IBattleshipGameAlgo
{
	Coordinate lastAttack;
	int rows;
	int cols;
	int depth;

public:
	NaiveAlgo() : lastAttack(Coordinate(0,0,0)) {};

	void setPlayer(int player) override
	{
	}

	void setBoard(const BoardData& board) override
	{
		rows = board.rows();
		cols = board.cols();
		depth = board.depth();
	}

	Coordinate attack() override
	{
		int row, col, dpth;
		if (lastAttack.row + 1 == rows)
		{
			if (lastAttack.col + 1 == cols)
			{
				lastAttack = Coordinate(0, 0, lastAttack.depth + 1);
			}
			lastAttack = Coordinate(0, lastAttack.col + 1, lastAttack.depth);
		}
		lastAttack = Coordinate(lastAttack.row + 1, lastAttack.col, lastAttack.depth);

		return lastAttack;
	}

	void notifyOnAttackResult(int player, Coordinate move, AttackResult result) override
	{
	}
};