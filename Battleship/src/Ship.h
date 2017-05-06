#pragma once
#include <utility>

using namespace std;

enum Type { Boat, Cruiser, Submarine, Destroyer };
#define BOAT_LEN 1
#define CRUISER_LEN 2
#define SUBMARINE_LEN 3
#define DESTROYER_LEN 4
#define BOAT_SCORE 2
#define CRUISER_SCORE 3
#define SUBMARINE_SCORE 7
#define DESTROYER_SCORE 8
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
	int score;

	void setScoreAndLength(Type t);
public:	
	
	Ship(){
	}

	/*
	Pre: vertlow <= verthigh, horzlow <= horzhigh and one must be an equallity.
	Also, the length of the ship is correct.
	Post: Creates a new ship
	*/
	Ship(int vertlow, int verthigh, int horzlow, int horzhigh, Type t);

	Ship(std::pair<int, int> v, std::pair<int, int> h, Type t);

	Ship(const Ship& ship);

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
	bool isInThisShip(int row, int col) const { return (row <= vert.second && row >= vert.first) && (col <= horz.second && col >= horz.first); }


	/*
		Pre: the ship was attacked, 
			AND this isn't the second attack at this tile!
		Post: lowers its size by 1, 
			and returns true iff the ship has sunk.
	*/
	bool hit() { return (--this->size) == 0 ? true : false;	}

	Ship& operator=(Ship& other);
};