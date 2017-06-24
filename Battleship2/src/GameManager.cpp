#include "GameManager.h"


void GameManager::play()
{
	// Initialize
	board.setPlayer(0, playerA);
	board.setPlayer(1, playerB);

	bool gameInProgress = true;
	AttackResult result;
	Coordinate coord(0, 0, 0);

	// Run the game
	while (gameInProgress)
	{
		bool noAttack = false;
		coord = board.attackPlayer(board.getTurn());
		if (coord.col == -1 && coord.depth == -1 && coord.row == -1)
		{
			if (noAttack == true)
			{
				// Both players can't attack for some reason
				gameInProgress = false;
			}
			board.changeTurn();
			noAttack = true;
		}
		else
		{
			int currentPlayer = board.getTurn();
			char val = board.getCoordValue(coord);
			if (val != static_cast<char>(Ship::Symbol::Blank) && val != static_cast<char>(Ship::Symbol::MISS) && val != static_cast<char>(Ship::Symbol::Hit))
			{
				result  = board.hitShip(coord, val) ? AttackResult::Sink : AttackResult::Hit;
				if (board.checkTarget(val) == true)
					board.changeTurn();

				if (board.hasPlayerWon(0) || board.hasPlayerWon(1))
					gameInProgress = false;
			}
			else
			{
				board.changeTurn();
				if (val == static_cast<char>(Ship::Symbol::Hit))
					result = AttackResult::Hit;
				else
				{
					board.setCoordValue(coord, Ship::Symbol::MISS);
					result = AttackResult::Miss;
				}
			}

			// Notify players on results
			board.notifyResult(currentPlayer, coord, result);
		}
	}
	
	// Save the game results
	if (board.hasPlayerWon(0))
		winner = 0;
	else
		winner = 1;
	pointsA = board.getScore(0);
	pointsB = board.getScore(1);
	
	/*
	winner = 0;
	pointsA = 5;
	pointsB = 10;
	*/
	
	//std::unique_lock<std::mutex> lock(ScoresController::mutexScores, std::defer_lock);
	//lock.lock();
	std::unique_lock<std::mutex> lockScores(ScoresController::mutexScores, std::defer_lock);
	lockScores.lock();
	//ScoresController::cvScores.wait(lockScores);
	
	//while (lock.try_lock() == false) {}
	//std::cout << "Board " << board<< " - Player " << playerAIndex << " vs. Player " << playerBIndex << std::endl;
	ScoresController::updateScores(playerAIndex, playerBIndex, winner, pointsA, pointsB);
	//std::cout << "Updating scores" << std::endl;
	lockScores.unlock();
	//lockScores.unlock();
	//ScoresController::cvScores.notify_one();
}