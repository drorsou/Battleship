#include "AttackNaive.h"



AttackNaive::AttackNaive()
{
}


AttackNaive::~AttackNaive()
{
}




std::pair<int, int> AttackNaive::getNextAttack()
{
	std::pair<int, int> temp;
	temp = list.front();
	list.pop();
	return temp;
}

bool AttackNaive::hasAttacks()
{
	
}










