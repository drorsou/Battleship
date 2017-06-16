#include "Board.h"
#include <sstream>
#include "BoardDataAccess.h"

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

bool Board::checkCoord(bool * sizeOShape, bool * adjacent, TestingBoard<TileStatus>& temp, Coordinate c, char t) const {

	int len = 0;
	bool res = true;
	if (checkShapeAtCoord(c, t) == false)
		*sizeOShape = true; // illegal ship size.
	// check top to down ships
	for (int i = c.row + 1; i < board.rows() && this->board.charAt(Coordinate(i,c.col,c.depth)) != static_cast<char>(Ship::Symbol::Blank); i++)
	{
		char currTile = this->board.charAt(Coordinate(i, c.col, c.depth));
		// Check for adjacent with the same type or if this tile was used in a different ship of this type.
		if ((currTile == t && temp[Coordinate(i,c.col,c.depth)] == Checked) || checkColumnForAShip(Coordinate(i, c.col, c.depth),t) || checkDepthForAShip(Coordinate(i, c.col, c.depth), t))
		{
			*sizeOShape = true;
			temp[Coordinate(i, c.col, c.depth)] = Checked;
			res = false;
		}
		// check if we already checked this tile (maybe a part of different ship)
		if (currTile != t)
		{
			*adjacent = true;// adjacent ships!			
			break;
		} // No problems found
		else
		{
			len++;
			temp[Coordinate(i, c.col, c.depth)] = Checked;
		}
	}	
	// check left to right ships - marks seen parts even if this is illegal ship size or shape!
	for (int j = c.col + 1; j < board.cols() && this->board.charAt(Coordinate(c.row,j,c.depth)) != static_cast<char>(Ship::Symbol::Blank); j++)
	{
		char currTile = this->board.charAt(Coordinate(c.row, j, c.depth));
		// Check for adjacent with the same type or if this tile was used in a different ship of this type.
		if ((currTile == t && temp[Coordinate(c.row,j,c.depth)] == Checked) || checkRowForAShip(Coordinate(c.row,j,c.depth),t) || checkDepthForAShip(Coordinate(c.row, j, c.depth), t))
		{
			*sizeOShape = true;
			temp[Coordinate(c.row, j, c.depth)] = Checked;
			res = false;
		}
		// check if we already checked this tile (maybe a part of different ship)
		if (currTile != t)
		{
			*adjacent = true;// adjacent ships!			
			break;
		} // No problems found
		else
		{
			len++;
			temp[Coordinate(c.row, j, c.depth)] = Checked;
		}
	}
	for (int k = c.depth + 1; k < board.depth() && this->board.charAt(Coordinate(c.row, c.col, k)) != static_cast<char>(Ship::Symbol::Blank); k++)
	{
		char currTile = this->board.charAt(Coordinate(c.row, c.col, k));
		// Check for adjacent with the same type or if this tile was used in a different ship of this type.
		if ((currTile == t && temp[Coordinate(c.row, c.col, k)] == Checked) || checkRowForAShip(Coordinate(c.row, c.col, k), t) || checkColumnForAShip(Coordinate(c.row, c.col, k), t))
		{
			*sizeOShape = true;
			temp[Coordinate(c.row, c.col, k)] = Checked;
			res = false;
		}
		// check if we already checked this tile (maybe a part of different ship)
		if (currTile != t)
		{
			*adjacent = true;// adjacent ships!			
			break;
		} // No problems found
		else
		{
			len++;
			temp[Coordinate(c.row, c.col, k)] = Checked;
		}
	}
	len++; // for the original point.
	bool dimensionFlag = Ship::checkDimensions(len, t); // check if the dimension create a legal ship, even if there is an adjacent ship
	if (dimensionFlag == false)
		*sizeOShape = true; // happens if the ship is shorter or longer than the legal size
	return (res && dimensionFlag);
}

bool Board::checkShapeAtCoord(Coordinate c, char t) const
{
	int count = 0;
	if ((c.row < board.rows() - 1 && this->board.charAt(Coordinate(c.row + 1, c.col, c.depth)) == t))
		count++;
	if ((c.col < board.cols() - 1 && this->board.charAt(Coordinate(c.row, c.col + 1, c.depth)) == t))
		count++;
	if ((c.depth < board.depth() - 1 && this->board.charAt(Coordinate(c.row, c.col, c.depth + 1)) == t))
		count++;
	return (count <= 1);
}


void Board::setPlayer(int color, IBattleshipGameAlgo* player)
{	
	if (color == 0)
	{
		this->playerA = player;	
	}
	else
	{
		this->playerB = player;		
	}
	this->playerA->setBoard(BoardDataAccess{ &this->board, color });	
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
	// creates a shadow board initialized to false.
	TestingBoard<TileStatus> temp = TestingBoard<TileStatus>(board.rows(), board.cols(), board.depth());
	temp.initVal(UnChecked);
	Type t;
	pair<int, int> vert;
	pair<int, int> horz;
	pair<int, int> depth;
	Coordinate curr(-1,-1,-1);
	int color;
	int currA = 0;
	int currB = 0;
	bool AsizeOShape[4] = { false };
	bool BsizeOShape[4] = { false };
	bool Adjacent = false;
	bool result = true;
	bool checkRes;
	for (int d = 0; d < board.depth(); d++)
	{
		for (int row = 0; row < board.rows(); row++)
			for (int col = 0; col < board.cols(); col++)
			{
				curr = Coordinate{ row, col, d };
				// check all unmarked (i.e not visited) non-blank tiles
				if (temp[curr] == UnChecked && this->board.charAt(curr) != static_cast<char>(Ship::Symbol::Blank))
				{
					color = this->coordColor(curr);
					t = this->shipType(curr);
					if (color == 0)
						checkRes = checkCoord(&(AsizeOShape[t]), &Adjacent, temp, curr, this->board.charAt(curr));
					else
						checkRes = checkCoord(&(BsizeOShape[t]), &Adjacent, temp, curr, this->board.charAt(curr));
					// Create the ship
					if (checkRes == true)
					{
						fillDimensionsOfShip(curr, t, vert, horz, depth);
						// create the new ship in the proper list, count the ships even if there are too many
						if (color == 0)
						{
							if (currA < SHIPS_PER_PLAYER)
								this->shipsA.at(currA) = Ship(vert, horz, depth, t);
							currA++;
						}
						else
						{
							if (currB < SHIPS_PER_PLAYER)
								this->shipsB.at(currB) = Ship(vert, horz, depth, t);
							currB++;
						}
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

void Board::fillDimensionsOfShip(Coordinate c, Type t, std::pair<int, int>& vert, std::pair<int, int>& horz, std::pair<int, int>& depth) const
{
	if (this->board.charAt(c) == static_cast<char>(Ship::Symbol::ABoat) || this->board.charAt(c) == static_cast<char>(Ship::Symbol::BBoat))
	{
		horz = make_pair(c.col + 1, c.col + 1);
		vert = make_pair(c.row + 1, c.row + 1);
		depth = make_pair(c.depth + 1, c.depth + 1);
	}
	else
	{
		switch (getShipDirectionAt(c))
		{
		case Down:
			depth = make_pair(c.depth + 1, c.depth + 1);
			horz = make_pair(c.col + 1, c.col + 1);
			vert = makePairByLength(t, c.row);
			break;
		case Right:
			depth = make_pair(c.depth + 1, c.depth + 1);
			vert = make_pair(c.row + 1, c.row + 1);
			horz = makePairByLength(t, c.col);
			break;
		case Forward:
			horz = make_pair(c.col + 1, c.col + 1);
			vert = make_pair(c.row + 1, c.row + 1);
			depth = makePairByLength(t, c.depth);
			break;
		}
	}
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

Board::ShipDirection Board::getShipDirectionAt(const Coordinate c) const
{
	if (c.row < board.rows() - 1 && this->board.charAt(Coordinate(c.row + 1, c.col, c.depth)) != static_cast<char>(Ship::Symbol::Blank))
		return Down;
	if (c.col < board.cols() - 1 && this->board.charAt(Coordinate(c.row, c.col + 1, c.depth)) != static_cast<char>(Ship::Symbol::Blank))
		return Right;
	return Forward;
}

std::pair<int, int> Board::makePairByLength(const Type t, int pos)
{
	int len = 1;
	switch (t)
	{
	case Cruiser:
		len = Ship::ShipLen::CruiserLen;
		break;
	case Submarine:
		len = Ship::ShipLen::SubmarineLen;
		break;
	case Destroyer:
		len = Ship::ShipLen::DestroyerLen;		
		break;
	}
	return make_pair(pos + 1, pos + len);
}



/*
* Parse Board into field 'board' from 'path'
* Return true if no error, otherwise false
*/
bool Board::parseBoard(std::string& path) {

	int _rows;
	int _cols;
	int _depth;	

	ifstream fin(path);
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


