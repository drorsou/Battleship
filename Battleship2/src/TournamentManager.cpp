#include "TournamentManager.h"


int TournamentManager::threads = DEFAULT_THREADS;
std::string TournamentManager::path;

std::vector<Board> TournamentManager::boardsVector;
std::vector<std::unique_ptr<IBattleshipGameAlgo>> TournamentManager::playersVector;


std::vector<std::condition_variable> TournamentManager::conditionsVector;
std::vector<std::mutex> TournamentManager::mutexVector;


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

	// Initialize the game and results variables
	for (int i = 0; i < playersVector.size(); i++)
	{
		//mutexVector.push_back(std::mutex);
	}
	Scores::initScores(playersVector.size());

	std::cout << "Number of legal players: " << playersVector.size() << std::endl;
	std::cout << "Number of legal boards: " << boardsVector.size() << std::endl;

	return true;
}


void TournamentManager::tournament()
{
	std::vector<std::thread> threadsVector;
	std::vector<GameManager> gamesVector;
	int gamesPlayed = 0;
	bool tournamentOn = true;

	TournamentManager::addGamesToVector(gamesVector);

	while (tournamentOn)
	{
		if (Scores::activeThreads == 0 && gamesPlayed == gamesVector.size())
			tournamentOn = false;
		if (Scores::activeThreads < threads && gamesPlayed < gamesVector.size())
		{
			threadsVector.push_back(std::thread(&GameManager::play, &gamesVector[gamesPlayed]));
			threadsVector.back().join();
			Scores::activeThreads++;
			gamesPlayed++;
		}


		// Check if a round is over and print scores


		/*
		threadsVector.push_back(std::thread([&](GameManager* game) {game->play(); }, &gamesVector[activeThreads]));
		threadsVector.push_back(std::thread(&GameManager::play, &gamesVector[activeThreads]));
		threadsVector[activeThreads].join();
		activeThreads++;
		if (activeThreads == 5)
			tournamentOn = false;
		*/
	}
}


void TournamentManager::addGamesToVector(std::vector<GameManager>& gamesVector)
{
	size_t lastPlayerIndex;

	if (playersVector.size() % 2 == 1)
		lastPlayerIndex = playersVector.size();
	else
		lastPlayerIndex = playersVector.size() - 1;


	for (int boardRound = 0; boardRound < boardsVector.size(); boardRound++)
	{
		for (int i = 0; i < lastPlayerIndex; i++)
		{
			int playerAIndex = 0;
			int playerBIndex = lastPlayerIndex - i;
			if (playerBIndex != playersVector.size())
			{
				gamesVector.push_back(GameManager(boardRound, &playersVector[playerAIndex], &playersVector[playerBIndex], playerAIndex, playerBIndex));
				gamesVector.push_back(GameManager(boardRound, &playersVector[playerBIndex], &playersVector[playerAIndex], playerBIndex, playerAIndex));
			}

			for (int j = 0; j < (playersVector.size() / 2) + (playersVector.size() % 2) - 1; j++)
			{
				playerAIndex = ((2 * lastPlayerIndex - i + j) % lastPlayerIndex) + 1;
				playerBIndex = ((2 * lastPlayerIndex - 2 - i - j) % lastPlayerIndex) + 1;
				if (playerAIndex != playersVector.size() && playerBIndex != playersVector.size())
				{
					gamesVector.push_back(GameManager(boardRound, &playersVector[playerAIndex], &playersVector[playerBIndex], playerAIndex, playerBIndex));
					gamesVector.push_back(GameManager(boardRound, &playersVector[playerBIndex], &playersVector[playerAIndex], playerBIndex, playerAIndex));
				}
			}
		}
	}

	std::cout << "Number of games: " << gamesVector.size() << std::endl; // Debug line
}