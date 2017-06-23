#pragma once
#include "IBattleshipGameAlgo.h"
#include "boardArray.h"
#include "Board.h"

class BoardDataAccess : public BoardData
{
	const boardArray * board;
	int player;
public:
	char charAt(Coordinate c) const override
	{		
		if (c.col < 1 || c.col > _cols || c.row < 1 || c.row > _rows || c.depth < 1 || c.depth > _depth)
			return static_cast<char>(Ship::Symbol::Blank);
		char result = board->charAt(c);
		if(Ship::checkColor(result) == player)
			return result;
		else
			return static_cast<char>(Ship::Symbol::Blank);
	}
	BoardDataAccess(const Board& b, int player) :
		BoardData(),
		board(&b.board),
		player(player)		
	{
		_rows = b.board.rows();
		_cols = b.board.cols();
		_depth = b.board.depth();
	}
	~BoardDataAccess() { board = nullptr; }
};

