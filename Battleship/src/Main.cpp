#include "Main.h"
#include <experimental/filesystem>
#include <stdio.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>


string fileInDirFileName = "dirFiles.txt";//the name of the file which will consust the name of the files in dir
string sboard_filename;
string attack_a_filename;
string attack_b_filename;



Main::Main() {
}


int main(int argc, char* argv[]) {
	
	string path;
	if (argc > 1)
		path = argv[1];
	else
		path = "";
	bool filesExist = checkFilesAndPrintErrorsInOrder(path);
	if (!filesExist)
	{
		getchar();
		return -1; //TODO-exit with errors
	}
	


	//Attack attack1 = Main::parseAttack(path.attack-a);
	//Attack attack2 = parseAttack(path.attack-b);

	string* parsed_board = new string[10];
	if (parseBoard(path + "\\" + sboard_filename, parsed_board))
		return -1; // Error in parsing

	cout << "Press ENTER to continue." << endl;
	getchar();

	// board.player1.setBoard(board.board, 10, 10);
	// board.player2.setBoard(board.board, 10, 10);
	// run attacks from both players and notify the results

	return 0;
};


int parseAttack(string path) {
	return 0;
};


int parseBoard(string path, string* b) {
	ifstream fin(path);
	for (int i = 0; i < 10; ++i) {
		std::getline(fin, b[i]);
	}

	return 0;
};



//check if the given path string is a valid and existing string
bool checkIsValidDir(string pathName) {

	if (pathName == "")//relative path->no need to check
		return true;
	std::experimental::filesystem::path path(pathName);

	if (!is_directory(path)) {
		return false;
	}
	return true;

}

std::pair<bool, string> findPathOfFile(char* requiredExtention)
{
	string line = "";
	bool fileExist = false;
	int indexOfSuffix = string::npos;
	std::ifstream fin(fileInDirFileName);

	if (!fin.fail()) {

		while (std::getline(fin, line))
		{
			indexOfSuffix = line.find_last_of(".") + 1;
			if (indexOfSuffix != string::npos)
			{
				string currSuffixOfFile = line.substr(indexOfSuffix);
				if (currSuffixOfFile.compare(requiredExtention) == 0)
				{
					fileExist = true;
					break;
				}
			}

		}
	}


	return   std::pair<bool, string>(fileExist, line);


}


//use system command to write all the files at the given dir to another file
void writeToFileTheFilesInDir(string path)
{
	string cmd = "dir ";
	cmd.append(path);
	cmd.append(" /b /a-d  > ");
	cmd.append(fileInDirFileName);//name of the file to erite to
	system(cmd.c_str());

}

void printErrorOfFiles(string fileType,string path)
{
	if (path == "") {
		//as wrriten in the forum in moodle
		path = "Working Directory";
	}
	string error;

	if (fileType == "wrong path")
		error = "Wrong path: ";
	if(fileType=="board")
		error = "Missing board file(*.sboard) looking in path : ";
	if (fileType == "attack-a")
		error = "Missing attack file for player A (*.attack-a) looking in path: ";
	if(fileType == "attack-b")
		error = "Missing attack file for player B (*.attack-b) looking in path: ";

	error.append(path);

	std::cout << error << endl;
}

//checks if there are errors and prints it at the right order.
bool checkFilesAndPrintErrorsInOrder(string path)
{
	if(!checkIsValidDir(path))
	{
		printErrorOfFiles("wrong path", path);
		return false; //TODO -throw exception
	}
	else
	{
		writeToFileTheFilesInDir(path);
		std::pair<bool, string> boardFileDetails = findPathOfFile("sboard");
		std::pair<bool, string> attackAFileDetails = findPathOfFile("attack-a");
		std::pair<bool, string> attackBFileDetails = findPathOfFile("attack-b");
		if(!boardFileDetails.first)
			printErrorOfFiles("board", path);
		if (!attackAFileDetails.first)
			printErrorOfFiles("attack-a", path);
		if (!attackBFileDetails.first)
			printErrorOfFiles("attack-b", path);
		sboard_filename = boardFileDetails.second;
		attack_a_filename = attackAFileDetails.second;
		attack_b_filename = attackBFileDetails.second;
		return boardFileDetails.first && attackAFileDetails.first && attackBFileDetails.first;
	}


}