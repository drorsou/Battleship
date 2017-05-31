#include "GameManager.h"


int GameManager::threads = 4;
std::string GameManager::path;
std::vector<std::string> boardsVector;
std::vector<std::unique_ptr<IBattleshipGameAlgo>> playersVector;


int main(int argc, char* argv[])
{
	if (GameManager::init(argc, argv) == false)
		return EXIT_FAILURE;

	GameManager::tournament();

	return 0;
}


bool GameManager::init(int argc, char* argv[])
{
	if (Utils::checkArgs(argc, argv) == false)
		return false;

	if (!FileReader::checkIsValidDir(GameManager::getPath()))
	{
		FileReader::printError(FileReader::Error::PATH, GameManager::getPath());
		return false;
	}
	FileReader::writeToVectorTheFilesInDir(GameManager::getPath());


	std::cout << "Number of threads: " << GameManager::getThreads() << std::endl; // Debug line
	std::cout << "Path is: " << GameManager::getPath() << std::endl; // Debug line


	// Read dlls and board files
	FileReader::importFromFilesToVectors(boardsVector, playersVector, GameManager::getPath());

	if (boardsVector.size() < 1)
	{
		FileReader::printError(FileReader::Error::BOARD, GameManager::getPath());
		return false;
	}
	if (playersVector.size() < 2)
	{
		FileReader::printError(FileReader::Error::DLL, GameManager::getPath());
		return false;
	}

	std::cout << "Number of legal players: " << playersVector.size() << std::endl;
	std::cout << "Number of legal boards: " << boardsVector.size() << std::endl;

	return true;
}


void GameManager::tournament()
{
	std::vector<std::thread> threadsVector;
	int activeThreads;


}


void GameManager::play(int board, std::unique_ptr<IBattleshipGameAlgo> playerA, std::unique_ptr<IBattleshipGameAlgo> playerB)
{
	playerA->setPlayer(0);
	playerB->setPlayer(1);

	// Set boards for both players

	bool game_in_progress = true;
	AttackResult result;
	int currentPlayer;

	while (game_in_progress)
	{
		//currentPlayer = board.getTurn();

	}
}
