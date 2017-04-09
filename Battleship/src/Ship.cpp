#include "Ship.h"

bool Ship::checkCoord(pair<int, int> coord) {
	return (coord.first <= this->horz.second && coord.first >= this->horz.first) &&
		(coord.second <= this->vert.second && coord.second >= this->vert.first);
}

bool Ship::checkDimensions(int vertlow, int verthigh, int horzlow, int horzhigh, Type t)
{
	int len;
	if (verthigh < vertlow || horzhigh < horzlow || (vertlow < verthigh && horzlow < horzhigh))
		return false;
	if (vertlow < verthigh)
	{
		len = verthigh - vertlow + 1;
	}
	else
	{
		len = horzhigh - horzlow + 1;
	}
	switch (t)
	{
	case Boat:
		return len == BOAT_LEN;
		break;
	case Cruiser:
		return len == CRUISER_LEN;
		break;
	case Submarine:
		return len == SUBMARINE_LEN;
		break;
	case Destroyer:
		return len == DESTROYER_LEN;
		break;
	}
}