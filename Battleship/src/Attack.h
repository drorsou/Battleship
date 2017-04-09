#pragma once
#include <utility>

using namespace std;

class Attack {
	pair<int, int> *list;
	int index;
	//// Made a change
public:
	Attack();
	pair<int, int> getNextAttack();
};