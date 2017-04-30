#pragma once

#include <string>
#include "Attack.h"
#include "Board.h"
#include "IBattleshipGameAlgo.h"
#include "FileReader.h"
#include <vector>


class Main {
	
public:
	Main();

	static bool init(const std::string& path);

	static void play();

	static int ArgPos(char *str, int argc, char **argv);

	static void replaceChar(std::string& str, char ch1, char ch2);
	static Board game_board;
#if DEBUG
	static string path; // Path to the directory containing the game files
	static bool isPrint; // Default true
	static int delay; // Default - half a second per attack
	static Board game_board; // Game progress details
#endif

};