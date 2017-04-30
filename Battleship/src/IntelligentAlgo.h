#pragma once
#include "IBattleshipGameAlgo.h"
#include "Ship.h"
#include <stack>
#include "Board.h"
enum tileMarks { DontAttack, Attack, Attacked };
class IntelligentAlgo : public IBattleshipGameAlgo
{
	int player_number;
	char ** board;
	int numOfRows;
	int numOfCols;
	tileMarks ** shadow_board;	
	std::stack<pair<int, int>> possibleAttacks;	
	std::pair<int, int> nextAttack;

	/*
	 * Pre: nextAttack is a valid coordinate.
	 * Invariant: All tiles up to nextAttack (from top left)
	 *			  were attacked or marked as Don't Attack
	 * Post: If the tile is smaller than 10,10 move it one tile
	 *		 to the right or to the start of the next row.
	 */
	void addOne()
	{
		if (nextAttack.second < numOfCols - 1)
			nextAttack.second++;
		else if (nextAttack.first < numOfRows - 1)
		{
			nextAttack.first++;
			nextAttack.second = 0;
		}
	}
	bool checkColor(int row, int col) const
	{
		return (this->board[row][col] >= 'A' && this->board[row][col] <= 'Z') ? 0 == player_number : 1 == player_number;
	}
public:


	IntelligentAlgo(int player_number, int num_of_rows, int num_of_cols)
		: player_number(player_number),
		  numOfRows(num_of_rows),
		  numOfCols(num_of_cols)
	{
		shadow_board = new tileMarks*[numOfRows];
		for (int i = 0; i < numOfRows; i++)
		{
			shadow_board[i] = new tileMarks[numOfCols];
			for (int j = 0; j < numOfCols; j++)
			{
				shadow_board[i][j] = tileMarks::Attack;
			}
		}
		nextAttack = make_pair(0, 0);
	}
	~IntelligentAlgo()
	{
		delete[] board;
		delete[] shadow_board;
	}

	void setBoard(int player, const char** board, int numRows, int numCols) override;
	bool init(const std::string& path) override;
	std::pair<int, int> attack() override;
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;
};
