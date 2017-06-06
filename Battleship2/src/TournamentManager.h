#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "GameManager.h"
#include "FileReader.h"
#include "Utils.h"
#include "Scores.h"


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

	/* Pre: GameManager vector
	 * Post: Add all the games to the vector - for each board, all possible algorithm combinations
	 */
	static void addGamesToVector(std::vector<GameManager>& gamesVector);




	static std::vector<std::string> boardsVector;
	static std::vector<std::unique_ptr<IBattleshipGameAlgo>> playersVector;

	static std::vector<std::condition_variable> conditionsVector;
	static std::vector<std::mutex> mutexVector;
};