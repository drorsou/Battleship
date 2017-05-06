#include "Main.h"


// Static variables
Board Main::game_board;
bool isPrint;
int delay;


Main::Main() {

}


int main(int argc, char* argv[])
{
	string path;
	isPrint = true;
	delay = 500;

	/*
	 * Check for arguments
	 */
	
	if(argc < 2)
		path = ".";
	else
	{
		if (std::string{ "-quiet" }.compare(argv[1]) == 0)
		{
			isPrint = false;
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
			path.append(argv[1]);
			FileReader::replaceChar(path, '/', '\\');
			if (argc > 2)
			{				
				if (std::string{ "-quiet" }.compare(argv[2]) == 0)
				{
					isPrint = false;
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


	if (Main::init(path) == false)
		return EXIT_FAILURE;

	Main::play();

	return 0;
};



bool Main::init(const std::string& path)
{
	// Check if directory exists and list all its files
	if (!FileReader::checkIsValidDir(path))
	{
		FileReader::printError(FileReader::Error::PATH, path);
		return false;
	}
	else
		FileReader::writeToVectorTheFilesInDir(path);


	IBattleshipGameAlgo* playerA;
	IBattleshipGameAlgo* playerB;

	//
	// Replace later with DLLs
	//
	playerA = new attackFromFileAlgo(0);
	playerB = new attackFromFileAlgo(1);
	//
	
	playerA->init(path);
	playerB->init(path);

	game_board = Board(path, 10, 10, playerA, playerB);

	// In case of wrong board init - quit, the errors are already printed on the console!
	if (game_board.getScore(0) == -1 || game_board.getScore(1) == -1)
		return false;

	return true;
}


void Main::play()
{
	/*
	 * Initialize vars
	 */
	bool game_in_progress = true;
	bool playerExhausted = false; // No more attacks from file 
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
		pair<int, int> attack_coord = game_board.attackPlayer(game_board.getTurn());
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
			char piece = game_board.getCoordValue(attack_coord.first, attack_coord.second);
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
				if (playerExhausted == false)
					game_board.changeTurn();
				if (piece == HIT_SYM)
					result = AttackResult::Hit;
				else
				{
					game_board.setCoordValue(attack_coord.first, attack_coord.second, MISS_SYM);
					result = AttackResult::Miss;
				}
			}


			// Notify players on results
			game_board.notifyResult(attack_coord.first, attack_coord.second, result);

			// Update the board print
			if (isPrint == true)
			{
				game_board.updateBoard(attack_coord.first, attack_coord.second);
				Sleep(delay);
			}
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
}





/*int Main::ArgPos(char *str, int argc, char **argv) {
	int a;
	for (a = 1; a < argc; a++)
	{
		if (!strcmp(str, argv[a]))
			return a;
	}
	return -1;
}*/