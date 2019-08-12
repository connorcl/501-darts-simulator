#pragma once

#include <vector>

using std::vector;


/* Represents a sequence of throws to finish a 501 darts game, including
   whether it ends in double 16 or 20 and its contingency score */
class FinishSequence
{
	// finish sequence
	vector<int> sequence = {};
	// whether finish sequence ends in double 16 or 20
	bool ends_d16_d20 = false;
	// contingency score of finish sequence
	int contingency = 0;

public:
	// default constructor
	FinishSequence();
	// destructor
	~FinishSequence();

	// get length in throws of finish sequence
	int get_length() const;
	// get first throw of finish
	int get_first_throw() const;
	// set finish sequence
	void set_sequence(const vector<int>&);
	// get whether finish ends in double 16 or 20
	bool get_ends_d16_d20() const;
	// set whether finish ends in double 16 or 20
	void set_ends_d16_d20(bool);
	// get contingency score of finish
	int get_contingency() const;
	// set contingency score of finish
	void set_contingency(int);
};

