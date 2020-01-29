#ifndef CANTSTOP_H
#define CANTSTOP_H
//#pragma once
// get a real compiler

#include <vector>
#include <utility>
#include <vector>
#include <sstream>
#include <memory>
#include <sstream>

#include "ai.h"
#include "dicey.h"

#define COLM 11

typedef std::vector<int> joli;
typedef std::pair<int, int> pii;
typedef std::pair<pii, int> amove; // (dice,dice), continue?

namespace cantstop {
	// tells to all it exists, is defined in .cpp
	extern const joli token_per_num;
	bool distinc(const pii& a, const pii& b);
	std::vector<pii> gen_pos();
}

struct State {
	int nply; // number of players TODO const
	int utokens; // current token on board, max is 3
	int toplay; //
	int turn; // total turns
	std::vector<joli> vtokens;
	joli cur_tokens;
	joli scores; // scores of each player, win if score = 3
	joli finished; // if a column is finished filled with player id + 1
	State(int snply);
	State(const State& s);
	void print(std::ostringstream& ss) const;
	int playable(int k) const;
// one more move : can playonly one element of the pair, or both but separately
	std::vector<pii> possible_moves() const ;
	int finish() const;
	void place_tokens(int a) ;
	void clean();
	void nextply();
	void save_pos();
	int step(amove m);
	void big_step(std::vector<std::unique_ptr<cs_ai::AI>>& players);
	int one_game(std::vector<std::unique_ptr<cs_ai::AI>>& players);
};


void print_amove(amove am, std::ostringstream& s);
void print_fvec(std::ostringstream& s, std::vector<amove> v);

#endif
