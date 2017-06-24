#include "IntelligentAlgo.h"

int IntelligentAlgo::randomInt(int range)
{
	/*
	 * This code was taken from https://ericlippert.com/2013/12/16/how-much-bias-is-introduced-by-the-remainder-technique/
	 * which lowers the bias of the reminder random techniuqe.
	 * A limit of 10 random attempts was chosen so it always stops (this is a Las-Vegas Algorithm which might never stop).
	 */
	for (int i = 0; i < 10; i++)
	{
		int value = rand();
		if (value < RAND_MAX - RAND_MAX % range)
			return value % range;
	}
	return rand() % range;
}

Coordinate IntelligentAlgo::generateRandom() const
{
	return Coordinate(randomInt(board->rows()) + 1, randomInt(board->cols()) + 1, randomInt(board->depth()) + 1);
}

bool IntelligentAlgo::setNextAttackIterator()
{
	while (gameAttacks != formerGamesAttacks.cend())
	{ // stops if there are no more lists to check!
		if (gameAttacks->second == player_number) // we need to check only setups relevant to our color!
		{
			// set the iterator from which we pick the attacks
			nextAttack = gameAttacks->first.cbegin();
			end = gameAttacks->first.cend();
			++gameAttacks;
			return true;
		}
		// keep looking for a list where the color matches our color.
		++gameAttacks;
	}
	memoryFlag = false;
	return false;
}

void IntelligentAlgo::addAttackAtDirection(Coordinate c, direction dir, bool AtStart)
{	
	Coordinate target{ -1,-1,-1 };
	switch(dir)
	{
	case Left: 
		if (c.col > 1)
			target = Coordinate{ c.row, c.col - 1, c.depth };
		break;

		case Up:
		if(c.row > 1)
			target = Coordinate{ c.row - 1, c.col, c.depth };
		break;

		case Back:
		if (c.depth > 1)
			target = Coordinate{ c.row, c.col, c.depth - 1 };
		break;

		case Right:
		if(c.col < board->cols())
			target = Coordinate{ c.row, c.col + 1, c.depth };
		break;
		
		case Down:
		if (c.row < board->rows())
			target = Coordinate{ c.row + 1, c.col, c.depth };
		break;

		case Forward:
		if (c.depth < board->depth())
			target = Coordinate{ c.row, c.col, c.depth + 1 };
		break;
		default:
			return;
			break;
	}
	if (AtStart && target.row != -1 && target.col != -1 && target.depth != -1)
		possibleAttacks.emplace_back(target, dir);
	else
		possibleAttacks.emplace_front(target, dir);
}

void IntelligentAlgo::addMarkAtDirection(Coordinate c, direction dir)
{
	Coordinate pos{ -1,-1,-1 };
	switch (dir)
	{
	case Left:
		if (c.col > 1)
			pos = Coordinate{ c.row, c.col - 1, c.depth };
		break;

	case Up:
		if (c.row > 1)
			pos = Coordinate{ c.row - 1, c.col, c.depth };
		break;

	case Back:
		if (c.depth > 1)
			pos = Coordinate{ c.row, c.col, c.depth - 1 };
		break;

	case Right:
		if (c.col < board->cols())
			pos = Coordinate{ c.row, c.col + 1, c.depth };
		break;

	case Down:
		if (c.row < board->rows())
			pos = Coordinate{ c.row + 1, c.col, c.depth };
		break;

	case Forward:
		if (c.depth < board->depth())
			pos = Coordinate{ c.row, c.col, c.depth + 1 };
		break;
	default:
		return;
		break;
	}
	if (pos.row != -1 && pos.col != -1 && pos.depth != -1 && markAt(pos) == Attack)
		markPosition(pos, DontAttack);
}

void IntelligentAlgo::markSink(Coordinate c, direction dir)
{
	switch (dir)
	{
	case Up:
		addMarkAtDirection(c, Up);
		addMarkAtDirection(c, Forward);
		addMarkAtDirection(c, Back);
		addMarkAtDirection(c, Left);
		addMarkAtDirection(c, Right);
		break;
	case Down:
		addMarkAtDirection(c, Down);
		addMarkAtDirection(c, Forward);
		addMarkAtDirection(c, Back);
		addMarkAtDirection(c, Left);
		addMarkAtDirection(c, Right);
		break;
	case Right:
		addMarkAtDirection(c, Right);
		addMarkAtDirection(c, Forward);
		addMarkAtDirection(c, Back);
		addMarkAtDirection(c, Up);
		addMarkAtDirection(c, Down);
		break;
	case Left:
		addMarkAtDirection(c, Left);
		addMarkAtDirection(c, Forward);
		addMarkAtDirection(c, Back);
		addMarkAtDirection(c, Up);
		addMarkAtDirection(c, Down);
		break;
	case Forward:
		addMarkAtDirection(c, Forward);
		addMarkAtDirection(c, Left);
		addMarkAtDirection(c, Right);
		addMarkAtDirection(c, Up);
		addMarkAtDirection(c, Down);
		break;
	case Back:
		addMarkAtDirection(c, Back);
		addMarkAtDirection(c, Left);
		addMarkAtDirection(c, Right);
		addMarkAtDirection(c, Up);
		addMarkAtDirection(c, Down);
		break;
	case None:
		addMarkAtDirection(c, Forward);
		addMarkAtDirection(c, Back);
		addMarkAtDirection(c, Left);
		addMarkAtDirection(c, Right);
		addMarkAtDirection(c, Up);
		addMarkAtDirection(c, Down);
		break;
	}
}

void IntelligentAlgo::markHit(Coordinate c, direction dir)
{
	Coordinate previous{ -1,-1,-1 };
	switch(dir)
	{
	case Up:
		addMarkAtDirection(c, Left);
		addMarkAtDirection(c, Right);
		addMarkAtDirection(c, Forward);
		addMarkAtDirection(c, Back);
		previous = Coordinate(c.row + 1, c.col, c.depth);
		addMarkAtDirection(previous, Left);
		addMarkAtDirection(previous, Right);
		addMarkAtDirection(previous, Forward);
		addMarkAtDirection(previous, Back);
		break;
	case Down:
		addMarkAtDirection(c, Left);
		addMarkAtDirection(c, Right);
		addMarkAtDirection(c, Forward);
		addMarkAtDirection(c, Back);
		previous = Coordinate(c.row - 1, c.col, c.depth);
		addMarkAtDirection(previous, Left);
		addMarkAtDirection(previous, Right);
		addMarkAtDirection(previous, Forward);
		addMarkAtDirection(previous, Back);
		break;
	case Left:
		addMarkAtDirection(c, Up);
		addMarkAtDirection(c, Down);
		addMarkAtDirection(c, Forward);
		addMarkAtDirection(c, Back);
		previous = Coordinate(c.row, c.col + 1, c.depth);
		addMarkAtDirection(previous, Up);
		addMarkAtDirection(previous, Down);
		addMarkAtDirection(previous, Forward);
		addMarkAtDirection(previous, Back);
		break;
	case Right:
		addMarkAtDirection(c, Up);
		addMarkAtDirection(c, Down);
		addMarkAtDirection(c, Forward);
		addMarkAtDirection(c, Back);
		previous = Coordinate(c.row, c.col - 1, c.depth);
		addMarkAtDirection(previous, Up);
		addMarkAtDirection(previous, Down);
		addMarkAtDirection(previous, Forward);
		addMarkAtDirection(previous, Back);
		break;
	case Forward:
		addMarkAtDirection(c, Up);
		addMarkAtDirection(c, Down);
		addMarkAtDirection(c, Left);
		addMarkAtDirection(c, Right);
		previous = Coordinate(c.row, c.col, c.depth - 1);
		addMarkAtDirection(previous, Up);
		addMarkAtDirection(previous, Down);
		addMarkAtDirection(previous, Left);
		addMarkAtDirection(previous, Right);
		break;
	case Back:
		addMarkAtDirection(c, Up);
		addMarkAtDirection(c, Down);
		addMarkAtDirection(c, Left);
		addMarkAtDirection(c, Right);
		previous = Coordinate(c.row, c.col, c.depth + 1);
		addMarkAtDirection(previous, Up);
		addMarkAtDirection(previous, Down);
		addMarkAtDirection(previous, Left);
		addMarkAtDirection(previous, Right);
		break;
	case None:
		break;
	}
}

void IntelligentAlgo::addAttacks(Coordinate c, direction dir, bool atStart)
{
	switch (dir)
	{
	case Up:
	case Down:
	case Left:
	case Right:
	case Forward:
	case Back:
		addAttackAtDirection(c, dir, atStart);
		break; // adds an attack only in the appropriate direction.
	case None: // if we don't know the direction of the ship, add attacks around
		addAttackAtDirection(c, Up, atStart);
		addAttackAtDirection(c, Down, atStart);
		addAttackAtDirection(c, Left, atStart);
		addAttackAtDirection(c, Right, atStart);
		addAttackAtDirection(c, Forward, atStart);
		addAttackAtDirection(c, Back, atStart);
		break;
	}
}

void IntelligentAlgo::markBoard()
{
	for (int depth = 1; depth <= board->depth(); depth++)
		for (int row = 1; row <= board->rows(); row++)
			for (int col = 1; col <= board->cols(); col++)
				markPosition(Coordinate(row, col, depth), tileMarks::Attack);

	for(int depth = 1; depth <= board->depth(); depth++)
		for(int row = 1; row <= board->rows(); row++)
			for (int col = 1; col <= board->cols(); col++)
			{
				Coordinate curr = Coordinate(row, col, depth);
				if (board->charAt(curr) != static_cast<char>(Ship::Symbol::Blank))
				{
					markPosition(curr, tileMarks::DontAttack);
					if (row > 1)
					{
						markPosition(Coordinate(row - 1, col, depth), tileMarks::DontAttack);
					}
					if (col > 1)
					{
						markPosition(Coordinate(row, col - 1, depth), tileMarks::DontAttack);
					}
					if(depth > 1)
					{
						markPosition(Coordinate(row, col, depth - 1), tileMarks::DontAttack);
					}
					if (row < board->rows())
					{
						markPosition(Coordinate(row + 1, col, depth), tileMarks::DontAttack);
					}
					if (col < board->cols())
					{
						markPosition(Coordinate(row, col + 1, depth), tileMarks::DontAttack);
					}
					if (depth < board->depth())
					{
						markPosition(Coordinate(row, col, depth + 1), tileMarks::DontAttack);
					}
				}
			}
}


void IntelligentAlgo::setPlayer(int player)
{
	if (!currentAttacks.empty())
	{
		this->formerGamesAttacks.push_front(make_pair(currentAttacks, player_number));
		this->currentAttacks.clear();
	}
	this->player_number = player;
}

void IntelligentAlgo::setBoard(const BoardData& board)
{
	this->board = &board;	
	if (this->shadow_board.get() == nullptr)
		this->shadow_board = unique_ptr<tileMarks>(new tileMarks[board.rows() * board.cols() * board.depth()]);
	else
	{
		auto * temp = this->shadow_board.release();
		delete[] temp;
		this->shadow_board.reset(new tileMarks[board.rows() * board.cols() * board.depth()]);
	}
	markBoard();
	if (!currentAttacks.empty())
	{
		this->formerGamesAttacks.push_front(make_pair(currentAttacks, player_number));
		this->currentAttacks.clear();
	}
	gameAttacks = formerGamesAttacks.cbegin();
	memoryFlag = setNextAttackIterator();	
}

Coordinate IntelligentAlgo::attack()
{	
	std::pair<Coordinate,direction> temp = make_pair(Coordinate(-1,-1,-1), None);
	Coordinate res{-1,-1,-1};
	while (memoryFlag)
	{
		if (nextAttack != end)
		{
			res = *nextAttack;
			++nextAttack;
			if(markAt(res) == Attack) // checks if the target is valid - marked as attackable.
				return res;
			else if(markAt(res) == DontAttack) // Wrong list - shouldn't attack this tile.
				memoryFlag = setNextAttackIterator();
		}
		else
			memoryFlag = setNextAttackIterator();
	}
	// If we passed all former games.
	if (!possibleAttacks.empty()) // there are available targets in the list.
	{
		do
		{
			temp = possibleAttacks.back();
			possibleAttacks.pop_back();
		} while (markAt(temp.first) != Attack && !possibleAttacks.empty());

		/* checking if the coord is a valid target - 
		 * we may reach this part when possibleAttacks is empty 
		 * AND "temp" isn't a valid target.
		 */
		if (markAt(temp.first) == Attack) 
		{
			res = temp.first;
			lastAttack = res;
			lastDirection = temp.second;
			return res;
		}
	}
	// otherwise randomly attack valid positions on the board.
	lastDirection = None;
	do
	{
		res = generateRandom();
	}
	while (markAt(res) != Attack); // Should stop if the game isn't over.
	lastAttack = res;	
	return res;
}

void IntelligentAlgo::notifyOnAttackResult(int player, Coordinate move, AttackResult result)
{	
	if (player == player_number)
	{
		if (result == AttackResult::Sink)
		{
			if (memoryFlag)
			{
				markSink(move, None);
			}
			else
			{
				markSink(move, lastDirection);
			}
		}
		else
		{
			// If we missed using our memory, change to a different list (this one is irrelevant)
			if (result == AttackResult::Miss && memoryFlag == true)
			{
				memoryFlag = setNextAttackIterator();
			}
			else if (result == AttackResult::Hit && memoryFlag == false)
			{
				// adding targets
				addAttacks(move, lastDirection, true);
				markHit(move, lastDirection);
			}
		}
	}
	else if (markAt(move) == Attack && result == AttackResult::Hit) // The opponent hit himself.
		addAttacks(move, None, false);
	if (player_number == player && (result == AttackResult::Sink || result == AttackResult::Hit))
		this->currentAttacks.push_front(move);
	// finally mark this tile as attacked.
	markPosition(move, Attacked);
}

IBattleshipGameAlgo* GetAlgorithm()
{
	return new IntelligentAlgo();
}