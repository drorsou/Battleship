#include "Board.h"
#include <sstream>

Board::~Board()
{
	delete playerA;
	delete playerB;
}

Board::Board(string path) : current_player_turn(0), 
						    playerA(nullptr),
							playerB(nullptr)
{	
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
			scoreA = 0;
			scoreB = 0;			
		}
		else
		{
			// Marking for main that the board is faulty.
			scoreA = -1;
			scoreB = -1;
			std::cout << "Error: The board is incorrect" << endl;
		}		
	}
}


void Board::notifyResult(int player, int row, int col, AttackResult result)
{
#ifdef IMPL
	playerA->notifyOnAttackResult(player, row, col, result);
	playerB->notifyOnAttackResult(player, row, col, result);
#endif
}


Type Board::shipType(Coordinate c) const {
	Type t;
	switch (this->board.charAt(c))
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
	default:
		t = None;
		break;
	}
	return t;
}

bool Board::checkCoord(bool * sizeOShape, bool * adjacent, bool ** temp, int row, int col, char t) const {
#ifdef IMPL
	int len = 0;
	bool res = true;
	if ((row < board.num_of_rows() - 1 && this->board.getPos(row + 1,col) == t) && (col < board.num_of_cols() - 1 && this->board.getPos(row,col + 1) == t))
		*sizeOShape = true; // illegal ship size.
	// check top to down ships
	for (int i = row + 1; i < board.num_of_rows() && this->board.getPos(i,col) != static_cast<char>(Ship::Symbol::Blank); i++)
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
	for (int j = col + 1; j < board.num_of_cols() && this->board.getPos(row,j) != static_cast<char>(Ship::Symbol::Blank); j++)
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
#endif
}


void Board::setPlayer(int color, IBattleshipGameAlgo* player)
{
#ifdef IMPL
	char ** b;
	if (color == 0)
	{
		this->playerA = player;
		b = prepareBoard(0);
		this->playerA->setBoard(0, const_cast<const char **>(b), this->board.num_of_rows(), this->board.num_of_cols());
	}
	else
	{
		this->playerB = player;
		b = prepareBoard(1);
		this->playerB->setBoard(1, const_cast<const char **>(b), this->board.num_of_rows(), this->board.num_of_cols());
	}
	delete b;	
#endif
}

bool Board::checkTarget(char target) const {	
	if (this->current_player_turn == 0)
		return target == static_cast<char>(Ship::Symbol::ABoat) || target == static_cast<char>(Ship::Symbol::ACruiser) || target == static_cast<char>(Ship::Symbol::ASubmarine) || target == static_cast<char>(Ship::Symbol::ADestroyer);
	else
		return target == static_cast<char>(Ship::Symbol::BBoat) || target == static_cast<char>(Ship::Symbol::BCruiser) || target == static_cast<char>(Ship::Symbol::BSubmarine) || target == static_cast<char>(Ship::Symbol::BDestroyer);
}

/*
 * Perform a hit on a ship of 'type' in coords <row, col>
 * Update the board accordingly
 * Return true if the ship is sunk, otherwise false
 */
bool Board::hitShip(Coordinate c, char type) {
	// Update the board
	setCoordValue(c, Ship::Symbol::Hit);

	// Check if the ship is sunk
	for (int i = 0; i < SHIPS_PER_PLAYER; i++)
	{
		if (shipsA.at(i).isInThisShip(c))
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
		if (shipsB.at(i).isInThisShip(c))
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
	return false;
#ifdef IMPL
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
			if (temp[row][col] == false && this->board.getPos(row,col) != static_cast<char>(Ship::Symbol::Blank))
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
						if (row < board.num_of_rows() - 1 && this->board.getPos(row + 1,col) != static_cast<char>(Ship::Symbol::Blank))
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
#endif
}






std::vector<std::string> Board::split(const std::string &s, char delim)
{
	std::vector<std::string> elems;
	std::stringstream strSt(s);
	std::string item;

	while (std::getline(strSt, item, delim))
	{
		Board::removeCharFromString(item, ' ');//erase spaces around the comma
		elems.push_back(item);
	}

	return elems;
}
void Board::removeCharFromString(std::string &str, char charToRemove)
{
	str.erase(remove(str.begin(), str.end(), charToRemove), str.end());
}

bool Board::is_number(const std::string &s) {
	return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

std::tuple<int, int, int> Board::ParseBoardShape(const std::string& line) 
{
	std::tuple<int, int, int> res = make_tuple(-1, -1, -1);
	std::vector<std::string> tokens = Board::split(line, 'x');
	int row;
	int col;
	int depth;
	if (tokens.size() != 3)
	{		
		return res; // invalid line format
	}
	if (!Board::is_number(tokens[0]) || !Board::is_number(tokens[1]) || !Board::is_number(tokens[2]))
	{		
		return  res; // ignore this line
	}
	try
	{
		row = stoi(tokens[0]);
		col = stoi(tokens[1]);
		depth = stoi(tokens[2]);
	}
	catch (const std::exception&)
	{		
		return res;
	}
	if (row < 1 || col < 1 || depth < 1)
	{		
		return res; // ignore this line
	}
	std::get<0>(res) = row;
	std::get<1>(res) = col;
	std::get<2>(res) = depth;
	return res;
}
/*
* Parse Board into field 'board' from 'path'
* Return true if no error, otherwise false
*/
bool Board::parseBoard(std::string& path) {

	int _rows;
	int _cols;
	int _depth;
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
	string sizes;
	std::getline(fin, sizes);	
	std::tie(_rows, _cols, _depth) = ParseBoardShape(sizes);
	if (_rows < 0 || _cols < 0 || _depth < 0)
		return false;
	board = boardArray{ _rows, _cols, _depth };
	string tempLine;
	Ship::Symbol sym;
	for (int depth = 0; depth < _depth; depth++)
	{
		std::getline(fin, tempLine); // skip a line
		for (int row = 0; row < _rows; row++) 
		{
			std::getline(fin, tempLine);
			for (int col = 0; col < _cols; col++) 
			{
				sym = Board::checkChar(tempLine[col]) 
					? static_cast<Ship::Symbol>(tempLine[col]) 
					: Ship::Symbol::Blank;
				board.setCharAt(Coordinate(row + 1, col + 1, depth + 1), sym);
			}
		}
	}

	return true;
}

bool Board::checkChar(char c) {
	return (c == static_cast<char>(Ship::Symbol::ABoat) || c == static_cast<char>(Ship::Symbol::BBoat) || c == static_cast<char>(Ship::Symbol::ACruiser) || c == static_cast<char>(Ship::Symbol::BCruiser) || c == static_cast<char>(Ship::Symbol::ASubmarine) || c == static_cast<char>(Ship::Symbol::BSubmarine) || c == static_cast<char>(Ship::Symbol::ADestroyer) || c == static_cast<char>(Ship::Symbol::BDestroyer));
}


