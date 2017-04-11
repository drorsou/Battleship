#include "Attack.h"

Attack::Attack(std::vector<std::pair<int, int>> list) : list(list), index(0) {

}

std::pair<int, int> Attack::getNextAttack()
{
	// Check if index < list.size
	index++;
	return list[index - 1];
}
