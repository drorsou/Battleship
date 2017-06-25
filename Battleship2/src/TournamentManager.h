#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "GameManager.h"
#include "FileReader.h"
#include "ScoresController.h"



class TournamentManager
{
	static int threads;
	static std::string path;

	static std::mutex mutexGames;
	static std::vector<bool> playersLocks;

	static bool tournamentOn;
	static std::queue<GameManager> gamesQueue;
	static int roundsAdded;
	static int boardRound;
	static int roundRobinIndex;

	enum Default { threadsDefault = 4};

public:
	TournamentManager() {};

	static void setThreads(int threads_) { threads = threads_; }
	static void emptyPath() { path = "."; }
	static void setPath(char* path_) { path.append(path_); }
	static std::string getPath() { return path; }

	static std::vector<Board> boardsVector;
	static std::vector<std::unique_ptr<IBattleshipGameAlgo>> playersVector;


	/* Pre: Get the program arguments
	 * Post: Initialize all tournament parameters, return false if there is an error - otherwise true
	 */
	static bool init(int argc, char* argv[]);


	/* Post: Run the tournament
	 */
	static void tournament();


	/* Post: Worker threads run with this function, and wait for games to enter the games queue.
	 *	Whenever there's a game available, one thread will run that game.
	 */
	static void TournamentManager::waitForGames();


	/* Pre: GameManager vector
	 * Post: Add the games to the vector - for each board, all possible algorithm combinations
	 *	Adds only two rounds each time it's used
	 */
	static void addGamesToQueue();
};