#pragma once
#include <vector>
#include <iostream>
#include <queue>
#include <string>
#include <iomanip>
#include <mutex>
#include <windows.h>


class ScoresController
{
	struct Scores
	{
		int result; // 0 = win, 1 = loss
		int pointsFor;
		int pointsAgainst;

		Scores(int result_, int pointsFor_, int pointsAgainst_) : result(result_), pointsFor(pointsFor_), pointsAgainst(pointsAgainst_) {};
	};

	static std::vector<std::string> playerNamesVector;
	static std::vector<std::queue<ScoresController::Scores>> playerScores;
	static std::vector<int> winsVector;
	static std::vector<int> lossesVector;
	static std::vector<int> pointsForVector;
	static std::vector<int> pointsAgainstVector;

public:

	struct winPercent
	{
		int index;
		float percent;

		winPercent(int index_, int percent_) : index(index_), percent(percent_) {};

		bool operator < (const winPercent& wp) const { return percent > wp.percent; }
	};

	static void addPlayerName(std::string name) { playerNamesVector.push_back(name); };

	/* Pre: The number of players (algorithms)
	 * Post: Initialize the ScoresController variables according to the number of players
	 */
	static void initScores(int numberOfPlayers, int numberOfGames);

	/* Pre: Index of players A,B and the results of the game
	 * Post: Update the scores according to who won (player A = 0, player B = 1)
	 */
	static void updateScores(int playerAIndex, int playerBIndex, int winner, int pointsA, int pointsB);

	/* Post: Check if a round is over, if true then print the results so far
	 */
	static void checkForResults();

	/* Pre: A vector of sorted percent values 'winPercents'
	 * Post: Print the sorted results table
	 */
	static void printResults(std::vector<ScoresController::winPercent>& winPercents);

	/* Pre: Values of 'row', 'col'
	 * Post: Moves the console cursor from current position to the position plus the given 'row' and 'col'
	 */
	static void gotoXY(int row, int col);

	
	static int totalRounds;
	static int round;
	static int activeThreads;

	static std::mutex mutexScores;
};
