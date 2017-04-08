#include "Main.h"

Main::Main() {
}

int main(int argc, char* argv[]) {
	
	string path;
	if (argc > 1)
		path = argv[1];
	else
		path = "";

	//Attack attack1 = parseAttack(path.attack-a);
	//Attack attack2 = parseAttack(path.attack-b);
	//Board board = parseBoard(path.sboard);

	return 0;
};

int Main::parseAttack(string path) {
	return 0;
};
int Main::parseBoard(string path) {
	return 0;
};



/*

char* pathName;
if (argc>0)
{
pathName = argv[1];
}
else
{
pathName = "";
}

readDirToFile(pathName);
std::pair<bool, string> boardFile = findPathOfFile(boardFileExtention);
printf("%s\n", get<1>(boardFile).c_str());
std::pair<bool, string> attackAFile = findPathOfFile(attackAFileExtention);
std::pair<bool, string> attackBFile = findPathOfFile(attackBFileExtention);
bool isErrorInFiles = printErrorsOfFiles(boardFile, attackAFile, attackBFile, pathName);
if (isErrorInFiles)//there ere errors in files
{
return -1;//TODO - exit with deallocation
}

 */


/*

std::pair<bool, string> findPathOfFile(char* requiredExtention)
{
string line = "";
bool foundFile = false;
int indexOfExtention = string::npos;
//read from file of files at the directory
ifstream fin("fileNamesInDir.txt");
while (!fin.eof())
{
getline(fin, line);
indexOfExtention = line.find_last_of(".");
//string::npos indicates no match is found(no dot at the string)
if (indexOfExtention != string::npos)//there s a dot
{
string extentionOfLine = line.substr(indexOfExtention + 1);
int result = extentionOfLine.compare(requiredExtention);
if (result == 0)
{
foundFile = true;
break;
}
}

}

return   std::pair<bool, string>(foundFile, line);


}

void printErrorOfExistenceOfFiles(string pathName, string err)
{
//err.append(pathName);
cout << err << endl;
}

void readDirToFile(char* pathName)
{
//TODO check if directory exist
string command = "dir /b /s  ";
//command.append(pathName);
command.append("> fileNamesInDir.txt");
//casting from string to char* and write to cmd
system(command.c_str());

}
//prints error in file by the required order. returns true if there were errors
bool printErrorsOfFiles(std::pair<bool, string> boardFile, std::pair<bool, string> attackAFile, std::pair<bool, string> attackBFile, char* pathName)
{
if (!get<0>(boardFile) || !get<0>(attackAFile) || !get<0>(attackBFile))
{
if (!get<0>(boardFile))
{
string err = "Missing board file(*.sboard) looking in path : ";
//	printErrorOfExistenceOfFiles(pathName, err);
cout << err << endl;
}
if (!get<0>(attackAFile))
{
string err = "Missing attack file for player A(*.attack - a) looking in path : ";
//printErrorOfExistenceOfFiles(pathName, err);
cout << err << endl;
}
if (!get<0>(attackBFile))
{
string err = "Missing attack file for player B (*.attack-b) looking in path: ";
//printErrorOfExistenceOfFiles(pathName, err);
cout << err << endl;
}

return true;//TODO- check valid exit

}

return false;//no error

}

 */