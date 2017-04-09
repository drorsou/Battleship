#include "Attack.h"

Attack::Attack() {

}

std::pair<int, int> Attack::getNextAttack()
{
	// Check if index < list.size
	index++;
	return list[index - 1];
}
