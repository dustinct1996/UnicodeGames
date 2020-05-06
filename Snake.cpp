#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>

using namespace std;
using std::fstream;

void sortLeaderBoard(const char *fileName)
{
	vector<int> scores;
	fstream leaderBoard;

	leaderBoard.open(fileName);

	string num;

	while (getline(leaderBoard, num))
		scores.push_back(stoi(num));

	leaderBoard.close();

	leaderBoard.open("snakeLeaderBoard.txt");

	sort(scores.begin(), scores.end(), greater<int>());

	for (int i = 0; i < scores.size(); i++)
		leaderBoard << scores.at(i) << "\n";

	leaderBoard.close();
}

int Snake()
{
	vector<vector<int>> snake = { { 40, 15 } , { 39, 15 } , { 38, 15 } , { 37, 15 } };
	fstream leaderBoard;
	string num = "1";
	int bKey[4];
	int quit = 0;
	int score = 0;
	int newGame = 0;
	int numEggs = 0;
	int gameOver = 0;
	int direction = 2;
	int screenWidth = 80;
	int screenHeight = 30;
	int curHeadPosX = snake.at(0).at(0);
	int curHeadPosY = snake.at(0).at(1);
	int screenArea = screenWidth * screenHeight;
	int eggPosX = rand() % screenWidth;
	int eggPosY = (rand() % (screenHeight - 3)) + 3;
	wchar_t *screen = new wchar_t[screenArea];

	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	srand(time(NULL));

	for (int i = 0; i < screenArea; i++)
		screen[i] = L' ';

	wsprintf(&screen[screenWidth + 5], L"SNAKE");

	for (int i = 0; i < screenWidth; i++)
		screen[2 * screenWidth + i] = L"-"[0];

	while (!gameOver)
	{
		this_thread::sleep_for(direction % 2 == 1 ? 200ms : 120ms);

		for (int i = 0; i < 4; i++)
			bKey[i] = (0x8000 & GetAsyncKeyState((unsigned char)("\x25\x26\x27\x28"[i]))) != 0;

		if (bKey[0] && direction != 2)
			direction = 0;
		else if (bKey[1] && direction != 3)
			direction = 1;
		else if (bKey[2] && direction != 0)
			direction = 2;
		else if (bKey[3] && direction != 1)
			direction = 3;

		switch (direction)
		{
			case 0:
				snake.insert(snake.begin(), { --curHeadPosX, curHeadPosY });
				break;
			case 1:
				snake.insert(snake.begin(), { curHeadPosX, --curHeadPosY });
				break;
			case 2:
				snake.insert(snake.begin(), { ++curHeadPosX, curHeadPosY });
				break;
			case 3:
				snake.insert(snake.begin(), { curHeadPosX, ++curHeadPosY });
				break;
		}

		if ((snake.at(0).at(0) < 0 || snake.at(0).at(0) >= screenWidth || snake.at(0).at(1) < 3 || snake.at(0).at(1) >= screenHeight) || (screen[snake.at(0).at(1) * screenWidth + snake.at(0).at(0)] == L"o"[0]))
		{
			snake.erase(snake.begin());

			leaderBoard.open("snakeLeaderBoard.txt");

			if (leaderBoard.peek() != std::ifstream::traits_type::eof())
			{
				getline(leaderBoard, num);
			}

			leaderBoard.close();

			leaderBoard.open("snakeLeaderBoard.txt", fstream::app);

			leaderBoard << score << "\n";

			leaderBoard.close();

			sortLeaderBoard("snakeLeaderBoard.txt");

			gameOver = true;
		}
		else
		{
			screen[snake.at(snake.size() - 1).at(1) * screenWidth + snake.at(snake.size() - 1).at(0)] = L" "[0];
			snake.pop_back();
		}
		
		if (screen[snake.at(0).at(1) * screenWidth + snake.at(0).at(0)] == L"0"[0])
		{
			numEggs++;
			score += snake.size() * 2;

			do
			{
				eggPosX = rand() % screenWidth;
				eggPosY = (rand() % (screenHeight - 3)) + 3;
			} while (screen[eggPosY * screenWidth + eggPosX] != L' ');

			for(int i = 0; i < numEggs; i++)
				snake.push_back({ snake.at(snake.size() - 1).at(0), snake.at(snake.size() - 1).at(1) });
		}

		while (gameOver)
		{
			screen[snake.at(0).at(1) * screenWidth + snake.at(0).at(0)] = L"X"[0];

			for (int i = 1; i < snake.size(); i++)
				screen[snake.at(i).at(1) * screenWidth + snake.at(i).at(0)] = L"x"[0];

			if (score > stoi(num))
				wsprintf(&screen[10 * screenWidth + 33], L"NEW HIGH SCORE");
			else
				wsprintf(&screen[10 * screenWidth + 28], L"CURRENT HIGH SCORE: %d", stoi(num));

			wsprintf(&screen[15 * screenWidth + 18], L"PRESS \"ENTER\" TO PLAY AGAIN OR \"ESC\" TO QUIT");
			wsprintf(&screen[screenWidth + 36], L"GAME OVER");

			bool newGame = (0x8000 & GetAsyncKeyState((unsigned char)("\x0D"[0]))) != 0;
			bool quit = (0x8000 & GetAsyncKeyState((unsigned char)("\x1B"[0]))) != 0;

			WriteConsoleOutputCharacter(hConsole, screen, screenArea, { 0, 0 }, &dwBytesWritten);

			if (newGame)
			{
				snake = { { 40, 15 } , { 39, 15 } , { 38, 15 } , { 37, 15 } };
				score = 0;
				numEggs = 0;
				gameOver = 0;
				direction = 2;
				eggPosX = rand() % screenWidth;
				eggPosY = (rand() % (screenHeight - 3)) + 3;

				for (int i = 0; i < screenArea; i++)
					screen[i] = L' ';

				wsprintf(&screen[screenWidth + 5], L"SNAKE");

				for (int i = 0; i < screenWidth; i++)
					screen[2 * screenWidth + i] = L"-"[0];

				break;
			}
			else if (quit)
				exit(0);
		}

		wsprintf(&screen[screenWidth + 66], L"Score: %d", score);

		screen[eggPosY * screenWidth + eggPosX] = L"0"[0];

		screen[snake.at(0).at(1) * screenWidth + snake.at(0).at(0)] = L"O"[0];

		for (int i = 1; i < snake.size(); i++)
			screen[snake.at(i).at(1) * screenWidth + snake.at(i).at(0)] = L"o"[0];

		curHeadPosX = snake.at(0).at(0);
		curHeadPosY = snake.at(0).at(1);

		WriteConsoleOutputCharacter(hConsole, screen, screenArea, { 0, 0 }, &dwBytesWritten);
	}

	leaderBoard.close();

	return 0;
}
