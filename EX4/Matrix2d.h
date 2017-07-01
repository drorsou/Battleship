#pragma once
#include <functional>
#include <vector>
#include <string>
#include <array>
#include <queue>
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
	public:
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
	size_t operator[](size_t pos) const
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
		numOfCols = 0;
		for (auto& row : list)
			numOfCols = MAX(row.size(), numOfCols);

		// Initialize the matrix
		matrix = new T[numOfRows*numOfCols];

		// Fill the matrix with values
		for (int i = 0; i < numOfRows*numOfCols; i++)
		{
			// Default set the matrix values to 0/nullptr/false
			//matrix[i] = 
		}

		int valuesIndex = 0;
		for (auto& row : list) {
			int countCols = 0;
			for (auto& val : row) {
				countCols++;
				matrix[valuesIndex] = val;
				valuesIndex++;
			}
			valuesIndex += numOfCols - countRows;
		}
	}


	T getValFromMatrix(int index) const { return matrix[index]; }
	T getValFromMatrix(const Coord<2>& coord) const { return matrix[getMatrixIndexFromCoord(coord)]; }
	size_t getMatrixIndexFromCoord(const Coord<2> coord) const { return coord[1] * numOfCols + coord[0]; }


	template<typename S> // Switch std::string with template S ?
	auto groupValues(const std::function<std::string(T)>& f)
	{
		//
		// Nested class to hold utility functions
		//
		struct MatrixGroup
		{
			std::string getGroupFromCoord(Coord<2> coord) const
			{
				return f(getValFromMatrix(coord));
			}
			// Maybe use lambda ? [](Matrix2d::Coord coord) { return f(matrix[coord.y * numOfCols + coord.x]); }

			void addFromQueue(Matrix2d * mat, std::queue<Coord<2>>& coordsToGroup, std::string& groupType, std::vector<Coord<2>>& groupCoords, bool* isValGrouped)
			{
				Coord<2> coord = coordsToGroup.front();
				coordsToGroup.pop();
				isValGrouped[mat->getMatrixIndexFromCoord(coord)] = true;

				Coord<2> checkCoord = { coord[0] + 1, coord[1] };				
				if (getGroupFromCoord(checkCoord) == groupType)
				{
					coordsToGroup.push(Coord<2>{checkCoord[0], checkCoord[1]});
				}
				// Copy all other options
			}
		};

		//
		// Beginning of function:
		//

		// Set the mirror matrix of which values are grouped
		bool isValGrouped[numOfRows*numOfCols] = { false };
//		for (int i = 0; i < numOfRows*numOfCols; i++)
//		{
//			isValGrouped[i] = false;
//		}


		std::vector<std::pair<std::string, std::vector<std::vector<Coord<2>>>>> allGroups;
		std::queue<Coord<2>> coordsToGroup;

		// Pass through the matrix and group all its values
		for (int i = 0; i < numOfRows*numOfCols; i++)
			if (isValGrouped[i] == false)
			{
				//coordsToGroup.push(Matrix2d:Coord(i % numOfCols , i / numOfRows));
				std::string type = f(matrix[i]);
				std::vector<Coord<2>> groupCoords;

				// Pass through the current coordinate and add the other coordinates from its group
				while (coordsToGroup.empty() == false)
					MatrixGroup::addFromQueue(coordsToGroup, groupType, groupCoords);

				// Add the type if it doesn't exist
				// Add the group of the current coordinate to the 'allGroups' vector
				bool typeExists = false;
				for (std::pair<std::string, std::vector<std::vector<Coord<2>>>>& groupType : allGroups)
					if (groupType.first == type)
					{
						typeExists = true;
						groupType.second.push_back(groupCoords);
					}
				if (typeExists == false)
					allGroups.push_back(std::pair<std::string, std::vector<std::vector<Coord<2>>>>(type, std::vector<std::vector<Coord<2>>>(1, groupCoords)));
			}

		return allGroups;
	}
};
