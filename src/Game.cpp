#include "Game.h"


// return 1 for 0 and 0 for 1, used to get the index of the other player
int Game::other_index(int i) const
{
	return (i == 0) ? 1 : 0;
}

// constructor which takes 2 player obejcts and a random device, by reference
Game::Game(const Player& player_1, const Player& player_2, random_device& main_random_device, int first_player) :
	players({ {&player_1, &player_2} }), // set up players array
	score(Score(player_1.get_id(), player_2.get_id(), 501)), // create score object
	dartboard(Dartboard(rd)), // create dartboard object
	rd(main_random_device), // initialize random device
	dist_start(uniform_int_distribution<int>(0, 1)) // initialize random distribution
{
	first_player_index = (first_player == -1) ? dist_start(rd) : first_player; // randomly select player to throw first
}

// destructor
Game::~Game()
{
}

int Game::play_game()
{
	bool game_won = false; // whether game has been won
	int player_index = 0; // index of current player in players array
	// while game is not won
	do {
		// for each player index
		for (int i = 0; i < 2 && !game_won; i++) {
			// get current player index based on index of player who throws first
			player_index = (first_player_index == 0) ? i : other_index(i);
			// play round and set index of winner
			game_won = play_round(player_index);
		}
	} while (!game_won);

	// swap player who throws first
	first_player_index = other_index(first_player_index);
	// reset player scores
	score.reset();
	// return index of player who ended game
	return player_index;
}