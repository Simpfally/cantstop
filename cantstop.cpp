#include <vector>
#include <utility>
#include <vector>
#include <sstream>
#include <memory>
#include <sstream>

#include "cantstop.h"


namespace cantstop {
	const joli token_per_num = {3, 5, 7, 9, 11, 13,
												11, 9, 7, 5, 3}; // how many token needed to finish a number
	// dices : 2 3 4 5 6 7 8 9 10 11 12 -> 11 columns

	// are those pairs the same or not
	bool distinc(const pii& a, const pii& b) {
		if (a.first == b.first and a.second == b.second)
			return false;
		if (a.first == b.second and a.second == b.first)
			return false;
		return true;
	}

	// return list of unique pairs from 4 dices
	std::vector<pii> gen_pos() {
		int a = rng::dice.roll();
		int b = rng::dice.roll();
		int c = rng::dice.roll();
		int d = rng::dice.roll();
		pii p1 = pii(a+b, c+d);
		pii p2 = pii(a+c, b+d);
		pii p3 = pii(a+d, b+c);
		std::vector<pii> v;

		v.push_back(p1);
		if (distinc(p1, p2)) {
			v.push_back(p2);
		}
		if (distinc(p3, p1) and distinc(p3, p2))
			v.push_back(p3);
		return v;
	}

}

// Constructor 1
State::State(int snply) {
	nply = snply;
	utokens = 0;
	toplay = 0;
	turn = 0;
	vtokens = std::vector<joli>(nply, joli(COLM, 0));
	cur_tokens = joli(COLM, 0);
	scores = joli(nply, 0);
	finished = joli(COLM, 0);
}



// Copy Constructor
State::State (const State& s) {
	nply = s.nply;
	utokens = s.utokens;
	toplay = s.toplay;
	turn = s.turn;
	vtokens = std::vector<joli>(s.nply, joli(COLM, 0));
	cur_tokens = joli(COLM, 0);
	scores = joli(s.nply, 0);
	finished = joli(COLM, 0);
	// no question asked copy
	for (int i = 0; i < s.nply; i++) {
		scores[i] = s.scores[i];
		for (int j = 0; j < COLM; j++) {
			vtokens[i][j] = s.vtokens[i][j];
		}
	}
	for (int j = 0; j < COLM; j++) {
		cur_tokens[j] = s.cur_tokens[j];
		finished[j] = s.finished[j];
	}
}

void State::print(std::ostringstream& ss) const {
	ss << "Turn " << turn << " ";
	for (int i = 0; i < nply; i++) {
		ss << "Player" << i << "[" << scores[i] << "]";
		if (i < nply - 1) {
			ss << ", ";
		}
	}
	ss << std::endl;
	ss << "Next to play : " << toplay << " has " << 3 - utokens << " tokens left" << std::endl;
	for (int i = 0; i < COLM; i++) {
		int dice = i + 2;
		if (finished[i]) {
			int p = finished[i]-1;
			ss << "Dice " << dice << " taken by player " << p << std::endl;						
		} else {
			for (int p = 0; p < nply; p++) {
				int x = cantstop::token_per_num[i] - vtokens[p][i];
				int y = 0;
				if (p == toplay) {
					y = cur_tokens[i];
				}
				ss << "Dice " << dice << "\t Player " << p << " ";
				for (int j = 0; j < cantstop::token_per_num[i]; j++) {
					if (j >= x) {
						ss << "- ";
					} else if (j >= x - (y - vtokens[p][i])) {
						ss << "x ";
					} else {
						ss << "* ";
					}
				}
				ss << std::endl;
			}
		}
	}
}


int State::playable(int k) const {
	if (finished[k])  // the column is already taken
		return -1;
	if (cur_tokens[k] >= cantstop::token_per_num[k]) // there is already enough token on that column
		return -1;
	return k;
}


// one more move : can playonly one element of the pair, or both but separately
std::vector<pii> State::possible_moves() const {
	std::vector<pii> v = cantstop::gen_pos();

	std::vector<pii> p;
	for (pii k : v) { // for each unique pair of dice
		int a = playable(k.first);
		int b = playable(k.second);


		if (a == -1 and b == -1)
			continue;

		if (a == -1) {
			a = b;
			b = -1;
		}
		// a != -1 now

		int tokens = 3 - utokens; // token left

		if (tokens <= 1) { // one or less token left
			int ac = cur_tokens[a] > 0;
			int bc = 0;
			if (b != -1)
				bc = cur_tokens[b] > 0;

			if (ac && bc) { // both numbers are already being played
				p.push_back(pii(a, b));
			} else if (tokens == 1) { // only 1 token left
				if (ac or bc) {
					p.push_back(pii(a, b)); // can use one token
				} else {
					p.push_back(pii(a, -1));
					if (b != -1)
						p.push_back(pii(b, -1));
				}
			} else { // zero token to place
				if (ac) {
					p.push_back(pii(a, -1));
				} else if (bc) {
					p.push_back(pii(b, -1));
				}
			}
		} else { // as many token as we want
			p.push_back(pii(a, b));
		}
	}
	return p;
}


void State::place_tokens(int a) {
	if (a != -1) {
		if (cur_tokens[a] > 0) {
			cur_tokens[a] = std::min(cur_tokens[a] + 1, cantstop::token_per_num[a]);
		} else {
			utokens++;
			cur_tokens[a] = vtokens[toplay][a] + 1;
		}
	}
}
void State::clean() {
	utokens = 0;
	for (int i = 0; i < COLM; i++) {
		cur_tokens[i] = 0;
	}
}
void State::nextply() {
		toplay = (toplay + 1)%nply;
}


void State::save_pos() {
	for (int i = 0; i < COLM; i++) {
		// take max from cur_tokens and our saved tokens
		vtokens[toplay][i] =	std::min(std::max(vtokens[toplay][i], cur_tokens[i]), cantstop::token_per_num[i]);
	}
	for (int i = 0; i < COLM; i++) {
		if ((vtokens[toplay][i] == cantstop::token_per_num[i]) && finished[i] == 0) {
			scores[toplay]++;
			finished[i] = toplay + 1;
		}
	}
}

int State::step(amove m) {
	place_tokens(m.first.first);
	place_tokens(m.first.second);
	if (not m.second) { // player decided to stop after placing tokens
		save_pos();
		nextply();
		clean();
		return false;
	}
	return true;
}

int State::finish() const {
	for (int i = 0; i < nply; i++) {
		if (scores[i] >= 3)
			return i;
	}
	return -1;
}

void State::big_step(std::vector<std::unique_ptr<cs_ai::AI>>& players) {
	turn++;
	auto pm = possible_moves();
	if (pm.size() == 0) {
		nextply();
		clean();
		return;
	} 
	auto m = players[toplay]->give_move(pm, *this);
	step(m);
}

int State::one_game(std::vector<std::unique_ptr<cs_ai::AI>>& players) {
	int i = 0;
	//ostringstream ss;
	while(finish() == -1) {
		big_step(players);
		
		i++;
	}

	return finish();
}

// TODO cleanup
void print_amove(amove am, std::ostringstream& s) {
	pii d = am.first;
	int a = d.first + 2;
	int b = d.second + 2;
	s << "(";
	if (d.first == -1) {
		s << b;
	} else	if (d.second == -1) {
		s << a;
	} else {
		s << a << ", " << b;
	}
	s << ", ";
	if (am.second == 0) {
		s << "stop";
	} else {
		s << "go";
	}
	s << ")";
}

// TODO cleanup
void print_fvec(std::ostringstream& s, std::vector<amove> v) {
	s << "[";
	int i = 0;
	for (amove am : v) {
		print_amove(am, s);
		if (++i < int(v.size()))
			s << ", ";
	}
	s << "]" << std::endl;
}
