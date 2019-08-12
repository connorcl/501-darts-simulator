#pragma once

#include "Game.h"
#include <vector>

using std::vector;


/* Represents a game of darts run as a computer simulation */
class SimulatedGame :
	public Game
{
	// results of a simulation of a number of matches
	vector<vector<int>> simulation_results;
	// number of simulations which have been run
	int simulations_run = 0;

	// play a round of 3 throws for a player and return whether the game is over
	bool play_round(int) override;
	// play a set consisting of a number of games and return the index of the winner
	int play_set(int);
	// play a match consisting of a number of sets and record the result
	void play_match(int, int);
	// play a game with only one player, returning the number of rounds taken to win
	int play_game_single_player(int);

public:
	// constructor which takes 2 player obejcts and a random device, by reference
	SimulatedGame(const Player&, const Player&, random_device&, int);
	// destructor
	~SimulatedGame();

	// simulate a number of single player games and output the result
	void run_simulation_single_player(int, int);
	// run a simulation of a number of matches
	void run_simulation(int, int, int);
	// output the results of a the simulation
	void output_results();
};