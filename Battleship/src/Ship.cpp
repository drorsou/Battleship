#include "Ship.h"


Ship::Ship(int vertlow, int verthigh, int horzlow, int horzhigh, Type t) {
	vert = std::make_pair(vertlow, verthigh);
	horz = std::make_pair(horzlow, horzhigh);
	type = t;
	switch (t)
	{
	case Boat:
		size = BOAT_LEN;
		score = BOAT_SCORE;
		break;
	case Cruiser:
		size = CRUISER_LEN;
		score = CRUISER_SCORE;
		break;
	case Submarine:
		size = SUBMARINE_LEN;
		score = SUBMARINE_SCORE;
		break;
	case Destroyer:
		size = DESTROYER_LEN;
		score = DESTROYER_SCORE;
		break;
	}
}


Ship::Ship(std::pair<int, int> v, std::pair<int, int> h, Type t) {
	vert = v;
	horz = h;
	type = t;
	switch (t)
	{
	case Boat:
		size = BOAT_LEN;
		score = BOAT_SCORE;
		break;
	case Cruiser:
		size = CRUISER_LEN;
		score = CRUISER_SCORE;
		break;
	case Submarine:
		size = SUBMARINE_LEN;
		score = SUBMARINE_SCORE;
		break;
	case Destroyer:
		size = DESTROYER_LEN;
		score = DESTROYER_SCORE;
		break;
	}
}


Ship::Ship(const Ship& ship) {
	this->horz = make_pair(ship.horz.first, ship.horz.second);
	this->vert = make_pair(ship.vert.first, ship.vert.second);
	this->type = ship.type;
	this->size = ship.size;
}


bool Ship::checkDimensions(int len, char t)
{	
	switch (t)
	{
	case ABOAT:	case BBOAT:
		return len == BOAT_LEN;
		break;
	case ACRUISER: case BCRUISER:
		return len == CRUISER_LEN;
		break;
	case ASUBMARINE: case BSUBMARINE:
		return len == SUBMARINE_LEN;
		break;
	case ADESTROYER: case BDESTROYER:
		return len == DESTROYER_LEN;
		break;
	}
	return false;
}