#include "Player.h"

Player::Player() {
	
}

Player::Player(Ship ships[SHIPS_PER_PLAYER]){
	for (int i = 0; i < SHIPS_PER_PLAYER; i++)
		this->ships[i] = ships[i];
};

void Player::setBoard(const char** board, int numRows, int numCols) {
	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numCols; j++)
			player_board[i][j] = board[i][j];
	}
};

pair<int, int> Player::attack() {
	return attack_from_file.getNextAttack();
};

void Player::notifyOnAttackResult(int player, int row, int col, AttackResult result) {
	if (result == AttackResult::Miss)
		player_board[row][col] = MISS_SYM;
	else if (result == AttackResult::Hit)
		player_board[row][col] = HIT_SYM;
	else if (result == AttackResult::Sink)
		player_board[row][col] = HIT_SYM;
};