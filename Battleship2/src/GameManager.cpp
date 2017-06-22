#include "GameManager.h"
#include <iostream>
#include <mutex>

void GameManager::play()
{
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

	winner = 0;
	pointsA = playerAIndex;
	pointsB = playerBIndex;


	static std::mutex mutex;
	std::unique_lock<std::mutex> lock(mutex, std::defer_lock);
	lock.lock(); //while (lock.try_lock() == false) {}
	//std::cout << "Board " << board<< " - Player " << playerAIndex << " vs. Player " << playerBIndex << std::endl;
	ScoresController::updateScores(playerAIndex, playerBIndex, winner, pointsA, pointsB);
	lock.unlock();
}
