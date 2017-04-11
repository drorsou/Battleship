#pragma once
#include "IBattleshipGameAlgo.h"
#include "Ship.h"
#include "Attack.h"

#define SHIPS_PER_PLAYER 5

class Player: IBattleshipGameAlgo {
	char player_board[10][10];
	Ship ships[SHIPS_PER_PLAYER];

public:
	Attack attack_from_file;
	int score;

	Player();
	Player(Ship ships[SHIPS_PER_PLAYER]);
	void setBoard(const char** board, int numRows, int numCols) override; // called once to notify player on his board
	pair<int, int> attack() override; // ask player for his move
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override; // notify on last move result
	void setAttackFromFile(Attack attack);
};