#pragma once

#include "Game.h"

#include <vector>
#include <string>

using std::vector;
using std::string;

// Represents an interactive game in which the current play is rendered graphically
class InteractiveGame :
	public Game
{
	// vector of strings representing lines in an ASCII dartboard
	static const vector<string> ascii_dartboard;

	/* coordinates of upper left hand character of 2x2 square area which visually
	   indicates a hit of that target. Ordered like Dartboard::targets */
	static const int target_locations[62][2];

	// render the scores of each player
	void render_score(int, int);
	// render the ASCII dartboard
	void render_dartboard();
	// render a target area, optionally highlighting it in colour
	void render_target(int, bool);
	// clear area below dartboard
	void clear_below();
	// play a round of 3 throws for a player and return whether the game is over
	bool play_round(int) override;

public:
	// constructor which takes 2 player obejcts and a random device, by reference
	InteractiveGame(const Player&, const Player&, random_device&, int);
	// destructor
	~InteractiveGame();
};
