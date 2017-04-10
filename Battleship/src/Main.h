#pragma once
#include <string>
#include "Attack.h"
#include "Board.h"
#include <vector>
#include <sstream>

class Main {

public:
	//static string fileInDirFileName ;//the name of the file which will consust the name of the files in dir
	
	

	static bool parseAttack(bool& errorOccur);
	static int parseBoard(string path);

	static std::pair<bool, string> findPathOfFile(char* requiredExtention);
	static bool checkIsValidDir(string pathName);
	static void writeToFileTheFilesInDir(string path);
	static void printErrorOfFiles(string fileType, string path);
	static vector<std::pair<int, int>> loadFromAttackFile(const string& attackPath, bool& errorOccur);
	static std::pair<int, int> processLine(const string& line, bool& errorOcuured);
	static void removeCharFromString(string &str, char charToRemove);

	static std::vector<std::string> split(const std::string &s, char delim);
	static bool checkFilesAndPrintErrorsInOrder(string path);
	Main();
	int parseAttack(string path);
	int parseBoard(string path);
};

int parseAttack(string path);
int parseBoard(string path, string* b);
std::pair<bool, string> findPathOfFile(char* requiredExtention);
void writeToFileTheFilesInDir(string path);
void printErrorOfFiles(string fileType, string path);
bool checkFilesAndPrintErrorsInOrder(string path);