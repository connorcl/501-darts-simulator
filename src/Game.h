#pragma once

#include <array>
#include "Player.h"
#include "Score.h"
#include "Dartboard.h"
#include <random>

using std::array;
using std::random_device;
using std::uniform_int_distribution;


/* Abstract class representing a 2-player game of 501 darts */
class Game
{
protected:
	// array of players
	array<const Player*, 2> players;
	// index of player who throws first
	int first_player_index;
	// game score
	Score score;
	// dartboard used
	Dartboard dartboard;
	// reference to random device
	random_device& rd;
	// random distribution used to determine which player throws first
	uniform_int_distribution<int> dist_start;

	// return 1 for 0 and 0 for 1, used to get the index of the other player
	int other_index(int) const;
	/* pure virtual function to play a round of 3 throws for a player and return
	   whether the game is over */
	virtual bool play_round(int) = 0;
	
public:
	// constructor which takes 2 player obejcts and a random device, by reference
	Game(const Player&, const Player&, random_device&, int);
	// destructor
	virtual ~Game() = 0;
	// function to play a game of darts and return the index of the winner
	int play_game();
};