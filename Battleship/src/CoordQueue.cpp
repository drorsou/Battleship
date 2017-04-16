#pragma once
#include "CoordQueue.h"

void CoordQueue::push(int first, int second) {
	Coord * temp = new Coord(first, second);
	if (this->size == 0) // empty Queue
	{
		front = temp;
		back = temp;
	}
	else
	{
		temp->setNext(back);
		back->setPrev(temp);
		back = temp;		
	}
	this->size++;
}

std::pair<int,int>* CoordQueue::pop(){
	std::pair<int, int> * res;
	Coord * temp;
	if (this->size == 0)
		return nullptr;
	if (this->front->getFirst() == 10 && this->front->getSecond() == 4)
		std::cout << "the problematic" << std::endl;
	res = new std::pair<int, int>(this->front->getFirst(), this->front->getSecond());
	if (this->size == 1)
	{
		//delete(this->front);
		back = nullptr;
		front = nullptr;
	}
	else
	{
		temp = this->front->getPrev();
		temp->setNext(nullptr);
		//delete(this->front);
		this->front = temp;
	}
#ifndef DEBUG
	this->printQueue();
#endif
	this->size--;
	return res;
}
void CoordQueue::printQueue() {
	Coord * temp = this->front;
	int i = 0;
	while (temp != nullptr)
	{
		std::cout << "item num " << i << "is:" << temp->getFirst() << ", " << temp->getSecond() << std::endl;
		i++;
		temp = temp->getPrev();
	}
	std::cout << "\n" << std::endl;
}