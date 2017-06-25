#include "TournamentManager.h"


int TournamentManager::threads = TournamentManager::Default::threadsDefault;
std::string TournamentManager::path;

std::mutex TournamentManager::mutexGames;
std::vector<bool> TournamentManager::playersLocks;

bool TournamentManager::tournamentOn = true;
std::queue<GameManager> TournamentManager::gamesQueue;
int TournamentManager::roundsAdded = 0;
int TournamentManager::boardRound = 0;
int TournamentManager::roundRobinIndex = 0;

std::vector<Board> TournamentManager::boardsVector;
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
	if (FileReader::checkArgs(argc, argv) == false)
		return false;

	if (!FileReader::checkIsValidDir(TournamentManager::getPath()))
	{
		FileReader::printError(FileReader::Error::PATH, TournamentManager::getPath());
		return false;
	}
	FileReader::writeToVectorTheFilesInDir(TournamentManager::getPath());


	std::cout << "Number of threads: " << threads << std::endl; // Debug line
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

	
	ScoresController::initScores(static_cast<int>(playersVector.size()), static_cast<int>(2 * boardsVector.size() * (playersVector.size() - 1)));

	std::cout << "Number of legal players: " << playersVector.size() << std::endl;
	std::cout << "Number of legal boards: " << boardsVector.size() << std::endl;

	
	for (int i = 0; i < playersVector.size(); i++)
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
	while (tournamentOn)
	{
		// Check if tournament is over
		if (ScoresController::activeThreads == 0 && gamesQueue.empty() == true && ScoresController::totalRounds == ScoresController::round)
			tournamentOn = false;

		// Add games to queue if there's not enough games there
		if (gamesQueue.size() < threads && boardRound < boardsVector.size())
			TournamentManager::addGamesToQueue();

		// Check if a round is over, if it is then print the results once
		std::unique_lock<std::mutex> lockScores(ScoresController::mutexScores, std::defer_lock);
		lockScores.lock();
		ScoresController::checkForResults();
		lockScores.unlock();
	}
}


void TournamentManager::waitForGames()
{
	while (tournamentOn)
	{
		GameManager game;
		bool startPlay = false;

		// Check if there is a game in the queue
		std::unique_lock<std::mutex> lockGames(TournamentManager::mutexGames, std::defer_lock);
		lockGames.lock();
		if (TournamentManager::gamesQueue.empty() == false)
		{
			ScoresController::activeThreads++;
			game = gamesQueue.front();
			gamesQueue.pop();

			startPlay = true;
		}
		lockGames.unlock();

		// If the thread got a game, it locks the players being used
		if (startPlay)
		{
			bool playersAreLocked = true;
			while (playersAreLocked)
			{
				lockGames.lock();
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

			// The thread runs the game		
			game.play();

			// After the game is finished, the thread unlocks the players used
			lockGames.lock();
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
	
	// Chooses the index being based if the number of players is even or unven
	if (playersVector.size() % 2 == 1)
		lastPlayerIndex = static_cast<int>(playersVector.size());
	else
		lastPlayerIndex = static_cast<int>(playersVector.size() - 1);

	int playerAIndex = 0;
	int playerBIndex = lastPlayerIndex - roundRobinIndex;
	
	std::unique_lock<std::mutex> lockGames(TournamentManager::mutexGames, std::defer_lock);
	lockGames.lock();

	// Add the first pair of games in the rounds
	if (playerBIndex != playersVector.size())
	{
		gamesQueue.push(GameManager(boardsVector[boardRound], playersVector[playerAIndex].get(), playersVector[playerBIndex].get(), playerAIndex, playerBIndex));
		gamesQueue.push(GameManager(boardsVector[boardRound], playersVector[playerBIndex].get(), playersVector[playerAIndex].get(), playerBIndex, playerAIndex));
	}

	// Add the rest of the rounds
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

	lockGames.unlock();

	roundRobinIndex++;
	if (roundRobinIndex == lastPlayerIndex)
	{
		boardRound++;
		roundRobinIndex = 0;
	}
}