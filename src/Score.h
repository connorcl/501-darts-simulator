#pragma once

#include <unordered_map>

using std::unordered_map;


// Represents the current score of each of two players
class Score
{
	// unordered map with player IDs as keys and scores as values
	unordered_map<int, int> scores;
	// starting score (501 in game of 501 darts)
	int starting_score;

public:
	// constructor which takes the IDs of two players and their starting score
	Score(int, int, int);
	// destructor
	~Score();

	// get a player's score based on their ID
	int get_score(int) const;
	// check if updating by amount is invalid (leads to bust)
	bool check_bust(int, int) const;
	// check if updating by amount results in win (score of 0)
	bool check_win(int, int) const;
	/* update (decrement) a player's score by an amount, and return whether
	   the game is over (score reaches 0 exactly) */
	bool update(int, int);
	// reset both scores to starting score
	void reset();
};