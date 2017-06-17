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

	char charAt(Coordinate c) const;
	
	void setCharAt(Coordinate c, Ship::Symbol s);


	boardArray(const boardArray& other)
		: _rows(other.rows()),
		  _cols(other.cols()),
		  _depth(other.depth())
	{
		if (this->arr.get() != nullptr)
		{
			this->arr.release();
			this->arr.reset(new Ship::Symbol[_rows * _cols * _depth]);
		}
		else
			arr = unique_ptr<Ship::Symbol>(new Ship::Symbol[_rows * _cols * _depth]);
		copyArr(other.arr);
		printf("BoardArray - Using regular copy ctor\n");
	}

	boardArray(boardArray&& other) noexcept
		: _rows(other.rows()),
		  _cols(other.cols()),
		  _depth(other.depth())
	{
		auto temp = arr.release();
		arr.reset(other.arr.release());
		other.arr.reset(temp);
		printf("BoardArray - Using regular move ctor\n");
	}

	boardArray& operator=(const boardArray& other)
	{
		if (this == &other)
			return *this;
		_rows = other.rows();
		_cols = other.cols();
		_depth = other.depth();
		arr = unique_ptr<Ship::Symbol>(new Ship::Symbol[_rows * _cols * _depth]);
		copyArr(other.arr);
		printf("BoardArray - Using = copy ctor\n");
		return *this;
	}

	boardArray& operator=(boardArray&& other) noexcept
	{
		if (this == &other)
			return *this;
		_rows = other.rows();
		_cols = other.cols();
		_depth = other.depth();
		auto temp = arr.release();
		arr.reset(other.arr.release());
		other.arr.reset(temp);
		printf("BoardArray - Using = move ctor\n");
		return *this;
	}
};
