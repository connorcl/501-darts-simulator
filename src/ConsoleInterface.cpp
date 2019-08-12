#include "ConsoleInterface.h"

#include <string>
#include <iostream>
#include <Windows.h>

using std::string;
using std::cout;
using std::endl;

// clear console
void ConsoleInterface::clear_screen(int lines)
{
	// generate line of 100 spaces
	string line = "";
	for (int i = 0; i < 100; i++) {
		line += " ";
	}
	// print lines of spaces
	for (int i = 0; i < lines; i++) {
		cout << line << endl;
	}
}

// move cursor to specified location
void ConsoleInterface::move_cursor(short col, short row)
{
	// create COORD structure
	COORD pos = { col, row };
	// move cursor
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
