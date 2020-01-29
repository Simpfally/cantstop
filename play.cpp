#include <vector>
#include <iostream>
#include <sstream>
#include <memory>

#include "cantstop.h"
#include "dicey.h"
#include "ai.h"

joli few_games(std::vector<std::unique_ptr<cs_ai::AI>>& players, int n) {
	int nply = players.size();
	std::vector<int> scores(nply, 0);
	int toplay = 0;
	for (int i = 0; i < n; i++) {
		State s(nply);
		s.toplay = toplay % nply;
		toplay++;
		int j = s.one_game(players);
		scores[j]++;
		double score = (double)scores[0]/(double)(i+1)*100;
		std::cout << score << " % winrate of player 0, = "  << i << std::endl;
	}
	return scores;
}

