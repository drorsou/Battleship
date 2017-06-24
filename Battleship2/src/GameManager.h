#pragma once
#include "IBattleshipGameAlgo.h"
#include "ScoresController.h"
#include <memory>
#include "Board.h"
#include <mutex>


class GameManager
{
	Board board;
	IBattleshipGameAlgo* playerA;
	IBattleshipGameAlgo* playerB;
	int playerAIndex;
	int playerBIndex;
	int winner;
	int pointsA;
	int pointsB;


public:
	GameManager() {};
	GameManager(Board board_, IBattleshipGameAlgo* playerA_, IBattleshipGameAlgo* playerB_,
		int playerAIndex_, int playerBIndex_) :
		board(board_), playerA(playerA_), playerB(playerB_), playerAIndex(playerAIndex_), playerBIndex(playerBIndex_) { };

	int getPlayerAIndex() const { return playerAIndex; };
	int getPlayerBIndex() const { return playerBIndex; };


	/* Post: Run the game with players 'playerA', 'playerB' on board 'board'
	 *		The results will be held in the variables 'winner', 'pointsA', 'pointsB'
	 */
	void play();
};
