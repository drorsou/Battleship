#pragma once
#include <utility>

class AttackNaive
{
	int currRowAttack;
	int currColAttack;
	public:
		AttackNaive();
		~AttackNaive();

		/*
		Post: returns the coords of the next attack
		*/
		std::pair<int, int> getNextAttack();

		bool hasAttacks();
};




	





