#include "attackFromFileAlgo.h"


bool attackFromFileAlgo::init(const std::string& path) {
	attack_from_file = Attack(path, player_number);
	return attack_from_file.init;
}


pair<int, int> attackFromFileAlgo::attack() {
	if (this->attack_from_file.hasAttacks() == true)
		return attack_from_file.getNextAttack();
	else
		return make_pair(-1, -1);
};


void attackFromFileAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result) {
	row--;
	col--;
	if (result == AttackResult::Miss)
		board[row][col] = static_cast<char>(Ship::Symbol::MISS);
	else if (result == AttackResult::Hit)
		board[row][col] = static_cast<char>(Ship::Symbol::Hit);
	else if (result == AttackResult::Sink)
		board[row][col] = static_cast<char>(Ship::Symbol::Hit);
}


void attackFromFileAlgo::setBoard(int player, const char** board, int numRows, int numCols) {
	this->numOfRows = numRows;
	this->numOfCols = numCols;
	this->player_number = player;
	this->board = new char*[this->numOfRows];
	for (int row = 0; row < numRows; row++)
	{
		this->board[row] = new char[this->numOfCols];
		for (int col = 0; col < numCols; col++)
		{
			this->board[row][col] = board[row][col];
		}
	}
		
}

IBattleshipGameAlgo* GetAlgorithm()
{
	return new attackFromFileAlgo();
}