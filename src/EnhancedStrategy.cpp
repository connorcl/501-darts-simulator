#include "EnhancedStrategy.h"

#include "Dartboard.h"


map<int, FinishSequence> EnhancedStrategy::finish_sequences = {};

bool EnhancedStrategy::initialized = false;

// get target index from index of array of doubles only
int EnhancedStrategy::double_to_target_index(int i)
{
	/* return 61 (outer bull) if final index (20), otherwise
	   index + 20 (offset) */
	return (i == 20) ? 61 : 20 + i;
}

/* remove all sequences which do not end in double 16 or 20, as long
   as at least one does */
void EnhancedStrategy::keep_ends_d16_d20(vector<vector<int>> &seqs) 
{
	// if no sequences, return
	if (seqs.empty()) {
		return;
	}

	// calculate score based on value of throws in sequence
	int score = 0;
	for (int i : seqs[0]) {
		score += Dartboard::get_targets()[i]->get_value();
	}

	// get length of throw sequences
	int n = static_cast<int>(seqs[0].size());
	// indices of sequences ending in double 16 or 20
	vector<int> indices_ends_d16_d20;

	int base = 0;
	/* find each sequence wihch ends in double 16 or 20
	   and record its index */
	for (unsigned int i = 0; i < seqs.size(); i++) {
		base = Dartboard::get_targets()[seqs[i][n - 1]]->get_base();
		if (base == 20 || base == 16) {
			indices_ends_d16_d20.push_back(i);
		}
	}

	// if at least one sequence ends with double 16 or 20
	if (!indices_ends_d16_d20.empty()) {
		// move all sequences ending in double 16 or 20 to front of vector
		for (int i = 0; i < indices_ends_d16_d20.size(); i++) {
			seqs[i] = seqs[indices_ends_d16_d20[i]];
		}
		// resize to number of sequences ending in double 16 or 20
		seqs.resize(indices_ends_d16_d20.size());
		// record that finish ending in double 16 or 20 is possible
		finish_sequences.at(score).set_ends_d16_d20(true);
	}
}

// calculates and returns the contingency score of a particular score
int EnhancedStrategy::get_contingency_of_score(int score)
{
	// return 0 if score is invalid
	if (score == 1 || score < 0) {
		return 0;
	}
	// return 6 (max) if score is 0 (win)
	else if (score == 0) {
		return 6;
	}

	// initialize contingency score as 0
	int contingency_score = 0;

	// if finish is possible
	if (finish_sequences.count(score) != 0) {
		// get reference to finish sequence
		FinishSequence& finish_seq = finish_sequences.at(score);
		// update contingency score, weighted by 4 - (number of darts finish)
		contingency_score += (4 - finish_seq.get_length());
		// if finish ending in double 16 or 20 is possible
		if (finish_seq.get_ends_d16_d20()) {
			contingency_score += (4 - finish_seq.get_length());
		}
	}

	return contingency_score;
}

// calculates and returns the contingency score of a particular sequence of throws
int EnhancedStrategy::get_contingency_of_seq(const vector<int> &seq)
{
	// calculate score based on value of throws in sequence
	int score = 0;
	for (int i : seq) {
		score += Dartboard::get_targets()[i]->get_value();
	}

	// get first throw, most likely miss(es) on this determine contingency
	int first_throw = seq[0];
	int score_if_miss = 0;
	int contingency_score = 0;

	// if first throw is outer bull, most likey miss is inner
	if (first_throw == 60) {
		score_if_miss = score - 50;
		contingency_score = get_contingency_of_score(score_if_miss);
	}
	// if first throw is inner bull, most likely miss is outer
	else if (first_throw == 61) {
		score_if_miss = score - 25;
		contingency_score = get_contingency_of_score(score_if_miss);
	}
	// if first throw is double or triple, most likely miss is single
	else if (Dartboard::get_targets()[first_throw]->get_multiplier() > 1) {
		score_if_miss = score - Dartboard::get_targets()[first_throw]->get_base();
		contingency_score = get_contingency_of_score(score_if_miss);
	}
	// if first throw is single, most likely misses are neighbouring singles
	else {
		// for each neighbour
		for (int i = 0; i <= 1; i++) {
			// calculate contingency score and add it to total
			score_if_miss = score - Dartboard::get_neighbours()[i][Dartboard::get_targets()[first_throw]->get_base()];
			contingency_score += get_contingency_of_score(score_if_miss);
		}
		// divide by 2 to make contingency score average based on two neighbours (rounded down)
		contingency_score /= 2;
	}

	return contingency_score;
}

// return the best contingency score of a vector of throw sequences
int EnhancedStrategy::get_best_contingency(const vector<vector<int>> &seqs)
{
	int current_score = 0;
	int best_score = 0;

	// traverse vector of sequences
	for (const vector<int> &seq : seqs) {
		// calculate contingency of sequence
		current_score = get_contingency_of_seq(seq);
		// update best if new best is found
		if (current_score > best_score) {
			best_score = current_score;
		}
	}

	return best_score;
}

// remove all sequences whose contingency score is lower than the best
void EnhancedStrategy::keep_best_contingency(vector<vector<int>> &seqs)
{
	// if vector of sequences is empty, return
	if (seqs.empty()) {
		return;
	}

	// calculate score based on value of throws in sequences
	int score = 0;
	for (int i : seqs[0]) {
		score += Dartboard::get_targets()[i]->get_value();
	}
	// get number of darts being considered
	int n = static_cast<int>(seqs[0].size());
	// get best contingency score
	int best_contingency_score = get_best_contingency(seqs);
	// record best contingency score
	finish_sequences.at(score).set_contingency(best_contingency_score);

	// generate vector of indices of sequences with best contingency score
	vector<int> indices_best_contingency;
	for (unsigned int i = 0; i < seqs.size(); i++) {
		if (get_contingency_of_seq(seqs[i]) == best_contingency_score) {
			indices_best_contingency.push_back(i);
		}
	}

	// move all sequences whose contingency is lower than best to front of vector
	for (int i = 0; i < indices_best_contingency.size(); i++) {
		seqs[i] = seqs[indices_best_contingency[i]];
	}
	// resize vector to number of seuqences with best contingency
	seqs.resize(indices_best_contingency.size());
}

// return a reference to the sequence with the highest first throw value
const vector<int>& EnhancedStrategy::get_highest_first_value(const vector<vector<int>> &seqs)
{
	int current_value = 0;
	int best_value = 0;
	int best_index = 0;

	// traverse vector of sequences
	for (unsigned int i = 0; i < seqs.size(); i++) {
		// calculate value of first throw
		current_value = Dartboard::get_targets()[seqs[i][0]]->get_value();
		// update best value and index if new best is found
		if (current_value > best_value) {
			best_value = current_value;
			best_index = i;
		}
	}

	return seqs[best_index];
}

// return a reference to the best throw sequence
const vector<int>& EnhancedStrategy::get_best_seq(vector<vector<int>> &seqs)
{
	// delete any sequences not ending in double 16 or 20, if applicable
	keep_ends_d16_d20(seqs);
	// delete any sequences whose contingency value is lower than the best
	keep_best_contingency(seqs);
	// of the remaining sequences, choose that with highest first throw value
	return get_highest_first_value(seqs);
}

// initialize finish sequences, calculating information about finishes possible from each score
void EnhancedStrategy::initialize()
{
	// return if already initialized
	if (initialized) {
		return;
	}

	// get the values of all throws
	int throw_values[62];
	for (int i = 0; i < 62; i++) {
		throw_values[i] = Dartboard::get_targets()[i]->get_value();
	}

	// arrays of 1, 2 and 3 dart finishes
	int finish_values_1_dart[21];
	int finish_values_2_darts[62][21];
	int finish_values_3_darts[62][62][21];

	// calculate all possible 1 dart finishes and their values
	for (int i = 0; i < 20; i++) {
		finish_values_1_dart[i] = (i + 1) * 2;
	}
	finish_values_1_dart[20] = 50;

	// calculate all possible 2 dart finishes and their values
	for (int i = 0; i < 62; i++) {
		for (int j = 0; j < 21; j++) {
			finish_values_2_darts[i][j] = throw_values[i] + finish_values_1_dart[j];
		}
	}

	// calculate all possible 3-dart finishes and their values
	for (int i = 0; i < 62; i++) {
		for (int j = 0; j < 62; j++) {
			for (int k = 0; k < 21; k++) {
				finish_values_3_darts[i][j][k] = throw_values[i] + finish_values_2_darts[j][k];
			}
		}
	}

	// target index of single dart finish
	int finish_1 = -1;
	// vector of 2-dart finish sequences (sequences of target indices)
	vector<vector<int>> finishes_2;
	// vector of 3-dart finish sequences (sequences of target indices)
	vector<vector<int>> finishes_3;

	/* loop through scores from 2 to 170 (max score which allows finish in 3 or less)
	   and calculate best 1, 2 and 3-dart finish for each score if these are possible */
	for (int s = 2; s <= 170; s++) {
		//cout << "Score: " << s << endl;
		
		// reset finish_1
		finish_1 = -1;
		// establish if 1-dart finish is possible
		// maximum range of 1-dart finishes
		if (s >= 2 && s <= 50) {
			for (int i = 0; i < 21 && finish_1 < 0; i++) {
				if (s == finish_values_1_dart[i]) {
					finish_1 = double_to_target_index(i);
					finish_sequences[s].set_contingency(get_contingency_of_seq({ finish_1 }));
				}
			}
		}


		// find all 2-dart finishes possible

		finishes_2.clear();

		if (s >= 3 && s <= 110 && finish_1 < 0) {
			for (int i = 0; i < 62; i++) {
				for (int j = 0; j < 21; j++) {
					if (s == finish_values_2_darts[i][j]) {
						finishes_2.push_back({ i, double_to_target_index(j) });
					}
				}
			}
		}

		// find all 3-dart finishes possible

		finishes_3.clear();

		if (s >= 4 && s <= 170 && finish_1 < 0 && finishes_2.empty()) {
			for (int i = 0; i < 62; i++) {
				for (int j = 0; j < 62; j++) {
					for (int k = 0; k < 21; k++) {
						if (s == finish_values_3_darts[i][j][k]) {
							finishes_3.push_back({ i, j, double_to_target_index(k) });
						}
					}
				}
			}
		}

		// for each number of darts from which finish is possible, calculate and record best finish
		if (finish_1 >= 0) {
			//cout << "1 dart finish: " << Dartboard::get_targets()[finish_1]->get_multiplier() << "x" << Dartboard::get_targets()[finish_1]->get_base() << " Cont: " << score_states[s].get_finish_contingency(1) << endl;
			finish_sequences[s].set_sequence({ finish_1 });
		}
		else if (!finishes_2.empty()) {
			finish_sequences.insert({ s, FinishSequence() });
			vector<int> best_finish_2_darts = get_best_seq(finishes_2);
			finish_sequences.at(s).set_sequence(best_finish_2_darts);
			//cout << "2 dart finish: " << Dartboard::get_targets()[best_finish_2_darts[0]]->get_multiplier() << "x" << Dartboard::get_targets()[best_finish_2_darts[0]]->get_base() << " " << Dartboard::get_targets()[best_finish_2_darts[1]]->get_multiplier() << "x" << Dartboard::get_targets()[best_finish_2_darts[1]]->get_base() << " Cont: " << score_states[s].get_finish_contingency(2) << endl;
		}
		else if (!finishes_3.empty()) {
			finish_sequences.insert({ s, FinishSequence() });
			vector<int> best_finish_3_darts = get_best_seq(finishes_3);
			finish_sequences.at(s).set_sequence(best_finish_3_darts);
			//cout << "3 dart finish: " << Dartboard::get_targets()[best_finish_3_darts[0]]->get_multiplier() << "x" << Dartboard::get_targets()[best_finish_3_darts[0]]->get_base() << " " << Dartboard::get_targets()[best_finish_3_darts[1]]->get_multiplier() << "x" << Dartboard::get_targets()[best_finish_3_darts[1]]->get_base() << " " << Dartboard::get_targets()[best_finish_3_darts[2]]->get_multiplier() << "x" << Dartboard::get_targets()[best_finish_3_darts[2]]->get_base() << " Cont: " << score_states[s].get_finish_contingency(3) << endl;
		}
	}
	// set initialized flag
	initialized = true;
}

// constructor
EnhancedStrategy::EnhancedStrategy()
{
}

// destructor
EnhancedStrategy::~EnhancedStrategy()
{
}

/* get best throw, based on player's score, opponent's score, number of darts remaining and
   player's score at the start of the turn */
int EnhancedStrategy::select_target(int score, int darts_remaining, int starting_score)
{
	// calculate best throws if not calculated already
	if (!initialized) {
		initialize();
	}

	// if finish is not possible
	if (finish_sequences.count(score) == 0) {
		// return triple 20
		return Dartboard::get_target_index(20, 3);
	}
	else {
		// if a single throw remains and bust is possible
		if (darts_remaining == 1 && score < 60 && finish_sequences.count(starting_score) != 0) {
			/* get length of current possible finish sequence, whether it ends with a double
			   16 or 20 and its contingency score */
			int current_finish_length = finish_sequences.at(score).get_length();
			int current_finish_ends_d16_d20 = finish_sequences.at(score).get_ends_d16_d20();
			int current_finish_contingency = finish_sequences.at(score).get_contingency();
			/* get length of finish sequence if throwing for bust, whether it ends with a double
			   16 or 20 and its contingency score */
			int bust_finish_length = finish_sequences.at(starting_score).get_length() + 1;
			int bust_finish_ends_d16_d20 = finish_sequences.at(starting_score).get_ends_d16_d20();
			int bust_finish_contingency = finish_sequences.at(starting_score).get_contingency();
			// if throw for bust is better, throw for bust
			if (bust_finish_length <= current_finish_length &&
				bust_finish_ends_d16_d20 >= current_finish_ends_d16_d20 &&
				bust_finish_contingency > current_finish_contingency) {
				return Dartboard::get_target_index(20, 3);
			}
		}
		// if bust is not possible or not advantageous, throw for current finish
		return finish_sequences.at(score).get_first_throw();
	}
}
