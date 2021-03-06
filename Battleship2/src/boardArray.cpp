#include "boardArray.h"

void boardArray::copyArr(const std::unique_ptr<Ship::Symbol>& otherArr)
{
	Ship::Symbol * thisBoard = arr.get();
	const Ship::Symbol * otherBoard = otherArr.get();
	if (otherArr.get() == nullptr)
		arr = unique_ptr<Ship::Symbol>(new Ship::Symbol[_rows * _cols * _depth]);	
	for (int pos = 0; pos < _rows*_cols*_depth; pos++)
	{		
		thisBoard[pos] = otherBoard[pos];		
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

boardArray::boardArray(const boardArray& other)
{
	_rows = other.rows();
	_cols = other.cols();
	_depth = other.depth();
	arr = unique_ptr<Ship::Symbol>(new Ship::Symbol[_rows * _cols * _depth]);
	copyArr(other.arr);
}

boardArray::boardArray(boardArray&& other) noexcept
{
	_rows = other.rows();
	_cols = other.cols();
	_depth = other.depth();
	auto temp = arr.release();
	arr.reset(other.arr.release());
	other.arr.reset(temp);
}

boardArray& boardArray::operator=(const boardArray& other)
{
	if (this == &other)
		return *this;
	_rows = other.rows();
	_cols = other.cols();
	_depth = other.depth();
	arr = unique_ptr<Ship::Symbol>(new Ship::Symbol[_rows * _cols * _depth]);
	copyArr(other.arr);
	return *this;
}

boardArray& boardArray::operator=(boardArray&& other) noexcept
{
	if (this == &other)
		return *this;
	_rows = other.rows();
	_cols = other.cols();
	_depth = other.depth();
	auto temp = arr.release();
	arr.reset(other.arr.release());
	other.arr.reset(temp);
	return *this;
}
