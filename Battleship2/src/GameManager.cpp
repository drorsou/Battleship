#include "GameManager.h"
#include <iostream>

void GameManager::play()
{
	if (playerAIndex < playerBIndex)
	{
		// Lock playerA mutex
		// Lock playerB mutex
	}
	else
	{
		// Lock playerB mutex
		// Lock playerA mutex
	}


	/*
	playerA->setPlayer(0);
	playerB->setPlayer(1);

	// Set boards for both players

	bool game_in_progress = true;
	AttackResult result;
	int currentPlayer;

	while (game_in_progress)
	{
		//currentPlayer = board.getTurn();

	}
	*/

	std::cout << "Board " << board<< " - Player " << playerAIndex << " vs. Player " << playerBIndex << std::endl;


	winner = 0;
	pointsA = 0;
	pointsB = 0;

	Scores::updateScores(playerAIndex, playerBIndex, winner, pointsA, pointsB);
	
	// Free mutex for both players

	Scores::activeThreads -= 1;
}
