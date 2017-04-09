#pragma once
#include "Player.h"

class Board {
	char board[10][10];
	Player player1;
	Player player2;
	int current_player_turn;


public:
	Board(Player p1, Player p2);
};