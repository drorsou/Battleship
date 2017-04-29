#pragma once
#include "IBattleshipGameAlgo.h"
#include "Ship.h"
#include <stack>
#include "Board.h"
enum tileMarks { DontAttack, Attack, Attacked };
class IntelligentAlgo : public IBattleshipGameAlgo
{	
	int player_number;
	char board[BOARD_SIZE][BOARD_SIZE];
	tileMarks shadow_board[BOARD_SIZE][BOARD_SIZE];	
	std::stack<pair<int, int>> possibleAttacks;
	

public:
	explicit IntelligentAlgo(int player_number)
		: player_number(player_number)
	{
	}

	void setBoard(int player, const char** board, int numRows, int numCols) override;
	bool init(const std::string& path) override;
	std::pair<int, int> attack() override;
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;
};
