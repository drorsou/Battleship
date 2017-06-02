#pragma once
#include "IBattleshipGameAlgo.h"
#include <memory>


class GameManager
{
	int board;
	std::unique_ptr<IBattleshipGameAlgo>* playerA;
	std::unique_ptr<IBattleshipGameAlgo>* playerB;


public:
	GameManager(int board_, std::unique_ptr<IBattleshipGameAlgo>* playerA_, std::unique_ptr<IBattleshipGameAlgo>* playerB_) :
		board(board), playerA(playerA_), playerB(playerB_) { };

	void play();
};