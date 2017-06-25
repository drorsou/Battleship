#pragma once
#include <vector>
#include "boardArray.h"
#include "Ship.h"
#include <array>
#include "TestingBoard.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <windows.h>
#include "BoardDataAccess.h"

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
	static const int SHIPS_PER_PLAYER = 5;
public:
	boardArray board;
private:
	int current_player_turn;
	IBattleshipGameAlgo * playerA;
	IBattleshipGameAlgo * playerB;
	BoardDataAccess AAccess;
	BoardDataAccess BAccess;
	std::array<Ship, SHIPS_PER_PLAYER> shipsA;
	std::array<Ship, SHIPS_PER_PLAYER> shipsB;
	int totalShipsAScore;
	int totalShipsBScore;
	int scoreA;
	int scoreB;		
	

	enum TileStatus { Checked, UnChecked, Occupied };
	enum ShipDirection { Right, Down, Forward };

	Type shipType(Coordinate c) const;
	
	/*
		Pre: gets a non-empty valid coordinate.
		Post: returns the color of the piece on the board.
	*/
	int coordColor(Coordinate c) const	{
		return Ship::checkColor(this->board.charAt(c));
	}

	static bool printSizeOrShapeError(int player, bool * arr);

	static bool printNumOfShipsError(int player, int count);

	static Coordinate generateCoord(const Coordinate c, ShipDirection dir, int val);

	bool checkDirection(int * len, bool * sizeOShape, bool * adjacent, TestingBoard<TileStatus>& temp, Coordinate c, char t, ShipDirection dir) const;

	/*
	Pre: gets a pointer to the size or shape flag,
	to the adjacent flag,
	The shadow board (which it changes),
	a non-empty valid coordinate and the char in it.
	Post: returns true if this coord makes a valid ship,
	if the size or shape aren't okay, update the flag,
	and if there is an adajacent ship, update the flag.
	*/
	bool checkCoord(bool*, bool*, TestingBoard<TileStatus>&, Coordinate c, char) const;

	/*
		Pre: None
		Post: returns true iff the board is legal.
	*/
	bool checkBoard();

	bool checkRowForAShip(Coordinate c, char type) const
	{
		return (c.col > 1 && this->board.charAt(Coordinate(c.row, c.col - 1, c.depth)) == type) || (c.col < this->board.cols() && this->board.charAt(Coordinate(c.row, c.col + 1, c.depth)) == type);
	}

	bool checkColumnForAShip(Coordinate c, char type) const
	{
		return (c.row > 1 && this->board.charAt(Coordinate(c.row - 1, c.col, c.depth)) == type) || (c.col < this->board.rows() && this->board.charAt(Coordinate(c.row + 1, c.col, c.depth)) == type);
	}

	bool checkDepthForAShip(Coordinate c, char type) const
	{
		return (c.depth > 1 && this->board.charAt(Coordinate(c.row, c.col, c.depth - 1)) == type) || (c.col < this->board.depth() && this->board.charAt(Coordinate(c.row, c.col, c.depth + 1)) == type);
	}

	/*
	 * Pre: gets the coordinate in question, the direction we want to check, and the type we have in c.
	 * Post: returns true if there is an adjacent ship of a different type.
	 */
	bool checkAdjacent(Coordinate c, ShipDirection dir, char type) const;
	

	
	bool checkShapeAtCoord(Coordinate c, char t) const;
	static std::vector<std::string> split(const std::string &s, char delim);
	static void removeCharFromString(std::string &str, char charToRemove);
	static bool Board::is_number(const std::string &s);
	static std::tuple<int, int, int> Board::ParseBoardShape(const std::string& line);
	ShipDirection getShipDirectionAt(const Coordinate c) const;
	static std::pair<int, int> makePairByLength(const Type t, int pos);
	void fillDimensionsOfShip(Coordinate c, Type t, std::pair<int,int> & vert, std::pair<int, int> & horz, std::pair<int, int> & depth) const;
public:	
//	void printBoard() { this->board.printBoard(); }

	Board()
		: playerA(nullptr),
		  playerB(nullptr)		  
	{
	}
	Board(string path);

	Board(const Board& other);

	Board(Board&& other) noexcept;

	Board& operator=(const Board& other);

	Board& operator=(Board&& other) noexcept;

	~Board();
	
	
	
	/*
		Pre: None
		Post: returns true if this player won.
	*/
	bool Board::hasPlayerWon(int player) const;

	/*
		Pre: gets a non-empty valid coordinate and ship char in it.
		Post: updates the board after shooting the coordinate,
				if a ship was sunk, update the score,
				return true iff a ship was sunk.
	*/
	bool hitShip(Coordinate c, char type);
		
	/*
		Notify both players.
	*/
	void notifyResult(int player, Coordinate c, AttackResult result);

	// Inline functions:

	char getCoordValue(Coordinate c) const { return board.charAt(c); }
	void setCoordValue(Coordinate c, Ship::Symbol val) { board.setCharAt(c, val); }
	int getScore(int color) const { return color == 0? scoreA: scoreB; }	
	Coordinate attackPlayer(int color) { return color == 0 ? playerA->attack() : playerB->attack(); }	
	int getTurn() const { return current_player_turn; }
	void changeTurn() { current_player_turn = 1 - current_player_turn ; }
	void setPlayer(int color, IBattleshipGameAlgo * player);

	/*
		Pre: gets a valid tile value.
		Post: return true if it was a self-hit.
	*/
	bool checkTarget(char target) const;

	bool parseBoard(std::string& path);

	static bool checkChar(char c);

	
};