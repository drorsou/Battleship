#pragma once
#include "IBattleshipGameAlgo.h"
#include "Ship.h"
#include <utility>
#include <memory>


class boardArray
{
private:
	int _rows = 0;
	int _cols = 0;
	int _depth = 0;
	std::unique_ptr<Ship::Symbol> arr = nullptr;

	void copyArr(const std::unique_ptr<Ship::Symbol>& otherArr);

	int position(int row, int col, int depth) const
	{
		return (col-1) + (row-1) * _cols + (depth-1) * _rows * _cols;
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
	boardArray(){}
	
	~boardArray() {}

	boardArray(int rows, int cols, int depth);

	//void setDimensions(int rows, int cols, int depth);
	//void printBoard();
	char charAt(Coordinate c) const;
	
	void setCharAt(Coordinate c, Ship::Symbol s);


	boardArray(const boardArray& other);

	boardArray(boardArray&& other) noexcept;

	boardArray& operator=(const boardArray& other);

	boardArray& operator=(boardArray&& other) noexcept;
};
