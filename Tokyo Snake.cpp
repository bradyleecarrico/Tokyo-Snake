#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#define KEY_LEFT 75
#define KEY_RIGHT 77

//global variables
using namespace std;
const int mapWidth = 30;
const int mapHeight = 20;
unsigned snakeHeadX, snakeHeadY, eggX, eggY, score;
bool dead;
int tailX[600], tailY[600];
int tailLength, lastX, lastY, dir;
int frames;

void setStartingDimensions()
{
	//set starting dimensions
	dead = false;
	dir = -1;
	score = 0;
	frames = 0;
	snakeHeadX = mapWidth / 2 - 1;
	snakeHeadY = mapHeight / 2 - 2;
	for (int i = 0; i < tailLength; i++) {
		tailX[i] = 0;
		tailY[i] = 0;
	}
	tailLength = 0;

	srand((unsigned)time(0));
	eggX = 0;
	eggY = 0;
	while (eggX == 0 || eggY == 0 || eggY >= mapHeight || eggX >= mapWidth) {
		eggX = (rand() % mapWidth) - 2;
		eggY = (rand() % mapHeight) - 3;
	}

	//Intro music :)
	Beep(261, 250); Beep(349, 250); Beep(440, 250);
}

void icelandicSnake()
{
	//"screen buffer"
	system("cls");

	//score
	cout << endl << "Tokyo Snake v 1.0" << endl;
	cout << endl << "Score: " << score << endl << endl;

	//snake movement logic
	if (dir == 0) {
		snakeHeadX--;
		snakeHeadY--;
	}
	if (dir == 1) {
		snakeHeadX--;
		snakeHeadY++;
	}
	if (dir == 2) {
		snakeHeadX++;
		snakeHeadY++;
	}
	if (dir == 3) {
		snakeHeadX++;
		snakeHeadY--;
	}

	//tail function
	lastX = tailX[0];
	lastY = tailY[0];
	tailX[0] = snakeHeadX;
	tailY[0] = snakeHeadY;
	int yHolder, xHolder;

	for (int i = 1; i < tailLength; i++) {
		xHolder = tailX[i];
		yHolder = tailY[i];
		tailX[i] = lastX;
		tailY[i] = lastY;
		lastX = xHolder;
		lastY = yHolder;
	}

	//print top wall
	for (int i = 0; i < mapWidth; i++)
		cout << "#";
	cout << endl;

	//print side walls and interior contents
	for (int i = 0; i < mapHeight - 2; i++)
	{
		for (int j = 0; j < mapWidth; j++)
			if (j == 0 || j == (mapWidth - 1))
				cout << "#";
			else if (i == snakeHeadY && j == snakeHeadX) {
				HANDLE hConsole; hConsole = GetStdHandle(STD_OUTPUT_HANDLE); SetConsoleTextAttribute(hConsole, 2);
				cout << "@"; SetConsoleTextAttribute(hConsole, 15);
			}
			else if (i == eggY && j == eggX)
				cout << "e";
			else if (tailLength > 0)
			{
				bool print = false;
				for (int k = 0; k < tailLength; k++)
				{
					if (tailX[k] == j && tailY[k] == i)
					{
						HANDLE hConsole; hConsole = GetStdHandle(STD_OUTPUT_HANDLE); SetConsoleTextAttribute(hConsole, 2);
						cout << "o"; SetConsoleTextAttribute(hConsole, 15);
						print = true;
					}
				}
				if (!print)
					cout << " ";
			}
			else
				cout << " ";
		cout << " " << endl;
	}

	//print bottom wall
	for (int i = 0; i < mapWidth; i++)
		cout << "#";

	//when snake eats egg
	if (eggX == snakeHeadX && eggY == snakeHeadY) {

		srand((unsigned)time(0));
		eggX = (rand() % mapWidth) - 2;
		eggY = (rand() % mapHeight) - 2;

		while (eggX == 0 || eggY < 0 || eggY >= mapHeight || eggX >= mapWidth || (eggX == 1 && eggY == 0) ||
			(eggX == 28 && eggY == 0) || (eggX == 28 && eggY == 17) || (eggX == 1 && eggY == 17)) {
			eggX = (rand() % mapWidth) - 2;
			eggY = (rand() % mapHeight) - 2;
		}

		if ((score + 1000 / frames) == 1000)
			score = score + 500;
		else
			score = score + 1000 / frames;

		tailLength = tailLength + 3;
		Beep(440, 180);
		frames = 0;
	}

	//START instructions
	if (dir == -1) {
		cout << endl << endl << "Press LEFT or RIGHT to begin . . .";
		cout << endl << endl << "Use Z and X to shift left or right";
	}

	//game over
	if (snakeHeadX < 1 || snakeHeadX > mapWidth - 2 || snakeHeadY < 0 || snakeHeadY > mapHeight - 3) {
		dead = true;
		Beep(415, 250); Beep(329, 250); Beep(261, 250);
	}
	for (int i = 1; i < tailLength; i++) {
		if (tailX[i] == snakeHeadX && tailY[i] == snakeHeadY) {
			dead = true;
			Beep(415, 250); Beep(329, 250); Beep(261, 250);
		}
	}
}

void consoleInput()
{
	if (_kbhit()) {
		switch (_getch()) {
		case KEY_RIGHT:
			dir--;
			if (dir == 4)
				dir = 0;
			if (dir <= -1)
				dir = 3;
			break;
		case KEY_LEFT:
			dir++;
			if (dir == 4)
				dir = 0;
			if (dir <= -1)
				dir = 3;
			break;
		case 'x':
			if (dir >= 0 && dir <= 3)
				snakeHeadX++;
			break;
		case 'z':
			if (dir >= 0 && dir <= 3)
				snakeHeadX--;
			break;
		default:
			break;
		}
	}
}

void timeElpased()
{
	//frame counter for score
	if (dir >= 0 && dir <= 3)
		frames++;
}

int main()
{
	//Game Intro
	HANDLE hConsole; hConsole = GetStdHandle(STD_OUTPUT_HANDLE); SetConsoleTextAttribute(hConsole, 15);
	cout << endl << "They say there are no snakes in Tokyo . . ." << endl << endl;
	system("pause");
	system("cls");
	cout << endl << ". . . Well, that's what they said about cats in America" << endl << endl;
	system("pause");

	bool play = true;

	//Game Loop
	while (play == true) {

		setStartingDimensions();

		while (dead == false) {
			icelandicSnake();
			consoleInput();
			consoleInput();
			timeElpased();
		}

		cout << endl << endl << "Enter 1 to play again - Enter 2 to exit" << endl;
		int playagain;
		cin >> playagain;

		if (playagain == 1) {
			system("cls");
			cout << "play again" << endl;
		}

		else if (playagain != 1) {
			play = false;
		}
	}
	return 0;
}