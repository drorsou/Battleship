#pragma once
#include <memory>
#include "IBattleshipGameAlgo.h"
template <typename T>
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

	TestingBoard(int rows, int cols, int depth)
		: _rows(rows),
		  _cols(cols),
		  _depth(depth)
	{
		board = unique_ptr<T>(new T[_rows*_cols*_depth]);
	}
	void initVal(T val)
	{
		for (int i = 0; i < _rows*_cols*_depth; i++)
			(board.get())[i] = val;
	}
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
		return (board.get())[position(c)];
	}
	void setAt(Coordinate c, T val)
	{
		(board.get())[position(c)] = val;
	}
};
