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
	

	//takes a string and splits it by the delimeter ','. creates a pair of ints of this row 
	bool processLine(const std::string& line, std::pair<int, int> * res) const;
	static bool is_number(const std::string &s);

	//splits string s to a vector by delimeter
	std::vector<std::string> split(const std::string &s, char delim) const;
	static void removeCharFromString(std::string &str, char charToRemove);

public:
	bool init;
	Attack() : init(false) {};
	Attack(const std::string & path, int player_num);
	
	/*
		Pre: there are available attacks (i.e index < list.size)
		Post: returns the coords of the next attack
	*/
	std::pair<int, int> getNextAttack();
	
	bool hasAttacks() const { return !list.empty(); }

	/*
		Pre: gets a valid path to the attack file.
		Post: parse the attacks from the file,
				save it into the queue.
	*/
	bool loadFromAttackFile(const std::string& attackPath, int player_num);
};
