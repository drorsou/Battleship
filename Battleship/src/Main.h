#pragma once

#include <string>
#include "Attack.h"
#include "Board.h"
#include "IBattleshipGameAlgo.h"
#include "FileReader.h"
#include <vector>


class Main {
public:

	static int ArgPos(char *str, int argc, char **argv);

	static void replaceChar(std::string& str, char ch1, char ch2);

	Main();
};