#include "Board.h"


Board::Board(char b[BOARD_SIZE][BOARD_SIZE], Attack attack1, Attack attack2) : current_player_turn(0) {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			board[i][j] = b[i][j];
		}
	}

	// Need to create here the ships lists for each player
	
	player1 = Player(0);
	player1.setAttackFromFile(attack1);
	player2 = Player(1);
	player2.setAttackFromFile(attack2);
}

void Board::gotoxy(int row, int col) {
	COORD coord;
	coord.X = col;
	coord.Y = row;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
bool Board::checkCoord(bool * sizeOShape, bool * adjacent, bool temp[BOARD_SIZE][BOARD_SIZE], int row, int col, char t) {
	int len = 0;
	bool res = true;
	bool dimensionFlag;
	if ((row < BOARD_SIZE - 1 && this->board[row + 1][col] == t) && (col < BOARD_SIZE - 1 && this->board[row][col + 1] == t))
		*sizeOShape = true; // illegal ship size.
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
			//res = false;
			break;
		} // No problems found
		else
		{
			len++;
			temp[i][col] = true;
		}
	}		
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
			//res = false;
			break;
		} // No problems found
		else
		{
			len++;
			temp[row][j] = true;
		}
	}
	len++; // for the original point.
	dimensionFlag = Ship::checkDimensions(len, t);
	if (dimensionFlag == false)
		*sizeOShape = true;
	return (res && dimensionFlag);
}
bool Board::checkBoard() {
	bool temp[BOARD_SIZE][BOARD_SIZE] = { false }; // creates a shadow board initialized to false.
	Ship AShips[SHIPS_PER_PLAYER];
	Ship BShips[SHIPS_PER_PLAYER];
	Type t;
	pair<int, int> start;
	pair<int, int> end;
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
			if (temp[row][col] == false && this->board[row][col] != BLANK)
			{
				color = this->coordColor(row, col); 
				t = this->shipType(row, col);
				if(color == 1)
					checkRes = checkCoord(&(AsizeOShape[t]), &Adjacent, temp, row, col, this->board[row][col]);
				else
					checkRes = checkCoord(&(BsizeOShape[t]), &Adjacent, temp, row, col, this->board[row][col]);
				if (checkRes == true)
				{
					start = make_pair(row, col);
					if (this->board[row][col] == ABOAT || this->board[row][col] == BBOAT)
					{
						end = make_pair(row, col);						
					}
					else
					{
						if (this->board[row + 1][col] != BLANK)
						{
							switch (t)
							{
							case Cruiser:								
								end = make_pair(row + CRUISER_LEN, col);
								break;
							case Submarine:								
								end = make_pair(row + SUBMARINE_LEN, col);
								break;
							case Destroyer:								
								end = make_pair(row + DESTROYER_LEN, col);
								break;
							}
						}
						else
						{
							switch (t)
							{
							case Cruiser:
								end = make_pair(row, col + CRUISER_LEN);
								break;
							case Submarine:
								end = make_pair(row, col + SUBMARINE_LEN);
								break;
							case Destroyer:
								end = make_pair(row, col + DESTROYER_LEN);
								break;
							}
						}
					}
					if (color == 1)
					{						
						if (currA < SHIPS_PER_PLAYER)
							AShips[currA] = Ship(start, end, t);
						currA++;
					}
					else
					{						
						if (currB < SHIPS_PER_PLAYER)
							BShips[currB] = Ship(start, end, t);
						currB++;
					}
				}				
			}
					
		}
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

void Board::printBoard(const Board & br) {
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
			switch (br.board[j][i])
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
			default:
				// Not writing anything!
				break;
			}
			cout << br.board[j][i] << " ";
			// resetting the color
			SetConsoleTextAttribute(hConsole, COLOR_WHITE);			
		}
		cout << "|\n";
		Board::printLine();
	}	
}
void Board::updateBoard(const Board & br, int row, int col) {
	row--;
	col--;
	Board::gotoxy(2 * row + 2, 4 * col + 4);
	cout << "\b";
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (br.board[row][col] == BLANK || br.board[row][col] == MISS_SYM)
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
	Board::gotoxy(22, 0); // moving cursor back to the bottom.
}
void Board::printLine(){
	int i;
	printf("  |");
	for (i = 1; i < BOARD_SIZE * 4; i++){
		printf("-");
	}
	printf("|\n");
}