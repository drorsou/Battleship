#pragma once
#include "Ship.h"
#include <stack>
#include <array>
#include <list>
#include <tuple>
#include "TestingBoard.h"
#include "BoardDataAccess.h"

enum tileMarks { DontAttack, Attack, Attacked };
enum direction { Left, Right, Up, Down, Forward, Back, None};
class IntelligentAlgo : public IBattleshipGameAlgo
{	
	static const size_t NUM_OF_SHIPS = 5;
	//std::array<Ship, NUM_OF_SHIPS> ships;
	std::list<std::pair<std::list<Coordinate>, int>> formerGamesAttacks;	
	std::list<std::pair<std::list<Coordinate>, int>>::const_iterator gameAttacks;
	std::list<Coordinate>::const_iterator nextAttack;
	std::list<Coordinate>::const_iterator end;
	std::list<Coordinate> currentAttacks;
	bool memoryFlag;
	std::list<std::pair<Coordinate, direction>> possibleAttacks;
	Coordinate lastAttack{-1,-1,-1};
	direction lastDirection = direction::None;
	
	int player_number;
	const BoardData * board = nullptr;
	unique_ptr<tileMarks> shadow_board = nullptr;
	int position(Coordinate c) const {
		return (c.col - 1) + (c.row - 1) * board->cols() + (c.depth - 1) * board->rows() * board->cols();
	}
	void markPosition(Coordinate c, tileMarks mark)	{
		(shadow_board.get())[position(c)] = mark;
	}
	tileMarks markAt(Coordinate c) const {
		return (shadow_board.get())[position(c)];
	}

	static int randomInt(int range);
	Coordinate generateRandom() const;	

	bool setNextAttackIterator();
	
	void addAttackAtDirection(Coordinate c, direction dir, bool AtStart);
	void addMarkAtDirection(Coordinate c, direction dir);
	
	void markSink(Coordinate c, direction dir);

	void markHit(Coordinate c, direction dir);

	void addAttacks(Coordinate c, direction dir, bool atStart);
	
	void markBoard();

	/*direction adjacentShot(int row, int col) const
	{
		if (std::get<0>(lastFired) - row >= -1 && std::get<0>(lastFired) - row <= 1 && std::get<1>(lastFired) == col)
			return Vertical;
		if (std::get<0>(lastFired) == row && std::get<1>(lastFired) - col >= -1 && std::get<1>(lastFired) - col <= 1)
			return Horizontal;
		return None;
	}*/
public:

	IntelligentAlgo()
		: player_number(-1),
		  shadow_board()
	{
	}


	void setPlayer(int player) override;
	void setBoard(const BoardData& board) override;
	Coordinate attack() override;
	void notifyOnAttackResult(int player, Coordinate move, AttackResult result) override;
};
