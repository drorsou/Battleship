#pragma once
#include "BattleshipGameAlgo.h"
#include "IBattleshipGameAlgo.h"
#include "Ship.h"
#include "Attack.h"


class attackFromFileAlgo: public IBattleshipGameAlgo {
	Attack attack_from_file;

	char player_board[10][10];
	int player_number; // 0 for A and 1 for B

public:
	explicit attackFromFileAlgo() {};

	void setBoard(int player, const char** board, int numRows, int numCols) override; // called once to notify player on his board
	bool init(const std::string& path) override; // called once to allow init from files if needed returns whether the init succeeded or failed
	std::pair<int, int> attack() override; // ask player for his move
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override; // notify on last move result
};