#include "Board.h"

Board::Board(Player p1, Player p2) : player1(p1), player2(p2) {
	
}
void Board::printBoard(const Board & br) {
	int i, j;
	Board::printLine();
	for (j = BOARD_SIZE - 1; j >= 0; j--)
	{
		printf((j < 9 ? " %d" : "%d"), j + 1);
		for (i = 0; i < BOARD_SIZE; i++){
			printf("| %c ", br.board[i][j]);
		}
		printf("|\n");
		Board::printLine();
	}
	printf("   ");
	for (j = 0; j < BOARD_SIZE; j++){
		printf(" %c  ", j);
	}
	printf("\n");
}
void Board::printLine(){
	int i;
	printf("  |");
	for (i = 1; i < BOARD_SIZE * 4; i++){
		printf("-");
	}
	printf("|\n");
}