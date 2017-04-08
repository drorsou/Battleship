#pragma once
#include <string>
#include "Attack.h"
#include "Board.h"

using namespace std;

class Main {

public:
	Main();
	int parseAttack(string path);
	int parseBoard(string path);
};