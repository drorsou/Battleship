#include "TournamentManager.h"


int TournamentManager::threads = DEFAULT_THREADS;
std::string TournamentManager::path;

std::list<Board> TournamentManager::boardsList;
std::vector<std::unique_ptr<IBattleshipGameAlgo>> TournamentManager::playersVector;


//std::vector<std::condition_variable> TournamentManager::conditionsVector;
//std::vector<std::mutex> TournamentManager::mutexVector;
//std::mutex* TournamentManager::mutex;
std::mutex TournamentManager::mutex;
std::vector<bool> TournamentManager::playersLocks;
bool TournamentManager::tournamentOn = true;
std::queue<GameManager> TournamentManager::gamesQueue;



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
	FileReader::importFromFilesToVectors(boardsList, playersVector, TournamentManager::getPath());

	if (boardsList.size() < 1)
	{
		FileReader::printError(FileReader::Error::BOARD, TournamentManager::getPath());
		return false;
	}
	if (playersVector.size() < 2)
	{
		FileReader::printError(FileReader::Error::DLL, TournamentManager::getPath());
		return false;
	}


	TournamentManager::addGamesToQueue();

	ScoresController::initScores(static_cast<int>(playersVector.size()), gamesQueue.size() / playersVector.size());

	std::cout << "Number of legal players: " << playersVector.size() << std::endl;
	std::cout << "Number of legal boards: " << boardsList.size() << std::endl;
	std::cout << "Number of games: " << gamesQueue.size() << std::endl; // Debug line

	// Initialize the game and results variables

	for (int i = 0; i < playersVector.size() + 1; i++)
	{
		playersLocks.push_back(true);
	}

	for (int i = 0; i < threads; i++)
	{
		std::thread thread = std::thread(&waitForGames);
		thread.detach();
	}

	return true;
}


void TournamentManager::tournament()
{
	//std::vector<std::thread> threadsVector;
	//int gamesPlayed = 0;

	while (tournamentOn)
	{
		if (ScoresController::activeThreads == 0 && gamesQueue.empty() == true && ScoresController::totalRounds == ScoresController::round)
			tournamentOn = false;


		ScoresController::checkForResults();

		/*if (ScoresController::activeThreads < threads && gamesPlayed < gamesVector.size())
		{
			threadsVector.push_back(std::thread(&GameManager::play, &gamesVector[gamesPlayed]));
			threadsVector.back().join();
			ScoresController::activeThreads++;
			gamesPlayed++;
		}*/


		


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



void TournamentManager::waitForGames()
{
	while (tournamentOn)
	{
		GameManager game;
		bool startPlay = false;
		std::unique_lock<std::mutex> lock(TournamentManager::mutex, std::defer_lock);

		//std::condition_variable cv;

		//while (lock.try_lock() == false) {}
		lock.lock(); // Replace with condition?
		if (TournamentManager::gamesQueue.empty() == false)
		{
			ScoresController::activeThreads++;
			game = gamesQueue.front();
			gamesQueue.pop();

			startPlay = true;
		}
		lock.unlock();

		if (startPlay)
		{
			bool playersAreLocked = true;
			while (playersAreLocked)
			{
				lock.lock(); // Replace with condition?
				if (TournamentManager::playersLocks[game.getPlayerAIndex()] == true && TournamentManager::playersLocks[game.getPlayerBIndex()] == true)
				{
					TournamentManager::playersLocks[game.getPlayerAIndex()] = false;
					TournamentManager::playersLocks[game.getPlayerBIndex()] = false;
					playersAreLocked = false;
					lock.unlock();
					break;
				}
				lock.unlock();
			}
			
			game.play();

			lock.lock(); // Replace with condition?
			TournamentManager::playersLocks[game.getPlayerAIndex()] = true;
			TournamentManager::playersLocks[game.getPlayerBIndex()] = true;
			ScoresController::activeThreads--;
			lock.unlock();
		}
	}
}


void TournamentManager::addGamesToQueue()
{
	size_t lastPlayerIndex;

	if (playersVector.size() % 2 == 1)
		lastPlayerIndex = static_cast<int>(playersVector.size());
	else
		lastPlayerIndex = static_cast<int>(playersVector.size() - 1);


	for (int boardRound = 0; boardRound < boardsList.size(); boardRound++)
	{
		for (int i = 0; i < lastPlayerIndex; i++)
		{
			int playerAIndex = 0;
			int playerBIndex = lastPlayerIndex - i;
			if (playerBIndex != playersVector.size())
			{				
				gamesQueue.push(GameManager(boardRound, &playersVector[playerAIndex], &playersVector[playerBIndex], playerAIndex, playerBIndex));
				gamesQueue.push(GameManager(boardRound, &playersVector[playerBIndex], &playersVector[playerAIndex], playerBIndex, playerAIndex));
			}

			for (int j = 0; j < (playersVector.size() / 2) + (playersVector.size() % 2) - 1; j++)
			{
				playerAIndex = ((2 * lastPlayerIndex - i + j) % lastPlayerIndex) + 1;
				playerBIndex = ((2 * lastPlayerIndex - 2 - i - j) % lastPlayerIndex) + 1;
				if (playerAIndex != playersVector.size() && playerBIndex != playersVector.size())
				{					
					gamesQueue.push(GameManager(boardRound, &playersVector[playerAIndex], &playersVector[playerBIndex], playerAIndex, playerBIndex));
					gamesQueue.push(GameManager(boardRound, &playersVector[playerBIndex], &playersVector[playerAIndex], playerBIndex, playerAIndex));
				}
			}
		}
	}
}