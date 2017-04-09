#pragma once
#include <string>
#include "Attack.h"
#include "Board.h"

using namespace std;

class Main {

public:
	//static string fileInDirFileName ;//the name of the file which will consust the name of the files in dir
	static bool checkFilesAndPrintErrorsInOrder(string path);
	Main();
	static int parseAttack(string path);
	static int parseBoard(string path);
	static std::pair<bool, string> findPathOfFile(char* requiredExtention);
	static void writeToFileTheFilesInDir(string path);
	static void printErrorOfFiles(string fileType, string path);
	
};

