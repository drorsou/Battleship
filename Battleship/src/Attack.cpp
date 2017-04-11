#include "Attack.h"
#include <iostream>

Attack::Attack()
{	

}

Attack::Attack(std::vector<std::pair<int, int>> list) : list(list), index(0) {}

std::pair<int, int> Attack::getNextAttack()
{
	index++;
	std::cout << index - 1 << " out of " << list.size() << std::endl;
	return list[index - 1];
}

bool Attack::hasAttacks()
{
	if (index < list.size())
		return true;
	else
		return false;
}
