#pragma once
#include <string>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>

class FileReader
{
	static const std::string fileInDirFileName; // The name of the file which will contain names of files in dir //TODO not working, fails on linker
	
public:
	FileReader();
	~FileReader();

	//static void setDirFileName(const std::string& name) { const std::string fileInDirFileName = name; };
	//static const std::string getDirFileName() { return fileInDirFileName; };
	static const std::string getDirFileName() { return "dirFiles.txt"; };


	/* Pre: error detail 'fileType', the path to the files 'path'
	 * Post: print the corresponding error
	 */
	static void printError(std::string fileType, std::string path);

	/* Pre: path to the files 'pathName'
	 * Post: return true if the path exists, otherwise false
	 */
	static bool checkIsValidDir(std::string pathName);

	/* Pre: file extension 'requiredExtention'
	 * Post: return the first file name found with the given extension (true),
	 *		if none exists return false
	 */
	static std::pair<bool, std::string> findPathOfFile(char* requiredExtention);

	/* Pre: assumes the files name are ordered lexicorgrphically in the file
	 * Post: returns pair with dll names (without the directory path). if didn't find two dlls returns pair of nullptr.
	 *		player A should get the first of the pait. player B should get second.
	 */
	static std::pair<std::string, std::string> findDllFiles();


	/* Pre: path for the files 'path'
	 * Post: write all the files at the given dir to another file,
	 *		the files are ordered lexicographically
	 */
	static void writeToFileTheFilesInDir(std::string path);

};