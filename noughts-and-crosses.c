#ifdef _WIN32
	#include <conio.h>
	#include <windows.h>
	#define CLEAR() system("cls")
	#define COORD coord = {0, 0};
	#define GOTOXY(x,y) { coord.X = x; coord.Y = y; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); }
#else
	#define GOTOXY(x,y) printf("%c[%d;%df",0x1B,y,x)
	#define CLEAR() system("clear")
#endif

#include <stdio.h>
#include <stdlib.h>

void leave_mark(char mk, int pos) {
	/* That's gonna leave a mark */
	int x = 27, y = 10;
	int i = pos;

	if(pos > 3)
		while(i > 3) {
			y += 3;
			i -= 3;
		}

	if(pos % 3 == 0)
		x += 16;
	else {
		pos %= 3;
		pos--;
		while(pos) {
			x += 8;
			pos--;
		}
	}

	GOTOXY(x, y);
	printf("%c", mk);
}

int board[10] = {2,2,2,2,2,2,2,2,2,2};

void draw_board() {
	/* Make the board */
	for(int i = 9; i < 17; i++) {
		GOTOXY(31, i);
		printf("|       |");
	}

	GOTOXY(24, 11);
	printf("=======================");
	GOTOXY(24, 14);
	printf("=======================");

	for(int i = 1; i < 10; i++) {
		if(board[i] == 3)
			leave_mark('X', i);
		else if(board[i] == 5)
			leave_mark('O', i);
	}
}

int turn = 1;

void check_draw() {
	if(turn > 9) {
		GOTOXY(30,20);
		printf("Game Draw");
		// getch();
		exit(0);
	}
	return;
}

int posswin(int player) {
	/* If 'player' == 1, mark X
	** if 'player' == 0, mark O */
	int pos, value;

	if(player == 1)
		value = 18;
	else
		value = 50;

	int i = 1;
	while(i <= 9) {
		//Row check
		if(board[i] * board[i+1] * board[i+2] == value) {
			if(board[i] == 2)
				return i;
			if(board[i+1] == 2)
				return i+1;
			if(board[i+2] == 2)
				return i+2;
		}
		i += 3;
	}

	i = 1;
	while(i <= 3) {
		//Column check
		if(board[i] * board[i+3] * board[i+6] == value) {
			if(board[i] == 2)
				return i;
			if(board[i+3] == 2)
				return i+3;
			if(board[i+6] == 2)
				return i+6;
		}
		i++;
	}

	if(board[1] * board[5] * board[9] == value) {
		//Main diagonal check
		if(board[1] == 2)
			return 1;
		if(board[5] == 2)
			return 5;
		if(board[9] == 2)
			return 9;
	}

	if(board[3] * board[5] * board[7] == value) {
		//Antidiagonal check
		if(board[3] == 2)
			return 3;
		if(board[5] == 2)
			return 5;
		if(board[7] == 2)
			return 7;
	}

	return 0;
}

void go(int n) {
	if(turn % 2)
		board[n] = 3;
	else
		board[n] = 5;
	turn++;
}

int player, comp;

void start_game();

int make2() {
	if(board[2] == 2)
		return 2;
	if(board[4] == 2)
		return 4;
	if(board[5] == 2)
		return 5;
	if(board[6] == 2)
		return 6;
	if(board[8] == 2)
		return 8;

	return 0;
}

int make4() {
	if(board[1] == 2)
		return 1;
	if(board[3] == 2)
		return 3;
	if(board[7] == 2)
		return 7;
	if(board[9] == 2)
		return 9;

	return 0;
}

void player_first() {
	int pos;

	check_draw();
	draw_board();
	GOTOXY(30, 18);
	printf("Your Turn: ");
	scanf("%d", &pos);

	if(board[pos] != 2)
		player_first();

	if(pos == posswin(player)) {
		go(pos);
		draw_board();
		GOTOXY(30, 20);

		printf("Player Wins");
		// getch();

		return;
	}

	go(pos);
	draw_board();
	start_game();
}

int flag = 0;

void start_game() {
	/* If 'player' == 1, mark X
	** if 'player' == 0, mark O */
	if(posswin(comp)) {
		go(posswin(comp));
		flag = 1;
	} else if(posswin(player))
		go(posswin(player));
	else if(make2())
		go(make2());
	else
		go(make4());
	draw_board();

	if(flag) {
		GOTOXY(30, 20);
		printf("Computer Wins");
		// getch();
	} else
		player_first();
}

int main() {
	int menu;

	menu:
	// CLEAR();
	system("clear");
	/*------------------------------- Main Menu -------------------------------*/
	printf("\n\t\t\t\t  MENU");
	printf("\n\t\t\t========================");
	printf("\n\t\t\t[1] Play with X");
	printf("\n\t\t\t[2] Play with O");
	printf("\n\t\t\t[3] Exit");
	printf("\n\t\t\t========================");
	printf("\n\t\t\tSelect: ");
	scanf("%d", &menu);

	switch(menu) {
		case 1:
			player = 1;
			comp = 0;
			CLEAR();
			printf("\n\n\n\n\n\n\t\t\t     Playing with X");
			player_first();
			break;

		case 2:
			player = 0;
			comp = 1;
			CLEAR();
			printf("\n\n\n\n\n\n\t\t\t     Playing with O");
			start_game();
			break;

		case 3:
			return 0;

		default:
			goto menu;
	}

	return 0;
}
