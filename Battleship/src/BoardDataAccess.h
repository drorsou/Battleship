#pragma once
#include "IBattleshipGameAlgo.h"
#include "boardArray.h"
class BoardDataAccess : public BoardData
{
	const boardArray * board;
	int player;
public:
	char charAt(Coordinate c) const override;

	BoardDataAccess(const boardArray* b, int player);
	~BoardDataAccess() { board = nullptr; }
};

