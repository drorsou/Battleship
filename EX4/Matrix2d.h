#pragma once
#include <functional>
#include <vector>
#include <string>
#include <array>
#define MAX(x,y) (((x)>(y))?(x):(y))

/*
template<class T, size_t DIMENSIONS>
class Matrix
{

};

template<class T>
using Matrix2d = Matrix<T, 2>;
template<class T>
using Matrix3d = Matrix<T, 3>;
*/
template <size_t SIZE>
class Coord
{
	std::array<size_t, SIZE> arr;
	Coord(std::initializer_list<size_t> list)
	{
		if (list.size() == SIZE)
		{
			auto itr = list.begin();
			auto end = list.end();
			for (size_t i = 0; i < SIZE; i++, ++itr)
				arr[i] = *itr;
		}
	}
public:
	size_t& operator[](size_t pos)
	{
		return arr[pos];
	}
	auto& begin() const
	{
		return arr.begin();
	}
	auto& end() const
	{
		return arr.end();
	}
	const auto& cbegin() const
	{
		return arr.cbegin();
	}
	const auto& cend() const
	{
		return arr.cend();
	}
};

template<typename T>
class Matrix2d
{
	T* matrix;
	size_t numOfRows;
	size_t numOfCols = 0;

	

public:

	Matrix2d(std::initializer_list<std::initializer_list<T>> list)
	{
		// Find the matrix' size
		numOfRows = list.size();
		for (auto& row : list)
			numOfCols = MAX(row.size(), numOfCols);

		// Initialize the matrix
		matrix = new T[numOfRows*numOfCols];

		// Fill the matrix with values
		// Not done
		int i = 0;
		for (auto& row : list) {
			for (auto& val : row) {
				matrix[i++] = val;
			}
		}
	}

	auto groupValues(const std::function<std::string(T)>& f)
	{
		std::vector<std::pair<std::string, std::vector<std::vector<Coord<2>>>>> all_groups;



		return all_groups;
	}
};
