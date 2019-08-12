#include "Target.h"


/* constructor which takes a base and multiplier and sets
   these as well as the resultant value */
Target::Target(int target_base, int target_multiplier) :
	base(target_base),
	multiplier(target_multiplier),
	value(target_base * target_multiplier) {}

// destructor
Target::~Target()
{
}

// getter for base
int Target::get_base() const {
	return base;
}

// getter for multiplier
int Target::get_multiplier() const {
	return multiplier;
}

// getter for value
int Target::get_value() const {
	return value;
}
