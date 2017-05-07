#include "FileReader.h"
std::vector<std::string> FileReader::filesVector;

FileReader::FileReader()
{
}


FileReader::~FileReader()
{
}


void FileReader::printError(const FileReader::Error& errorType, const std::string& path)
{
	std::string error;

	// As written in the forum in moodle
	std::string pathForError = path == "" ? "Working Directory" : path;

	switch (errorType) {
	case FileReader::Error::PATH:
		{
		error = "Wrong path: ";
		error.append(pathForError);
		}
	case FileReader::Error::BOARD:
		{
		error = "Missing board file (*.sboard) looking in path: ";
		error.append(pathForError);
		}
	case FileReader::Error::DLL:
		{
		error = "Missing an algorithm (dll) file looking in path: ";
		}
	case FileReader::Error::LOAD_DLL:
		{
		error = "Cannot load dll: ";
		error.append(pathForError);
		}
	case FileReader::Error::AlGO_INIT:
		{
		error = "Algorithm initialization failed for dll: ";
		error.append(pathForError);
		}
	}

	/*if (fileType == "attack-a")
		error = "Missing attack file for player A (*.attack-a) looking in path: ";
	if (fileType == "attack-b")
		error = "Missing attack file for player B (*.attack-b) looking in path: ";*/

	std::cout << error << std::endl;
}


bool FileReader::checkIsValidDir(const std::string& pathName) {

	if (pathName == "")//relative path->no need to check
		return true;
	std::experimental::filesystem::path path(pathName);

	if (!is_directory(path)) {
		return false;
	}
	return true;

}


// TODO - Remove ?
//
/*std::pair<bool, std::string> FileReader::findPathOfFile(char* requiredExtention)
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
}*/


/*std::pair<std::string, std::string> FileReader::findDllFiles()
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
}*/


// TODO - Remove?
//
/*void FileReader::writeToFileTheFilesInDir(const std::string& path)
{
	std::string cmd = "dir ";
	cmd.append(path);
	//	cmd.append(" /b /a-d  > ");
	cmd.append(" /b /a-d /o-n > ");// /o-n for sorted order by file name(lexicographically)
	cmd.append(getDirFileName());//name of the file to erite to
	cmd.append(" 2> nul");
	system(cmd.c_str());


}*/


std::pair<std::string, std::string> FileReader::findFilesLexicographically(std::string requiredExtention)
{
	std::string name = "";
	int indexOfSuffix = std::string::npos;
	int numFilesWithExtentionFound = 0;//counts how many files with the given extention found
	std::pair<std::string, std::string> fileNames;
	std::sort(filesVector.begin(), filesVector.end());
	for (std::vector<std::string>::const_iterator itr = filesVector.cbegin(); itr != filesVector.cend(); itr++) {
		
		name = *itr;
		indexOfSuffix = name.find_last_of(".") + 1;
		if (indexOfSuffix != std::string::npos)
		{
			std::string currSuffixOfFile = name.substr(indexOfSuffix);
			if (currSuffixOfFile.compare(requiredExtention) == 0)
			{
				if (numFilesWithExtentionFound == 0)
					fileNames.first = name;
				else
					fileNames.second = name;
				numFilesWithExtentionFound++;
				if (numFilesWithExtentionFound == 2)
					break;
			}
		}
	}


	return fileNames;
}


void FileReader::writeToVectorTheFilesInDir(const std::string& path)
{
	//std::vector<std::string> filesIndir;
	char buffer[4096];
	std::string data_str;
	std::string cmd = "2>NUL dir /a-d /b /o-n ";
	cmd.append(path);

	//get all files in dir in lexicographic order
	FILE* fp = _popen(cmd.c_str(), "r");
	//insert all the files in the dir to the vector of names of files
	while (fgets(buffer, 4095, fp))
	{
		buffer[strcspn(buffer, "\n")] = 0;
		filesVector.push_back(std::string(buffer));
	}
	_pclose(fp);
}


void FileReader::replaceChar(std::string& str, char ch1, char ch2) {
	for (unsigned int i = 0; i < str.length(); i++)
	{
		if (str[i] == ch1)
			str[i] = ch2;
	}
}


/*std::tuple<HINSTANCE, FileReader::GetAlgorithmFuncType> loadDLL(const std::string& path)
{
	// Load dynamic library
	HINSTANCE hDll = LoadLibraryA(path.c_str()); // Notice: Unicode compatible version of LoadLibrary
	if (!hDll)
	{
		std::cout << "could not load the dynamic library" << std::endl;
		//return std::make_tuple(NULL, NULL);
	}

	// Get function pointer
	FileReader::GetAlgorithmFuncType getAlgoritmeFunc = (FileReader::GetAlgorithmFuncType)GetProcAddress(hDll, "GetShape");
	if (!getAlgoritmeFunc)
	{
		std::cout << "could not load function GetAlgoritm()" << std::endl;
		//return false;
	}

	return std::make_tuple(hDll, getAlgoritmeFunc);
}*/