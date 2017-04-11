#pragma once
#include "Player.h"
#include <iostream>
#include <windows.h>

#define BOARD_SIZE 10
#define COLOR_RED     0x0004
#define COLOR_LIGHT_RED 0x000C
#define COLOR_GREEN 0x0002
#define COLOR_LIGHT_GREEN 0x000A
#define COLOR_PURPLE 0x0005
#define COLOR_LIGHT_PURPLE 0x000D
#define COLOR_YELLOW 0x0006
#define COLOR_LIGHT_YELLOW 0x000E
#define COLOR_AQUA 0x0003
#define COLOR_LIGHT_AQUA 0x000B
#define COLOR_WHITE 0x0007

#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

class Board {
	
	/*
		privte methods
	*/
	static void printLine();
	static void gotoxy(int, int);

public:
	char board[BOARD_SIZE][BOARD_SIZE];
	Player player1;
	Player player2;
	int current_player_turn;

	Board();
	Board(char b[BOARD_SIZE][BOARD_SIZE], Attack attack1, Attack attack2);
	static void printBoard(const Board &);
	static void updateBoard(const Board &, int, int);
};