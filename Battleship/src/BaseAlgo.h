#pragma once
#include "IBattleshipGameAlgo.h"
#include "Ship.h"
#include "Board.h"
enum tileMarks { DontAttack, Attack, Attacked };

class BaseAlgo : public IBattleshipGameAlgo
{	
protected:
	int player_number;
	char ** board;
	int numOfRows;
	int numOfCols;
	tileMarks ** shadow_board;

	void createBoards();

	BaseAlgo() {}
	~BaseAlgo();
	
public:


	void setBoard(int player, const char** board, int numRows, int numCols) override;
	bool init(const std::string& path) override;
};
