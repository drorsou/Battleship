#include "ScoresController.h"
#include <iomanip>


std::vector<std::string> ScoresController::playerNamesVector;
std::vector<std::queue<ScoresController::Scores>> ScoresController::playerScores;

//std::vector<int> ScoresController::gamesPlayedVector;
std::vector<int> ScoresController::winsVector;
std::vector<int> ScoresController::lossesVector;
std::vector<int> ScoresController::pointsForVector;
std::vector<int> ScoresController::pointsAgainstVector;

int ScoresController::round = 0;
int ScoresController::totalRounds;

int ScoresController::activeThreads = 0;



void ScoresController::initScores(int numberOfPlayers, int numberOfGames)
{
	for (int i = 0; i < numberOfPlayers; i++)
	{
		//gamesPlayedVector.push_back(0);
		playerScores.push_back(std::queue<ScoresController::Scores>());

		winsVector.push_back(0);
		lossesVector.push_back(0);
		pointsForVector.push_back(0);
		pointsAgainstVector.push_back(0);
	}

	totalRounds = numberOfGames;
}


void ScoresController::updateScores(int playerAIndex, int playerBIndex, int winner, int pointsA, int pointsB)
{
	// If winner == 1 then player A won, otherwise player B won
	playerScores[playerAIndex].push(ScoresController::Scores(winner, pointsA, pointsB));
	playerScores[playerBIndex].push(ScoresController::Scores(1 - winner, pointsB, pointsA));

	/*gamesPlayedVector[playerAIndex] += 1;
	gamesPlayedVector[playerBIndex] += 1;
	winsVector[winner ? playerBIndex : playerAIndex] += 1;
	lossesVector[winner ? playerAIndex : playerBIndex] += 1;

	pointsForVector[playerAIndex] += pointsA;
	pointsAgainstVector[playerAIndex] += pointsB;
	pointsForVector[playerBIndex] += pointsB;
	pointsAgainstVector[playerBIndex] += pointsA;*/
}


void ScoresController::checkForResults()
{
	bool isRoundComplete = true;
	for (int i = 0; i < playerScores.size(); i++)
	{
		if (playerScores[i].size() == 0)
			isRoundComplete = false;
	}

	if (isRoundComplete == true)
	{
		std::vector<ScoresController::winPercent> winPercents;

		for (int i = 0; i < playerScores.size(); i++)
		{
			if (playerScores[i].front().result)
				winsVector[i] += 1;
			else
				lossesVector[i] += 1;
			pointsForVector[i] += playerScores[i].front().pointsFor;
			pointsAgainstVector[i] += playerScores[i].front().pointsAgainst;

			playerScores[i].pop();

			winPercents.push_back(ScoresController::winPercent(i, (winsVector[i] / (static_cast<float>(winsVector[i]) + lossesVector[i])) * 100));
		}

		std::sort(winPercents.begin(), winPercents.end());
		

		std::cout << "#" << round << "\tTeam Name\t\t" << "Wins\t" << "Losses\t" << "%\t" << "Pts For\t" << "Pts Against" << std::endl;
		for (int i = 0; i < playerScores.size(); i++)
		{
			int index = winPercents[i].index;
			std::cout << i + 1 << ".\t" << playerNamesVector[index] << "\t\t" << winsVector[index] << "\t" << lossesVector[index] << "\t"
				<< winPercents[i].percent
				<< "\t" << pointsForVector[index] << "\t" << pointsAgainstVector[index] << std::endl;
			//std::cout << winsVector[i] / (static_cast<float>(winsVector[i]) + lossesVector[i]) <<"* 100" << std::endl;
		}

		round++;
	}
}