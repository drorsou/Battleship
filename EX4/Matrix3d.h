#pragma once
#include <vector>
#include "Coord.h"
#include <queue>
#include <mutex>

template<typename T>
class Matrix3d
{
	std::unique_ptr<T> matrix;
	size_t _rows = 0;
	size_t _cols = 0;
	size_t _depth = 0;

public:

	Matrix3d(std::initializer_list<std::initializer_list<std::initializer_list<T>>> list);

	Matrix3d(size_t rows, size_t cols, size_t depth, T val);


	T getValAt(size_t index) const { return (matrix.get())[index]; }
	T getValAt(const Coord<3>& coord) const { return (matrix.get())[position(coord)]; }
	void setValAt(const Coord<3>& coord, T val) { (matrix.get())[position(coord)] = val; }
	void setValAt(size_t index, T val) { (matrix.get())[index] = val; }
	size_t position(const Coord<3> coord) const { return coord[0] * _rows * _cols + coord[1] * _cols + coord[2]; }

	template<typename G = T, typename GroupingFunc, typename GroupingType = std::result_of_t<GroupingFunc(G)>>
	GroupingType getGroupFromCoord(GroupingFunc f, Coord<3> coord) const
	{
		return f(getValAt(coord));
	}

	template<typename G = T, typename GroupingFunc, typename GroupingType = std::result_of_t<GroupingFunc(G)>>
	void checkCoordinate(Coord<3> coord, GroupingFunc f, std::queue<Coord<3>>& coordsToGroup, GroupingType type, std::vector<Coord<3>>& groupCoords, Matrix3d<bool>& isValGrouped)
	{
		if (getGroupFromCoord(f, coord) == type && isValGrouped.getValAt(coord) == false)
		{
			isValGrouped.setValAt(this->position(coord), true);
			coordsToGroup.push(Coord < 3 > {coord[0], coord[1], coord[2]});
		}
	}


	/**
	* \brief This function tests candidate coordinates and adds any suitable candidte to the group. It also adds more candidates to the queue if possible.
	* \param f a reference to a function
	* \param coordsToGroup a Queue holding the coordinates which need to be tested and possibly added to the group
	* \param type the result of f on this group members, in order to compare possible new members with it.
	* \param groupCoords the vector of members in this group
	* \param isValGrouped a shadow matrix showing which members should not be tested!
	*/
	template<typename G = T, typename GroupingFunc, typename GroupingType = std::result_of_t<GroupingFunc(G)>>
	void createGroupFromQueue(GroupingFunc f, std::queue<Coord<3>>& coordsToGroup, GroupingType& type, std::vector<Coord<3>>& groupCoords, Matrix3d<bool>& isValGrouped)
	{
		// Add current coordinate from queue to the group
		Coord<3> coord = coordsToGroup.front();
		coordsToGroup.pop();
		//std::cout << "Adding to type " << type << " : {" << coord[0] << "," << coord[1] << "," << coord[2] << "} - " << getValAt(coord) << " with isValGrouped " << isValGrouped.getValAt(coord) << std::endl;			
		groupCoords.push_back(coord);
		//isValGrouped.setValAt(this->position(coord), true);

		// Add all adjacent coordinates from the matrix of the same group to the queue
		if (coord[1] < this->_rows - 1)
			checkCoordinate({ coord[0], coord[1] + 1, coord[2] }, f, coordsToGroup, type, groupCoords, isValGrouped);

		if (coord[1] > 0)
			checkCoordinate({ coord[0], coord[1] - 1, coord[2] }, f, coordsToGroup, type, groupCoords, isValGrouped);

		if (coord[0] < this->_depth - 1)
			checkCoordinate({ coord[0] + 1, coord[1], coord[2] }, f, coordsToGroup, type, groupCoords, isValGrouped);

		if (coord[0] > 0)
			checkCoordinate({ coord[0] - 1, coord[1], coord[2] }, f, coordsToGroup, type, groupCoords, isValGrouped);

		if (coord[2] < this->_cols - 1)
			checkCoordinate({ coord[0], coord[1], coord[2] + 1 }, f, coordsToGroup, type, groupCoords, isValGrouped);

		if (coord[2] > 0)
			checkCoordinate({ coord[0], coord[1], coord[2] - 1 }, f, coordsToGroup, type, groupCoords, isValGrouped);
	}


	//template<typename S> // Switch std::string with template S ?
	template<typename G = T, typename GroupingFunc>
	auto groupValues(GroupingFunc f)
	{
		using GroupingType = std::result_of_t<GroupingFunc(G)>;
		// Set the mirror matrix of which values are grouped
		Matrix3d<bool> isValGrouped = { _rows, _cols, _depth, false };

		std::vector<std::pair<GroupingType, std::vector<std::vector<Coord<3>>>>> allGroups;
		std::queue<Coord<3>> coordsToGroup;

		// Pass through the matrix and group all its values
		for (size_t depth = 0; depth < _depth; depth++)
		{
			for( size_t row = 0; row < _rows; row++)
			{
				for (size_t col = 0; col < _cols; col++)
				{
					if (isValGrouped.getValAt(Coord<3>{ depth, row, col }) == false)
					{
						//coordsToGroup.push(Coord<2>{i % _cols, i / _rows});
						coordsToGroup.push(Coord<3>{ depth, row, col });
						isValGrouped.setValAt(this->position(coordsToGroup.front()), true);
						GroupingType type = f(getValAt(Coord<3>{ depth, row, col }));
						std::vector<Coord<3>> groupCoords;

						// Pass through the current coordinate and add the other coordinates from its group
						while (coordsToGroup.empty() == false)
							createGroupFromQueue(f, coordsToGroup, type, groupCoords, isValGrouped);

						// Add the type if it doesn't exist
						// Add the group of the current coordinate to the 'allGroups' vector
						bool typeExists = false;
						for (std::pair<GroupingType, std::vector<std::vector<Coord<3>>>>& groupType : allGroups)
							if (groupType.first == type)
							{
								typeExists = true;
								groupType.second.push_back(groupCoords);
							}
						if (typeExists == false)
							allGroups.push_back(std::pair<GroupingType, std::vector<std::vector<Coord<3>>>>(type, std::vector<std::vector<Coord<3>>>(1, groupCoords)));
					}
				}
			}
		}			
		return allGroups;
	}
};

template <typename T>
Matrix3d<T>::Matrix3d(std::initializer_list<std::initializer_list<std::initializer_list<T>>> list)
{	
	// Find the matrix' size
	_depth = list.size();
	_rows = 0;
	_cols = 0;
	for (auto& row : list)
	{
		_rows = MAX(row.size(), _rows);
		for (auto& col : row)
			_cols = MAX(col.size(), _cols);
	}

	// Initialize the matrix
	T* temp = new T[_rows * _cols * _depth];

	size_t valuesIndex = 0;
	for (auto& depth : list)
	{
		size_t countRows = 0;
		for (auto& row : depth)
		{
			size_t countCols = 0;
			for (auto& val : row)
			{
				countCols++;
				temp[valuesIndex++] = val;
			}
			valuesIndex += _cols - countCols;
			countRows++;
		}
		valuesIndex += (_rows - countRows) * _cols;
	}
	matrix = std::unique_ptr<T>(temp);
}

template <typename T>
Matrix3d<T>::Matrix3d(size_t rows, size_t cols, size_t depth, T val) 
		: _rows(rows),
		  _cols(cols),
		  _depth(depth)
{
	T* temp = new T[_rows * _cols * _depth];
	for (size_t i = 0; i < _rows * _cols * _depth; i++)
		temp[i] = val;
	matrix = std::unique_ptr<T>(temp);
}