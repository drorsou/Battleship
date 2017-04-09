#pragma once
#include <utility>

class Attack {
	std::pair<int, int> *list;
	int index;
	
public:
	Attack();
	std::pair<int, int> getNextAttack();
};