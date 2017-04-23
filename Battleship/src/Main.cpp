#include "Main.h"

//#define DEBUG


Main::Main() {

}


int main(int argc, char* argv[]) {
	// Check for arguments
	string path;
	bool isPrint = true; // default true
	bool playerExhausted = false;
	int delay = 500; // defualt - half a second per attack
	
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
	

	// Check if directory exists and list all its files
	//FileReader::setDirFileName("dirFiles.txt");
	if (!FileReader::checkIsValidDir(path))
	{
		FileReader::printError("wrong path", path);
		return EXIT_FAILURE;
	}
	else
		FileReader::writeToFileTheFilesInDir(path);


	/*
	 *Initialize game
	 */
	string chooseAlgoA = "file"; // Temporary until we have DLLs - "naive" or "file"
	string chooseAlgoB = "file"; // Temporary until we have DLLs - "naive" or "file"

	IBattleshipGameAlgo* playerA;
	IBattleshipGameAlgo* playerB;

	if (chooseAlgoA == "file")
		playerA = &attackFromFileAlgo::attackFromFileAlgo();
	else
		playerA = &attackFromFileAlgo::attackFromFileAlgo();
	if (chooseAlgoB == "file")
		playerB = &attackFromFileAlgo::attackFromFileAlgo();
	else
		playerB = &attackFromFileAlgo::attackFromFileAlgo();
	
	Board game_board = Board(path, playerA, playerB);
	
	// In case of wrong board init - quit, the errors are already printed on the console!
	if (game_board.getScore(0) == -1 || game_board.getScore(1) == -1)
		return EXIT_FAILURE;	

	bool game_in_progress = true;
	pair<int, int> attack_coord;
	AttackResult result;
	char piece;
	
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

		// Update the board print
		if (isPrint == true && attack_coord.first != -1 && attack_coord.second != -1)
		{
			game_board.updateBoard(attack_coord.first, attack_coord.second);
			Sleep(delay);
		}
	}

	// End the game
	if (game_board.hasPlayerWon(0))
	{
		std::cout << "Player A won" << endl;
	}
	else if (game_board.hasPlayerWon(1))
	{
		std::cout << "Player B won" << endl;		
	}
	std::cout << "Points:" << endl;
	std::cout << "Player A: " << game_board.getScore(0) << endl;
	std::cout << "Player B: " << game_board.getScore(1) << endl;
	
	std::getchar(); // TODO Remove!!!
	return 0;
};



void Main::replaceChar(std::string& str, char ch1, char ch2) {
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] == ch1)
			str[i] = ch2;
	}
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