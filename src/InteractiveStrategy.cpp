#include "InteractiveStrategy.h"
#include <iostream>
#include "Dartboard.h"
#include "ConsoleInterface.h"

using std::cout;
using std::cin;
using std::endl;


// constructor
InteractiveStrategy::InteractiveStrategy()
{
}

// destructor
InteractiveStrategy::~InteractiveStrategy()
{
}

/* get best throw, based on player's score, number of darts remaining and
   player's score at the start of the turn */
int InteractiveStrategy::select_target(int, int, int)
{
	int base = 0;
	int multiplier = 0;
	// prompt for input
	cout << "Enter throw multiplier (1 = single, 2 = double, 3 = triple): ";
	cin >> multiplier;
	cin.clear();
	cin.ignore();
	cout << "Enter throw base (1-20, 25 for bull): ";
	cin >> base;
	cin.clear();
	cin.ignore();
	// default to triple 20 if input is not valid
	if (multiplier < 1 || multiplier > 3) {
		multiplier = 3;
	}
	if (base < 1 || (base > 20 && base != 25) || (base == 25 && multiplier == 3)) {
		base = 20;
	}
	// return target index
	return Dartboard::get_target_index(base, multiplier);
}
