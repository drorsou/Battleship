#include "ScoresController.h"



std::mutex ScoresController::mutexScores;
std::condition_variable ScoresController::cvScores;


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

			winPercents.push_back(ScoresController::winPercent(i, static_cast<int>((winsVector[i] / (static_cast<float>(winsVector[i]) + lossesVector[i])) * 100)));
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



void getCursorXY(bool toOrigin) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
		if (toOrigin)
		{
			this->origin.X = csbi.dwCursorPosition.X;
			this->origin.Y = csbi.dwCursorPosition.Y;
		}
		else
		{
			this->end.X = csbi.dwCursorPosition.X;
			this->end.Y = csbi.dwCursorPosition.Y;
		}
	}
	else
	{
		if (toOrigin)
		{
			this->origin.X = 0;
			this->origin.Y = 0;
		}
		else
		{
			this->end.X = 0;
			this->end.Y = this->origin.Y + (2 * this->board.num_of_rows() + 2);
		}
	}
}
void Board::gotoxy(int row, int col) const {
	COORD coord;
	coord.X = col + this->origin.X;
	coord.Y = row + this->origin.Y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void Board::gotoEnd(int row, int col) const
{
	COORD coord;
	coord.X = col + this->end.X;
	coord.Y = row + this->end.Y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void Board::printBoard() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	/*
	Printing the first line with the column numbers.
	First box is empty
	*/
	for (int col = 0; col < this->board.num_of_cols(); col++)
	{
		printf((col == 0 ? "  | %d |" : (col < 9 ? " %d |" : " %d|")), col + 1);
	}
	printf("\n");
	Board::printLine();
	/*
	Printing the rest of the board.
	*/
	for (int row = 0; row <= this->board.num_of_rows() - 1; row++)
	{
		printf((row < 9 ? "%d " : "%d"), row + 1);
		for (int col = 0; col <= this->board.num_of_cols() - 1; col++) {
			cout << "| ";
			/*
			Color text according to player and ship type
			*/
			changeColor(hConsole, row, col);
			cout << this->board.getPos(row, col) << " ";
			// resetting the color
			SetConsoleTextAttribute(hConsole, COLOR_WHITE);
		}
		cout << "|\n";
		Board::printLine();
	}
	getCursorXY(false);
}


void Board::updateBoard(int row, int col) const {
	row--;
	col--;
	this->gotoxy(2 * row + 2, 4 * col + 4);
	std::cout << "\b";
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (this->board.getPos(row, col) == static_cast<char>(Ship::Symbol::Blank) || this->board.getPos(row, col) == static_cast<char>(Ship::Symbol::MISS))
	{
		SetConsoleTextAttribute(hConsole, COLOR_RED);
		cout << " " << static_cast<char>(Ship::Symbol::MISS) << " ";
		// resetting the color
		SetConsoleTextAttribute(hConsole, COLOR_WHITE);
	}
	else
	{
		SetConsoleTextAttribute(hConsole, COLOR_LIGHT_RED);
		std::cout << " " << static_cast<char>(Ship::Symbol::Hit) << " ";
		// resetting the color
		SetConsoleTextAttribute(hConsole, COLOR_WHITE);
	}
	this->gotoEnd(0, 0); // moving cursor back to the bottom.
}