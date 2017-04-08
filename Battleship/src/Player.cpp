#include "Player.h"

Player::Player(Ship ships[SHIPS_PER_PLAYER]) {

};

void Player::setBoard(const char** board, int numRows, int numCols) {

};

pair<int, int> Player::attack() {
	return make_pair(0,1);
};

void Player::notifyOnAttackResult(int player, int row, int col, AttackResult result) {
	
};