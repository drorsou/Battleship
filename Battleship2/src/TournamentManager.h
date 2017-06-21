#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "GameManager.h"
#include "FileReader.h"
#include "Utils.h"
#include "ScoresController.h"


#define DEFAULT_THREADS 4


class TournamentManager
{
	static int threads;
	static std::string path;

public:
	TournamentManager() {};

	static void setThreads(int threads_) { threads = threads_; }
	static int getThreads() { return threads; }
	static void emptyPath() { path = "."; }
	static void setPath(char* path_) { path.append(path_); }
	static std::string getPath() { return path; }


	/* Pre: Get the program arguments
	 * Post: Initialize all tournament parameters, return false if there is an error - otherwise true
	 */
	static bool init(int argc, char* argv[]);

	/* Post: Run the tournament
	 */
	static void tournament();


	static void TournamentManager::waitForGames();


	/* Pre: GameManager vector
	 * Post: Add all the games to the vector - for each board, all possible algorithm combinations
	 */
	static void addGamesToQueue();




	static std::list<Board> boardsList;
	static std::vector<std::unique_ptr<IBattleshipGameAlgo>> playersVector;

	//static std::vector<std::condition_variable> conditionsVector;
	//static std::vector<std::mutex> mutexVector;
	//static std::mutex* mutex;
	static std::mutex mutex;
	static std::vector<bool> playersLocks;
	static bool tournamentOn;
	static std::queue<GameManager> gamesQueue;
};