#pragma once
#include <functional>
#include <vector>
#include <string>

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


template<typename T>
class Matrix2d
{
	T* matrix;
	int numOfRows;
	int numOfCols = 0;

	class Coord
	{
		int x = 0, y = 0;
		// needs iterator
	};

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
		std::vector<std::pair<std::string, std::vector<Matrix2d::Coord>>> all_groups;



		return all_groups;
	}
};
