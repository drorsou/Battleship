#include "FileReader.h"


FileReader::FileReader()
{
}


FileReader::~FileReader()
{
}


void FileReader::printError(std::string fileType, std::string path)
{
	if (path == "") {
		//as written in the forum in moodle
		path = "Working Directory";
	}
	std::string error;

	if (fileType == "wrong path")
		error = "Wrong path: ";
	if (fileType == "board")
		error = "Missing board file(*.sboard) looking in path : ";
	if (fileType == "attack-a")
		error = "Missing attack file for player A (*.attack-a) looking in path: ";
	if (fileType == "attack-b")
		error = "Missing attack file for player B (*.attack-b) looking in path: ";
	if (fileType == "dll")
		error = "Missing an algorithm(dll) file looking in path : ";
	//TODO- print two other errors for load and intialization of dll

	error.append(path);

	std::cout << error << std::endl;
}


bool FileReader::checkIsValidDir(std::string pathName) {

	if (pathName == "")//relative path->no need to check
		return true;
	std::experimental::filesystem::path path(pathName);

	if (!is_directory(path)) {
		return false;
	}
	return true;

}


std::pair<bool, std::string> FileReader::findPathOfFile(char* requiredExtention)
{
	std::string line = "";
	bool fileExist = false;
	int indexOfSuffix = std::string::npos;
	std::ifstream fin(getDirFileName());

	if (!fin.fail()) {


		while (std::getline(fin, line))
		{
			indexOfSuffix = line.find_last_of(".") + 1;
			if (indexOfSuffix != std::string::npos)
			{
				std::string currSuffixOfFile = line.substr(indexOfSuffix);
				if (currSuffixOfFile.compare(requiredExtention) == 0)
				{
					fileExist = true;
					break;
				}
			}

		}
	}
	
	return std::pair<bool, std::string>(fileExist, line);
}


std::pair<std::string, std::string> FileReader::findDllFiles()
{
	char* requiredExtention = "dll"; //TODO -uppercase?
	std::string line = "";
	int indexOfSuffix = std::string::npos;
	std::ifstream fin(getDirFileName());
	int numDllsFound = 0;
	std::pair<std::string, std::string> dllsNames;

	if (!fin.fail()) {
		while (std::getline(fin, line) && numDllsFound != 2)
		{
			indexOfSuffix = line.find_last_of(".") + 1;
			if (indexOfSuffix != std::string::npos)
			{
				std::string currSuffixOfFile = line.substr(indexOfSuffix);
				if (currSuffixOfFile.compare(requiredExtention) == 0)
				{
					if (numDllsFound == 0)
						dllsNames.first = line;
					else
						dllsNames.second = line;
					numDllsFound++;
				}
			}
		}
	}
	//there are no two dll files-> initialize with pair of nullptr
	if (numDllsFound != 2)
	{
		dllsNames.first = nullptr;
		dllsNames.second = nullptr;
	}
	return dllsNames;
}


void FileReader::writeToFileTheFilesInDir(std::string path)
{
	std::string cmd = "dir ";
	cmd.append(path);
	//	cmd.append(" /b /a-d  > ");
	cmd.append(" /b /a-d /o-n > ");// /o-n for sorted order by file name(lexicographically)
	cmd.append(getDirFileName());//name of the file to erite to
	cmd.append(" 2> nul");
	system(cmd.c_str());


}