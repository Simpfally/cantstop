#ifndef DICEY_H
#define DICEY_H 

#include <random>

namespace rng {
	struct Dice {
		std::random_device                      rd;
		std::mt19937                            mt;
		std::uniform_int_distribution<int> dist;

		Dice();
		Dice(int seed);

		int roll();
		int rollc(int a, int b);
		double rolld();
	};
	extern Dice dice; // a dice has been defined somewhere
}
#endif
