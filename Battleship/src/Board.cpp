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
		FileReader::printError(FileReader::Error::BOARD, path);
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
			this->playerA->setBoard(0, const_cast<const char **>(b), numOfRows, numOfCols);
			delete b;
			b = prepareBoard(1);
			this->playerB->setBoard(1, const_cast<const char **>(b), numOfRows, numOfCols);
			delete b;
			/*if (this->playerA->init(path) == false)
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
			{*/
				scoreA = 0;
				scoreB = 0;
			//}
		}
		else
		{
			// Marking for main that the board is faulty.
			scoreA = -1;
			scoreB = -1;
			std::cout << "Error: The board is incorrect" << endl;
		}
		getCursorXY(true);
	}
}


void Board::notifyResult(int player, int row, int col, AttackResult result)
{
	playerA->notifyOnAttackResult(player, row, col, result);
	playerB->notifyOnAttackResult(player, row, col, result);
}

void Board::getCursorXY(bool toOrigin) {
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

Type Board::shipType(int row, int col) const {
	Type t;
	switch (this->board.getPos(row,col))
	{
	case static_cast<char>(Ship::Symbol::ABoat): case static_cast<char>(Ship::Symbol::BBoat):
		t = Boat;
		break;
	case static_cast<char>(Ship::Symbol::ACruiser) : case static_cast<char>(Ship::Symbol::BCruiser) :
		t = Cruiser;
		break;
	case static_cast<char>(Ship::Symbol::ASubmarine) : case static_cast<char>(Ship::Symbol::BSubmarine) :
		t = Submarine;
		break;
	case static_cast<char>(Ship::Symbol::ADestroyer) : case static_cast<char>(Ship::Symbol::BDestroyer) :
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
				res[row][col] = static_cast<char>(Ship::Symbol::Blank);
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


bool Board::printSizeOrShapeError(int player, bool * arr)
{
	bool res = true;
	char sh;
	int dif;
	char pl = player == 0 ? 'A' : 'B';
	for(int i = 0; i < 4; i++)
		if(arr[i])
		{
			switch(i)
			{
			case 0:
				dif = static_cast<int>(Ship::Symbol::ABoat);
				break;
			case 1:
				dif = static_cast<int>(Ship::Symbol::ACruiser);
				break;
			case 2:
				dif = static_cast<int>(Ship::Symbol::ASubmarine);
				break;
			case 3:
				dif = static_cast<int>(Ship::Symbol::ADestroyer);
				break;
			}
			dif -= static_cast<int>(Ship::Symbol::ABoat);
			res = false;
			if(player == 0)
			{
				sh = static_cast<char>(static_cast<int>(Ship::Symbol::ABoat) + dif);				
			}
			else
			{
				sh = static_cast<char>(static_cast<int>(Ship::Symbol::BBoat) + dif);
			}
			std::cout << "Wrong size or shape for ship " << sh << " for player " << pl << std::endl;
		}
	return res;
}

bool Board::printNumOfShipsError(int player, int count)
{
	char pl = player == 0 ? 'A' : 'B';
	if (count > SHIPS_PER_PLAYER)
	{
		std::cout << "Too many ships for player " << pl << std::endl;
		return false;
	}
	if (count < SHIPS_PER_PLAYER)
	{
		std::cout << "Too few ships for player " << pl << std::endl;
		return false;
	}
	return true;
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
					if (this->board.getPos(row,col) == static_cast<char>(Ship::Symbol::ABoat) || this->board.getPos(row,col) == static_cast<char>(Ship::Symbol::BBoat))
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
								vert = make_pair(row + 1, row + Ship::ShipLen::CruiserLen);
								break;
							case Submarine:								
								vert = make_pair(row + 1, row + Ship::ShipLen::SubmarineLen);
								break;
							case Destroyer:								
								vert = make_pair(row + 1, row + Ship::ShipLen::DestroyerLen);
								break;
							}
						}
						else
						{
							vert = make_pair(row + 1, row + 1);
							switch (t)
							{
							case Cruiser:
								horz = make_pair(col + 1, col + Ship::ShipLen::CruiserLen);
								break;
							case Submarine:
								horz = make_pair(col + 1, col + Ship::ShipLen::SubmarineLen);
								break;
							case Destroyer:
								horz = make_pair(col + 1, col + Ship::ShipLen::DestroyerLen);
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
	/*if (AsizeOShape[Boat])
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
	}	*/
	/*if (currA > SHIPS_PER_PLAYER)
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
	}*/
	result = result & Board::printSizeOrShapeError(0, AsizeOShape);
	result = result & Board::printSizeOrShapeError(1, BsizeOShape);
	result = result & Board::printNumOfShipsError(0, currA);
	result = result & Board::printNumOfShipsError(1, currB);
	if (Adjacent)
	{
		std::cout << "Adjacent Ships on Board" << std::endl;
		result = false;
	}
	return result;
}


void Board::printBoard(){
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
			changeColor(hConsole, row, col);
			cout << this->board.getPos(row,col) << " ";
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
	if (this->board.getPos(row,col) == static_cast<char>(Ship::Symbol::Blank)|| this->board.getPos(row,col) == static_cast<char>(Ship::Symbol::MISS))
	{
		SetConsoleTextAttribute(hConsole, COLOR_RED);
		cout << " " << MISS_SYM << " ";
		// resetting the color
		SetConsoleTextAttribute(hConsole, COLOR_WHITE);
	}
	else
	{
		SetConsoleTextAttribute(hConsole, COLOR_LIGHT_RED);
		std::cout << " "<< static_cast<char>(Ship::Symbol::Hit) << " ";
		// resetting the color
		SetConsoleTextAttribute(hConsole, COLOR_WHITE);
	}
	this->gotoEnd(0, 0); // moving cursor back to the bottom.
}


void Board::printLine(){
	int i;
	printf("  |");
	for (i = 1; i < BOARD_SIZE * 4; i++){
		printf("-");
	}
	printf("|\n");
}

void Board::changeColor(HANDLE& hConsole, int row, int col) const
{
	Ship::Symbol temp = (Ship::Symbol)this->board.getPos(row, col);
	switch (temp)
	{
	case Ship::Symbol::ABoat:
		SetConsoleTextAttribute(hConsole, COLOR_AQUA);
		break;
	case Ship::Symbol::BBoat:
		SetConsoleTextAttribute(hConsole, COLOR_LIGHT_AQUA);
		break;
	case Ship::Symbol::ACruiser:
		SetConsoleTextAttribute(hConsole, COLOR_GREEN);
		break;
	case Ship::Symbol::BCruiser:
		SetConsoleTextAttribute(hConsole, COLOR_LIGHT_GREEN);
		break;
	case Ship::Symbol::ASubmarine:
		SetConsoleTextAttribute(hConsole, COLOR_YELLOW);
		break;
	case Ship::Symbol::BSubmarine:
		SetConsoleTextAttribute(hConsole, COLOR_LIGHT_YELLOW);
		break;
	case Ship::Symbol::ADestroyer:
		SetConsoleTextAttribute(hConsole, COLOR_PURPLE);
		break;
	case Ship::Symbol::BDestroyer:
		SetConsoleTextAttribute(hConsole, COLOR_LIGHT_PURPLE);
		break;
	case Ship::Symbol::Hit:
		SetConsoleTextAttribute(hConsole, COLOR_LIGHT_RED);
		break;
	case Ship::Symbol::MISS:
		SetConsoleTextAttribute(hConsole, COLOR_RED);
		break;
	}
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
				board.setPos(i, j, static_cast<char>(Ship::Symbol::Blank));
		}
	}

	return true;
}

bool Board::checkChar(char c) {
	return (c == static_cast<char>(Ship::Symbol::ABoat) || c == static_cast<char>(Ship::Symbol::BBoat) || c == static_cast<char>(Ship::Symbol::ACruiser) || c == static_cast<char>(Ship::Symbol::BCruiser) || c == static_cast<char>(Ship::Symbol::ASubmarine) || c == static_cast<char>(Ship::Symbol::BSubmarine) || c == static_cast<char>(Ship::Symbol::ADestroyer) || c == static_cast<char>(Ship::Symbol::BDestroyer));
}


