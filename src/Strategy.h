#pragma once


/* Abstract class representing a strategy for selecting a throw based
   on a player's score, their number of darts remaining and 
   their score at the start of the round */
class Strategy
{
public:
	// pure virtual destructor
	virtual ~Strategy() = 0;
	/* get best throw, based on player's score, number of darts remaining and
       player's score at the start of the turn */
	virtual int select_target(int, int, int) = 0;
};

