#include <iostream>
#include <sstream>
#include <chrono>
#include <string>
#include <vector>
#include <iomanip>
#include <map>
#include <algorithm>
#include <random>
#include <chrono> // for random
#include <thread>
#include <utility>

#include "cantstop.h"
#include "dicey.h"
#include "ai.h"
#include "play.h"
#include "monte.h"

using namespace std;

namespace rng {
	rng::Dice dice;
}

int main() {
	vector<unique_ptr<cs_ai::AI>> plys;
	//plys.push_back(make_unique<cs_ai::AImonte>(500));
	plys.push_back(make_unique<cs_ai::AIplayer>("Vivi"));
	//plys.push_back(make_unique<cs_ai::AImonteUCT>(100000, 1.2));
	plys.push_back(make_unique<cs_ai::AIcantstop>());
	plys.push_back(make_unique<cs_ai::AIcantstop>());
	plys.push_back(make_unique<cs_ai::AIcantstop>());
	int n = 1;
	chrono::steady_clock::time_point begin = chrono::steady_clock::now();
	joli scores = few_games(plys, n);
	chrono::steady_clock::time_point end = chrono::steady_clock::now();
	cout << (float)scores[0]/(float)n*100 << " % winrate of player 0" << endl;

	cout << "In " << (float)chrono::duration_cast<chrono::microseconds>(end - begin).count()/1E6 << "s" << endl;
	return 0;
}
