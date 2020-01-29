#include <vector>
#include <iostream>
#include <sstream>
#include <memory>
#include <math.h>

#include "monte.h"

namespace cs_ai {
	// full retard monte carlo
	AImonte::AImonte(int ig) {
		n = ig;
	}

	amove AImonte::give_move(const std::vector<pii>& moves_p, const State& s) {
		std::vector<std::unique_ptr<AI>> plys;
		for (int i = 0; i < s.nply; i++)
			plys.push_back(std::make_unique<AIrand>());

		std::vector<amove> moves;
		for (auto m : moves_p) {
			moves.push_back(amove(m, 0));
			moves.push_back(amove(m, 1));
		}

		int quota = n/((int)moves.size());

		int bmove_i = -1;
		int bscore = 0;
		//std::cout << "\nTURN " << s.turn << std::endl;
		for (int i = 0; i < (int)moves.size(); i++) {
			int score = 0;
			for (int k = 0; k < quota; k++) {

				if(simulate(s.toplay, s, moves, i, plys))
					score++;
			}
			if ((bmove_i == -1) || (score > bscore)) {
				bscore = score;
				bmove_i = i;
			}
			/*std::ostringstream sx;
			sx << j << " > ";
			print_amove(amove(moves[i], go), sx);
			sx << " score\t" << (double)score/(double)n << std::endl;
			std::cout << sx.str(); */
		}/*
		std::ostringstream ss;
		ss << "turn " << s.turn << " : ";
		ss << s.turn << ">";
		ss << "chosen ";
		print_amove(amove(moves[bmove], bgo), ss);
		ss << " mit score " << (double)max/(double)n*100 << "%";
		std::cout << ss.str() << std::endl;*/
		return moves[bmove_i]; 
	}
	AImonteUCT::AImonteUCT(int ig, double Cg) {
		n = ig;
		C = Cg;
	}

	int UCT_select_move(const double C, const std::vector<amove>& moves, const std::vector<int>& moves_playcount, const std::vector<int>& moves_wincount, const int quota) {
		int chosen = -1;
		double chosen_score = 0;
		for (int i = 0; i < (int)moves.size(); i++) {
			if (moves_playcount[i] == 0)
				return i;

			double exploit = (double)moves_wincount[i] / (double)moves_playcount[i];
			double explora = sqrt(log((double)quota) / (double)moves_playcount[i]);
			double score = exploit + C*explora;
			if (chosen == -1 || chosen_score < score) {
				chosen = i;
				chosen_score = score;
			}
		}
		return chosen;
	}

	int simulate(int player, const State& s, const std::vector<amove>& moves, const int move_i, std::vector<std::unique_ptr<AI>>& plys) {
		State ss = State(s);
		ss.step(moves[move_i]);
		return ss.one_game(plys) == player;
	}

	amove AImonteUCT::give_move(const std::vector<pii>& moves_p, const State& s) {
		int quota = 0;

		std::vector<std::unique_ptr<AI>> plys; // TODO maybe put this in the struct if it's slow, probably not
		for (int i = 0; i < s.nply; i++)
			plys.push_back(std::make_unique<AIrand>());

		std::vector<amove> moves;
		for (auto m : moves_p) {
			moves.push_back(amove(m, 0));
			moves.push_back(amove(m, 1));
		}

		std::vector<int> moves_playcount(moves.size(), 0);
		std::vector<int> moves_wincount(moves.size(), 0);
		while (quota < n) {
			int move_i = UCT_select_move(C, moves, moves_playcount, moves_wincount, quota);
			int win = simulate(s.toplay, s, moves, move_i, plys);
			if (win) {
				moves_wincount[move_i]++;
			}
			moves_playcount[move_i]++;
			quota++;
		}

		int bmove_i = -1;
		double bscore = 0.;
		std::ostringstream ss;
		ss << std::endl << "UCT turn " << s.turn << " : ";
		ss << s.turn << ">" << std::endl;
		for (int i = 0; i < (int)moves.size(); i++) {
			double score = (double)moves_wincount[i]/(double)moves_playcount[i];
			if (bmove_i == -1 || score > bscore) {
				bmove_i = i;
				bscore = score;
			}
			print_amove(moves[i], ss);
			ss << " mit score " << score*100 << "% " << moves_playcount[i] << std::endl;
		}
		ss << "chosen ";
		print_amove(moves[bmove_i], ss);
		ss << " mit score " << bscore*100 << "%";
		std::cout << ss.str() << std::endl;

		return moves[bmove_i];
	}
}
