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
	short int checkCoord(bool [BOARD_SIZE][BOARD_SIZE], int, int, char);
	/*
		Pre: None
		Post: returns true iff the board is legal.
	*/
	bool checkBoard();

	int coordColor(int row,int col){
		return (this->board[row][col] >= 'A' && this->board[row][col] <= 'Z') ? 1 : 2;
	}

	Type shipType(int row, int col) {
		Type t;
		switch (this->board[row][col])
		{
		case ABOAT: case BBOAT:
			t = Boat;
			break;
		case ACRUISER: case BCRUISER:
			t = Cruiser;
			break;
		case ASUBMARINE: case BSUBMARINE:
			t = Submarine;
			break;
		case ADESTROYER: case BDESTROYER:
			t = Destroyer;
			break;
		}
		return t;
	}
	/*
		Pre: Gets a legal board.
		Post: prints the board in the console.
	*/
	static void printBoard(const Board &);
	/*
		Pre: Gets a legal board, and the attack coordinate.
			Assuming the board is already printed in the console.
		Post: updates the board to show the attack according to the appropriate attack result.
	*/
	static void updateBoard(const Board &, int, int);

};