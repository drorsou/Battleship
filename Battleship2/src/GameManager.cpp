#include "GameManager.h"


void GameManager::play()
{
	bool toPrint = false;
	if (board.board.rows() == 10 && board.board.cols() == 10 && board.board.depth() == 1)
		toPrint = true;
	// Initialize
	board.setPlayer(0, playerA);
	board.setPlayer(1, playerB);

	bool gameInProgress = true;
	AttackResult result;
	Coordinate coord(0, 0, 0);
	int i = 0;
	// Run the game

//	board.printBoard();
	while (gameInProgress)
	{
		bool noAttack = false;

		// Ask a player for attack coordinate and check if there was no error
		coord = board.attackPlayer(board.getTurn());

//		std::cout << "Player " << (board.getTurn() == 0 ? "A" : "B") << " moved (" << coord.row << ", " << coord.col << ", " << coord.depth << ")" << std::endl;
		
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
	
	std::unique_lock<std::mutex> lockScores(ScoresController::mutexScores, std::defer_lock);
	lockScores.lock();
	ScoresController::updateScores(playerAIndex, playerBIndex, winner, pointsA, pointsB);
	lockScores.unlock();
}