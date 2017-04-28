#pragma once
#include "attackFromFileAlgo.h"
#include "NaiveAlgoPlayer.h"
#include "FileReader.h"
#include <iostream>
#include <windows.h>

#define BOARD_SIZE 10
#define SHIPS_PER_PLAYER 5
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
	char** board;
	int current_player_turn;
	IBattleshipGameAlgo * playerA;
	IBattleshipGameAlgo * playerB;
	Ship shipsA[SHIPS_PER_PLAYER];
	Ship shipsB[SHIPS_PER_PLAYER];
	int totalShipsAScore;
	int totalShipsBScore;
	int scoreA;
	int scoreB;
	POINT origin;

	static void printLine();

	void gotoxy(int, int);

	Type shipType(int row, int col);
	/*
		Pre: None
		Post: sets the current cursor position as the origin.
	*/
	void getCursorXY();

	/*
		Pre: gets a non-empty valid coordinate.
		Post: returns the color of the piece on the board.
	*/
	int coordColor(int row, int col) {
		return (this->board[row][col] >= 'A' && this->board[row][col] <= 'Z') ? 1 : 2;
	}

	/*
		Pre: None
		Post: returns true iff the board is legal.
	*/
	bool checkBoard();

	/*
	Pre: gets a pointer to the size or shape flag, 
			to the adjacent flag,
			The shadow board (which it changes), 
			a non-empty valid coordinate and the char in it.
	Post: returns true if this coord makes a valid ship,
			if the size or shape aren't okay, update the flag,
			and if there is an adajacent ship, update the flag.
	*/
	bool checkCoord(bool*, bool*, bool[BOARD_SIZE][BOARD_SIZE], int, int, char);

public:
	
	Board(string path, IBattleshipGameAlgo * playerA, IBattleshipGameAlgo * playerB);

	// Set the board size
	enum Size { ROWS = 10, COLS = 10};
	
	
	/*
		Pre: None
		Post: returns true if this player won.
	*/
	bool Board::hasPlayerWon(int player);

	/*
		Pre: gets a non-empty valid coordinate and ship char in it.
		Post: updates the board after shooting the coordinate,
				if a ship was sunk, update the score,
				return true iff a ship was sunk.
	*/
	bool hitShip(int row, int col, char type);

	/*
		Pre: Gets a legal board.
		Post: prints the board in the console.
	*/
	void printBoard();
	/*
		Pre: Gets a legal board, and the attack coordinate.
			Assuming the board is already printed in the console.
		Post: updates the board to show the attack according to the appropriate attack result.
	*/
	void updateBoard(int, int);

	/*
		Notify both players.
	*/
	void notifyResult(int row, int col, AttackResult result);

	char getCoordValue(int row, int col) { return board[row - 1][col - 1]; }
	void setCoordValue(int row, int col, char val) { board[row - 1][col - 1] = val; }
	int getScore(int color) { return color == 0? scoreA: scoreB; }	
	std::pair<int, int> attackPlayer(int color) { return color == 0 ? playerA->attack() : playerB->attack(); }	
	int getTurn() { return current_player_turn; }
	void changeTurn() { current_player_turn = 1 - current_player_turn ; }

	/*
		Pre: gets a valid tile value.
		Post: return true if it was a self-hit.
	*/
	bool checkTarget(char target);





	bool parseBoard(string path);

	bool checkChar(char c);
};