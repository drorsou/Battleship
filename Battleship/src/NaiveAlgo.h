#pragma once
#include "BaseAlgo.h"
#include "Ship.h"


class NaiveAlgo : public BaseAlgo
{	
	std::pair<int, int> nextAttack;

	void addOne();

public:


	explicit NaiveAlgo();
	//~NaiveAlgo();

	std::pair<int, int> attack() override;
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;
};