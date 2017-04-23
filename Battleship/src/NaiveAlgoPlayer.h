#pragma once
#include "C:\Users\הדר\Source\Repos\Battleship3\Battleship\src\IBattleshipGameAlgo.h"
#include "C:\Users\הדר\Source\Repos\Battleship3\Battleship\src\Ship.h"
#include "AttackNaive.h"
#include <list>


class NaiveAlgoPlayer :public IBattleshipGameAlgo
{
	char player_board[10][10];
	int player_number; // 0 for A and 1 for B
	AttackNaive attack_naive;
	int currRowAttack=0;
	int currColAttack=0;
	list<Ship> playerShips;
	

public:
	NaiveAlgoPlayer();
	~NaiveAlgoPlayer();


	
	void setBoard(const char** board, int numRows, int numCols) override; // called once to notify player on his board
	std::pair<int, int> attack() override; // ask player for his move
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override; // notify on last move result



	std::pair<int, int> getNextAttack();
	bool hasNextAttack();
	bool checkLogicalAttack(int row, int col);
	bool isShipOfPlayer(int row, int col);
	std::pair<int, int> calcNextAttack();
};

