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
		break;
		}
	case FileReader::Error::BOARD:
		{
		error = "Missing board file (*.sboard) looking in path: ";
		error.append(pathForError);
		break;
		}
	case FileReader::Error::DLL:
		{
		error = "Missing an algorithm (dll) file looking in path: ";
		error.append(pathForError);
		break;
		}
	case FileReader::Error::LOAD_DLL:
		{
		error = "Cannot load dll: ";
		error.append(pathForError);
		break;
		}
	case FileReader::Error::AlGO_INIT:
		{
		error = "Algorithm initialization failed for dll: ";
		error.append(pathForError);
		break;
		}
	}

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


std::pair<std::string, std::string> FileReader::findFilesLexicographically(std::string requiredExtention)
{
	std::string name = "";
	size_t indexOfSuffix = std::string::npos;
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


IBattleshipGameAlgo* FileReader::loadDLL(const std::string& path)
{
	// Load dynamic library
	HINSTANCE hDll = LoadLibraryA(path.c_str()); // Notice: Unicode compatible version of LoadLibrary
	if (!hDll)
	{
		std::cout << "Error: could not load the dynamic library from: " << path << std::endl;
		return nullptr;
	}

	// Get function pointer
	FileReader::GetAlgorithmFuncType getAlgoritmeFunc = (FileReader::GetAlgorithmFuncType)GetProcAddress(hDll, "GetAlgorithm");
	if (!getAlgoritmeFunc)
	{
		std::cout << "Error: could not load function GetAlgoritm() from: " << path << std::endl;
		return nullptr;
	}

	return getAlgoritmeFunc();
}