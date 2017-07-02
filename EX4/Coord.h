#include <array>
#pragma once
#define MAX(x,y) (((x)>(y))?(x):(y))


template <size_t SIZE>
class Coord
{
	std::array<size_t, SIZE> arr;
public:
	Coord() {}
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