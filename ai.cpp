#include <vector>
#include <iostream>
#include <sstream>
#include <memory>

#include "cantstop.h"
#include "dicey.h"
#include "ai.h"


namespace cs_ai {
	amove AI::give_move(const std::vector<pii>& moves, const State& s) {
		(void)s;
		return amove(moves[0], 0);
	}

	amove AIdebug::give_move(const std::vector<pii>& moves, const State& s) {
		int x = rng::dice.rollc(0, moves.size() - 1);
		int go = rng::dice.rollc(0, 1);
		std::ostringstream ss;
		ss << "\nTURN " << s.turn << std::endl;
		//print_fvec(ss, moves);
		print_amove(amove(moves[x], go), ss);
		std::cout << ss.str() << std::endl;
		return amove(moves[x], go);
	}
	amove AIrand::give_move(const std::vector<pii>& moves, const State& s) {
		(void)s;
		int x = rng::dice.rollc(0, moves.size() - 1);
		int go = rng::dice.rollc(0, 1);
		return amove(moves[x], go);
	}


	amove AIcantstop::give_move(const std::vector<pii>& moves, const State& s) {
		int x = 0;
		int got = 0;
		for (int i = 0; i < (int)moves.size(); i++) {
			int a = moves[i].first;
			int b = moves[i].second;
			int fa = s.cur_tokens[a] + 1 == cantstop::token_per_num[a];
			int fb = s.cur_tokens[b] + 1 == cantstop::token_per_num[b];
			if (!got && (fa || fb)) {
				x = i;
				got = 1;
			}
		}
		int go = 0;
		if (!got) {
			x = rng::dice.rollc(0, moves.size() - 1);
			go = 1;
		}
			//go = dice.rollc(0, 1);
		return amove(moves[x], go);
	}

	// attribute a value to each column, stop once thresold is reached
	amove AIvalue::give_move(const std::vector<pii>& moves, const State& s) {
		double threshold = 4*(double)s.turn/5;
		double sum = 0;

		for (int i =0; i < COLM; i++) {
			if (s.cur_tokens[i] > 0) {
				int dif = s.cur_tokens[i] - s.vtokens[s.toplay][i] + 1; // how many token in col
				int sommet = cantstop::token_per_num[i] - s.cur_tokens[i] + 1; // how many token left to finish
				sum += ((double)dif+1/(double)sommet)/(double)cantstop::token_per_num[i]; // the less token needed, the more the token are valuable
			}
		}
		for (int i = 0; i < (int)moves.size(); i++) {
			int a = moves[i].first;
			int b = moves[i].second;
			int fa = s.cur_tokens[a] + 1 == cantstop::token_per_num[a];
			int fb = s.cur_tokens[b] + 1 == cantstop::token_per_num[b];
			if (fa || fb) {
				return amove(moves[i], 0);
			}
		}
		int go = 1;
		if (sum > threshold) {
			go = 0;
		}
		int x = rng::dice.rollc(0, moves.size() - 1);
		return amove(moves[x], go);
	}


	amove AIstop::give_move(const std::vector<pii>& moves, const State& s) {
		//int x = dice.rollc(0, moves.size() - 1);
		int mx = 0;
		int m = -1;
		int mj = -1;
		for (int i = 0; i < (int)moves.size(); i++) {
			int a = moves[i].first;
			int b = moves[i].second;
			int fa = s.cur_tokens[a] > 0;
			int fb = s.cur_tokens[b] > 0;
			int da = s.cur_tokens[a] + 1 == cantstop::token_per_num[a];
			int db = s.cur_tokens[b] + 1 == cantstop::token_per_num[b];
			if (da || db) {
				return amove(moves[i], 0);
			} else if (fa+fb == 0) {
				// 2 3 4 5 6 7 8 9 10 11 12
				// 0 1 2 3 4 5 6 7 8  9  10
				int ca = abs(a - 5);
				int cb = abs(b - 5);
				int j = ca+cb;
				if (mj == -1 || (mj > j)) {
					mj = j;
					mx = i;
				}
			} else if (m == -1 || (fa+fb > m)) {
				m = fa+fb;
				mx = i;
			}
		}
		int go = 1;
		if (s.utokens > 2 && rng::dice.rolld() > 0.1) {
			go = 0;
		}
		return amove(moves[mx], go);
	}
	AIplayer::AIplayer(std::string nameg) {
		name = nameg;
	}
	amove AIplayer::give_move(const std::vector<pii>& moves_p, const State& s) {
		std::vector<amove> moves;
		for (auto m : moves_p) {
			moves.push_back(amove(m, 0));
			moves.push_back(amove(m, 1));
		}
		std::ostringstream ss;
		s.print(ss);
		for (int i = 0; i < (int)moves.size(); i++) {
			ss << i << " > ";
			print_amove(moves[i], ss);
			ss << std::endl;
		}
		ss << s.turn << " : " << name << "'s choice : ";
		int move_i;
		std::cout << ss.str();
		std::cin >> move_i;
		return moves[move_i];
	}
}
