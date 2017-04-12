#pragma once
#include <utility>

using namespace std;

enum Type { Boat, Cruiser, Submarine, Destroyer };
#define BOAT_LEN 1
#define CRUISER_LEN 2
#define SUBMARINE_LEN 3
#define DESTROYER_LEN 4
#define ABOAT 'B'
#define BBOAT 'b'
#define ACRUISER 'P'
#define BCRUISER 'p'
#define ASUBMARINE 'M'
#define BSUBMARINE 'm'
#define ADESTROYER 'D'
#define BDESTROYER 'd'
#define BLANK ' '
#define HIT_SYM '*'
#define MISS_SYM '@'
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
	Ship(const Ship& ship) {
		this->horz = make_pair(ship.horz.first, ship.horz.second);
		this->vert = make_pair(ship.vert.first, ship.vert.second);
		this->type = ship.type;
		this->size = ship.size;
	}

	Type getType() { 
		return this->type;
	}
	/*
		Pre: gets coordinates, and a ship type.
		Post: return true iff these coordinates are legal for this ship type.
	*/
	static bool checkDimensions(int, char);

	/*
		Pre: the ship was attacked, 
			AND this isn't the second attack at this tile!
		Post: lowers its size by 1, 
			and returns true iff the ship has sunk.
	*/
	bool hit() {
		return (--this->size) == 0 ? true : false;
	}
};