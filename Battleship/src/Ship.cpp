#include "Ship.h"

bool Ship::checkCoord(pair<int, int> coord) {
	return (coord.first <= this->horz.second && coord.first >= this->horz.first) &&
		(coord.second <= this->vert.second && coord.second >= this->vert.first);
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