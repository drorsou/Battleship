#pragma once
#include "IBattleshipGameAlgo.h"
#include "Scores.h"
#include <memory>


class GameManager
{
	int board;
	std::unique_ptr<IBattleshipGameAlgo>* playerA;
	std::unique_ptr<IBattleshipGameAlgo>* playerB;
	int playerAIndex;
	int playerBIndex;
	bool finished;
	int winner;
	int pointsA;
	int pointsB;


public:
	GameManager(int board_, std::unique_ptr<IBattleshipGameAlgo>* playerA_, std::unique_ptr<IBattleshipGameAlgo>* playerB_,
		int playerAIndex_, int playerBIndex_) :
		board(board_), playerA(playerA_), playerB(playerB_), playerAIndex(playerAIndex_), playerBIndex(playerBIndex_) { };

	void play();
};