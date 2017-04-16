#pragma once
class Coord
{
	int first;
	int second;
	Coord * next;
	Coord * prev;
public:
	Coord(int first, int second) : first(first), second(second), next(nullptr), prev(nullptr) {}
	~Coord() { next = nullptr; }
	Coord * getNext() { return next; }
	Coord * getPrev() { return prev; }
	void setNext(Coord * next) { this->next = next; }
	void setPrev(Coord * prev) { this->prev = prev; }
	int getFirst() { return first; }
	int getSecond() { return second; }
};