#pragma once
#include <utility>
class boardArray
{
	char * arr;
	int numOfRows;
	int numOfCols;
public:
	boardArray()
		: arr(nullptr),
		  numOfRows(-1), 
		  numOfCols(-1)
	{}
	boardArray(int num_of_rows, int num_of_cols)
		: numOfRows(num_of_rows),
		  numOfCols(num_of_cols)
	{
		arr = new char[num_of_rows*num_of_cols];
	}
	~boardArray() { delete[] arr; }
	int num_of_rows() const
	{
		return numOfRows;
	}
	void setDimensions(int num_of_rows, int num_of_cols)
	{
		if (arr == nullptr)
		{
			this->numOfRows = num_of_rows;
			this->numOfCols = num_of_cols;
			arr = new char[num_of_rows*num_of_cols + 1];
			arr[num_of_rows*num_of_cols] = '\0';
		}
	}
	int num_of_cols() const
	{
		return numOfCols;
	}
	char getPos(int row, int col) const
	{
		return arr[row*numOfCols + col];
	}
	void setPos(int row, int col, char c) const
	{
		 arr[row*numOfCols + col] = c;
	}

	boardArray(const boardArray& other)
		: arr(other.arr),
		  numOfRows(other.numOfRows),
		  numOfCols(other.numOfCols)
	{
	}

	boardArray(boardArray&& other)
		: 
		  numOfRows(other.numOfRows),
		  numOfCols(other.numOfCols)
	{
		this->arr = other.arr;
		other.arr = nullptr;
	}

	boardArray& operator=(const boardArray& other)
	{
		if (this == &other)
			return *this;
		arr = other.arr;
		numOfRows = other.numOfRows;
		numOfCols = other.numOfCols;
		return *this;
	}

	boardArray& operator=(boardArray&& other) noexcept
	{
		if (this == &other)
			return *this;
		this->arr = other.arr;
		other.arr = nullptr;
		numOfRows = other.numOfRows;
		numOfCols = other.numOfCols;
		return *this;
	}
};