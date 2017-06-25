#pragma once
#include <string>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include "Board.h"
#include "IBattleshipGameAlgo.h"
#include "ScoresController.h"
#include <tuple>
#include <windows.h>
#include <algorithm>
#include "TournamentManager.h"

class FileReader
{
	static std::vector<std::string> filesVector;
	
public:
	FileReader();
	~FileReader();

	enum Error {PATH, BOARD, DLL};

	/* Pre: error detail 'errorType', the path to the files 'path'
	 * Post: print the corresponding error
	 */
	static void printError(const FileReader::Error& errorType, const std::string& path);

	/* Pre: path to the files 'pathName'
	 * Post: return true if the path exists, otherwise false
	 */
	static bool checkIsValidDir(const std::string& pathName);

	// Creates a pipe and executes a command for getting all find in the path in lexical order.
	// All the files are saved in a vector sent back from the function.
	static void writeToVectorTheFilesInDir(const std::string& path);

	// Methods for fileVector
	static std::vector<std::string>* getFilesVector() { return &filesVector; }
	static std::vector<std::string>::const_iterator getFilesVectorBegin() { return filesVector.cbegin(); }
	static std::vector<std::string>::const_iterator getFilesVectorEnd() { return filesVector.cend(); }
	static bool isFilesVectorEmpty() { return filesVector.empty(); };


	// Load the DLL from the filepath and return a player algorithm
	typedef IBattleshipGameAlgo *(*GetAlgorithmFuncType)();
	static IBattleshipGameAlgo* loadDLL(const std::string& path);




	// New functions

	/* Pre: Get number 'argc' of the program arguments and their pointers 'argv'
	* Post: Set the TournamentManager variables according to the arguments,
	* returns false if there was an error and true otherwise
	*/
	static bool checkArgs(int argc, char** argv);

	/* Pre: Empty vectors for boards and player algorithms, path to files
	 * Post: Read all the files from 'path', add legal boards to 'boardsVector' and legel players to 'playersVector'
	 */
	static void importFromFilesToVectors(std::vector<Board>& boardsVector, std::vector<std::unique_ptr<IBattleshipGameAlgo>>& playersVector, const std::string& path);
};
