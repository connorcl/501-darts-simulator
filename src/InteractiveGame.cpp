#include "InteractiveGame.h"
#include <iostream>
#include "termcolor/termcolor.hpp"
#include <Windows.h>
#include <algorithm>
#include <thread>
#include <chrono>
#include "Dartboard.h"
#include "ConsoleInterface.h"

using std::cout;
using std::endl;
using termcolor::red;
using termcolor::on_red;
using termcolor::green;
using termcolor::reset;
using std::swap;


const vector<string> InteractiveGame::ascii_dartboard = {
	"MMMMMMMMMMMMMMMMMMMMMMMMMMWMMMMMMMMMWWWWWWWMMMMMMMMWMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
	"MMMMMMMMMMMMMMMMMMMMMMMMMMWNX0kxolcc::;:::::cclddk0KNWMMMMMMMMMMMMMMMMMMMMMMMMMM",
	"MMMMMMMMMMMMMMMMMMMMMWNKkdc;'...  ......... .  ....';cokKNWMMMMMMMMMMMMMMMMMMMMM",
	"MMMMMMMMMMMMMMMMMMWXkl;... .''. ....'  20  .......'.....':okXWMMMMMMMMMMMMMMMMMM",
	"MMMMMMMMMMMMMMMWXOl'. .... 5    ....':,;cc'......  1 .......,lkXWMMMMMMMMMMMMMMM",
	"MMMMMMMMMMMMMWKo,...... .. .,c;,;collooolooool:;;;;............;oKWMWMMMMMMMMMMM",
	"MMMMMMMMMMMW0l'.  12 , ..;;:clodooxOOOOOOOO0koodddolcc;.., 18 ...'l0WMMMMMMMMMMM",
	"MMMMMMMMMWKl'... ,,':;,cdxxolc:;,.:0WWMWWMMNo.',;:cldkxdcc:........'oKWMMMMMMMMM",
	"MMMMMMMMNx,........,ldxxk0Kd..... .xWMMWWMMK;......:0X0Okxdc,........,kNMMMMMMMM",
	"MMMMMMWKl........,lxxk0XNWWXc......cNMMWWMMk. ....,OWWWWX0kkxo;...:,...lKMMMMMMM",
	"MMMMMW0:... 9 ..:dxx0XWWWWMMXl....'c0KKKKKXx;'...,OWWWMMWWKkddoc' 4 ....:0WMMMMM",
	"MMMMW0;......c:lodl;;dXMWWMWWKocloodxkkkkkkxddolckNWWMWWNk:',lddo;'......;0WMMMM",
	"MMMWK:.......;odoc....,dXWNK0kkxl:;,lKNNNN0:,;:okkkOKNNO:.....:odo:.......;0WMMM",
	"MMMXc.......lxdo;.......:OOxkOKXd.. 'OWWWWx...,ONKOkkOx,.......:dxxo'......cXMMM",
	"MMWx..'....:xxOOo:'....;lood0WWWNd. .dWWMNl..,kWWWNkoooo;...':oOK0xxc,: .. .dWMM",
	"MMK:.. 14 :dxkXWMNKko:cool;.'oKWWNd..:XMMK; 'kWWNOc..,lodolkKNMWWXkxx:,'13..;0MM",
	"MWx...''':oxxKWMMWWWWKkxko'...'oKWNx.'OWWk''kWNOc....;dOxkKWMMWMWWKxko.......oWM",
	"MNl......,dkOXWMWWMWNOxONNKko:'.'oKNx,dWNo,kN0c..,cx0NWN0xkXWMWWMWXOkx:......:XM",
	"MX:......:dxxoodxkOK0kkKWWWWWWKko::oOxdOOdxOo:cdOXWWWWNX0kxOkdolc::oddc......,0M",
	"MK;......:ddl......,oddl:clooxkO0OkxxxxxxxxxxkOOkxoll:;,:ddo'......cddc.   ..O)M",
	"MK:  11 :codl.......loo, .....';:ldkkxxddxxkkdlc:,'.....,ool'......cddc. 6 ..'OM",
	"MNc..'..';odl'.....;odddodxk0KXNX0xoxkxxkkOkodOXNNXKKOkxxxxxl:;,'.'ldd:......,0M",
	"MWo......,dxxxxkO0KXXkx0NWWWWKko;,:kKdoKXxl00l,,cdOXWWWW0xkXWWNNXK0kxx:......:XM",
	"MMO'......lxxKWWWWWWW0xx0XOo:'..;kNNo.oWWk':KW0l...,cdOKkx0NWWWWWWKxxo,..'...dWM",
	"MMXc......;oxkXWWWWWWKxddl'  .;xXWNd..kWMK:.:KWW0l'. .cdddkKNWWWWNOxdc:.....,0MM",
	"MMMO'.. 8 .;dxOXWXOdc,.:ooc,;xXWWNo. ;KWWNo..;0WWWKo;cooc'.':okKN0xxc. 10 ..dWMM",
	"MMMNd. .....cxxkd;......,lddOKNWNo...lNWWWO' .,OWNXOxxo;.......:xxxo'......cXMMM",
	"MMMMXl..... .:ool;......,kKOxxkOx;...xWWWWK:..'lOkxxOKKd'.....,looc'......;0WWWM",
	"MMMMMXl.......;looc'..,dXWWWNKOdoolclk0OO0Odllooox0NWWWWXd,..:lol:;......;0WMMMM",
	"MMMMMMXo.  16 :.:lolcdKWWWWWWNd'',;:okkkkkOkl;,'.,kWWWWWWWKdoolc'.  15 .:0WMMMMM",
	"MMMMMWWNk,..','..'cdxk0XNWWWNo......oNWWWWWX:......xNWWWX0kxdo;.  .....lXWWMMMMM",
	"MMMMMMMMW0c..    ..':odxk0XXd.......kWWWWWWWo......'xXKOxxdc,..   ...,xNMMMMMMMM",
	"MMMMMMMMMMNk;.     .';;:ldxdc;,'...;0WWWWWWWO'..',;:lxxoc;,:'     .'oKWMMMMMMMMM",
	"MMMMMMMMMMMWXx;.   7  . .,:::llllc:oO0O000OOkocclllc::;..  2    .'oKWWWMMMMMMMMM",
	"MMMMMMMMWMMMMMNkc'  ..     ..';:;;:lllolllllll:;,;,''.    .....;dKWMMWMMMMMMMMMM",
	"MMMMMMMMMMMMMMMMWKx:..     . 19 . .....':;'....   17 .     .;oONWMMMMMMMMMMMMMMM",
	"MMMMMMMMMMMMMMMMMMMWKxc,..  ..',.       3        .......':d0NWMMMMMMMMMMMMMMMMMM",
	"MMMMMMMMMMMMMMMMMMMMMWWX0xo:,..        ....     ...':lx0XWMMMMMMMMMMMMMMMMMMMMMM",
	"MMMMMMMMMMMMMMMMMMMMMMWWMMWWNKOkdoolccccccclllodkOKNWWMMWWMMMMMMMMMMMMMMMMMMMMMM",
	"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
};

const int InteractiveGame::target_locations[62][2] = {
	// singles
	{46, 8},
	{54, 29},
	{40, 30},
	{58, 12},
	{31, 8},
	{63, 19},
	{25, 29},
	{17, 23},
	{19, 12},
	{62, 23},
	{15, 19},
	{25, 10},
	{62, 15},
	{16, 16},
	{59, 26},
	{20, 26},
	{47, 30},
	{53, 9},
	{32, 30},
	{39, 7},
	// doubles
	{50, 5},
	{57, 31},
	{40, 34},
	{65, 11},
	{30, 5},
	{69, 19},
	{22, 31},
	{12, 24},
	{14, 11},
	{69, 23},
	{9, 19},
	{22, 7},
	{69, 14},
	{11, 15},
	{66, 27},
	{15, 27},
	{49, 33},
	{58, 7},
	{31, 33},
	{39, 4},
	// triples
	{46, 11},
	{51, 26},
	{40, 28},
	{55, 14},
	{33, 11},
	{58, 19},
	{29, 26},
	{23, 22},
	{25, 14},
	{57, 22},
	{21, 19},
	{29, 12},
	{57, 16},
	{22, 17},
	{55, 24},
	{25, 25},
	{46, 28},
	{52, 12},
	{34, 28},
	{39, 10},
	// inner and outer bull
	{38, 17},
	{40, 19}
};

// constructor which takes 2 player obejcts and a random device, by reference
InteractiveGame::InteractiveGame(const Player& player_1, const Player& player_2, random_device& main_random_device, int first_player) :
	Game(player_1, player_2, main_random_device, first_player) // call base class constructor
{
}

// destructor
InteractiveGame::~InteractiveGame()
{
}

// render the scores of each player
void InteractiveGame::render_score(int current_player_id, int score_current_round) {
	// output each player's score
	for (const Player* p : players) {
		// get player id
		int player_id = p->get_id();
		// get current score
		int current_score = score.get_score(p->get_id());
		// if player is current player
		if (player_id == current_player_id) {
			// subtract total value this round from current score
			current_score -= score_current_round;
			// output name and score in green
			cout << green << p->get_name() << ": " << current_score << "    " << reset;
		}
		// if player is not current player
		else {
			// output name and score
			cout << p->get_name() << ": " << current_score << "    ";
		}
	}
	cout << "\n" << endl;
}

// render the ASCII dartboard
void InteractiveGame::render_dartboard()
{
	// print each line of ascii dartboard
	for (const string& line : ascii_dartboard) {
		cout << line << endl;
	}
	cout << endl;
}

// render a target area, optionally highlighting it in colour
void InteractiveGame::render_target(int target, bool colour)
{
	// exit if target is miss
	if (target < 0) {
		return;
	}
	// loop across square area
	for (int i = 0; i <= 1; i++) {
		for (int j = 0; j <= 1; j++) {
			// move cursor over dartboard to element of target area
			ConsoleInterface::move_cursor(target_locations[target][0] + i, target_locations[target][1] + j + 2);
			// if specified, draw in green
			if (colour) {
				cout << green;
			}
			// output character of dartboard
			cout << ascii_dartboard[target_locations[target][1] + j][target_locations[target][0] + i];
			// if output in colour, reset colour to default
			if (colour) {
				cout << reset;
			}
		}
	}
}

// clear area below dartboard
void InteractiveGame::clear_below()
{
	// move cursor below dartboard
	ConsoleInterface::move_cursor(0, 43);
	// print 4 lines
	ConsoleInterface::clear_screen(4);
	// reset cursor to below dartboard
	ConsoleInterface::move_cursor(0, 43);
}

// play a round of 3 throws for a player and return whether the game is over
bool InteractiveGame::play_round(int player_index)
{
	// get reference to current player
	const Player* player = players[player_index];
	// get id of current player
	int player_id = player->get_id();
	// get start-of-round score
	int starting_score = score.get_score(player_id);
	// total value of turn
	int total_value = 0;

	ConsoleInterface::move_cursor(0, 0); // reset cursor
	render_score(player_id, total_value); // print scores
	render_dartboard(); // draw dartboard

	bool round_over = false; // whether round is over
	int target_hit = -1; // actual target hit
	int throw_value = 0; // value of throw
	// for each of 3 darts in round
	for (int i = 3; i >= 1 && !round_over; i--) {
		// move cursor to below dartboard
		ConsoleInterface::move_cursor(0, 43);
		// select and perform throw
		target_hit = player->select_and_throw(dartboard, starting_score - total_value, i, starting_score);
		// get its value, 0 if target hit is -1 (miss)
		throw_value = (target_hit == -1) ? 0 : Dartboard::get_targets()[target_hit]->get_value();
		// update total value
		total_value += throw_value;
		// clear any prompts and inputs below dartboard
		clear_below();
		// highlight hit target
		render_target(target_hit, true);
		// if total value leads to bust, make total value 0 and end round
		if (score.check_bust(player_id, total_value)) {
			total_value = 0;
			round_over = true;
		}
		// round is over if throw leads to 0
		else if (score.check_win(player_id, total_value)) {
			// if throw was a double, player has won
			if (Dartboard::get_targets()[target_hit]->get_multiplier() == 2) {
				clear_below();
				// print name of winning player
				cout << "Winner: " << player->get_name() << endl;
			}
			// otherwise, round is invalidated
			else {
				total_value = 0;
			}
			round_over = true;
		}
		ConsoleInterface::move_cursor(0, 0); // reset cursor
		render_score(player_id, total_value); // print player scores
		std::this_thread::sleep_for(std::chrono::milliseconds(500)); // pause
		render_target(target_hit, false); // remove highlight of target hit
		ConsoleInterface::move_cursor(0, 43); // move cursor to below dartboard
		cout << endl; // print newline
	}
	// update player's score and return whether game is won
	return score.update(player_id, total_value);
}
