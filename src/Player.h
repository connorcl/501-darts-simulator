#pragma once

#include <string>
#include "Strategy.h"
#include "Dartboard.h"

using std::string;


/* Represents a player in a game of darts */
class Player
{
	// incremented and taken as player ID every time a player is created
	static int current_id;
	int id; // player id
	string name; // player name
	int accuracy; // player accuracy %
	Strategy* strategy; // player's Strategy object

public:
	/* constructor, which takes a player name, accuracy, and which strategy
	   they make use of */
	Player(const string&, int, char);
	// destructor
	virtual ~Player();
	// getter for id
	int get_id() const;
	// getter for name
	const string& get_name() const;
	// getter for accuracy
	int get_accuracy() const;
	// select and perform throw based on strategy, returning index of target hit
	int select_and_throw(const Dartboard&, int, int, int) const;
};