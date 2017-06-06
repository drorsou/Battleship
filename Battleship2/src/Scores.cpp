#include "Scores.h"



std::vector<int> Scores::winsVector;
std::vector<int> Scores::lossesVector;
std::vector<int> Scores::pointsForVector;
std::vector<int> Scores::pointsAgainstVector;


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