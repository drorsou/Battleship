#include "Board.h"

Board::Board() {
	
}


Board::Board(char b[BOARD_SIZE][BOARD_SIZE], Attack attack1, Attack attack2) : current_player_turn(1) {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			board[i][j] = b[i][j];
		}
	}

	// Need to create here the ships lists for each player
	
	player1 = Player();
	player1.setAttackFromFile(attack1);
	player2 = Player();
	player2.setAttackFromFile(attack2);
}

void Board::gotoxy(int row, int col) {
	COORD coord;
	coord.X = col;
	coord.Y = row;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Board::printBoard(const Board & br) {
	int i, j;	
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	/*
		Printing the first line with the column numbers.
		First box is empty
	*/	
	for (j = 0; j < BOARD_SIZE; j++)
	{
		printf((j == 0 ? "  | %d |" : (j < 9 ? " %d |" : " %d|")), j + 1);
	}
	printf("\n");
	Board::printLine();
	/*
		Printing the rest of the board.
	*/
	for (j = 0; j <= BOARD_SIZE-1; j++)
	{
		printf((j < 9 ? "%d " : "%d"), j + 1);
		for (i = 0; i <= BOARD_SIZE-1; i++){
			cout << "| ";
			/* 
				Color text according to player and ship type 
			*/
			switch (br.board[j][i])
			{
			case ABOAT:
				SetConsoleTextAttribute(hConsole, COLOR_AQUA);				
				break;
			case BBOAT:
				SetConsoleTextAttribute(hConsole, COLOR_LIGHT_AQUA);
				break;
			case ACRUISER:
				SetConsoleTextAttribute(hConsole, COLOR_GREEN);				
				break;
			case BCRUISER:
				SetConsoleTextAttribute(hConsole, COLOR_LIGHT_GREEN);				
				break;
			case ASUBMARINE:
				SetConsoleTextAttribute(hConsole, COLOR_YELLOW);
				break;
			case BSUBMARINE:
				SetConsoleTextAttribute(hConsole, COLOR_LIGHT_YELLOW);
				break;
			case ADESTROYER:
				SetConsoleTextAttribute(hConsole, COLOR_PURPLE);
				break;
			case BDESTROYER:
				SetConsoleTextAttribute(hConsole, COLOR_LIGHT_PURPLE);
				break;
			case HIT_SYM:
				SetConsoleTextAttribute(hConsole, COLOR_LIGHT_RED);
				break;
			case MISS_SYM:
				SetConsoleTextAttribute(hConsole, COLOR_RED);
				break;
			default:
				// Not writing anything!
				break;
			}
			cout << br.board[j][i] << " ";
			// resetting the color
			SetConsoleTextAttribute(hConsole, COLOR_WHITE);			
		}
		cout << "|\n";
		Board::printLine();
	}	
}
void Board::updateBoard(const Board & br, int row, int col) {
	row--;
	col--;
	Board::gotoxy(2 * row + 2, 4 * col + 4);
	cout << "\b";
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (br.board[row][col] == BLANK || br.board[row][col] == MISS_SYM)
	{
		SetConsoleTextAttribute(hConsole, COLOR_RED);
		cout << " " << MISS_SYM << " ";
		// resetting the color
		SetConsoleTextAttribute(hConsole, COLOR_WHITE);
	}
	else
	{
		SetConsoleTextAttribute(hConsole, COLOR_LIGHT_RED);
		cout << " "<< HIT_SYM << " ";
		// resetting the color
		SetConsoleTextAttribute(hConsole, COLOR_WHITE);
	}
	Board::gotoxy(22, 0); // moving cursor back to the bottom.
}
void Board::printLine(){
	int i;
	printf("  |");
	for (i = 1; i < BOARD_SIZE * 4; i++){
		printf("-");
	}
	printf("|\n");
}