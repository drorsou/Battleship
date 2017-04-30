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


void attackFromFileAlgo::setAttackFromFile(Attack attack) {
	attack_from_file = attack;
}


void attackFromFileAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result) {
	row--;
	col--;
	if (result == AttackResult::Miss)
		player_board[row][col] = MISS_SYM;
	else if (result == AttackResult::Hit)
		player_board[row][col] = HIT_SYM;
	else if (result == AttackResult::Sink)
		player_board[row][col] = HIT_SYM;
}


void attackFromFileAlgo::setBoard(int player, const char** board, int numRows, int numCols) {
	player_number = player;
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
}