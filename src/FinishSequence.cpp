#include "FinishSequence.h"


// default constructor
FinishSequence::FinishSequence()
{
}

// destructor
FinishSequence::~FinishSequence()
{
}

// get length in throws of finish sequence
int FinishSequence::get_length() const
{
	return static_cast<int>(sequence.size());
}

// get first throw of finish
int FinishSequence::get_first_throw() const
{
	// return first throw if finish is recorded
	if (!sequence.empty()) {
		return sequence[0];
	}
	// otherwise return -1 (invalid)
	else {
		return -1;
	}
}

// set finish sequence
void FinishSequence::set_sequence(const vector<int> &new_sequence)
{
	sequence = new_sequence;
}

// get whether finish ends in double 16 or 20
bool FinishSequence::get_ends_d16_d20() const
{
	return ends_d16_d20;
}

// set whether finish ends in double 16 or 20
void FinishSequence::set_ends_d16_d20(bool new_ends_d16_d20)
{
	ends_d16_d20 = new_ends_d16_d20;
}

// get contingency score of finish
int FinishSequence::get_contingency() const
{
	return contingency;
}

// set contingency score of finish
void FinishSequence::set_contingency(int new_contingency)
{
	contingency = new_contingency;
}
