#pragma once
#include <utility>

using namespace std;

enum Type { Boat, Cruiser, Submarine, Destroyer };
#define BOAT_LEN 1
#define CRUISER_LEN 2
#define SUBMARINE_LEN 3
#define DESTROYER_LEN 4


class Ship {
	std::pair<int, int> vert;
	std::pair<int, int> horz;
	Type type;
	short int size;
public:	
	/*
		Pre: vertlow <= verthigh, horzlow <= horzhigh and one must be an equallity.
				Also, the length of the ship is correct.
		Post: Creates a new ship
	*/
	Ship(){
	}
	Ship(int vertlow, int verthigh, int horzlow, int horzhigh, Type t) {
		vert = std::make_pair(vertlow, verthigh);
		horz = std::make_pair(horzlow, horzhigh);
		type = t;
		switch (t)
		{
		case Boat:
			size = BOAT_LEN;
			break;
		case Cruiser:
			size = CRUISER_LEN;
			break;
		case Submarine:
			size = SUBMARINE_LEN;
			break;
		case Destroyer:
			size = DESTROYER_LEN;
			break;
		}
	}
	Ship(std::pair<int, int> v, std::pair<int, int> h, Type t) {
		vert = v;
		horz = h;
		type = t;
		switch (t)
		{
		case Boat:
			size = BOAT_LEN;
			break;
		case Cruiser:
			size = CRUISER_LEN;
			break;
		case Submarine:
			size = SUBMARINE_LEN;
			break;
		case Destroyer:
			size = DESTROYER_LEN;
			break;
		}
	}	
	/*
		Pre: gets a coord.
		Post: Checks if the coord is part of the current ship.
	*/
	bool checkCoord(pair<int, int>);
	/*
		Pre: gets coordinates, and a ship type.
		Post: return true iff these coordinates are legal for this ship type.
	*/
	static bool checkDimensions(int, int, int, int, Type);
};