#include "Attack.h"

Attack::Attack()
{	

}

Attack::Attack(std::vector<std::pair<int, int>> list) : list(list), index(0) {}

std::pair<int, int> Attack::getNextAttack()
{
	index++;
	return list[index - 1];
}

bool Attack::hasAttacks()
{
	if (list.size() < index)
		return true;
	else
		return false;
}
