#include "TournamentManager.h"


int TournamentManager::threads = 4;
std::string TournamentManager::path;

std::vector<std::string> TournamentManager::boardsVector;
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
		//TournamentManager::mutexVector.push_back(std::mutex());
		Scores::winsVector.push_back(0);
		Scores::lossesVector.push_back(0);
		Scores::pointsForVector.push_back(0);
		Scores::pointsAgainstVector.push_back(0);
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
	int gamesPlayed = 0;
	int round = 0;
	bool tournamentOn = true;

	TournamentManager::addGamesToVector(gamesVector);

	while (tournamentOn)
	{


		//threadsVector.push_back(std::thread([&](GameManager* game) {game->play(); }, &gamesVector[activeThreads]));
		threadsVector.push_back(std::thread(&GameManager::play, &gamesVector[activeThreads]));
		threadsVector[activeThreads].join();
		activeThreads++;
		if (activeThreads == 5)
			tournamentOn = false;
		


		//Scores::updateScores(playerAIndex, playerBIndex, winner, pointsA, pointsB);
	}
}


void TournamentManager::addGamesToVector(std::vector<GameManager>& gamesVector)
{
	int lastPlayerIndex = playersVector.size() - 1;

	// If number of players is uneven
	//
	//


	// If number of players is even
	for (int boardRound = 0; boardRound < boardsVector.size(); boardRound++)
	{
		for (int i = 0; i < lastPlayerIndex; i++)
		{
			int playerAIndex = 0;
			int playerBIndex = lastPlayerIndex - i;
			gamesVector.push_back(GameManager(boardRound, &playersVector[playerAIndex], &playersVector[playerBIndex], playerAIndex, playerBIndex));
			gamesVector.push_back(GameManager(boardRound, &playersVector[playerBIndex], &playersVector[playerAIndex], playerBIndex, playerAIndex));

			for (int j = 0; j < (playersVector.size() / 2) - 1; j++)
			{
				playerAIndex = ((2 * lastPlayerIndex - i + j) % lastPlayerIndex) + 1;
				playerBIndex = ((2 * lastPlayerIndex - 2 - i - j) % lastPlayerIndex) + 1;
				gamesVector.push_back(GameManager(boardRound, &playersVector[playerAIndex], &playersVector[playerBIndex], playerAIndex, playerBIndex));
				gamesVector.push_back(GameManager(boardRound, &playersVector[playerBIndex], &playersVector[playerAIndex], playerBIndex, playerAIndex));
			}
		}
	}

	std::cout << "Number of games: " << gamesVector.size() << std::endl; // Debug line
}