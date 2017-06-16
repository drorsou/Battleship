#include "Scores.h"


std::vector<std::string> Scores::playerNamesVector;
std::vector<int> Scores::winsVector;
std::vector<int> Scores::lossesVector;
std::vector<int> Scores::pointsForVector;
std::vector<int> Scores::pointsAgainstVector;

int Scores::activeThreads = 0;



void Scores::initScores(int numberOfPlayers)
{
	for (int i = 0; i < numberOfPlayers; i++)
	{
		winsVector.push_back(0);
		lossesVector.push_back(0);
		pointsForVector.push_back(0);
		pointsAgainstVector.push_back(0);
	}
}



void Scores::updateScores(int playerAIndex, int playerBIndex, int winner, int pointsA, int pointsB)
{
	// If winner == 0 then player A won, otherwise player B won
	winsVector[winner ? playerBIndex : playerAIndex] += 1;
	lossesVector[winner ? playerAIndex : playerBIndex] += 1;

	pointsForVector[playerAIndex] += pointsA;
	pointsAgainstVector[playerAIndex] += pointsB;
	pointsForVector[playerBIndex] += pointsB;
	pointsAgainstVector[playerBIndex] += pointsA;
}