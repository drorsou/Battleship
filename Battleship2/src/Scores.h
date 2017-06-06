#pragma once
#include <vector>


class Scores
{
	static std::vector<std::string> playerNamesVector;
	static std::vector<int> winsVector;
	static std::vector<int> lossesVector;
	static std::vector<int> pointsForVector;
	static std::vector<int> pointsAgainstVector;


public:
	static void Scores::updateScores(int playerAIndex, int playerBIndex, int winner, int pointsA, int pointsB)
};