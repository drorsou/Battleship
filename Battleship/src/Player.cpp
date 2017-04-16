#include "Player.h"



Player::Player(int player_number) : player_number(player_number) {
	
}


void Player::setBoard(const char** board, int numRows, int numCols) {
	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numCols; j++)
			player_board[i][j] = board[i][j];
	}

	if (player_number == 0)
	{
		// Change the player_board, ships accordingly // TODO
	}
	else if (player_number == 1)
	{
		// Change the player_board, ships accordingly // TODO
	}
};

pair<int, int> Player::attack() {
	if (this->attack_from_file.hasAttacks() == true)
		return attack_from_file.getNextAttack();
	else
		return make_pair(-1, -1);
};

void Player::notifyOnAttackResult(int player, int row, int col, AttackResult result) {
	row--;
	col--;
	if (result == AttackResult::Miss)
		player_board[row][col] = MISS_SYM;
	else if (result == AttackResult::Hit)
		player_board[row][col] = HIT_SYM;
	else if (result == AttackResult::Sink)
		player_board[row][col] = HIT_SYM;
};

void Player::setAttackFromFile(Attack attack) {
	attack_from_file = attack;
}
