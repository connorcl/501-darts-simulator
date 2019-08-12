#include "Player.h"
#include "BasicStrategy.h"
#include "EnhancedStrategy.h"
#include "InteractiveStrategy.h"


int Player::current_id = 0;

/* constructor, which takes a player name, accuracy, and which strategy
   they make use of */
Player::Player(const string& player_name, int player_accuracy, char strategy_type) :
	name(player_name), accuracy(player_accuracy)
{
	// calculate player id
	current_id++;
	id = current_id;
	// create relevant strategy object
	switch (strategy_type) {
	case 'b':
		strategy = new BasicStrategy(); break;
	case 'e':
		strategy = new EnhancedStrategy(); break;
	case 'i':
		strategy = new InteractiveStrategy(); break;
	default:
		strategy = new BasicStrategy(); break;
	}
}

// select and perform throw based on strategy, returning index of target hit
int Player::select_and_throw(const Dartboard& dartboard, int score, int darts_remaining, int starting_score) const
{
	int best_throw = strategy->select_target(score, darts_remaining, starting_score);
	return dartboard.throw_by_target(best_throw, accuracy);
}

Player::~Player()
{
	// delete strategy object
	delete strategy;
}

// getter for id
int Player::get_id() const
{
	return id;
}

// getter for name
const string& Player::get_name() const
{
	return name;
}

// getter for accuracy
int Player::get_accuracy() const
{
	return accuracy;
}