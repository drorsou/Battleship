#pragma once
#include <string>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
//#include <bemapiset.h>
#include "IBattleshipGameAlgo.h"
//#include <tuple>
//#include <windows.h>

class FileReader
{
	//static const std::string fileInDirFileName; // The name of the file which will contain names of files in dir //TODO not working, fails on linker
	static std::vector<std::string> filesVector;
	
public:
	FileReader();
	~FileReader();

	/*static void setDirFileName(const std::string& name) { const std::string fileInDirFileName = name; };
	static const std::string getDirFileName() { return fileInDirFileName; };
	static const std::string getDirFileName() { return "dirFiles.txt"; };*/


	enum Error {PATH, BOARD, DLL, LOAD_DLL, AlGO_INIT};

	/* Pre: error detail 'errorType', the path to the files 'path'
	 * Post: print the corresponding error
	 */
	static void printError(const FileReader::Error& errorType, const std::string& path);


	/* Pre: path to the files 'pathName'
	 * Post: return true if the path exists, otherwise false
	 */
	static bool checkIsValidDir(const std::string& pathName);


	/* Pre: file extension 'requiredExtention'
	 * Post: return the first file name found with the given extension (true),
	 *		if none exists return false
	 */
	//static std::pair<bool, std::string> findPathOfFile(char* requiredExtention);


	/* Pre: assumes the files name are ordered lexicorgrphically in the file
	 * Post: returns pair with dll names (without the directory path). if didn't find two dlls returns pair of nullptr.
	 *		player A should get the first of the pait. player B should get second.
	 */
	//static std::pair<std::string, std::string> findDllFiles();


	/* Pre: path for the files 'path'
	 * Post: write all the files at the given dir to another file,
	 *		the files are ordered lexicographically
	 */
	//void writeToFileTheFilesInDir(const std::string & path);


	//finds the first and second files with the given extention at the folder(assumes ordered in lexicographic order)
	//Used for finding attack and dll files. Looking for the files at the vector of files at the folder
	//Assums filesAtPathLexiVector is ordered lexico. If not found then empty string
	static std::pair<std::string, std::string> findFilesLexicographically(std::string requiredExtention);


	// Creates a pipe and executes a command for getting all find in the path in lexical order.
	// All the files are saved in a vector sent back from the function.
	//static std::vector<std::string>& writeToVectorTheFilesInDir(const std::string& path);
	static void writeToVectorTheFilesInDir(const std::string& path);


	// Replacea all 'ch1' in string 'str' with 'ch2'
	static void replaceChar(std::string& str, char ch1, char ch2);


	typedef IBattleshipGameAlgo *(*GetAlgorithmFuncType)();
	//static std::tuple<HINSTANCE, FileReader::GetAlgorithmFuncType> loadDLL(const std::string& path);


	static std::vector<std::string>* getFilesVector() { return &filesVector; }
};