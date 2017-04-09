#pragma once
#include <utility>

using namespace std;

enum Type { Boat, Cruiser, Submarine, Destroyer };

class Ship {
	pair<int, int> vert;
	pair<int, int> horz;
	Type type;

public:
	Ship();
	Ship(int, int, int, int, Type);
	Ship(pair<int, int>, pair<int, int>, Type);
	bool checkCoord(pair<int, int>);
};