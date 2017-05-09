#pragma once
#include "IBattleshipGameAlgo.h"
#include "Ship.h"
#include "Board.h"
enum tileMarks { DontAttack, Attack, Attacked };
class NaiveAlgo : public IBattleshipGameAlgo
{
	int player_number;
	char ** board;
	int numOfRows;
	int numOfCols;
	tileMarks ** shadow_board;
	std::pair<int, int> nextAttack;

	void addOne();

public:


	explicit NaiveAlgo(int player_number, int num_of_rows, int num_of_cols);
	~NaiveAlgo();
};