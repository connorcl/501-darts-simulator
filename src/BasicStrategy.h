#pragma once

#include "Strategy.h"
#include <map>

using std::map;


/* Represents a basic strategy which does not take
   into account the eventuality of missing the target */
class BasicStrategy :
	public Strategy
{
	// map with scores as keys and best throws as values
	static map<int, int> best_throws;
	// whether map of scores to throws has been initialized
	static bool initialized;
	
	/* calculate the best throw for a particular score, aiming to hit
       the lowest double possible if any */
	static void calculate_best_throw(int);

public:
	/* record the best throw possible for every score which either is a double
       or from which a double can be hit within one throw */
	static void initialize();

	// constructor
	BasicStrategy();
	// destructor
	~BasicStrategy();

	/* get best throw, based on player's score, number of darts remaining and
	   player's score at the start of the turn */
	virtual int select_target(int, int, int) override;
};

