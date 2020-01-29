#include "dicey.h"

namespace rng {
	//Dice dice(); // Has to be in at least one cpp

	Dice::Dice() {
		mt = std::mt19937(rd());
		dist = std::uniform_int_distribution<int>{0, 5};
	}
	Dice::Dice(int seed) {
		mt = std::mt19937(rd());
		mt.seed(seed);			
		dist = std::uniform_int_distribution<int>{0, 5};
	}

	int Dice::roll() {
		return dist(mt);
	}

	int Dice::rollc(int a, int b) {
		std::uniform_int_distribution<int> now(a, b);
		return now(mt);
	}
	double Dice::rolld() {
		std::uniform_real_distribution<double> now(0.0, 1.0);
		return now(mt);
	}
};
