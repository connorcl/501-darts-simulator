#pragma once

#include <string>

using std::string;

// Represents the main menu interface for selecting different actions
class MainMenu
{
	// error message for invalid input
	static const string error_msg;
	
	// initialize dartboard and strategies
	void initialize();
	// run a simulated series of matches
	void simulated_game();
	// run interactive game
	void interactive_game();
	// test performance of each strategy
	void test_strategies();

public:
	// constructor
	MainMenu();
	// destructor
	~MainMenu();

	// main menu
	void menu();
};