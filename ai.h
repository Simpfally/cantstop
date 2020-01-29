#ifndef AI_H
#define AI_H

#include <vector>
#include <utility>
#include <string>
#include "cantstop.h"
#include "dicey.h"

typedef std::vector<int> joli;
typedef std::pair<int, int> pii;
typedef std::pair<pii, int> amove;

struct State;

namespace cs_ai {
	struct AI {
		virtual amove give_move(const std::vector<pii>& moves, const State& s);
	};

	struct AIdebug : AI {
		amove give_move(const std::vector<pii>& moves, const State& s) override;
	};
	struct AIplayer : AI {
		std::string name;
		AIplayer(std::string nameg);
		amove give_move(const std::vector<pii>& moves, const State& s) override;
	};
	struct AIrand: AI {
		amove give_move(const std::vector<pii>& moves, const State& s) override;
	};
	struct AIcantstop: AI {
		amove give_move(const std::vector<pii>& moves, const State& s) override;
	};
	struct AIvalue: AI {
		amove give_move(const std::vector<pii>& moves, const State& s) override;
	};
	struct AIstop: AI {
		amove give_move(const std::vector<pii>& moves, const State& s) override;
	};
}

#endif
