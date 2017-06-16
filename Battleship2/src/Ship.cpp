#include "Ship.h"


void Ship::setScoreAndLength(Type t)
{
	switch (t)
	{
	case Boat:
		size = ShipLen::BoatLen;
		score = Score::BoatScore;
		break;
	case Cruiser:
		size = ShipLen::CruiserLen;
		score = Score::CruiserScore;
		break;
	case Submarine:
		size = ShipLen::SubmarineLen;
		score = Score::SubmarineScore;
		break;
	case Destroyer:
		size = ShipLen::DestroyerLen;
		score = Score::DestroyerScore;
		break;
	}
}

Ship::Ship(int vertlow, int verthigh, int horzlow, int horzhigh, Type t) {
	vert = std::make_pair(vertlow, verthigh);
	horz = std::make_pair(horzlow, horzhigh);
	type = t;
	setScoreAndLength(t);
}


Ship::Ship(std::pair<int, int> v, std::pair<int, int> h, Type t) {
	vert = v;
	horz = h;
	type = t;
	setScoreAndLength(t);
}

bool Ship::checkDimensions(int len, char t)
{	
	switch (t)
	{
	case static_cast<char>(Symbol::ABoat):	case static_cast<char>(Symbol::BBoat):
		return len == ShipLen::BoatLen;
		break;
	case static_cast<char>(Symbol::ACruiser) : case static_cast<char>(Symbol::BCruiser) :
		return len == ShipLen::CruiserLen;
		break;
	case static_cast<char>(Symbol::ASubmarine) : case static_cast<char>(Symbol::BSubmarine) :
		return len == ShipLen::SubmarineLen;
		break;
	case static_cast<char>(Symbol::ADestroyer) : case static_cast<char>(Symbol::BDestroyer) :
		return len == ShipLen::DestroyerLen;
		break;
	}
	return false;
}

Ship::Ship(const Ship& other) :
	type(other.type),
	size(other.size),
	score(other.score)
{
	vert = make_pair(other.vert.first, other.vert.second);
	horz = make_pair(other.horz.first, other.horz.second);
}

Ship& Ship::operator=(Ship&& other)
{
	if (this == &other)
		return *this;
	vert = make_pair(other.vert.first, other.vert.second);
	horz = make_pair(other.horz.first, other.horz.second);
	type = other.type;
	size = other.size;
	score = other.score;
	return *this;
}
