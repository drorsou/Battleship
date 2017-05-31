#pragma once
#include <string>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include "IBattleshipGameAlgo.h"
#include <tuple>
#include <windows.h>
#include <algorithm>

class FileReader
{
	static std::vector<std::string> filesVector;
	
public:
	FileReader();
	~FileReader();

	enum Error {PATH, BOARD, DLL, LOAD_DLL, AlGO_INIT};

	/* Pre: error detail 'errorType', the path to the files 'path'
	 * Post: print the corresponding error
	 */
	static void printError(const FileReader::Error& errorType, const std::string& path);


	/* Pre: path to the files 'pathName'
	 * Post: return true if the path exists, otherwise false
	 */
	static bool checkIsValidDir(const std::string& pathName);

	//finds the first and second files with the given extention at the folder(assumes ordered in lexicographic order)
	//Used for finding attack and dll files. Looking for the files at the vector of files at the folder
	//Assums filesAtPathLexiVector is ordered lexico. If not found then empty string
	static std::pair<std::string, std::string> findFilesLexicographically(std::string requiredExtention);


	// Creates a pipe and executes a command for getting all find in the path in lexical order.
	// All the files are saved in a vector sent back from the function.
	static void writeToVectorTheFilesInDir(const std::string& path);

	// Methods for fileVector
	static std::vector<std::string>* getFilesVector() { return &filesVector; }
	static std::vector<std::string>::const_iterator getFilesVectorBegin() { return filesVector.cbegin(); }
	static std::vector<std::string>::const_iterator getFilesVectorEnd() { return filesVector.cend(); }
	static bool isFilesVectorEmpty() { return filesVector.empty(); };


	// Replacea all 'ch1' in string 'str' with 'ch2'
	static void replaceChar(std::string& str, char ch1, char ch2);

	// Load the DLL from the filepath and return a player algorithm
	typedef IBattleshipGameAlgo *(*GetAlgorithmFuncType)();
	static IBattleshipGameAlgo* loadDLL(const std::string& path);




	// New functions


	/* Pre: Empty vectors for boards and player algorithms, path to files
	 * Post: Read all the files from 'path', add legal boards to 'boardsVector' and legel players to 'playersVector'
	 */
	static void importFromFilesToVectors(std::vector<std::string>& boardsVector, std::vector<std::unique_ptr<IBattleshipGameAlgo>>& playersVector, const std::string& path);
};
