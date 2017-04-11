#pragma once
#include <utility>
#include <vector>

class Attack {
	std::vector<std::pair<int, int>> list;
	int index;
	
public:
	Attack();
	Attack(std::vector<std::pair<int, int>> list);
	std::pair<int, int> getNextAttack();
	bool hasAttacks();
};
