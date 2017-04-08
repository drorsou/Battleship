#pragma once
#include <utility>

using namespace std;

enum Type { Boat, Cruiser, Submarine, Destroyer };

class Ship {
	pair<int, int> *coords;
	Type type;

public:
	Ship();

};