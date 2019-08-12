#include "MainMenu.h"

#include <iostream>
#include "Dartboard.h"
#include "BasicStrategy.h"
#include "EnhancedStrategy.h"
#include "Player.h"
#include "SimulatedGame.h"
#include <random>
#include "InteractiveGame.h"
#include "ConsoleInterface.h"

using std::cout;
using std::endl;
using std::cin;
using std::random_device;


const string MainMenu::error_msg = "[!] Invalid input!";

// initialize dartboard and strategies
void MainMenu::initialize()
{
	Dartboard::initialize();
	BasicStrategy::initialize();
	EnhancedStrategy::initialize();
}

// run a simulated series of matches
void MainMenu::simulated_game()
{
	// prompt for number of simulations
	int num_matches = 0;
	cout << "Enter number of matches to simulate: ";
	// loop while cin produces an error
	while (!(cin >> num_matches)) {
		cin.clear();
		cin.ignore(10000, '\n');
		cout << error_msg << endl;
		cout << "--> ";
	}
	cout << endl;
	// prompt user to select strategy
	int strategy = 0;
	cout << "Player 1: Sid (73% accuracy, basic strategy)" << endl;
	cout << "Player 2: Joe (71% accuracy)\n" << endl;
	cout << "Select Joe's strategy:" << endl;
	cout << "1. Basic strategy (default)" << endl;
	cout << "2. Enhanced strategy" << endl;
	cout << "--> ";
	// loop while cin produces an error
	while (!(cin >> strategy)) {
		cin.clear();
		cin.ignore(10000, '\n');
		cout << error_msg << endl;
		cout << "--> ";
	}
	cout << endl;

	// initialize dartboard and strategies
	initialize();

	// create players
	Player sid("Sid", 73, 0);
	Player joe("Joe", 71, (strategy == 2) ? 'e' : 'b');
	
	random_device main_rd;
	SimulatedGame game(sid, joe, main_rd, 0); // create game
	game.run_simulation(num_matches, 13, 5); // run simulated matches
	game.output_results(); // ouput simulation results

	// exit when enter is pressed
	cout << "\nPress Enter to exit...";
	cin.ignore(10000, '\n');
	cin.get();
}

// run interactive game
void MainMenu::interactive_game()
{
	// print automated player details
	cout << "Player 1: Sid (73% accuracy, basic strategy)" << endl;

	// get user name from console input
	string name;
	cout << "Player 2: Enter your name (your accuracy: 71%): ";
	cin >> name;
	// clear screen
	ConsoleInterface::move_cursor(0, 0);
	ConsoleInterface::clear_screen(30);
	ConsoleInterface::move_cursor(0, 0);

	// initialize and create players
	initialize();
	Player user(name, 71, 'i');
	Player sid("Sid", 73, 'b');

	random_device main_rd;

	// play game, randomly choosing first player
	InteractiveGame game(sid, user, main_rd, -1);
	game.play_game();

	// exit when enter is pressed
	cout << "\nPress Enter to exit...";
	cin.ignore(10000, '\n');
	cin.get();
}

// test performance of each strategy
void MainMenu::test_strategies()
{
	// prompt for accuracy to test with
	cout << "Enter player accuracy in %" << endl;
	cout << "--> ";
	int accuracy = 0;
	while (!(cin >> accuracy)) {
		cout << error_msg << endl;
		cin.clear();
		cin.ignore(10000, '\n');
		cout << "--> ";
	}
	cout << endl;

	// initialize and create players for each strategy
	initialize();
	Player basic("", accuracy, 'b');
	Player enhanced("", accuracy, 'e');

	random_device main_rd;

	// simulate single player games for each player
	SimulatedGame game(basic, enhanced, main_rd, -1);
	cout << "Basic strategy: ";
	game.run_simulation_single_player(10000, basic.get_id());
	cout << "Enhanced strategy: ";
	game.run_simulation_single_player(10000, enhanced.get_id());

	// exit when enter is pressed
	cout << "\nPress Enter to exit...";
	cin.ignore(10000, '\n');
	cin.get();
}

// constructor
MainMenu::MainMenu()
{
}

// destructor
MainMenu::~MainMenu()
{
}

// main menu
void MainMenu::menu()
{
	// prompt user to select action
	int option;
	cout << "Select an option: " << endl;
	cout << "1. Run simulation" << endl;
	cout << "2. Play interactive game (maximize console window for best results)" << endl;
	cout << "3. Test strategies" << endl;
	cout << "4. Quit" << endl;
	cout << "--> ";
	while (!(cin >> option)) {
		cin.clear();
		cin.ignore(10000, '\n');
		cout << error_msg << endl;
		cout << "--> ";
	}
	// clear screen
	ConsoleInterface::move_cursor(0, 0);
	ConsoleInterface::clear_screen(30);
	ConsoleInterface::move_cursor(0, 0);
	// perform chosen action
	switch (option) {
	case 1:
		simulated_game(); break;
	case 2:
		interactive_game(); break;
	case 3:
		test_strategies(); break;
	case 4:
		return;
	default: break;
	}
}
