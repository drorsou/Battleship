#pragma once
#include "IBattleshipGameAlgo.h"
#include "Ship.h"
#include <stack>
#include <tuple>
#include "Board.h"
enum tileMarks { DontAttack, Attack, Attacked };
enum direction { Left, Right, Up, Down, None };
class IntelligentAlgo : public IBattleshipGameAlgo
{
	int player_number;
	char ** board;
	int numOfRows;
	int numOfCols;
	tileMarks ** shadow_board;	
	std::stack<std::tuple<int, int, direction>> possibleAttacks;	
	std::pair<int, int> nextAttack;
	std::tuple<int,int,direction> lastFired;
	int numberOfRuns;
	/*
	 * Pre: nextAttack is a valid coordinate.
	 * Invariant: All tiles up to nextAttack (from top left)
	 *			  were attacked or marked as Don't Attack
	 * Post: If the tile is smaller than 10,10 move it one tile
	 *		 to the right or to the start of the next row.
	 */
	void addTwo();

	void markLeft(int row, int col);
	void markRight(int row, int col);
	void markUp(int row, int col);
	void markDown(int row, int col);
	void addAttackLeft(int row, int col);
	void addAttackRight(int row, int col);
	void addAttackUp(int row, int col);
	void addAttackDown(int row, int col);

	void markSink(int row, int col, direction dir);

	void markHit(int row, int col, direction dir);

	void addAttacks(int row, int col, direction dir);
	

	/*direction adjacentShot(int row, int col) const
	{
		if (std::get<0>(lastFired) - row >= -1 && std::get<0>(lastFired) - row <= 1 && std::get<1>(lastFired) == col)
			return Vertical;
		if (std::get<0>(lastFired) == row && std::get<1>(lastFired) - col >= -1 && std::get<1>(lastFired) - col <= 1)
			return Horizontal;
		return None;
	}*/
public:


	IntelligentAlgo(int player_number, int num_of_rows, int num_of_cols)
		: player_number(player_number),
		  numOfRows(num_of_rows),
		  numOfCols(num_of_cols),
		  numberOfRuns(0)
	{
		shadow_board = new tileMarks*[numOfRows];
		board = new char*[numOfRows];
		for (int i = 0; i < numOfRows; i++)
		{
			shadow_board[i] = new tileMarks[numOfCols];
			board[i] = new char[numOfCols];
			for (int j = 0; j < numOfCols; j++)
			{
				shadow_board[i][j] = tileMarks::Attack;
			}
		}
		nextAttack = make_pair(0, 0);
	}
	~IntelligentAlgo()
	{
		for (int i = 0; i < numOfRows; i++)
		{
			delete[] board[i];
			delete[] shadow_board[i];
		}
		delete[] board;
		delete[] shadow_board;
	}

	void setBoard(int player, const char** board, int numRows, int numCols) override;
	bool init(const std::string& path) override;
	std::pair<int, int> attack() override;
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;
};
