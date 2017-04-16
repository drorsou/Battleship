#pragma once
#include "Coord.h"
#include <utility>
#include <iostream>
class CoordQueue {
	Coord * front;
	Coord * back;
	int size;
public:
	CoordQueue() { front = nullptr; back = nullptr; size = 0; }

	std::pair<int, int>* pop();
	int getSize() { return this->size; }
	void push(int, int);
	void printQueue();
};