#pragma once
#include <vector>
#include <iostream>
#include <queue>
#include <string>


class ScoresController
{
	struct Scores
	{
		int result; // True = win, false = loss
		int pointsFor;
		int pointsAgainst;

		Scores(int result_, int pointsFor_, int pointsAgainst_) : result(result_), pointsFor(pointsFor_), pointsAgainst(pointsAgainst_) {};
	};

	static std::vector<std::string> playerNamesVector;
	static std::vector<std::queue<ScoresController::Scores>> playerScores;



	//static std::vector<int> gamesPlayedVector;
	static std::vector<int> winsVector;
	static std::vector<int> lossesVector;
	static std::vector<int> pointsForVector;
	static std::vector<int> pointsAgainstVector;


public:

	static void addPlayerName(std::string name) { playerNamesVector.push_back(name); };

	/* Pre: The number of players (algorithms)
	 * Post: Initialize the ScoresController variables according to the number of players
	 */
	static void initScores(int numberOfPlayers, int numberOfGames);

	/* Pre: Index of players A,B and the results of the game
	 * Post: Update the scores according to who won (player A = 0, player B = 1)
	 */
	static void updateScores(int playerAIndex, int playerBIndex, int winner, int pointsA, int pointsB);


	static void checkForResults();

	static int totalRounds;
	static int round;

	static int activeThreads;
};
