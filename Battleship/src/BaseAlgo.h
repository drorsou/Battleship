#pragma once
#include "IBattleshipGameAlgo.h"
#include "Ship.h"
#include "Board.h"
enum tileMarks { DontAttack, Attack, Attacked };

class BaseAlgo : public IBattleshipGameAlgo
{
	int player_number;
	char ** board;
	int numOfRows;
	int numOfCols;
	tileMarks ** shadow_board;
	
	friend class IntelligentAlgo;
	friend class NaiveAlgo;
protected:

	BaseAlgo() {}
	~BaseAlgo();
public:


	void setBoard(int player, const char** board, int numRows, int numCols) override;
	bool init(const std::string& path) override;
};
