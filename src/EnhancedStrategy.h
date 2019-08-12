#pragma once

#include "Strategy.h"
#include <map>
#include "FinishSequence.h"
#include <vector>

using std::map;
using std::vector;


/* Represents an enhanced strategy which takes into account
   imperfect accuracy */
class EnhancedStrategy :
	public Strategy
{
	// map of scores from which finish is possible to finish sequences
	static map<int, FinishSequence> finish_sequences;
	// whether map of scores to finish sequences is initialized
	static bool initialized;

	// get target index from index of array of only doubles
	static int double_to_target_index(int);

	/* remove all sequences which do not end in double 16 or 20, as long
	   as at least one does */
	static void keep_ends_d16_d20(vector<vector<int>>&);

	// calculates and returns the contingency score of a particular score
	static int get_contingency_of_score(int);
	// calculates and returns the contingency score of a particular sequence of throws
	static int get_contingency_of_seq(const vector<int>&);
	// return the best contingency score of a vector of throw sequences
	static int get_best_contingency(const vector<vector<int>>&);
	// remove all sequences whose contingency score is lower than the best
	static void keep_best_contingency(vector<vector<int>>&);

	// return a reference to the sequence with the highest first throw value
	static const vector<int>& get_highest_first_value(const vector<vector<int>>&);
	// return a reference to the best throw sequence
	static const vector<int>& get_best_seq(vector<vector<int>>&);

public:
	// initialize score states, calculating information about finish possible from each score
	static void initialize();

	// constructor
	EnhancedStrategy();
	// destructor
	~EnhancedStrategy();
	/* get best throw, based on player's score, number of darts remaining and
	   player's score at the start of the turn */
	virtual int select_target(int, int, int) override;
};