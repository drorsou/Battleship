#include "NaiveAlgoPlayer.h"



NaiveAlgoPlayer::NaiveAlgoPlayer()
{
}

void NaiveAlgoPlayer::setBoard(const char** board, int numRows, int numCols) {
	for (int i = 0; i < numRows; i++)
		for (int j = 0; j < numCols; j++) {
			player_board[i][j] = board[i][j];
			if (player_number == 0) {
				if (board[i][j] == BBOAT || board[i][j] == BCRUISER || board[i][j] == BSUBMARINE || board[i][j] == BDESTROYER)
					player_board[i][j] = BLANK;
			}
			if (player_number == 1) {
				if (board[i][j] == ABOAT || board[i][j] == ACRUISER || board[i][j] == ASUBMARINE || board[i][j] == ADESTROYER)
					player_board[i][j] = BLANK;
			}
		}
};

pair<int, int> NaiveAlgoPlayer::attack() {
	if (this->attack_naive.hasAttacks())
		return attack_naive.getNextAttack();
	else
		return make_pair(-1, -1);
};

void NaiveAlgoPlayer::notifyOnAttackResult(int player, int row, int col, AttackResult result) {
	row--;
	col--;
	if (result == AttackResult::Miss)
		player_board[row][col] = MISS_SYM;
	else if (result == AttackResult::Hit)
		player_board[row][col] = HIT_SYM;
	else if (result == AttackResult::Sink)
		player_board[row][col] = HIT_SYM;
};

NaiveAlgoPlayer::~NaiveAlgoPlayer()
{
}
//curr attack indice are on the previous attack indices
bool NaiveAlgoPlayer::hasNextAttack()
{
	if (NaiveAlgoPlayer::getNextAttack().first != -1)
		return true;
	return false;
}
//PRE: assums cords are valid.
//POST: returns true if the attack is logic(the player doesnt attack itself of surounding cells are sink).
bool NaiveAlgoPlayer::checkLogicalAttack(int row, int col)
{
	AttackResult prevResult = AttackResult::Sink;
	NaiveAlgoPlayer::notifyOnAttackResult(player_number, row, col, prevResult);
	if (NaiveAlgoPlayer::isShipOfPlayer(row, col))
		return false;
	//check the serrounding of the cell is not a sink
	if ((col-1>=0 && player_board[row][col-1] == SINK_SYM)|| (col + 1<10 && player_board[row][col + 1]== SINK_SYM)
		||( row + 1<10 && player_board[row+1][col] == SINK_SYM)||(row-1>=0 && player_board[row -1][col] == SINK_SYM))
	{
		return false;
	}
	return true;

		
}

//checks if it is the ship of the player
bool NaiveAlgoPlayer::isShipOfPlayer (int row, int col)
{
	for (Ship ship: playerShips)
	{
		if (ship.isInThisShip(row, col))
			return true;
	}
	return false;
}

//calc next attack and update the currRowAttack and currColAttack
std::pair<int, int> NaiveAlgoPlayer::getNextAttack()
{
	std::pair<int, int> nextAttack=NaiveAlgoPlayer::calcNextAttack();
	currRowAttack = nextAttack.first;
	currColAttack = nextAttack.second;
	return nextAttack;
	
}

//calc next attack without update the currRowAttack and currColAttack
std::pair<int, int> NaiveAlgoPlayer::calcNextAttack()
{
	bool foundAttack = false;
	int col, row;
	if (currColAttack < 9)
	{
		col = currColAttack + 1;
		row = currRowAttack;
	}
	else if (currRowAttack<9) {
		col = 0;
		row = currRowAttack + 1;
	}
	else {
		return  std::pair<int, int>(-1, -1);
	}

	while (col < 9 && row < 9)
	{
		if (NaiveAlgoPlayer::checkLogicalAttack(row, col))
		{
			foundAttack = true;
			break;
		}
		if (col < 9)
			col++;
		else {
			row++;
			col = 0;
		}
	}

	if (foundAttack)
		return  std::pair<int, int>(row, col);
	else
		return  std::pair<int, int>(-1, -1);
}
