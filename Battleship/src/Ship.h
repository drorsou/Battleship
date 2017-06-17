#pragma once
#include <utility>
#include "IBattleshipGameAlgo.h"

using namespace std;

enum Type { None, Boat, Cruiser, Submarine, Destroyer };
class Ship {
	std::pair<int, int> vert;
	std::pair<int, int> horz;
	std::pair<int, int> depth;
	Type type;
	short int size;
	int score;

	void setScoreAndLength(Type t);
public:	
	static bool checkChar(char c);
	static int checkColor(char c);


	Ship(){
	}

	/*
	Pre: vertlow <= verthigh, horzlow <= horzhigh and one must be an equallity.
	Also, the length of the ship is correct.
	Post: Creates a new ship
	*/
	Ship(int vertlow, int verthigh, int horzlow, int horzhigh, int depthlow, int depthhigh, Type t);

	Ship(std::pair<int, int> v, std::pair<int, int> h, std::pair<int,int> d, Type t);

	Ship(const Ship& ship);

	Ship(Ship&& other) = delete;

	enum ShipLen { BoatLen = 1, CruiserLen = 2, SubmarineLen = 3, DestroyerLen = 4};
	enum Score { BoatScore = 2, CruiserScore = 3, SubmarineScore = 7, DestroyerScore = 8};
	enum class Symbol : char { ABoat = 'B', BBoat = 'b', ACruiser = 'P', BCruiser = 'p', ASubmarine = 'M', BSubmarine = 'm', ADestroyer = 'D', BDestroyer = 'd', Blank = ' ', Hit = '*', MISS = '@' };
	


	Type getType() const { return this->type; }

	int Ship::getScore() const { return this->score; }


	/*
		Pre: gets coordinates, and a ship type.
		Post: return true iff these coordinates are legal for this ship type.
	*/
	static bool checkDimensions(int, char);

	/*
		Pre: gets a valid coordinate.
		Post: returns true iff the coordinate is part of this ship
	*/
	bool isInThisShip(Coordinate c) const { return (c.row <= vert.second && c.row >= vert.first) && (c.col <= horz.second && c.col >= horz.first) && (c.depth <= depth.second && c.depth >= depth.first); }


	/*
		Pre: the ship was attacked, 
			AND this isn't the second attack at this tile!
		Post: lowers its size by 1, 
			and returns true iff the ship has sunk.
	*/
	bool hit() { return (--this->size) == 0 ? true : false;	}

};