#pragma once
#include <memory>
#include "IBattleshipGameAlgo.h"
template <enum T>
class TestingBoard
{
	std::unique_ptr<T> board;
	int _rows;
	int _cols;
	int _depth;

	int position(Coordinate c) const
	{
		return (c.col - 1) + (c.row - 1) * _cols + (c.depth - 1) * _rows * _cols;
	}
public:
	int rows() const
	{
		return _rows;
	}

	int cols() const
	{
		return _cols;
	}

	int depth() const
	{
		return _depth;
	}
	T& operator[](Coordinate c)
	{
		return (board.get())(position(c));
	}
};
