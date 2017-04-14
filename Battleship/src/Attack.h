#pragma once
#include <utility>
#include <vector>

class Attack {
	std::vector<std::pair<int, int>> list;
	unsigned int index;
	
public:
	Attack() {}

	Attack(std::vector<std::pair<int, int>> list) : list(list), index(0) {}
	/*
		Pre: there are available attacks (i.e index < list.size)
		Post: returns the coords of the next attack
	*/
	std::pair<int, int> getNextAttack() { return list[index++]; }

	bool hasAttacks() { return index < list.size(); }
};
