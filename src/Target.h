#pragma once


/* Represents a target on a dartboard, with a base, 
   a multipler and a resultant value */
class Target
{
	int base;
	int multiplier;
	int value;

public:
	/* constructor which takes a base and multiplier and sets 
	   these as well as the resultant value */
	Target(int, int);
	// destructor
	~Target();

	// getter for base
	int get_base() const;
	// getter for multiplier
	int get_multiplier() const;
	// getter for value
	int get_value() const;
};

