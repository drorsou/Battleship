#include "Board.h"


Board::Board(string path, IBattleshipGameAlgo * playerA, IBattleshipGameAlgo * playerB) : current_player_turn(0) {

	if (!parseBoard(path))
	{
		scoreA = -1;
		scoreB = -1;
	}
	
	this->playerA = playerA;
	this->playerB = playerB;
	if (this->checkBoard() == true)
	{
		totalShipsAScore = 0;
		totalShipsBScore = 0;
		for (int i = 0; i < SHIPS_PER_PLAYER; i++)
		{
			totalShipsAScore += shipsA[i].getScore();
			totalShipsBScore += shipsB[i].getScore();
		}

		this->playerA->setBoard(0, const_cast<const char**> (board), BOARD_SIZE, BOARD_SIZE);
		this->playerB->setBoard(1, const_cast<const char**> (board), BOARD_SIZE, BOARD_SIZE);
		
		if (this->playerA->init(path) == false)
		{
			scoreA = -1;
			scoreB = -1;
			FileReader::printError("", path); // TODO Which error?
		};
		if (this->playerB->init(path) == false)
		{
			scoreA = -1;
			scoreB = -1;
			FileReader::printError("", path); // TODO Which error?
		};
	}
	else
	{
		// Marking for main that the board is faulty.
		scoreA = -1;
		scoreB = -1;
	}
	getCursorXY();	
}


void Board::notifyResult(int row, int col, AttackResult result)
{
	playerA->notifyOnAttackResult(0, row, col, result);
	playerB->notifyOnAttackResult(1, row, col, result);
}


void Board::gotoxy(int row, int col) {
	COORD coord;
	coord.X = col + this->origin.x;
	coord.Y = row + this->origin.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

Type Board::shipType(int row, int col) {
	Type t;
	switch (this->board[row][col])
	{
	case ABOAT: case BBOAT:
		t = Boat;
		break;
	case ACRUISER: case BCRUISER:
		t = Cruiser;
		break;
	case ASUBMARINE: case BSUBMARINE:
		t = Submarine;
		break;
	case ADESTROYER: case BDESTROYER:
		t = Destroyer;
		break;
	}
	return t;
}

bool Board::checkCoord(bool * sizeOShape, bool * adjacent, bool temp[BOARD_SIZE][BOARD_SIZE], int row, int col, char t) {
	int len = 0;
	bool res = true;
	bool dimensionFlag;
	if ((row < BOARD_SIZE - 1 && this->board[row + 1][col] == t) && (col < BOARD_SIZE - 1 && this->board[row][col + 1] == t))
		*sizeOShape = true; // illegal ship size.
	// check top to down ships
	for (int i = row + 1; i < BOARD_SIZE && this->board[i][col] != BLANK; i++)
	{
		// Check for adjacent with the same type or if this tile was used in a different ship of this type.
		if ((this->board[i][col] == t && temp[i][col] == true) || (col > 0 && this->board[i][col - 1] == t) || (col < BOARD_SIZE - 1 && this->board[i][col+1]) == t)
		{
			*sizeOShape = true;
			temp[i][col] = true;
			res = false;
		}
		// check if we already checked this tile (maybe a part of different ship)
		if (this->board[i][col] != t)
		{
			*adjacent = true;// adjacent ships!			
			break;
		} // No problems found
		else
		{
			len++;
			temp[i][col] = true;
		}
	}	
	// check left to right ships - marks seen parts even if this is illegal ship size or shape!
	for (int j = col + 1; j < BOARD_SIZE && this->board[row][j] != BLANK; j++)
	{
		// Check for adjacent with the same type or if this tile was used in a different ship of this type.
		if ((this->board[row][j] == t && temp[row][j] == true) || (row > 0 && this->board[row - 1][j] == t) || (row < BOARD_SIZE - 1 && this->board[row + 1][j]) == t)
		{
			*sizeOShape = true;
			temp[row][j] = true;
			res = false;
		}
		// check if we already checked this tile (maybe a part of different ship)
		if (this->board[row][j] != t)
		{
			*adjacent = true;// adjacent ships!			
			break;
		} // No problems found
		else
		{
			len++;
			temp[row][j] = true;
		}
	}
	len++; // for the original point.
	dimensionFlag = Ship::checkDimensions(len, t); // check if the dimension create a legal ship, even if there is an adjacent ship
	if (dimensionFlag == false)
		*sizeOShape = true; // happens if the ship is shorter or longer than the legal size
	return (res && dimensionFlag);
}

bool Board::checkTarget(char target) {
	if (this->current_player_turn == 0)
		return target == ABOAT || target == ACRUISER || target == ASUBMARINE || target == ADESTROYER;
	else
		return target == BBOAT || target == BCRUISER || target == BSUBMARINE || target == BDESTROYER;
}

/*
 * Perform a hit on a ship of 'type' in coords <row, col>
 * Update the board accordingly
 * Return true if the ship is sunk, otherwise false
 */
bool Board::hitShip(int row, int col, char type) {
	// Update the board
	setCoordValue(row, col, HIT_SYM);

	// Check if the ship is sunk
	for (int i = 0; i < SHIPS_PER_PLAYER; i++)
	{
		if (shipsA[i].isInThisShip(row, col))
		{
			if (shipsA[i].hit())
			{
				scoreB += shipsA[i].getScore();
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	for (int i = 0; i < SHIPS_PER_PLAYER; i++)
	{
		if (shipsB[i].isInThisShip(row, col))
		{
			if (shipsB[i].hit())
			{
				scoreA += shipsB[i].getScore();
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}


/*
 *
 */
bool Board::hasPlayerWon(int player)
{
	if (player == 0)
		return scoreA == totalShipsBScore ? true : false;
	if (player == 1)
		return scoreB == totalShipsAScore ? true : false;

	return false;
}


bool Board::checkBoard() {
	bool temp[BOARD_SIZE][BOARD_SIZE] = { false }; // creates a shadow board initialized to false.
	Type t;
	pair<int, int> vert;
	pair<int, int> horz;
	int color;
	int currA = 0;
	int currB = 0;
	bool AsizeOShape[4] = { false };
	bool BsizeOShape[4] = { false };
	bool Adjacent = false;
	bool result = true;
	bool checkRes;
	for(int row = 0; row < BOARD_SIZE; row++)
		for (int col = 0; col < BOARD_SIZE; col++)
		{
			// check all unmarked (i.e not visited) non-blank tiles
			if (temp[row][col] == false && this->board[row][col] != BLANK)
			{
				color = this->coordColor(row, col); 
				t = this->shipType(row, col);
				if(color == 1)
					checkRes = checkCoord(&(AsizeOShape[t]), &Adjacent, temp, row, col, this->board[row][col]);
				else
					checkRes = checkCoord(&(BsizeOShape[t]), &Adjacent, temp, row, col, this->board[row][col]);
				// Create the ship
				if (checkRes == true)
				{					
					if (this->board[row][col] == ABOAT || this->board[row][col] == BBOAT)
					{
						horz = make_pair(col + 1, col + 1);	
						vert = make_pair(row + 1, row + 1);
					}
					else
					{
						if (row < BOARD_SIZE - 1 && this->board[row + 1][col] != BLANK)
						{
							horz = make_pair(col + 1, col + 1);
							switch (t)
							{
							case Cruiser:								
								vert = make_pair(row + 1, row + CRUISER_LEN);
								break;
							case Submarine:								
								vert = make_pair(row + 1, row + SUBMARINE_LEN);
								break;
							case Destroyer:								
								vert = make_pair(row + 1, row + DESTROYER_LEN);
								break;
							}
						}
						else
						{
							vert = make_pair(row + 1, row + 1);
							switch (t)
							{
							case Cruiser:
								horz = make_pair(col + 1, col + CRUISER_LEN);
								break;
							case Submarine:
								horz = make_pair(col + 1, col + SUBMARINE_LEN);
								break;
							case Destroyer:
								horz = make_pair(col + 1, col + DESTROYER_LEN);
								break;
							}
						}
					}
					// create the new ship in the proper list, count the ships even if there are too many
					if (color == 1)
					{						
						if (currA < SHIPS_PER_PLAYER)
							this->shipsA[currA] = Ship(vert, horz, t);
						currA++;
					}
					else
					{						
						if (currB < SHIPS_PER_PLAYER)
							this->shipsB[currB] = Ship(vert, horz, t);
						currB++;
					}
				}				
			}
					
		}

	/*
		Printing the error messages according to the following order - first player A then B:
		Wrong size/shape.
		Too many
		Too few
		Adjacent (not player oriented!)
	*/
	if (AsizeOShape[Boat])
	{
		cout << "Wrong size or shape for ship B for player A\n";
		result = false;
	}
	if (AsizeOShape[Cruiser])
	{
		cout << "Wrong size or shape for ship P for player A\n";
		result = false;
	}
	if (AsizeOShape[Submarine])
	{
		cout << "Wrong size or shape for ship M for player A\n";
		result = false;
	}
	if (AsizeOShape[Destroyer])
	{
		cout << "Wrong size or shape for ship D for player A\n";
		result = false;
	}
	if (BsizeOShape[Boat])
	{
		cout << "Wrong size or shape for ship b for player B\n";
		result = false;
	}
	if (BsizeOShape[Cruiser])
	{
		cout << "Wrong size or shape for ship p for player B\n";
		result = false;
	}
	if (BsizeOShape[Submarine])
	{
		cout << "Wrong size or shape for ship m for player B\n";
		result = false;
	}
	if (BsizeOShape[Destroyer])
	{
		cout << "Wrong size or shape for ship d for player B\n";
		result = false;
	}	
	if (currA > SHIPS_PER_PLAYER)
	{
		cout << "Too many ships for player A\n";
		result = false;
	}
	else if (currA < SHIPS_PER_PLAYER)
	{
		cout << "Too few ships for player A\n";
		result = false;
	}
	if (currB > SHIPS_PER_PLAYER)
	{
		cout << "Too many ships for player B\n";
		result = false;
	}
	else if (currB < SHIPS_PER_PLAYER)
	{
		cout << "Too few ships for player B\n";
		result = false;
	}
	if (Adjacent)
	{
		cout << "Adjacent Ships on Board\n";
		result = false;
	}
	return result;
}


void Board::printBoard() {
	int i, j;	
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	/*
		Printing the first line with the column numbers.
		First box is empty
	*/	
	for (j = 0; j < BOARD_SIZE; j++)
	{
		printf((j == 0 ? "  | %d |" : (j < 9 ? " %d |" : " %d|")), j + 1);
	}
	printf("\n");
	Board::printLine();
	/*
		Printing the rest of the board.
	*/
	for (j = 0; j <= BOARD_SIZE-1; j++)
	{
		printf((j < 9 ? "%d " : "%d"), j + 1);
		for (i = 0; i <= BOARD_SIZE-1; i++){
			cout << "| ";
			/* 
				Color text according to player and ship type 
			*/
			switch (this->board[j][i])
			{
			case ABOAT:
				SetConsoleTextAttribute(hConsole, COLOR_AQUA);				
				break;
			case BBOAT:
				SetConsoleTextAttribute(hConsole, COLOR_LIGHT_AQUA);
				break;
			case ACRUISER:
				SetConsoleTextAttribute(hConsole, COLOR_GREEN);				
				break;
			case BCRUISER:
				SetConsoleTextAttribute(hConsole, COLOR_LIGHT_GREEN);				
				break;
			case ASUBMARINE:
				SetConsoleTextAttribute(hConsole, COLOR_YELLOW);
				break;
			case BSUBMARINE:
				SetConsoleTextAttribute(hConsole, COLOR_LIGHT_YELLOW);
				break;
			case ADESTROYER:
				SetConsoleTextAttribute(hConsole, COLOR_PURPLE);
				break;
			case BDESTROYER:
				SetConsoleTextAttribute(hConsole, COLOR_LIGHT_PURPLE);
				break;
			case HIT_SYM:
				SetConsoleTextAttribute(hConsole, COLOR_LIGHT_RED);
				break;
			case MISS_SYM:
				SetConsoleTextAttribute(hConsole, COLOR_RED);
				break;			
			}
			cout << this->board[j][i] << " ";
			// resetting the color
			SetConsoleTextAttribute(hConsole, COLOR_WHITE);			
		}
		cout << "|\n";
		Board::printLine();
	}	
}

void Board::getCursorXY() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
	    this->origin.x = csbi.dwCursorPosition.X;
	    this->origin.y = csbi.dwCursorPosition.Y;
	}
	else
	{
		this->origin.x = 0;
		this->origin.y = 0;
	}
}
void Board::updateBoard(int row, int col) {
	row--;
	col--;	
	this->gotoxy(2 * row + 2, 4 * col + 4);
	cout << "\b";
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (this->board[row][col] == BLANK || this->board[row][col] == MISS_SYM)
	{
		SetConsoleTextAttribute(hConsole, COLOR_RED);
		cout << " " << MISS_SYM << " ";
		// resetting the color
		SetConsoleTextAttribute(hConsole, COLOR_WHITE);
	}
	else
	{
		SetConsoleTextAttribute(hConsole, COLOR_LIGHT_RED);
		cout << " "<< HIT_SYM << " ";
		// resetting the color
		SetConsoleTextAttribute(hConsole, COLOR_WHITE);
	}
	this->gotoxy(22, 0); // moving cursor back to the bottom.
}


void Board::printLine(){
	int i;
	printf("  |");
	for (i = 1; i < BOARD_SIZE * 4; i++){
		printf("-");
	}
	printf("|\n");
}


/*
* Parse Board into field 'board' from 'path'
* Return true if no error, otherwise false
*/
bool Board::parseBoard(string path) {
	
	std::pair<bool, string> boardFileDetails = FileReader::findPathOfFile("sboard");
	if (boardFileDetails.first == false)
	{
		FileReader::printError("board", path);
		return false;
	}

	ifstream fin(path + "\\" + boardFileDetails.second);
	string* temp_board = new string[BOARD_SIZE];
	board = new char*[BOARD_SIZE];
	for (int i = 0; i < BOARD_SIZE; i++) {
		std::getline(fin, temp_board[i]);
		board[i] = new char[BOARD_SIZE];

		for (int j = 0; j < BOARD_SIZE; j++) {
			if (checkChar(temp_board[i][j]))
				board[i][j] = temp_board[i][j];
			else
				board[i][j] = BLANK;
		}
	}

	return true;
}

bool Board::checkChar(char c) {
	return (c == ABOAT || c == BBOAT || c == ACRUISER || c == BCRUISER || c == ASUBMARINE || c == BSUBMARINE || c == ADESTROYER || c == BDESTROYER);
}