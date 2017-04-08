#pragma once
#include <list>

enum Type { Boat, Cruiser, Submarine, Destroyer };

class Ship {
private:
	std::list<int, int> coords;
	Type type;

public:
	Ship();

};