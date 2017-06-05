#include "TournamentManager.h"


int TournamentManager::threads = 4;
std::string TournamentManager::path;
std::vector<std::string> TournamentManager::boardsVector;
std::vector<std::unique_ptr<IBattleshipGameAlgo>> TournamentManager::playersVector;


int main(int argc, char* argv[])
{
	if (TournamentManager::init(argc, argv) == false)
		return EXIT_FAILURE;

	TournamentManager::tournament();

	return 0;
}


bool TournamentManager::init(int argc, char* argv[])
{
	if (Utils::checkArgs(argc, argv) == false)
		return false;

	if (!FileReader::checkIsValidDir(TournamentManager::getPath()))
	{
		FileReader::printError(FileReader::Error::PATH, TournamentManager::getPath());
		return false;
	}
	FileReader::writeToVectorTheFilesInDir(TournamentManager::getPath());


	std::cout << "Number of threads: " << TournamentManager::getThreads() << std::endl; // Debug line
	std::cout << "Path is: " << TournamentManager::getPath() << std::endl; // Debug line


	// Read dlls and board files
	FileReader::importFromFilesToVectors(boardsVector, playersVector, TournamentManager::getPath());

	if (boardsVector.size() < 1)
	{
		FileReader::printError(FileReader::Error::BOARD, TournamentManager::getPath());
		return false;
	}
	if (playersVector.size() < 2)
	{
		FileReader::printError(FileReader::Error::DLL, TournamentManager::getPath());
		return false;
	}

	std::cout << "Number of legal players: " << playersVector.size() << std::endl;
	std::cout << "Number of legal boards: " << boardsVector.size() << std::endl;

	return true;
}


void TournamentManager::tournament()
{
	std::vector<std::thread> threadsVector;
	std::vector<GameManager> gamesVector;
	volatile int activeThreads = 0;

	bool tournamentOn = true;

	while (tournamentOn)
	{
		for (int boardRound = 0; boardRound < boardsVector.size(); boardRound++)
		{
			for (int i = 0; i < playersVector.size() - 1; i++)
			{
				gamesVector.push_back(GameManager(boardRound, &playersVector[0], &playersVector[playersVector.size() - 1 - i]));

				for (int j = 0; j < playersVector.size() - 1; j++)
				{
					gamesVector.push_back(GameManager(boardRound, &playersVector[(playersVector.size() - i) % playersVector.size() + 1], &playersVector[playersVector.size() - 1 - i]));
				}


				

				//gamesVector.push_back(GameManager(0, &playersVector[j], &playersVector[playersVector.size() - j]));
			}

			//gamesVector.push_back(GameManager(activeThreads));
			//threadsVector.push_back(std::thread([&](GameManager* game) {game->play(); }, &gamesVector[activeThreads]));
			threadsVector.push_back(std::thread(&GameManager::play, &gamesVector[activeThreads]));
			threadsVector[activeThreads].join();
			activeThreads++;
			if (activeThreads == 5)
				tournamentOn = false;
		}
	}
}