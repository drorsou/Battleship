#pragma once
#include <utility>
#include <vector>
#include <queue>
#include <sstream>
#include <iostream>
#include <fstream>
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
	Attack() {}
	Attack(std::string & path);
	
	/*
		Pre: there are available attacks (i.e index < list.size)
		Post: returns the coords of the next attack
	*/
	std::pair<int, int> getNextAttack();
	
	bool hasAttacks() { return !list.empty(); }

	bool loadFromAttackFile(const std::string& attackPath);
	
};
