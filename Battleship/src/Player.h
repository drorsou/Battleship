#pragma once
#include "IBattleshipGameAlgo.h"
#include "Ship.cpp"

#define SHIPS_PER_PLAYER 5

class Player: IBattleshipGameAlgo {
	Ship ships[SHIPS_PER_PLAYER];
	int score;

public:
	Player(Ship ships[SHIPS_PER_PLAYER]);
	void setBoard(const char** board, int numRows, int numCols) override; // called once to notify player on his board
	std::pair<int, int> attack() override; // ask player for his move
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override; // notify on last move result
};