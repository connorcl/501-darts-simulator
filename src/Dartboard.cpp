#include "Dartboard.h"


const array<array<int, 21>, 2> Dartboard::neighbours = { { { { 0,20,15,17,18,12,13,19,16,14,6,8,9,4,11,10,7,2,1,3,5 } },
																     { { 0,18,17,19,13,20,10,16,11,12,15,14,5,6,9,2,8,3,4,7,1 } } } };

array<Target*, 62> Dartboard::targets = {};

bool Dartboard::initialized = false;

uniform_int_distribution<int> Dartboard::dist_percent(0, 99);

uniform_int_distribution<int> Dartboard::dist_section(1, 20);


// throw at a target (single) and return index of target hit
int Dartboard::throw_single(int base) const
{
	// generate a random number from 0-99
	int rand_percent = dist_percent(random_device);

	// if target is outer bull
	if (base == 25) {
		// hit outer bull 80% of the time
		if (rand_percent < 80) {
			return get_target_index(25, 1);
		}
		// hit inner bull 10% of the time
		else if (rand_percent < 90) {
			return get_target_index(25, 2);
		}
		// hit random single 10% of the time
		else {
			return get_target_index(dist_section(random_device), 1);
		}
	}
	// if target is any other single
	else {
		// hit target 88% of the time
		if (rand_percent < 88) {
			return get_target_index(base, 1);
		}
		// hit neighbouring single 4% of the time
		else if (rand_percent < 92) {
			return get_target_index(neighbours[0][base], 1);
		}
		// hit other neighbouring single 4% of the time
		else if (rand_percent < 96) {
			return get_target_index(neighbours[1][base], 1);
		}
		// hit double 2% of the time
		else if (rand_percent < 98) {
			return get_target_index(base, 2);
		}
		// hit triple 2% of the time
		else {
			return get_target_index(base, 3);
		}
	}
}

// throw at a target (double) and return index of target hit
int Dartboard::throw_double(int base) const
{
	// generate a random number from 0-99
	int rand_percent = dist_percent(random_device);

	// hit intended target 80% of the time
	if (rand_percent < 80) {
		return get_target_index(base, 2);
	}
	// miss the board 5% of the time
	else if (rand_percent < 85) {
		return -1;
	}
	// hit single 5% of the time
	else if (rand_percent < 90) {
		return get_target_index(base, 1);
	}
	// hit neighbouring double 3% of the time
	else if (rand_percent < 93) {
		return get_target_index(neighbours[0][base], 2);
	}
	// hit other neighbouring double 3% of the time
	else if (rand_percent < 96) {
		return get_target_index(neighbours[1][base], 2);
	}
	// hit neighbouring single 2% of the time
	else if (rand_percent < 98) {
		return get_target_index(neighbours[0][base], 1);
	}
	// hit other neighbouring single 2% of the time
	else {
		return get_target_index(neighbours[1][base], 1);
	}
}

// throw at a target (triple) and return index of target hit
int Dartboard::throw_triple(int base, int accuracy) const
{
	// generate a random number from 0-99
	int rand_percent = dist_percent(random_device);

	// hit intended target accuracy % of the time
	if (rand_percent < accuracy) {
		return get_target_index(base, 3);
	}
	// hit single (90 - accuracy) % of the time
	else if (rand_percent < 90) {
		return get_target_index(base, 1);
	}
	// hit neighbouring triple 3% of the time
	else if (rand_percent < 93) {
		return get_target_index(neighbours[0][base], 3);
	}
	// hit other neighbouring triple 3% of the time
	else if (rand_percent < 96) {
		return get_target_index(neighbours[1][base], 3);
	}
	// hit neighbouring single 2% of the time
	else if (rand_percent < 98) {
		return get_target_index(neighbours[0][base], 1);
	}
	// hit other neighbouring single 2% of the time
	else {
		return get_target_index(neighbours[1][base], 1);
	}
}

// throw at the bull and return index of target hit
int Dartboard::throw_bull(int accuracy) const
{	
	// generate a random number from 0-99
	int rand_percent = dist_percent(random_device);

	// hit the bull (accuracy - 20) % of the time
	if (rand_percent < accuracy - 20) {
		return get_target_index(25, 2);
	}
	// hit the outer bull (85 - (accuracy - 20)) % of the time
	else if (rand_percent < 85) {
		return get_target_index(25, 1);
	}
	// hit a random single 15% of the time
	else {
		return get_target_index(dist_section(random_device), 1);
	}
}

// constructor which takes a reference to a random device
Dartboard::Dartboard(std::random_device& main_random_device) :
	random_device(main_random_device) {}

// destructor
Dartboard::~Dartboard()
{
}

// return a const reference to the array of neighbours
const array<array<int, 21>, 2>& Dartboard::get_neighbours()
{
	return neighbours;
}

// return a const reference to the array of targets
const array<Target*, 62>& Dartboard::get_targets()
{
	// initialize targets if they are not already initialized
	if (!initialized) {
		Dartboard::initialize();
	}
	return targets;
}

// get target index based on base and multiplier
int Dartboard::get_target_index(int base, int multiplier)
{
	return ((base == 25) ? (59 + multiplier) : (20 * (multiplier - 1)) + (base - 1));
}

// initialize array of targets
void Dartboard::initialize()
{
	// exit if already initialized
	if (initialized) {
		return;
	}
	// loop over possible multipliers
	for (int m = 1; m <= 3; m++) {
		// loop over possible bases (excluding bull)
		for (int b = 1; b <= 20; b++) {
			// set target, ordered firstly by multiplier and secondly by base
			targets[get_target_index(b, m)] = new Target(b, m);
		}
	}
	// loop over possible multipliers for bull
	for (int m = 1; m <= 2; m++) {
		targets[get_target_index(25, m)] = new Target(25, m);
	}
	// set initialized flag
	initialized = true;
}

/* throw for a target based on its target index and player accuracy,
   returning index of target hit */
int Dartboard::throw_by_target(int target, int accuracy) const
{
	// get base and multiplier of target
	int base = targets[target]->get_base();
	int multiplier = targets[target]->get_multiplier();
	
	// choose throw function based on multiplier
	switch (multiplier) {
	case 1:
		return throw_single(base);
	case 2:
		// use throw_bull if base is 25
		return ((base == 25) ? throw_bull(accuracy) : throw_double(base));
	case 3:
		return throw_triple(base, accuracy);
	default:
		return -1;
	}
}