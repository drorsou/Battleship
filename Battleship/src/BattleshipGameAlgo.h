#pragma once
#include "IBattleshipGameAlgo.h"
//#include "Board.h"

class BattleshipGameAlgo :public IBattleshipGameAlgo
{

public:
	//char player_board[Board::Size::ROWS][Board::Size::COLS];
	int player_number; // 0 for A and 1 for B
	
	void setBoard(int player, const char** board, int numRows, int numCols) override; // called once to notify player on his board
	bool init(const std::string& path) override; // called once to allow init from files if needed returns whether the init succeeded or failed
	std::pair<int, int> attack() override; // ask player for his move
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override; // notify on last move result
	
	BattleshipGameAlgo();

	~BattleshipGameAlgo();
};
