#include "Main.h"
#include <experimental/filesystem>
#include <stdio.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>


string fileInDirFileName = "dirFiles.txt";//the name of the file which will consust the name of the files in dir
string boardFile;
string attackAFile;
string attackBFile;
vector<std::pair<int, int>> attackAVector;
vector<std::pair<int, int>> attackBVector;



Main::Main() {
}



//check if the given path string is a valid and existing string
bool Main::checkIsValidDir(string pathName){
	
	if (pathName == "")//relative path->no need to check
		return true;
	std::experimental::filesystem::path path(pathName);
	
	if (!is_directory(path)) {
		return false;
	}
	return true;
	
}


//checks if there are errors and prints it at the right order.
bool Main::checkFilesAndPrintErrorsInOrder(string path)
{
	if (!Main::checkIsValidDir(path))
	{
		Main::printErrorOfFiles("wrong path", path);
		return true; //TODO -throw exception
	}
	else
	{
		Main::writeToFileTheFilesInDir(path);
		std::pair<bool, string> boardFileDetails = Main::findPathOfFile("sboard");
		boardFile = path +"\\"+boardFileDetails.second;
		std::pair<bool, string> attackAFileDetails = Main::findPathOfFile("attack-a");
		attackAFile = path+"\\"+ attackAFileDetails.second;
		std::pair<bool, string> attackBFileDetails = Main::findPathOfFile("attack-b");
		attackBFile = path +"\\"+attackBFileDetails.second;
		if (!boardFileDetails.first)
			Main::printErrorOfFiles("board", path);
		if (!attackAFileDetails.first)
			Main::printErrorOfFiles("attack-a", path);
		if (!attackBFileDetails.first)
			Main::printErrorOfFiles("attack-b", path);
		return !boardFileDetails.first || !attackAFileDetails.first|| !attackBFileDetails.first;
	}




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
	

	bool errorOccur = false;
	Main::parseAttack(errorOccur);
	if (errorOccur)//error accuured
		return -1;///TODO-exit on errors
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
//return bool- if true-error occured
bool Main::parseAttack(bool& errorOccur) {
	
	attackAVector = Main::loadFromAttackFile(attackAFile, errorOccur);
	attackBVector = Main::loadFromAttackFile(attackBFile, errorOccur);
	return errorOccur;
};


int parseBoard(string path, string* b) {
	ifstream fin(path);
	for (int i = 0; i < 10; ++i) {
		std::getline(fin, b[i]);
	}

	return 0;
};





std::pair<bool, string> Main::findPathOfFile(char* requiredExtention)
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
void Main::writeToFileTheFilesInDir(string path)
{
	string cmd = "dir ";
	cmd.append(path);
	cmd.append(" /b /a-d  > ");
	cmd.append(fileInDirFileName);//name of the file to erite to
	cmd.append(" 2> nul");
	system(cmd.c_str());
	

}

void Main::printErrorOfFiles(string fileType,string path)
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




//loads a file (this is public becouse we allow to initialize our object with no param)
vector<std::pair<int, int>> Main::loadFromAttackFile(const string& attackPath,bool& errorOccur)
{
	vector<std::pair<int, int>> vector;

	string line;
	std::pair<int, int> singleAttak;
	ifstream fin(attackAFile); //creating an ifstream object and opening file in path attackPath 
	if (fin.fail()) //error openning file
	{
		std::cout << "Error Occured opening file of attack" << endl;
		errorOccur = true;
		return vector;
	}
	while (getline(fin, line)) //getline returns null if we get to end of file.
	{
		
		if (line.size() && line[line.size() - 1] == '\r') //to handle \r\n(getline is only for \n)
			line = line.substr(0, line.size() - 1);
		
		singleAttak=Main::processLine(line,errorOccur);
		if (errorOccur)//error occured in prcessingLine
			return vector;
		vector.push_back(singleAttak);
	}
	return vector;

}




//takes a string and splits it by the delimeter ','. creates a pair of ints of this row 
std::pair<int, int> Main::processLine(const string& line,bool& errorOcuured) {
	vector<string> tokens = split(line, ',');
	if (tokens.size() != 2)
	{
		std::cout << "A row in the attack file is not valid" << endl;
		errorOcuured = true;//error occurred
		return  std::pair<int, int>(-1, -1);//error -doesnt matter
	}
	
	//TODO- if its not a number-what to do?it's written there is an exception
	return  std::pair<int, int> (stoi(tokens[0]), stoi(tokens[1]));
}
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

	

//splits string s to a vector by delimeter
std::vector<std::string> Main::split(const std::string &s, char delim)
{
	std::vector<std::string> elems;
	std::stringstream strSt(s);
	std::string item;

	while (std::getline(strSt, item, delim)) 
	{
		Main::removeCharFromString(item, ' ');//earse spaces around the comma
		elems.push_back(item);
	}

	return elems;
}

//earse the all occurences of the given char from the given string
void Main::removeCharFromString(string &str, char charToRemove) {

		str.erase(remove(str.begin(), str.end(), charToRemove), str.end());
}




	



 