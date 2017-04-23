#pragma once
#include <utility>
#include <vector>
#include <queue>
#include <sstream>
#include <iostream>
#include <fstream>
#include "FileReader.h"


class Attack {
	std::queue<std::pair<int, int>> list;	
	
	/*
		private methods
	*/
	bool processLine(const std::string& line, std::pair<int, int> * res);
	bool is_number(const std::string &s);
	std::vector<std::string> split(const std::string &s, char delim);
	void removeCharFromString(std::string &str, char charToRemove);
public:
	bool init;
	Attack() {};
	Attack(const std::string & path);
	
	/*
		Pre: there are available attacks (i.e index < list.size)
		Post: returns the coords of the next attack
	*/
	std::pair<int, int> getNextAttack();
	
	bool hasAttacks() { return !list.empty(); }

	/*
		Pre: gets a valid path to the attack file.
		Post: parse the attacks from the file,
				save it into the queue.
	*/
	bool loadFromAttackFile(const std::string& attackPath);
	
};
