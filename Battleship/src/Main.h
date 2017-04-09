#pragma once
#include <string>
#include "Attack.h"
#include "Board.h"

using namespace std;

class Main {

public:
	Main();
	int parseAttack(string path);
	int parseBoard(string path);
};

std::pair<bool, string> findPathOfFile(char* requiredExtention);
void writeToFileTheFilesInDir(string path);
void printErrorOfFiles(string fileType, string path);
bool checkFilesAndPrintErrorsInOrder(string path);