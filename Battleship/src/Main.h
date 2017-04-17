#pragma once

#include <string>
#include "Attack.h"
#include "Board.h"
#include <vector>
#include <experimental/filesystem>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

class Main {
public:	

	static bool checkChar(char);

	static int parseBoard(string path, char b[BOARD_SIZE][BOARD_SIZE]);

	static std::pair<bool, string> findPathOfFile(char* requiredExtention);

	static bool checkIsValidDir(string pathName);

	static void writeToFileTheFilesInDir(string path);

	static void printErrorOfFiles(string fileType, string path);
	
	static bool checkFilesAndPrintErrorsInOrder(string path);

	static int ArgPos(char *str, int argc, char **argv);

	Main();
};