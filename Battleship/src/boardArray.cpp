#include "boardArray.h"

void boardArray::copyArr(const std::unique_ptr<Ship::Symbol>& otherArr)
{
	if (arr.get() == nullptr)
		arr = unique_ptr<Ship::Symbol>(new Ship::Symbol[_rows * _cols * _depth]);	
	for (int pos = 0; pos < _rows*_cols*_depth; pos++)
	{		
		(arr.get())[pos] = (otherArr.get())[pos];		
	}
}

boardArray::boardArray(int rows, int cols, int depth)
{
	if (arr == nullptr)
	{
		this->_rows = rows;
		this->_cols = cols;
		this->_depth = depth;
		arr = unique_ptr<Ship::Symbol>(new Ship::Symbol[_rows * _cols * _depth]);
	}
}

char boardArray::charAt(Coordinate c) const
{
	return static_cast<char>((arr.get())[position(c.row, c.col, c.depth)]);
}

void boardArray::setCharAt(Coordinate c, Ship::Symbol s)
{
	(arr.get())[position(c.row, c.col, c.depth)] = s;
}
