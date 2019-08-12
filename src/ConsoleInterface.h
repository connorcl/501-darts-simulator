#pragma once

// contains functions useful for console interfaces
class ConsoleInterface
{
public:
	// clear console
	static void clear_screen(int);
	// move cursor to specified location
	static void move_cursor(short, short);
};

