#include "Board.h"

Board::~Board()
{
	delete playerA;
	delete playerB;
}

Board::Board(string path, int numOfRows, int numOfCols, IBattleshipGameAlgo * playerA, IBattleshipGameAlgo * playerB) : current_player_turn(0) {

	this->board.setDimensions(numOfRows, numOfCols);
	this->playerA = playerA;
	this->playerB = playerB;
	if (!parseBoard(path))
	{
		scoreA = -1;
		scoreB = -1;		
	}
	else
	{		
		if (this->checkBoard() == true)
		{
			totalShipsAScore = 0;
			totalShipsBScore = 0;
			for (int i = 0; i < SHIPS_PER_PLAYER; i++)
			{
				totalShipsAScore += shipsA.at(i).getScore();
				totalShipsBScore += shipsB.at(i).getScore();
			}
			char ** b = prepareBoard(0);
			this->playerA->setBoard(0, const_cast<const char **>(b), BOARD_SIZE, BOARD_SIZE);
			delete b;
			b = prepareBoard(1);
			this->playerB->setBoard(1, const_cast<const char **>(b), BOARD_SIZE, BOARD_SIZE);
			delete b;
			if (this->playerA->init(path) == false)
			{
				scoreA = -1;
				scoreB = -1;
				FileReader::printError(FileReader::Error::AlGO_INIT, path); // TODO Which error?
			}
			else if (this->playerB->init(path) == false)
			{
				scoreA = -1;
				scoreB = -1;
				FileReader::printError(FileReader::Error::AlGO_INIT, path); // TODO Which error?
			}
			else
			{
				scoreA = 0;
				scoreB = 0;
			}
		}
		else
		{
			// Marking for main that the board is faulty.
			scoreA = -1;
			scoreB = -1;
		}
		getCursorXY();
	}
}


void Board::notifyResult(int row, int col, AttackResult result)
{
	playerA->notifyOnAttackResult(0, row, col, result);
	playerB->notifyOnAttackResult(1, row, col, result);
}


void Board::gotoxy(int row, int col) const {
	COORD coord;
	coord.X = col + this->origin.x;
	coord.Y = row + this->origin.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

Type Board::shipType(int row, int col) const {
	Type t;
	switch (this->board.getPos(row,col))
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

bool Board::checkCoord(bool * sizeOShape, bool * adjacent, bool ** temp, int row, int col, char t) const {
	int len = 0;
	bool res = true;
	if ((row < board.num_of_rows() - 1 && this->board.getPos(row + 1,col) == t) && (col < board.num_of_cols() - 1 && this->board.getPos(row,col + 1) == t))
		*sizeOShape = true; // illegal ship size.
	// check top to down ships
	for (int i = row + 1; i < board.num_of_rows() && this->board.getPos(i,col) != BLANK; i++)
	{
		// Check for adjacent with the same type or if this tile was used in a different ship of this type.
		if ((this->board.getPos(i,col) == t && temp[i][col] == true) || (col > 0 && this->board.getPos(i,col - 1) == t) || (col < BOARD_SIZE - 1 && this->board.getPos(i,col+1) == t))
		{
			*sizeOShape = true;
			temp[i][col] = true;
			res = false;
		}
		// check if we already checked this tile (maybe a part of different ship)
		if (this->board.getPos(i,col) != t)
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
	for (int j = col + 1; j < board.num_of_cols() && this->board.getPos(row,j) != BLANK; j++)
	{
		// Check for adjacent with the same type or if this tile was used in a different ship of this type.
		if ((this->board.getPos(row,j) == t && temp[row][j] == true) || (row > 0 && this->board.getPos(row - 1,j) == t) || (row < board.num_of_rows() - 1 && this->board.getPos(row + 1,j) == t))
		{
			*sizeOShape = true;
			temp[row][j] = true;
			res = false;
		}
		// check if we already checked this tile (maybe a part of different ship)
		if (this->board.getPos(row,j) != t)
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
	bool dimensionFlag = Ship::checkDimensions(len, t); // check if the dimension create a legal ship, even if there is an adjacent ship
	if (dimensionFlag == false)
		*sizeOShape = true; // happens if the ship is shorter or longer than the legal size
	return (res && dimensionFlag);
}

char** Board::prepareBoard(int player) const
{
	char ** res = new char*[board.num_of_rows()];
	for (int i = 0; i < board.num_of_rows(); i++)
	{
		res[i] = new char[board.num_of_cols()];		
	}
	for(int row = 0; row < board.num_of_rows(); row++)
		for(int col = 0; col < board.num_of_cols(); col++)
		{
			if (coordColor(row, col) == player)
			{
				res[row][col] = board.getPos(row, col);
			}
			else
				res[row][col] = BLANK;
		}
	return res;
}

bool Board::checkTarget(char target) const {
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
		if (shipsA.at(i).isInThisShip(row, col))
		{
			if (shipsA.at(i).hit())
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
		if (shipsB.at(i).isInThisShip(row, col))
		{
			if (shipsB.at(i).hit())
			{
				scoreA += shipsB.at(i).getScore();
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
bool Board::hasPlayerWon(int player) const {
	return (player == 0 && scoreA == totalShipsBScore) || (player == 1 && scoreB == totalShipsAScore);
}


bool Board::checkBoard() {
	// creates a shadow board initialized to false.
	bool ** temp = new bool*[board.num_of_rows()]; 
	for(int i = 0; i < board.num_of_rows(); i++)
	{
		temp[i] = new bool[board.num_of_cols()];
		for(int j = 0; j < board.num_of_cols(); j++)
		{
			temp[i][j] = false;
		}
	}
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
	for(int row = 0; row < board.num_of_rows(); row++)
		for (int col = 0; col < board.num_of_cols(); col++)
		{
			// check all unmarked (i.e not visited) non-blank tiles
			if (temp[row][col] == false && this->board.getPos(row,col) != BLANK)
			{
				color = this->coordColor(row, col); 
				t = this->shipType(row, col);
				if(color == 0)
					checkRes = checkCoord(&(AsizeOShape[t]), &Adjacent, temp, row, col, this->board.getPos(row,col));
				else
					checkRes = checkCoord(&(BsizeOShape[t]), &Adjacent, temp, row, col, this->board.getPos(row,col));
				// Create the ship
				if (checkRes == true)
				{					
					if (this->board.getPos(row,col) == ABOAT || this->board.getPos(row,col) == BBOAT)
					{
						horz = make_pair(col + 1, col + 1);	
						vert = make_pair(row + 1, row + 1);
					}
					else
					{
						if (row < board.num_of_rows() - 1 && this->board.getPos(row + 1,col) != BLANK)
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
					if (color == 0)
					{						
						if (currA < SHIPS_PER_PLAYER)
							this->shipsA.at(currA) = Ship(vert, horz, t);
						currA++;
					}
					else
					{						
						if (currB < SHIPS_PER_PLAYER)
							this->shipsB.at(currB) = Ship(vert, horz, t);
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


void Board::printBoard() const {
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
	for (int row = 0; row <= this->board.num_of_rows() -1; row++)
	{
		printf((row < 9 ? "%d " : "%d"), row + 1);
		for (int col = 0; col <= this->board.num_of_cols() -1; col++){
			cout << "| ";
			/* 
				Color text according to player and ship type 
			*/
			switch (this->board.getPos(row,col))
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
			cout << this->board.getPos(row,col) << " ";
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
void Board::updateBoard(int row, int col) const {
	row--;
	col--;	
	this->gotoxy(2 * row + 2, 4 * col + 4);
	cout << "\b";
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (this->board.getPos(row,col) == BLANK || this->board.getPos(row,col) == MISS_SYM)
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
bool Board::parseBoard(std::string& path) {
	
	std::pair<std::string, string> boardFileDetails = FileReader::findFilesLexicographically("sboard");
	if (boardFileDetails.first.empty())
	{
		FileReader::printError(FileReader::Error::BOARD, path);
		return false;
	}

	ifstream fin(path + "\\" + boardFileDetails.first);
	if(!fin.is_open())
	{
		std::cout << "Error: Cannot open the board file for parsing." << endl;
		return false;
	}
	string* temp_board = new string[board.num_of_cols()];
	for (int i = 0; i < board.num_of_rows(); i++) {
		std::getline(fin, temp_board[i]);
		for (int j = 0; j < board.num_of_rows(); j++) {
			if (Board::checkChar(temp_board[i][j]))
				board.setPos(i,j,temp_board[i][j]);
			else
				board.setPos(i, j, BLANK);
		}
	}

	return true;
}

bool Board::checkChar(char c) {
	return (c == ABOAT || c == BBOAT || c == ACRUISER || c == BCRUISER || c == ASUBMARINE || c == BSUBMARINE || c == ADESTROYER || c == BDESTROYER);
}