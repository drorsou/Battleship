#pragma once
#include "Player.cpp"

#define BOARD_SIZE 10

class Board {
	char board[BOARD_SIZE][BOARD_SIZE];
	Player player1;
	Player player2;
	int current_player_turn;


public:
	Board(Player p1, Player p2);
};