#pragma once
#include <functional>
#include <vector>
#include <string>

template<typename T>
class Matrix2d
{
	T matrix[10*10] = {};

	class Coord
	{
		int x = 0, y = 0;
		// needs iterator
	};

public:

	Matrix2d(std::initializer_list<std::initializer_list<T>> list)
	{
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
