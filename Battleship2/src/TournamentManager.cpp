#include "TournamentManager.h"


int TournamentManager::threads = TournamentManager::Default::threadsDefault;
std::string TournamentManager::path;

int TournamentManager::roundsAdded = 0;
int TournamentManager::boardRound = 0;
int TournamentManager::roundRobinIndex = 0;

std::vector<Board> TournamentManager::boardsVector;
std::vector<std::unique_ptr<IBattleshipGameAlgo>> TournamentManager::playersVector;


//std::vector<std::condition_variable> TournamentManager::conditionsVector;
//std::vector<std::mutex> TournamentManager::mutexVector;
//std::mutex* TournamentManager::mutex;
std::mutex TournamentManager::mutex;
//std::condition_variable TournamentManager::cvGames;
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

	
	ScoresController::initScores(static_cast<int>(playersVector.size()), 2 * boardsVector.size() * (playersVector.size() - 1));

	std::cout << "Number of legal players: " << playersVector.size() << std::endl;
	std::cout << "Number of legal boards: " << boardsVector.size() << std::endl;
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


		if (gamesQueue.size() < threads && boardRound < boardsVector.size())
			TournamentManager::addGamesToQueue();


		std::unique_lock<std::mutex> lockScores(ScoresController::mutexScores, std::defer_lock);
		lockScores.lock();
		//ScoresController::cvScores.wait(lockScores, [] {return ScoresController::activeThreads == 0; });
		ScoresController::checkForResults();
		//std::cout << "Checking scores" << std::endl;
		lockScores.unlock();



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
		std::unique_lock<std::mutex> lockGames(TournamentManager::mutex, std::defer_lock);
		lockGames.lock();
		//cvGames.wait(lockGames, [] {return TournamentManager::gamesQueue.empty() == false; });
		
		//while (lock.try_lock() == false) {}
		//lockGames.lock(); // Replace with condition?
		if (TournamentManager::gamesQueue.empty() == false)
		{
		ScoresController::activeThreads++;
		game = gamesQueue.front();
		gamesQueue.pop();

		startPlay = true;
		}
		lockGames.unlock();
		//cvGames.notify_one();

		if (startPlay)
		{
			bool playersAreLocked = true;
			while (playersAreLocked)
			{
				lockGames.lock(); // Replace with condition?
				if (TournamentManager::playersLocks[game.getPlayerAIndex()] == true && TournamentManager::playersLocks[game.getPlayerBIndex()] == true)
				{
					TournamentManager::playersLocks[game.getPlayerAIndex()] = false;
					TournamentManager::playersLocks[game.getPlayerBIndex()] = false;
					playersAreLocked = false;
					lockGames.unlock();
					break;
				}
				lockGames.unlock();
			}



			
			game.play();

			lockGames.lock(); // Replace with condition?
			TournamentManager::playersLocks[game.getPlayerAIndex()] = true;
			TournamentManager::playersLocks[game.getPlayerBIndex()] = true;
			ScoresController::activeThreads--;
			lockGames.unlock();
		}
	}
}


void TournamentManager::addGamesToQueue()
{
	int lastPlayerIndex;

	if (playersVector.size() % 2 == 1)
		lastPlayerIndex = static_cast<int>(playersVector.size());
	else
		lastPlayerIndex = static_cast<int>(playersVector.size() - 1);


	

	int playerAIndex = 0;
	int playerBIndex = lastPlayerIndex - roundRobinIndex;
	if (playerBIndex != playersVector.size())
	{
		gamesQueue.push(GameManager(boardsVector[boardRound], playersVector[playerAIndex].get(), playersVector[playerBIndex].get(), playerAIndex, playerBIndex));
		gamesQueue.push(GameManager(boardsVector[boardRound], playersVector[playerBIndex].get(), playersVector[playerAIndex].get(), playerBIndex, playerAIndex));
	}

	for (int j = 0; j < (playersVector.size() / 2) + (playersVector.size() % 2) - 1; j++)
	{
		playerAIndex = ((2 * lastPlayerIndex - roundRobinIndex + j) % lastPlayerIndex) + 1;
		playerBIndex = ((2 * lastPlayerIndex - 2 - roundRobinIndex - j) % lastPlayerIndex) + 1;
		if (playerAIndex != playersVector.size() && playerBIndex != playersVector.size())
		{
			gamesQueue.push(GameManager(boardsVector[boardRound], playersVector[playerAIndex].get(), playersVector[playerBIndex].get(), playerAIndex, playerBIndex));
			gamesQueue.push(GameManager(boardsVector[boardRound], playersVector[playerBIndex].get(), playersVector[playerAIndex].get(), playerBIndex, playerAIndex));
		}
	}


	roundRobinIndex++;
	if (roundRobinIndex == lastPlayerIndex)
	{
		boardRound++;
		roundRobinIndex = 0;
	}
	

	/*for (int boardRound = 0; boardRound < boardsVector.size(); boardRound++)
	{
		for (int i = 0; i < lastPlayerIndex; i++)
		{
			int playerAIndex = 0;
			int playerBIndex = lastPlayerIndex - i;
			if (playerBIndex != playersVector.size())
			{				
				gamesQueue.push(GameManager(boardsVector[boardRound], playersVector[playerAIndex].get(), playersVector[playerBIndex].get(), playerAIndex, playerBIndex));
				gamesQueue.push(GameManager(boardsVector[boardRound], playersVector[playerBIndex].get(), playersVector[playerAIndex].get(), playerBIndex, playerAIndex));
			}

			for (int j = 0; j < (playersVector.size() / 2) + (playersVector.size() % 2) - 1; j++)
			{
				playerAIndex = ((2 * lastPlayerIndex - i + j) % lastPlayerIndex) + 1;
				playerBIndex = ((2 * lastPlayerIndex - 2 - i - j) % lastPlayerIndex) + 1;
				if (playerAIndex != playersVector.size() && playerBIndex != playersVector.size())
				{					
					gamesQueue.push(GameManager(boardsVector[boardRound], playersVector[playerAIndex].get(), playersVector[playerBIndex].get(), playerAIndex, playerBIndex));
					gamesQueue.push(GameManager(boardsVector[boardRound], playersVector[playerBIndex].get(), playersVector[playerAIndex].get(), playerBIndex, playerAIndex));
				}
			}
		}
	}*/
}