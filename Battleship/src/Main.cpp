#include "Main.h"

string fileInDirFileName = "dirFiles.txt";//the name of the file which will consust the name of the files in dir
string boardFile;
string attackAFile;
string attackBFile;
vector<std::pair<int, int>> attackAVector;
vector<std::pair<int, int>> attackBVector;

//#define DEBUG

void Main::replaceChar(std::string& str, char ch1, char ch2) {
	for (int i = 0; i < str.length(); i++) 
	{
		if (str[i] == ch1)
			str[i] = ch2;
	}	
}

Main::Main() {

}

int Main::ArgPos(char *str, int argc, char **argv) {
	int a;
	for (a = 1; a < argc; a++)
	{
		if (!strcmp(str, argv[a]))		
			return a;
	}
	return -1;
}

int main(int argc, char* argv[]) {
	// Check for arguments
	string path;
	bool isPrint = true; // default true
	bool playerExhausted = false;//no more attcks from file 
	int delay = 500; // defualt - half a second per attack
	int pos;	
	char piece;

	if(argc < 2)
		path = ".";
	else
	{
		if (std::string{ "-quiet" }.compare(argv[1]) == 0)
		{
			isPrint = false;
			path = "";
		}
		else if(std::string{"-delay"}.compare(argv[1]) == 0) 
		{
			if (argc == 2) // illegal parameter - no delay set
			{
				std::cout << "Error: No value for delay parameter\n";
				return EXIT_FAILURE;
			}
			else
			{
				delay = atoi(argv[2]);
			}
		}
		else
		{
			path = argv[1];
			Main::replaceChar(path, '/', '\\');
			if (argc > 2)
			{
				if (std::string{ "-quiet" }.compare(argv[2]) == 0)
				{
					isPrint = false;
					path = ".";
				}
				else if (std::string{ "-delay" }.compare(argv[2]) == 0)
				{
					if (argc == 3) // illegal parameter - no delay set
					{
						std::cout << "Error: No value for delay parameter\n";
						return EXIT_FAILURE;
					}
					else
					{
						delay = atoi(argv[3]);
					}
				}
			}
		}
	}	
	bool filesExist = Main::checkFilesAndPrintErrorsInOrder(path);
	
	if (!filesExist)
	{
		getchar();
		return EXIT_FAILURE;
	}	

	// Parse Board
	char parsed_board[BOARD_SIZE][BOARD_SIZE];
	if (Main::parseBoard(boardFile, parsed_board))
		return EXIT_FAILURE; // Error in parsing
	
	/*
	 *Initialize game
	 */
	Player playerA = Player::Player(0);
	Player playerB = Player::Player(1);
	Attack attack1 = Attack(attackAFile);
	Attack attack2 = Attack(attackBFile);
	playerA.setAttackFromFile(attack1);
	playerB.setAttackFromFile(attack2);
	Board game_board = Board(parsed_board, &playerA, &playerB);
	
	// In case of wrong board init - quit, the errors are already printed on the console!
	if (game_board.getScore(0) == -1 || game_board.getScore(1) == -1)
		return EXIT_FAILURE;	

	bool game_in_progress = true;
	pair<int, int> attack_coord;
	bool player_a_won = false;
	bool player_b_won = false;
	AttackResult result;
	
	if (isPrint == true)
		game_board.printBoard();
	// End initialize
	

	/*
	 * Battleships Game
	 */
	while (game_in_progress)
	{
		// pulling a new attack, where -1,-1 represent an empty queue.
		attack_coord = game_board.attackPlayer(game_board.getTurn());
		if (attack_coord.first == -1 && attack_coord.second == -1)
		{
			// if both players are exhausted, end the game, otherwise continue attacking with the first player
			if (playerExhausted == false)
			{
				playerExhausted = true;
				game_board.changeTurn();				
			}
			else
			{
				game_in_progress = false;
			}
		}
		else
		{
			piece = game_board.getCoordValue(attack_coord.first, attack_coord.second);
			if (piece != BLANK && piece != MISS_SYM && piece != HIT_SYM)
			{				
				result = game_board.hitShip(attack_coord.first, attack_coord.second, piece) ? AttackResult::Sink : AttackResult::Hit;
				if (game_board.checkTarget(piece) == true && playerExhausted == false)
					game_board.changeTurn();
				game_board.setCoordValue(attack_coord.first, attack_coord.second, HIT_SYM);
				result = AttackResult::Hit;
				
				if (game_board.hasPlayerWon(0) || game_board.hasPlayerWon(1))
				{					
					game_in_progress = false;
				}
				
			}
			else
			{
				if(playerExhausted == false)
					game_board.changeTurn();
				if (piece == HIT_SYM)
					result = AttackResult::Hit;
				else
				{
					game_board.setCoordValue(attack_coord.first, attack_coord.second, MISS_SYM);
					result = AttackResult::Miss;
				}
			}
				
		}
		// Notify players on results
		game_board.notifyResult(attack_coord.first, attack_coord.second, result);

		if (isPrint == true && attack_coord.first != -1 && attack_coord.second != -1) // update the board print
		{
			game_board.updateBoard(attack_coord.first, attack_coord.second);
			Sleep(delay);
		}
	}
	if (game_board.hasPlayerWon(0))
	{
		std::cout << "Player A won" << endl;
	}
	else if (game_board.hasPlayerWon(1))
	{
		std::cout << "Player B won" << endl;		
	}
	// End the game
	std::cout << "Points:" << endl;
	std::cout << "Player A: " << game_board.getScore(0) << endl;
	std::cout << "Player B: " << game_board.getScore(1) << endl;
	
	// Free memory?	
	getchar();
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
		return false; //TODO -throw exception
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

/*
//return bool- if true-error occured
bool Main::parseAttack(bool& errorOccur) {

	attackAVector = Main::loadFromAttackFile(attackAFile, errorOccur);
	attackBVector = Main::loadFromAttackFile(attackBFile, errorOccur);
	return errorOccur;
};*/

bool Main::checkChar(char c) {
	return (c == ABOAT || c == BBOAT || c == ACRUISER || c == BCRUISER || c == ASUBMARINE || c == BSUBMARINE || c == ADESTROYER || c == BDESTROYER);
}
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
			if (checkChar(temp_board[i][j]))
				b[i][j] = temp_board[i][j];
			else
				b[i][j] = BLANK;
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

//find the first dll files at the folder(assumes ordered in lexicographic order)
std::pair<string, string> Main::findDllFiles()
{
	char* requiredExtention = "dll"; //TODO -uppercase?
	string line = "";
	int indexOfSuffix = string::npos;
	std::ifstream fin(fileInDirFileName);
	int numDllsFound = 0;
	std::pair<string, string> dllsNames;

	if (!fin.fail()) {
		while (std::getline(fin, line)&& numDllsFound!=2)
		{
			indexOfSuffix = line.find_last_of(".") + 1;
			if (indexOfSuffix != string::npos)
			{
				string currSuffixOfFile = line.substr(indexOfSuffix);
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

void Main::printErrorsForDllFiles(string fileType, string path)
{
	if (path == "") {
		//as written in the forum in moodle
		path = "Working Directory";
	}
	string error;

	if (fileType == "wrong path")
		error = "Wrong path: ";
	if (fileType == "board")
		error = "Missing board file(*.sboard) looking in path : ";
	if (fileType == "dll")
		error = "Missing an algorithm(dll) file looking in path : ";
	//TODO- print two other errors for load and intialization of dll

	error.append(path);

	std::cout << error << endl;
	
}


//use system command to write all the files at the given dir to another file
//the files are ordered lexicographically
void Main::writeToFileTheFilesInDir(string path)
{
	string cmd = "dir ";
	cmd.append(path);
//	cmd.append(" /b /a-d  > ")
	cmd.append(" /b /a-d /o-n > ");// /o-n for sorted order by file name(lexicographically)
	cmd.append(fileInDirFileName);//name of the file to erite to
	cmd.append(" 2> nul");
	system(cmd.c_str());


}


void Main::printErrorOfFiles(string fileType, string path)
{
	if (path == "") {
		//as written in the forum in moodle
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





