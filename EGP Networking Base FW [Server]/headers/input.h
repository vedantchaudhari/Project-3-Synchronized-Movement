#pragma once

//getch and kbhit
#include <conio.h>
#include "../headers/CommonInclude.h"


enum Commands
{
	PAUSE = '\n',
};

char getInput(std::string &s)
{
	if (_kbhit())
	{
		char c = _getch();
		if (c == 13)
		{
			return PAUSE;
		}
		return c;
	}
	return 0;
}
