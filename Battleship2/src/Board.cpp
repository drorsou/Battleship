#include "Board.h"
#include "BoardDataAccess.h"


Board::Board(Board&& other) noexcept: board(std::move(other.board)),
                                      current_player_turn(other.current_player_turn),
                                      playerA(nullptr),
                                      playerB(nullptr),
                                      totalShipsAScore(other.totalShipsAScore),
                                      totalShipsBScore(other.totalShipsBScore),
                                      scoreA(other.scoreA),
                                      scoreB(other.scoreB)
{	
	shipsA = std::move(other.shipsA);
	shipsB = std::move(other.shipsB);
}

Board& Board::operator=(const Board& other)
{	
	if (this == &other)
		return *this;
	board = other.board;
	current_player_turn = other.current_player_turn;
	playerA = nullptr;
	playerB = nullptr;
	shipsA = other.shipsA;
	shipsB = other.shipsB;
	totalShipsAScore = other.totalShipsAScore;
	totalShipsBScore = other.totalShipsBScore;
	scoreA = other.scoreA;
	scoreB = other.scoreB;
	return *this;
}

Board& Board::operator=(Board&& other) noexcept
{	
	if (this == &other)
		return *this;
	board = std::move(other.board);
	current_player_turn = other.current_player_turn;
	playerA = nullptr;
	playerB = nullptr;
	shipsA = std::move(other.shipsA);
	shipsB = std::move(other.shipsB);
	totalShipsAScore = other.totalShipsAScore;
	totalShipsBScore = other.totalShipsBScore;
	scoreA = other.scoreA;
	scoreB = other.scoreB;
	return *this;
}

Board::~Board()
{	
}

Board::Board(string path) : current_player_turn(0), 
						    playerA(nullptr),
							playerB(nullptr)
{	
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

Board::Board(const Board& other): board(other.board),
                                  current_player_turn(other.current_player_turn),
                                  playerA(nullptr),
                                  playerB(nullptr),
								  shipsA(other.shipsA),
								  shipsB(other.shipsB),
                                  totalShipsAScore(other.totalShipsAScore),
                                  totalShipsBScore(other.totalShipsBScore),
                                  scoreA(other.scoreA),
                                  scoreB(other.scoreB)
{	
}


void Board::notifyResult(int player, Coordinate c, AttackResult result)
{
	playerA->notifyOnAttackResult(player, c, result);
	playerB->notifyOnAttackResult(player, c, result);
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
		t = NoType;
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
	res = res && checkDirection(&len, sizeOShape, adjacent, temp, c, t, Right);
	res = res && checkDirection(&len, sizeOShape, adjacent, temp, c, t, Down);
	res = res && checkDirection(&len, sizeOShape, adjacent, temp, c, t, Forward);
	len++; // for the original point.
	bool dimensionFlag = Ship::checkDimensions(len, t); // check if the dimension create a legal ship, even if there is an adjacent ship
	if (dimensionFlag == false)
		*sizeOShape = true; // happens if the ship is shorter or longer than the legal size
	return (res && dimensionFlag);
}

bool Board::checkAdjacent(Coordinate c, ShipDirection dir, char type) const
{
	int end = 1;
	int val = 1;
	bool res = false;
	switch (dir)
	{
	case Right:
		end = this->board.cols();
		val = c.col;
		break;
	case Down:
		end = this->board.rows();
		val = c.row;
		break;
	case Forward:
		end = this->board.depth();
		val = c.depth;
		break;
	}

	if (val > 1)
	{
		Coordinate prev = generateCoord(c, dir, val - 1);
		char prChar = this->board.charAt(prev);
		res = res || (prChar != type && prChar != static_cast<char>(Ship::Symbol::Blank));
	}
	if (val < end)
	{
		Coordinate next = generateCoord(c, dir, val + 1);
		char nxChar = this->board.charAt(next);
		res = res || (nxChar != type && nxChar != static_cast<char>(Ship::Symbol::Blank));
	}
	return res;
}

bool Board::checkShapeAtCoord(Coordinate c, char t) const
{
	int count = 0;
	if ((c.row < board.rows() && this->board.charAt(Coordinate(c.row + 1, c.col, c.depth)) == t))
		count++;
	if ((c.col < board.cols() && this->board.charAt(Coordinate(c.row, c.col + 1, c.depth)) == t))
		count++;
	if ((c.depth < board.depth() && this->board.charAt(Coordinate(c.row, c.col, c.depth + 1)) == t))
		count++;
	return (count <= 1);
}

void Board::setPlayer(int color, IBattleshipGameAlgo* player)
{
	if (color == 0)
		playerA = player;
	else
		playerB = player;
	player->setPlayer(color);
	if (color == 0)
	{
		AAccess = { &this->board, color };
		player->setBoard(AAccess);
	}
	else
	{
		BAccess = { &this->board, color };
		player->setBoard(BAccess);		
	}	
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

Coordinate Board::generateCoord(const Coordinate c, ShipDirection dir, int val)
{
	switch(dir)
	{
	case Right:
		return Coordinate(c.row, val, c.depth);
		break;
	case Down:
		return Coordinate(val, c.col, c.depth);
		break;
	case Forward: default:
		return Coordinate(c.row, c.col, val);
		break;
	}
}

bool Board::checkDirection(int* len, bool* sizeOShape, bool* adjacent, TestingBoard<TileStatus>& temp, Coordinate c, char t, ShipDirection dir) const
{
	bool res = true;
	int start = 1, end = 1;
	switch(dir)
	{
	case Right:
		start = c.col + 1;
		end = board.cols();
		break;
	case Down:
		start = c.row + 1;
		end = board.rows();
		break;
	case Forward:
		start = c.depth + 1;
		end = board.depth();
		break;	
	}
	Coordinate curr = {-1, -1, -1};
	int i;
	for (i = start, curr = generateCoord(c, dir, i); i <= end && this->board.charAt(curr) != static_cast<char>(Ship::Symbol::Blank); i++, curr = generateCoord(c, dir, i))
	{
		char currTile = this->board.charAt(curr);
		bool checkForAdjacentShips = false;
		switch(dir)
		{
		case Right:
			checkForAdjacentShips = checkAdjacent(curr, Down, t) 
								 || checkAdjacent(curr, Forward, t);
			break;
		case Down:
			checkForAdjacentShips = checkAdjacent(curr, Right, t)
								 || checkAdjacent(curr, Forward, t);
			break;
		case Forward:
			checkForAdjacentShips = checkAdjacent(curr, Right, t)
								 || checkAdjacent(curr, Down, t);
			break;
		}
		// Check for adjacent with the same type or if this tile was used in a different ship of this type.
		if ((currTile == t && temp[curr] != UnChecked) || checkShapeAtCoord(curr, t) == false)
		{
			*sizeOShape = true;
			temp.setAt(curr, Checked);
			res = false;
		}
		else if (checkForAdjacentShips && currTile == t)
		{
			*adjacent = true;// adjacent ships!
			(*len)++;
			temp.setAt(curr, Checked);
		}
		// check if we already checked this tile (maybe a part of different ship)
		else if (currTile != t && currTile != static_cast<char>(Ship::Symbol::Blank) || checkForAdjacentShips)
		{
			*adjacent = true;// adjacent ships!				
		}		
		else if (currTile == static_cast<char>(Ship::Symbol::Blank))
		{
			temp.setAt(curr, Checked);
			break;
		}// No problems found
		else
		{
			(*len)++;
			temp.setAt(curr, Checked);
		}
	}
	return res;
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
				curr = Coordinate{ row + 1, col + 1, d + 1 };
				// check all unmarked (i.e not visited) non-blank tiles
				if (temp[curr] == UnChecked && this->board.charAt(curr) != static_cast<char>(Ship::Symbol::Blank))
				{
					color = this->coordColor(curr);
					t = this->shipType(curr);
					if (t != NoType)
					{
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
		horz = make_pair(c.col, c.col);
		vert = make_pair(c.row, c.row);
		depth = make_pair(c.depth, c.depth);
	}
	else
	{
		switch (getShipDirectionAt(c))
		{
		case Down:
			depth = make_pair(c.depth, c.depth);
			horz = make_pair(c.col, c.col);
			vert = makePairByLength(t, c.row);
			break;
		case Right:
			depth = make_pair(c.depth, c.depth);
			vert = make_pair(c.row, c.row);
			horz = makePairByLength(t, c.col);
			break;
		case Forward:
			horz = make_pair(c.col, c.col);
			vert = make_pair(c.row, c.row);
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
	return make_pair(pos, pos + len - 1);
}



/*
* Parse Board into field 'board' from 'path'
* Return true if no error, otherwise false
*/
bool Board::parseBoard(std::string& path) {

	int _rows;
	int _cols;
	int _depth;	

	std::ifstream fin(path);
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


