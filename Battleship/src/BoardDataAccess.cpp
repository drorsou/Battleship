#include "BoardDataAccess.h"

char BoardDataAccess::charAt(Coordinate c) const
{
	if (c.col < 1 || c.col > _cols || c.row < 1 || c.row > _rows || c.depth < 1 || c.depth > _depth)
		return static_cast<char>(Ship::Symbol::Blank);
	char result = board->charAt(c);
	if (Ship::checkColor(result) == player)
		return result;
	else
		return static_cast<char>(Ship::Symbol::Blank);
}

BoardDataAccess::BoardDataAccess(const boardArray* b, int player):
	BoardData(),
	board(b),
	player(player)
{
	_rows = b->rows();
	_cols = b->cols();
	_depth = b->depth();
}
