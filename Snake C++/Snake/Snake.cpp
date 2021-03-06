#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <conio.h>
#include <time.h>

using namespace std;


struct object
{
	int x, y;
};

const int w = 30;
const int h = 20;

int width, height, x1, y1;

object head, food, door;
object tail[w*h];
int nTail;

bool doorGenerated, paused, gameOver, gameWon, gameLost, quit, exited;

int key, wallnum, retry_question_case;

int score, bonus, finalscr, highscr;

bool hsbeated, pauseInfoPrinted;

//the position of the snake, food, etc.
//will be by default at the beginning of the game

bool GoodConditionsForGeneratingFood() {
	for (int i = 0; i < nTail; i++) {
		if (food.x == tail[i].x && food.y == tail[i].y) return false;
	}
	return true;
}

void Initial() {
	width = w;
	height = h;

	score = 0;
	ifstream highscore("highscore.txt");
	highscore >> highscr;
	highscore.close();

	paused = true; pauseInfoPrinted = false;
	key = 0;
	nTail = 0;
	doorGenerated = false;
	hsbeated = false;

	srand(time(NULL));
	gameOver = gameWon = gameLost = quit = exited = false;
	bonus = 0;
	head.x = width / 2;
	head.y = height / 2;
	food.x = rand() % width + 1;
	food.y = rand() % height + 1;
}

//the function that draws everything

void Draw() {
	system("cls");
	for (int i = 0; i < width + 2; i++) {
		if (wallnum == 1 && i == door.x && doorGenerated) cout << "D";
		else cout << "#";
	}
	cout << endl;

	for (int i = 1; i <= height; i++) {
		for (int j = 0; j < width + 2; j++) {
			if (j == 0 || j == width + 1) {
				if (i == door.y && j == door.x && doorGenerated) cout << "D";
				else cout << "#";
				if (j == width + 1) cout << endl;
			}
			else {
				if (i == head.y && j == head.x)
					cout << "O";
				else if (i == food.y && j == food.x)
					cout << "$";
				else {
					bool print = false;
					for (int k = 1; k <= nTail; k++)
					{
						if (tail[k].y == i && tail[k].x == j)
						{
							cout << "o";
							print = true;
						}
					}
					if (!print)
						cout << " ";
				}
			}
		}
	}

	for (int i = 0; i < width + 2; i++) {
		if (wallnum == 2 && i == door.x && doorGenerated) cout << "D";
		else cout << "#";
	}
	cout << endl;
	cout << "Current Score: " << score << endl;
	cout << "High Score: " << highscr << endl;
	if (doorGenerated)
		cout << "A DOOR HAS APPEARED ON YOUR MAP!" << endl;
}

void Input()
{
	if (_kbhit()) {
		char c = _getch();

		if (c != 'p') paused = false;

		switch (c)
		{
		case 'w':
			if (key != 2) key = 1;
			break;
		case 's':
			if (key != 1) key = 2;
			break;
		case 'a':
			if (key != 4) key = 3;
			break;
		case 'd':
			if (key != 3) key = 4;
			break;
		case 'p':
			if (!paused)
			{
				pauseInfoPrinted = false;
				paused = true;
			}
			else pauseInfoPrinted = true;

			break;
		case VK_ESCAPE:
			gameOver = quit = true;
			break;
		}
	}
}

void TailMove()
{
	tail[0] = head;
	x1 = tail[0].x, y1 = tail[0].y;
	int x2, y2;

	for (int i = 1; i <= nTail; i++) {
		x2 = tail[i].x;
		y2 = tail[i].y;
		tail[i].x = x1;
		tail[i].y = y1;
		x1 = x2;
		y1 = y2;
	}
}


void HeadMove()
{
	switch (key) {
	case 1: head.y--;
		break;
	case 2: head.y++;
		break;
	case 3: head.x--;
		break;

	case 4: head.x++;
		break;
	}

}


void GenerateDoor() {
	srand(time(0));
	wallnum = rand() % 4 + 1;
	switch (wallnum) {
	case 1:
		door.y = 0;
		door.x = rand() % width + 1;
		break;
	case 2:
		door.y = height + 1;
		door.x = rand() % width + 1;
		break;
	case 3:
		door.x = 0;
		door.y = rand() % height + 1;
		break;
	case 4:
		door.x = width + 1;
		door.y = rand() % height + 1;
		break;
	}
	doorGenerated = true;
}

void Rules()
{
	if (head.x == food.x && head.y == food.y)
	{
		srand(time(0));
		score += 10;
		nTail++;

		tail[nTail].x = x1;
		tail[nTail].y = y1;

		food.x = rand() % width + 1;
		food.y = rand() % height + 1;
		while (!GoodConditionsForGeneratingFood()) {
			food.x = rand() % width + 1;
			food.y = rand() % height + 1;
		}
	}

	for (int i = 1; i <= nTail; i++)
		if (head.x == tail[i].x && head.y == tail[i].y)
			gameOver = gameLost = true;

	if (score >= 300 && !doorGenerated) {
		GenerateDoor();
	}

	if (head.x == 0 || head.x > width || head.y == 0 || head.y > height) {
		gameOver = true;
		if (head.x == door.x && head.y == door.y && doorGenerated) {
			gameWon = true;
			bonus = 100;
		}
		else gameLost = true;
	}
}

void GameEndTitles() {

	if (gameWon) {
		cout << "\n\n\n     CONGRATULATIONS! YOU WON THE GAME!\n\n";
	}

	else if (gameLost) {
		cout << "\n\n\n             YOU LOST!\n\n";
	}

	else if (quit) {
		cout << "You quit the game.\n\n";
	}

}

void NewHighscore() {
	finalscr = score + bonus;
	if (finalscr > highscr) {
		hsbeated = true;
		ofstream hsWrite("highscore.txt");
		hsWrite << finalscr;
		hsWrite.close();
	}
}

void ScoreStats(bool sleep = false) {
	cout << "Your score: ";
	if (sleep)
		Sleep(750);

	cout << score;
	if (sleep)
		Sleep(500);

	cout << " + ";
	if (sleep)
		Sleep(500);

	cout << bonus << "(bonus) = ";
	if (sleep) Sleep(1000);

	cout << finalscr;
	if (hsbeated) {
		if (sleep)
			Sleep(1000);

		cout << "    NEW HIGHSCORE!" << endl;
		cout << "Previous highscore: " << highscr << endl;
	}

	else cout << "\nHighscore: " << highscr << endl;
}

int MillisecondsFaster() {
	int n = 5 * (nTail / 10);
	int max = 40;
	if (n >= max) return max;
	return n;
}

void Retry() {
	cout << "\nDo you want to retry?\n(press Enter to choose an option, UpArrow to go up, and DownArrow to go down)\n";
	bool chosen = false;
	bool optionchanged = false;
	retry_question_case = 1;
	cout << "Exit   <---\nRetry";
	while (!chosen) {
		if (optionchanged) {
			system("cls");
			GameEndTitles();
			ScoreStats();
			cout << "\nDo you want to retry?\n(press Enter to choose an option, UpArrow to go up, and DownArrow to go down)\n";
			if (retry_question_case == 1) cout << "Exit   <---\nRetry";
			else if (retry_question_case == 2) cout << "Exit  \nRetry  <---";
			optionchanged = false;
		}

		if (_kbhit()) {
			switch (_getch()) {

			case 80:
				if (retry_question_case == 1) {
					optionchanged = true;
					retry_question_case = 2;
				}
				break;

			case 72:
				if (retry_question_case == 2) {
					retry_question_case = 1;
					optionchanged = true;
				}
				break;

			case VK_RETURN:
				chosen = true;
			}
		}

		Sleep(10);
	}

	if (retry_question_case == 1) exited = true;
}

int main() {
	while (true) {
		Initial();
		Draw();
		while (true) {
			Input();
			if (!paused) {
				TailMove();
				HeadMove();
				Rules();
				if (gameOver)
					break;

				Draw();
			}
			else if (!pauseInfoPrinted) {
				cout << "\n\nYour game is in pause.";
				pauseInfoPrinted = true;
			}

			Sleep(100 - MillisecondsFaster());
		}

		Sleep(500);

		system("cls");

		GameEndTitles();

		NewHighscore();

		ScoreStats(true);

		Retry();
		if (exited)
			return 0;
	}
}