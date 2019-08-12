#include "BasicStrategy.h"

#include "Dartboard.h"
#include <iostream>

using std::cout;
using std::endl;


map<int, int> BasicStrategy::best_throws = {};

bool BasicStrategy::initialized = false;

/* calculate the best throw for a particular score, aiming to hit 
   the lowest double possible if any */
void BasicStrategy::calculate_best_throw(int score)
{
	bool done = false;
	// for each double value in ascending order
	for (int d = 2; d <= 50 && !done; d += (d == 40) ? 10 : 2) {
		// for each possible target in ascending order
		for (int i = 0; i < 62 && !done; i++) {
			int value = Dartboard::get_targets()[i]->get_value();
			// if score - throw_value equals double value
			if ((score - value) == d) {
				best_throws[score] = i;
				done = true;
			}
		}
	}
}

/* record the best throw possible for every score which either is a double
   or from which a double can be hit within one throw */
void BasicStrategy::initialize()
{
	// return if already initialized
	if (initialized) {
		return;
	}

	/* loop over doubles possible (2 to 40, 50) and record best throw
	   at each of these as the relevant double */
	for (int i = 2; i <= 50; i += (i == 40) ? 10 : 2) {
		best_throws[i] = Dartboard::get_target_index(i / 2, 2);
	}

	/* loop from 3 to 100, incrementing by 2 from 3 to 39 and at 49 in order
	   to miss out the doubles which have already been recorded */
	for (int i = 3; i <= 110; i += (i < 40 || i == 49) ? 2 : 1) {
		calculate_best_throw(i);
	}
	// set initialized flag
	initialized = true;
}

// constructor
BasicStrategy::BasicStrategy()
{
}

// destructor
BasicStrategy::~BasicStrategy()
{
}

/* get best throw, based on player's score, number of darts remaining and
   player's score at the start of the turn */
int BasicStrategy::select_target(int score, int darts_remaining, int starting_score)
{
	// calculate best throws if not calculated already
	if (!initialized) {
		initialize();
	}
	// if finishing or hitting a double is possible, get pre-calculated best throw
	if (best_throws.count(score) != 0) {
		return best_throws.at(score);
	}
	// otherwise, throw for triple 20
	else {
		return Dartboard::get_target_index(20, 3);
	}
}
