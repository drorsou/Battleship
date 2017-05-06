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

	//static int ArgPos(char *str, int argc, char **argv);

	static Board game_board;
};