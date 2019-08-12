#pragma once

#include <array>
#include "Target.h"
#include <random>

using std::array;
using std::uniform_int_distribution;


/* Represents a dartboard, storing all possible targets and the order
   of the sections (statically) and defining the mechanics of throwing a dart */
class Dartboard
{
	/* 2D array storing the neighbours of each section of the dartboard,
	   e.g. neighbours[0][12] and neighbours[1][12] are the neighbours
	   of section 12 */
	static const array<array<int, 21>, 2> neighbours;
	// array of all possible targets, allowing targets to be referred to by an integer index
	static array<Target*, 62> targets;
	// whether the array of targets has been generated
	static bool initialized;
	// distribution for determining whether a dart hits its intended target
	static uniform_int_distribution<int> dist_percent;
	// distribution for choosing a random section of the board
	static uniform_int_distribution<int> dist_section;

	// reference to random device
	std::random_device& random_device;

	// functions for throwing for a target, returning the index of the target hit
	int throw_single(int) const;
	int throw_double(int) const;
	int throw_triple(int, int) const;
	int throw_bull(int) const;

public:
	// constructor which takes a reference to a random device
	Dartboard(std::random_device&);
	// destructor
	~Dartboard();

	// return a const reference to the array of neighbours
	static const array<array<int, 21>, 2>& get_neighbours();
	// return a const reference to the array of targets
	static const array<Target*, 62>& get_targets();
	// get target index based on base and multiplier
	static int get_target_index(int, int);
	// initialize array of targets
	static void initialize();

	/* throw for a target based on its target index and player accuracy,
	   returning index of target hit */
	int throw_by_target(int, int) const;
};

