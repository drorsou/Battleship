#pragma once
#include <functional>
#include <vector>
#include <string>
#include <array>
#include <queue>
#include <mutex>
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
	Coord(){}
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
	auto begin() const
	{
		return arr.begin();
	}
	auto end() const
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
	std::unique_ptr<T> matrix;
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
		T * temp = new T[numOfRows*numOfCols]; 

		// Fill the matrix with values
		for (size_t i = 0; i < numOfRows*numOfCols; i++)
		{
			// Default set the matrix values to 0/nullptr/false
			//matrix[i] = 
		}

		size_t valuesIndex = 0;
		for (auto& row : list) {
			size_t countCols = 0;
			for (auto& val : row) {
				countCols++;
				temp[valuesIndex] = val;
				valuesIndex++;
			}
			valuesIndex += numOfCols - countCols;
		}
		matrix = std::unique_ptr<T>(temp);
	}
	Matrix2d(size_t rows, size_t cols, T val)
		: numOfRows(rows),
		  numOfCols(cols)
	{
		T * temp = new T[numOfRows*numOfCols];
		for (size_t i = 0; i < numOfRows*numOfCols; i++)
			temp[i] = val;
		matrix = std::unique_ptr<T>(temp);
	}


	T getValAt(size_t index) const { return (matrix.get())[index]; }
	T getValAt(const Coord<2>& coord) const { return (matrix.get())[position(coord)]; }
	void setValAt(const Coord<2>& coord, T val) { (matrix.get())[position(coord)] = val; }
	void setValAt(size_t index, T val) { (matrix.get())[index] = val; }
	size_t position(const Coord<2> coord) const { return coord[0] * numOfCols + coord[1]; }


	std::string getGroupFromCoord(const std::function<std::string(T)>& f, Coord<2> coord) const
	{
		return f(getValAt(coord));
	}


	void checkCoordinate(Coord<2> coord, const std::function<std::string(T)>& f, std::queue<Coord<2>>& coordsToGroup, std::string& type, std::vector<Coord<2>>& groupCoords, Matrix2d<bool>& isValGrouped)
	{
		if (getGroupFromCoord(f, coord) == type && isValGrouped.getValAt(coord) == false)
			coordsToGroup.push(Coord < 2 > {coord[0], coord[1]});
	}


	void createGroupFromQueue(const std::function<std::string(T)>& f, std::queue<Coord<2>>& coordsToGroup, std::string& type, std::vector<Coord<2>>& groupCoords, Matrix2d<bool>& isValGrouped)
	{
		// Add current coordinate from queue to the group
		Coord<2> coord = coordsToGroup.front();
		//std::cout << "Adding to type " << type << " : {" << coord[0] << "," << coord[1] << "} - " << getValAt(coord) << std::endl;
		coordsToGroup.pop();
		groupCoords.push_back(coord);
		isValGrouped.setValAt(this->position(coord), true);

		// Add all adjacent coordinates from the matrix of the same group to the queue
		if (coord[1] + 1 < this->numOfRows)
			checkCoordinate({ coord[0], coord[1] + 1 }, f, coordsToGroup, type, groupCoords, isValGrouped);

		if (coord[1] > 0)
			checkCoordinate({ coord[0], coord[1] - 1 }, f, coordsToGroup, type, groupCoords, isValGrouped);

		if (coord[0] + 1 < this->numOfCols)
			checkCoordinate({ coord[0] + 1, coord[1] }, f, coordsToGroup, type, groupCoords, isValGrouped);

		if (coord[0] > 0)
			checkCoordinate({ coord[0] - 1, coord[1] }, f, coordsToGroup, type, groupCoords, isValGrouped);
	}


	//template<typename S> // Switch std::string with template S ?
	auto groupValues(const std::function<std::string(T)>& f)
	{		
		// Set the mirror matrix of which values are grouped
		Matrix2d<bool> isValGrouped = { numOfRows, numOfCols, false };

		std::vector<std::pair<std::string, std::vector<std::vector<Coord<2>>>>> allGroups;
		std::queue<Coord<2>> coordsToGroup;

		// Pass through the matrix and group all its values
		for (size_t i = 0; i < numOfRows*numOfCols; i++)
			if (isValGrouped.getValAt(i) == false)
			{
				//coordsToGroup.push(Coord<2>{i % numOfCols, i / numOfRows});
				coordsToGroup.push(Coord<2>{i / numOfRows , i % numOfCols});
				std::string type = f(getValAt(i));
				std::vector<Coord<2>> groupCoords;

				// Pass through the current coordinate and add the other coordinates from its group
				while (coordsToGroup.empty() == false)
					createGroupFromQueue(f, coordsToGroup, type, groupCoords, isValGrouped);

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
