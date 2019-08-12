#include "Score.h"

using std::pair;


// constructor which takes the IDs of two players and their starting score
Score::Score(int player1_id, int player2_id, int game_starting_score) :
	starting_score(game_starting_score) // set starting score
{
	// insert player IDs and initial scores into scores map
	scores.insert({ player1_id, starting_score });
	scores.insert({ player2_id, starting_score });
}

// destructor
Score::~Score()
{
}

// get a player's score based on their ID
int Score::get_score(int id) const {
	return scores.at(id);
}

// check if updating by amount is invalid (leads to bust)
bool Score::check_bust(int id, int amount) const
{
	int resulting_score = scores.at(id) - amount;
	return (resulting_score < 0 || resulting_score == 1);
}

// check if updating by amount results in win (score of 0)
bool Score::check_win(int id, int amount) const
{
	return scores.at(id) == amount;
}

/* update (decrement) a player's score by an amount, and return whether
   the game is over (score reaches 0 exactly) */
bool Score::update(int id, int amount)
{
	// get player's current score
	int current_score = scores.at(id);
	// calculate resulting score
	int resulting_score = current_score - amount;

	// if resulting score is valid (between 0 and 501 excluding 1), update player's score
	if (resulting_score <= 501 && (resulting_score > 1 || resulting_score == 0)) {
		scores.at(id) = resulting_score;
		// if resulting score is 0, return true (game is over), otherwise return false
		return (resulting_score == 0);
	}
	return false;
}

// reset both scores to starting score
void Score::reset()
{
	// for each player-score pair
	for (pair<const int, int> &p : scores) {
		// set score of player to starting score
		p.second = starting_score;
	}
}
