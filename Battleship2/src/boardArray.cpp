#include "boardArray.h"
#include <iostream>

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

//void boardArray::printBoard()
//{
//	Ship::Symbol * b = arr.get();
//	for(int d = 1; d <= this->depth(); d++)
//	{
//		for (int row = 1; row <= this->rows(); row++)
//		{
//			for (int col = 1; col <= this->cols(); col++)
//				std::cout << static_cast<char>(b[position(row, col, d)]);
//			std::cout << endl;
//		}
//		std::cout << "------------------" << std::endl;
//	}
//}

char boardArray::charAt(Coordinate c) const
{
	return static_cast<char>((arr.get())[position(c.row, c.col, c.depth)]);
}

void boardArray::setCharAt(Coordinate c, Ship::Symbol s)
{
	(arr.get())[position(c.row, c.col, c.depth)] = s;	
}
