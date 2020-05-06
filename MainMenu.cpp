#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>
#include "Games.h"

using namespace std;

int main()
{
	int bKey[2];
	int screenWidth = 80;
	int screenHeight = 30;
	int screenArea = screenWidth * screenHeight;
	wchar_t* screen = new wchar_t[screenArea];

	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	for (int i = 0; i < screenArea; i++)
		screen[i] = L' ';

	wsprintf(&screen[2 * screenWidth + 4], L"WELCOME TO THE BEST FREAKIN' GAME CONSOLE EVER DEVISED BY A MORAL BEING.");
	wsprintf(&screen[8 * screenWidth + 15], L"PRESS \"1\" TO PLAY SNAKE. PRESS \"2\" TO PLAY SPACE INVADERS.");
	wsprintf(&screen[12 * screenWidth + 28], L"MORE GAMES COMING SOON...");

	WriteConsoleOutputCharacter(hConsole, screen, screenArea, { 0, 0 }, &dwBytesWritten);

	while (1)
	{
		for (int i = 0; i < 2; i++)
			bKey[i] = (0x8000 & GetAsyncKeyState((unsigned char)("\x31\x32"[i]))) != 0;

		if (bKey[0])
			Snake();
		else if (bKey[1])
			SpaceInvaders();
	}

	return 0;
}
