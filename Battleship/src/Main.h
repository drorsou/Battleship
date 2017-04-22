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

	//PRE: assumes the files name are ordered lexicorgrphically in the file
	//POST: returns pair with dll names (without the directory path). if didn't find two dlls returns pair of nullptr.
	//player A should get the first of the pait. player B should get second.
	static std::pair<string, string> Main::findDllFiles();

	static void Main::printErrorsForDllFiles(string fileType, string path);

	static bool checkIsValidDir(string pathName);

	static void writeToFileTheFilesInDir(string path);

	static void printErrorOfFiles(string fileType, string path);
	
	static bool checkFilesAndPrintErrorsInOrder(string path);

	static int ArgPos(char *str, int argc, char **argv);

	static void replaceChar(std::string& str, char ch1, char ch2);



	Main();
};