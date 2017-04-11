#include "Main.h"

string fileInDirFileName = "dirFiles.txt";//the name of the file which will consust the name of the files in dir
string boardFile;
string attackAFile;
string attackBFile;
vector<std::pair<int, int>> attackAVector;
vector<std::pair<int, int>> attackBVector;



Main::Main() {

}


int main(int argc, char* argv[]) {
	// Check for arguments
	string path;
	if (argc > 1)
		path = argv[1];
	else
		path = "";
	bool filesExist = Main::checkFilesAndPrintErrorsInOrder(path);
	if (!filesExist)
	{
		getchar();
		return -1; //TODO-exit with errors
	}

	// Check for -quiet
	// Check for -delay

	// Parse Attacks
	bool errorOccur = false;
	Main::parseAttack(errorOccur);
	if (errorOccur)//error accuured
		return -1;///TODO-exit on errors

	// Parse Board
	char parsed_board[BOARD_SIZE][BOARD_SIZE];
	if (Main::parseBoard(boardFile, parsed_board))
		return -1; // Error in parsing


	// Initialize game
	Attack attack1 = Attack(attackAVector);
	Attack attack2 = Attack(attackBVector);
	Board game_board = Board(parsed_board, attack1, attack2);
	bool game_in_progress = true;
	pair<int, int> attack_coord;
	bool player_a_won = false;
	bool player_b_won = false;
	AttackResult result;
	// board.player1.setBoard(board.board, 10, 10);
	// board.player2.setBoard(board.board, 10, 10);
	
	// End initialize
	

	// Some testing
	game_board.printBoard(game_board);
	getchar();
	Board::updateBoard(game_board, 5, 5);
	getchar();
	Board::updateBoard(game_board, 8, 4);
	getchar();
	Board::updateBoard(game_board, 7, 4);
	getchar();


	/*
	 * Battleships Game
	 */
	while (game_in_progress)
	{
		// Get the next attack
		if (game_board.current_player_turn == 0)
		{
			attack_coord = game_board.player1.attack();
		}
		else if (game_board.current_player_turn == 1)
		{
			attack_coord = game_board.player2.attack();
		}

		// Check validity of the attack coordinate
		if (attack_coord.first > 9 || attack_coord.first < 0 || attack_coord.second > 9 || attack_coord.second < 0)
		{
			continue;
		}

		char piece = game_board.board[attack_coord.first][attack_coord.second];
		// Attack player A ship
		if (piece == ABOAT || piece == ACRUISER || piece == ASUBMARINE || piece == ADESTROYER)
		{
			game_board.board[attack_coord.first][attack_coord.second] = HIT_SYM;
			// change score
			// check if ship is sunk
			result = AttackResult::Hit;
			// check if game is over

			game_board.current_player_turn = 1;
		}
		// Attack player B ship
		else if (piece == BBOAT || piece == BCRUISER || piece == BSUBMARINE || piece == BDESTROYER)
		{
			game_board.current_player_turn = 0;
			// change score
			// check if ship is sunk
			result = AttackResult::Hit;
			// check if game is over

			game_board.current_player_turn = 0;
		}
		// Attack an empty or already missed spot
		else if (piece == BLANK || piece == MISS_SYM)
		{
			game_board.board[attack_coord.first][attack_coord.second] = MISS_SYM;
			game_board.current_player_turn = 1 - game_board.current_player_turn;
			result = AttackResult::Miss;
		}
		// Attack an already bombed spot
		else if (piece == HIT_SYM)
		{
			game_board.board[attack_coord.first][attack_coord.second] = HIT_SYM;
			game_board.current_player_turn = 1 - game_board.current_player_turn;
			result = AttackResult::Miss;
		}

		// Notify players on results
		game_board.player1.notifyOnAttackResult(0, attack_coord.first, attack_coord.second, result);
		game_board.player2.notifyOnAttackResult(0, attack_coord.first, attack_coord.second, result);
	}

	// End the game
	if (player_a_won)
		cout << "Player A won" << endl;
	if (player_b_won)
		cout << "Player B won" << endl;
	cout << "Points:" << endl;
	cout << "Player A: " << game_board.player1.score << endl;
	cout << "Player B: " << game_board.player2.score << endl;

	// Free memory?

	return 0;
};


//check if the given path string is a valid and existing string
bool Main::checkIsValidDir(string pathName) {

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
		std::pair<bool, string> attackAFileDetails = Main::findPathOfFile("attack-a");
		std::pair<bool, string> attackBFileDetails = Main::findPathOfFile("attack-b");

		if (!boardFileDetails.first)
			Main::printErrorOfFiles("board", path);
		if (!attackAFileDetails.first)
			Main::printErrorOfFiles("attack-a", path);
		if (!attackBFileDetails.first)
			Main::printErrorOfFiles("attack-b", path);

		boardFile = path + "\\" + boardFileDetails.second;
		attackAFile = path + "\\" + attackAFileDetails.second;
		attackBFile = path + "\\" + attackBFileDetails.second;

		return boardFileDetails.first && attackAFileDetails.first && attackBFileDetails.first;
	}
}


//return bool- if true-error occured
bool Main::parseAttack(bool& errorOccur) {

	attackAVector = Main::loadFromAttackFile(attackAFile, errorOccur);
	attackBVector = Main::loadFromAttackFile(attackBFile, errorOccur);
	return errorOccur;
};


/*
 * Parse Board into 'b' from 'path'
 * Return 0 if no error, otherwise 1
 */
int Main::parseBoard(string path, char b[BOARD_SIZE][BOARD_SIZE]) {
	ifstream fin(path);
	string* temp_board = new string[10];;
	for (int i = 0; i < 10; i++) {
		std::getline(fin, temp_board[i]);

		for (int j = 0; j < 10; j++) {
			b[i][j] = temp_board[i][j];
		}
	}

	// Check for error?

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


void Main::printErrorOfFiles(string fileType, string path)
{
	if (path == "") {
		//as wrriten in the forum in moodle
		path = "Working Directory";
	}
	string error;

	if (fileType == "wrong path")
		error = "Wrong path: ";
	if (fileType == "board")
		error = "Missing board file(*.sboard) looking in path : ";
	if (fileType == "attack-a")
		error = "Missing attack file for player A (*.attack-a) looking in path: ";
	if (fileType == "attack-b")
		error = "Missing attack file for player B (*.attack-b) looking in path: ";

	error.append(path);

	std::cout << error << endl;
}


//loads a file (this is public becouse we allow to initialize our object with no param)
vector<std::pair<int, int>> Main::loadFromAttackFile(const string& attackPath, bool& errorOccur)
{
	vector<std::pair<int, int>> vector;

	string line;
	std::pair<int, int> singleAttak;
	ifstream fin(attackPath); //creating an ifstream object and opening file in path attackPath 

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
		bool isInvalidLine = false;//if invalid dont add to vector of attack
		singleAttak = Main::processLine(line, errorOccur, isInvalidLine);
		if (errorOccur)//error occured in prcessingLine
			return vector;
		if (isInvalidLine) {//if invalid dont add to vector of attack
			isInvalidLine = false;//clear boolean for next line
			continue;
		}
		vector.push_back(singleAttak);
	}
	return vector;

}


//takes a string and splits it by the delimeter ','. creates a pair of ints of this row 
std::pair<int, int> Main::processLine(const string& line, bool& errorOcuured, bool& invalidAttack) {
	vector<string> tokens = split(line, ',');
	if (tokens.size() != 2)
	{
		//std::cout << "A row in the attack file is not valid" << endl;
		invalidAttack = true;//error occurred
		return  std::pair<int, int>(-1, -1);//error -doesnt matter
	}
	if (!Main::is_number(tokens[0]) || !Main::is_number(tokens[1]))
	{
		invalidAttack = true;
		return  std::pair<int, int>(-1, -1);
	}
	if (stoi(tokens[0]) < 1 || stoi(tokens[0]) > 10)
		invalidAttack = true;
	if (stoi(tokens[1]) < 1 || stoi(tokens[1]) > 10)
		invalidAttack = true;
	//TODO- if its not a number-what to do?it's written there is an exception
	return  std::pair<int, int>(stoi(tokens[0]), stoi(tokens[1]));
}


bool Main::is_number(const std::string &s) {
	return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
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


// Remove the all occurences of the given char from the given string
void Main::removeCharFromString(string &str, char charToRemove) {

	str.erase(remove(str.begin(), str.end(), charToRemove), str.end());
}








