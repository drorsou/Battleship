#pragma once
#include <vector>


class Scores
{



public:

	static void initScores(int numberOfPlayers);


	static void updateScores(int playerAIndex, int playerBIndex, int winner, int pointsA, int pointsB);

	static std::vector<std::string> playerNamesVector;
	static std::vector<int> winsVector;
	static std::vector<int> lossesVector;
	static std::vector<int> pointsForVector;
	static std::vector<int> pointsAgainstVector;

	static int activeThreads;
};