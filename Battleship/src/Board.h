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
	char board[BOARD_SIZE][BOARD_SIZE];
	int current_player_turn;
	IBattleshipGameAlgo * playerA;
	IBattleshipGameAlgo * playerB;
	Ship shipsA[SHIPS_PER_PLAYER];
	Ship shipsB[SHIPS_PER_PLAYER];
	int totalShipsAScore;
	int totalShipsBScore;
	int scoreA;
	int scoreB;

	static void printLine();
	static void gotoxy(int, int);

	Type shipType(int row, int col);

	int coordColor(int row, int col) {
		return (this->board[row][col] >= 'A' && this->board[row][col] <= 'Z') ? 1 : 2;
	}

	/*
	Pre: None
	Post: returns true iff the board is legal.
	*/
	bool checkBoard();

	
public:

	Board(char b[BOARD_SIZE][BOARD_SIZE], IBattleshipGameAlgo * playerA, IBattleshipGameAlgo * playerB);
	bool checkCoord(bool*, bool*, bool [BOARD_SIZE][BOARD_SIZE], int, int, char);
	

	bool Board::hasPlayerWon(int player);
	bool hitShip(int row, int col, char type);

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

	void notifyResult(int row, int col, AttackResult result);

	char getCoordValue(int row, int col) { return board[row][col]; }
	void setCoordValue(int row, int col, char val) { board[row][col] = val; }
	int getScore(int color) { return color == 0? scoreA: scoreB; }	
	std::pair<int, int> attackPlayer(int color) { return color == 0 ? playerA->attack() : playerB->attack(); }	
	int getTurn() { return current_player_turn; }
	void changeTurn() { current_player_turn = 1 - current_player_turn ; }
	bool checkTarget(char target);
};