#pragma once
#include "Player.h"
#include <windows.h>

#define BOARD_SIZE 10
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

class Board {
	char board[BOARD_SIZE][BOARD_SIZE];
	Player player1;
	Player player2;
	int current_player_turn;
	
	/*
		privte methods
	*/
	static void printLine();

public:
	Board(Player p1, Player p2);
	static void printBoard(const Board &);
};