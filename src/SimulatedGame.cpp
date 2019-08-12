#include "SimulatedGame.h"
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::to_string;

// play a round of 3 throws for a player and return whether the game is over
bool SimulatedGame::play_round(int player_index)
{
	// get reference to current player
	const Player* player = players[player_index];
	// get id of current player
	int player_id = player->get_id();
	// get start-of-round score
	int starting_score = score.get_score(player->get_id());
	// total value of turn
	int total_value = 0;

	bool round_over = false; // whether round is over
	int target_hit = -1; // actual target hit
	int throw_value = 0; // value of throw
	// for each of 3 darts in round, if round is not over
	for (int i = 3; i >= 1 && !round_over; i--) {
		// select and perform throw
		target_hit = player->select_and_throw(dartboard, starting_score - total_value, i, starting_score);
		// get its value, 0 if target hit is -1 (miss)
		throw_value = (target_hit == -1) ? 0 : Dartboard::get_targets()[target_hit]->get_value();
		// update total value
		total_value += throw_value;
		// if total value leads to bust, make total value 0 and end round
		if (score.check_bust(player_id, total_value)) {
			total_value = 0;
			round_over = true;
		}
		// round is over if throw leads to score of 0
		else if (score.check_win(player_id, total_value)) {
			// if throw was not a double, round is invalidated
			if (Dartboard::get_targets()[target_hit]->get_multiplier() != 2) {
				total_value = 0;
			}
			round_over = true;
		}
	}
	// update score and return whether game is won
	return score.update(player_id, total_value);
}

// play a set consisting of a number of games and return the index of the winner
int SimulatedGame::play_set(int set_length)
{
	// wins of each player by player index
	array<int, 2> wins = { 0, 0 };
	// index of winner
	int set_winner_index = -1;

	int game_winner_index = -1; // winner of current game
	// while no winner has been recorded
	do {
		// play a game and get the index of the winner
		game_winner_index = play_game();
		// increment wins for this player
		wins[game_winner_index]++;
		// if wins is over half of total sets to be played
		if (wins[game_winner_index] >= (set_length + 1) / 2) {
			// record winner of set
			set_winner_index = game_winner_index;
		}
	} while (set_winner_index < 0);

	// return index of winner of set
	return set_winner_index;
}

// play a match consisting of a number of sets and record the result
void SimulatedGame::play_match(int match_length, int set_length)
{
	int win_threshold = (match_length + 1) / 2; // sets needed to win match
	array<int, 2> result = { 0 }; // set wins for each player
	bool match_won = false; // whether match has been won
	
	// index of winner of set
	int set_winner_index = -1;
	// while match has not been won
	do {
		// play set and get winner
		set_winner_index = play_set(set_length);
		// record winner of set
		result[set_winner_index]++;
		// if match has been won
		if (result[set_winner_index] >= win_threshold) {
			// record result and end match
			simulation_results[set_winner_index][result[other_index(set_winner_index)]]++;
			match_won = true;
		}
	} while (!match_won);	
}

// play a game with only one player, returning the number of rounds taken to win
int SimulatedGame::play_game_single_player(int player_id)
{
	int player_index = -1; // index of player
	// find player index from player id
	for (int i = 0; i < 2; i++) {
		if (players[i]->get_id() == player_id) {
			player_index = i;
		}
	}

	int rounds = 0; // number of rounds played
	bool game_over = false; // whether game is won
	// while game is not over
	do {
		// play a round
		game_over = play_round(player_index);
		// increment rounds counter
		rounds++;
	} while (!game_over);

	// reset score
	score.reset();
	// return number of rounds played
	return rounds;
}

// constructor which takes 2 player obejcts and a random device, by reference
SimulatedGame::SimulatedGame(const Player& player_1, const Player& player_2, random_device& main_random_device, int first_player = -1) :
	Game(player_1, player_2, main_random_device, first_player) // call base class constructor
{
}

// destructor
SimulatedGame::~SimulatedGame()
{
}

// simulate a number of single player games and output the result
void SimulatedGame::run_simulation_single_player(int num_simulations, int player_id)
{
	// reset simulations counter
	simulations_run = 0;
	// run a number of simulations and add rounds played to total
	for (int i = 0; i < num_simulations; i++) {
		simulations_run += play_game_single_player(player_id);
	}

	// calculate average rounds needed by player to win and output result
	cout << "Average rounds needed to finish (based on " << num_simulations << " games): " << static_cast<double>(simulations_run) / num_simulations << endl;
}

// run a simulation of a number of matches
void SimulatedGame::run_simulation(int num_simulations, int match_length, int set_length)
{
	// make simulation results the correct size
	simulation_results.clear();
	for (int i = 0; i < 2; i++) {
		simulation_results.push_back(vector<int>((match_length + 1) / 2));
	}
	// reset simulations counter
	simulations_run = 0;
	// run a number of matches with specified number of sets and games each
	for (int i = 0; i < num_simulations; i++) {
		play_match(match_length, set_length);
		simulations_run++;
	}
}

// output the results of a the simulation
void SimulatedGame::output_results()
{
	int max_occurence = -1; // highest number of occurences of specific result
	int most_likely_winner = -1; // index of player who wins in most likely result
	int most_likely_margin = -1; // score of loser in most likely scenario

	// for each player
	for (int i = 0; i <= 1; i++) {
		// for each losing score
		for (int j = 0; j <= simulation_results[0].size(); j++) {
			// if occurence is higher than max previously found
			if (simulation_results[i][j] > max_occurence) {
				// update highest occurence and details of result
				max_occurence = simulation_results[i][j];
				most_likely_winner = i;
				most_likely_margin = j;
			}
		}
	}

	// output title with names of players
	cout << players[0]->get_name() << " : " << players[1]->get_name() << " - Likelihood based on " << simulations_run << " matches" << endl;
	cout << "--------------------" << endl;
	// for each player index
	for (int i = 0; i <= 1; i++) {
		// for each possible match score (in sets) of losing player
		for (int j = 0; j < simulation_results[0].size(); j++) {
			// for first player
			if (i == 0) {
				// output match result for first player's win
				cout << to_string(simulation_results[0].size()) << " : " << j << " - ";
			}
			// for second player
			else if (i == 1) {
				// output match result for second player's win
				cout << j << " : " << to_string(simulation_results[0].size()) << " - ";
			}
			// output probability of result based on simulated matches
			cout << (static_cast<double>(simulation_results[i][j]) / simulations_run) * 100 << "%";
			// indicate which result is most likely
			if (i == most_likely_winner && j == most_likely_margin) {
				cout << " [Most likely result]";
			}
			cout << endl;
		}
	}
}