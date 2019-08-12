#pragma once

#include "Strategy.h"


/* Class representing an interactive strategy where the throw is chosen by the user */
class InteractiveStrategy :
	public Strategy
{
public:
	// constructor
	InteractiveStrategy();
	// destructor
	~InteractiveStrategy();
	/* get best throw, based on player's score, number of darts remaining and
	   player's score at the start of the turn */
	virtual int select_target(int, int, int) override;
};

