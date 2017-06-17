#pragma once
#include <vector>


class Scores
{
	static std::vector<std::string> playerNamesVector;
	static std::vector<int> gamesPlayedVector;
	static std::vector<int> winsVector;
	static std::vector<int> lossesVector;
	static std::vector<int> pointsForVector;
	static std::vector<int> pointsAgainstVector;

	static int round;

public:

	static void addPlayerName(std::string name) { playerNamesVector.push_back(name); };

	/* Pre: The number of players (algorithms)
	 * Post: Initialize the Scores variables according to the number of players
	 */
	static void initScores(int numberOfPlayers);

	/* Pre: Index of players A,B and the results of the game
	 * Post: Update the scores according to who won (player A = 0, player B = 1)
	 */
	static void updateScores(int playerAIndex, int playerBIndex, int winner, int pointsA, int pointsB);


	static void checkForResults();

	static int activeThreads;
};