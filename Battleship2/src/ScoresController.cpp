#include "ScoresController.h"


std::vector<std::string> ScoresController::playerNamesVector;
std::vector<std::queue<ScoresController::Scores>> ScoresController::playerScores;
std::vector<int> ScoresController::winsVector;
std::vector<int> ScoresController::lossesVector;
std::vector<int> ScoresController::pointsForVector;
std::vector<int> ScoresController::pointsAgainstVector;

int ScoresController::round = 0;
int ScoresController::totalRounds;
int ScoresController::activeThreads = 0;

std::mutex ScoresController::mutexScores;


void ScoresController::initScores(int numberOfPlayers, int totalRounds)
{
	for (int i = 0; i < numberOfPlayers; i++)
	{
		playerScores.push_back(std::queue<ScoresController::Scores>());

		winsVector.push_back(0);
		lossesVector.push_back(0);
		pointsForVector.push_back(0);
		pointsAgainstVector.push_back(0);
	}
	
	ScoresController::totalRounds = totalRounds;
}


void ScoresController::updateScores(int playerAIndex, int playerBIndex, int winner, int pointsA, int pointsB)
{
	// If winner == 0 then player A won, otherwise player B won
	playerScores[playerAIndex].push(ScoresController::Scores(winner, pointsA, pointsB));
	playerScores[playerBIndex].push(ScoresController::Scores(1 - winner, pointsB, pointsA));
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
			if (playerScores[i].front().result == 0)
				winsVector[i] += 1;
			else
				lossesVector[i] += 1;
			pointsForVector[i] += playerScores[i].front().pointsFor;
			pointsAgainstVector[i] += playerScores[i].front().pointsAgainst;

			playerScores[i].pop();

			winPercents.push_back(ScoresController::winPercent(i, (winsVector[i] / (static_cast<float>(winsVector[i]) + lossesVector[i])) * 100));
		}

		std::sort(winPercents.begin(), winPercents.end());
		ScoresController::printResults(winPercents);
	}
}


void ScoresController::printResults(std::vector<ScoresController::winPercent>& winPercents)
{
	int maxNameLength = 0;
	for (int i = 0; i < playerNamesVector.size(); i++)
		if (playerNamesVector[i].length() > maxNameLength)
			maxNameLength = static_cast<int>(playerNamesVector[i].length());

	std::cout << "#\t" << "Team Name";
	ScoresController::gotoXY(0, maxNameLength);
	std::cout << "Wins";
	ScoresController::gotoXY(0, 5);
	std::cout << "Losses";
	ScoresController::gotoXY(0, 3);
	std::cout << "%";
	ScoresController::gotoXY(0, 7);
	std::cout << "Pts For";
	ScoresController::gotoXY(0, 3);
	std::cout << "Pts Against" << std::endl;

	// Print all the players results
	for (int i = 0; i < playerScores.size(); i++)
	{
		int index = winPercents[i].index;
		std::cout << i + 1 << ".\t" << playerNamesVector[index];
		ScoresController::gotoXY(0, static_cast<int>(maxNameLength + 9 - playerNamesVector[index].length()));

		std::cout << winsVector[index];
		int m = 10;
		int k = 1;
		while (winsVector[index] % 10 > 0)
		{
			m = m * 10;
			k++;
		}
		ScoresController::gotoXY(0, 9 - k);

		std::cout << lossesVector[index];


		std::cout << winPercents[i].percent;


		std::cout << pointsForVector[index];
		

		std::cout << pointsAgainstVector[index] << std::endl;

	}

	round++;
}


void ScoresController::gotoXY(int row, int col)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD origin;
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
	{
		origin.X = csbi.dwCursorPosition.X;
		origin.Y = csbi.dwCursorPosition.Y;
	}
	else
	{
		origin.X = 0;
		origin.Y = 0;
	}
	COORD coord;
	coord.X = col + origin.X;
	coord.Y = row + origin.Y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}