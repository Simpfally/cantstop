#ifndef MONTE_H
#define MONTE_H

#include <vector>
#include <utility>
#include "cantstop.h"
#include "dicey.h"
#include "ai.h"

typedef std::vector<int> joli;
typedef std::pair<int, int> pii;
typedef std::pair<pii, int> amove;

struct State;

namespace cs_ai {
	struct AImonte : AI {
		int n;

		AImonte(int ng);
		amove give_move(const std::vector<pii>& moves, const State& s) override;
	};
	struct AImonteUCT : AI {
		int n;
		double C;

		AImonteUCT(int ng, double Cg);
		amove give_move(const std::vector<pii>& moves, const State& s) override;
	};
	int UCT_select_move(const double C, const std::vector<amove>& moves, const std::vector<int>& moves_playcount, const std::vector<int>& moves_wincount, const int quota);
	int simulate(int player, const State& s, const std::vector<amove>& moves, const int move_i, std::vector<std::unique_ptr<AI>>& plys);
}

#endif
