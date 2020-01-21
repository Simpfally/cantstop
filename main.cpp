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
using namespace std;

using namespace std;
typedef vector<int> joli;
typedef pair<int, int> pii;
typedef pair<pii, int> amove; // (dice,dice), continue?
// dices : 2 3 4 5 6 7 8 9 10 11 12 -> 11 columns

joli token_per_num = {3, 5, 7, 9, 11, 13,
											11, 9, 7, 5, 3}; // how many token needed to finish a number
int COLM = 11;

struct State {
	int nply; // number of players
	int utokens; // current token on board
	int toplay; //
	int turn; // total turns
	vector<joli> vtokens;
	joli cur_tokens;
	joli scores; // scores of each player, win if score = 3
	joli finished; // if a column is finished filled with player id + 1
};

State initial(int nply) {
	State s = {
		.nply = nply,
		.utokens = 0,
		.toplay = 0,
		.turn = 0,
		.vtokens = vector<joli>(nply, joli(COLM, 0)),
		.cur_tokens = joli(COLM, 0),
		.scores = joli(nply, 0),
		.finished = joli(COLM, 0)
	};
	return s;
}

State copy(const State& s) {
	State ss = {
		.nply = s.nply,
		.utokens = s.utokens,
		.toplay = s.toplay,
		.turn = s.turn,
		.vtokens = vector<joli>(s.nply, joli(COLM, 0)),
		.cur_tokens = joli(COLM, 0),
		.scores = joli(s.nply, 0),
		.finished = joli(COLM, 0)
	};
	for (int i = 0; i < ss.nply; i++) {
		ss.scores[i] = s.scores[i];
		for (int j = 0; j < COLM; j++) {
			ss.vtokens[i][j] = s.vtokens[i][j];
		}
	}
	for (int j = 0; j < COLM; j++) {
		ss.cur_tokens[j] = ss.cur_tokens[j];
		ss.finished[j] = s.finished[j];
	}

	return ss;
}



class dice_t {
  public:
    random_device                      rd;
    mt19937                            mt;
    //uniform_real_distribution<double>  dist;
		uniform_int_distribution<int> dist;

    dice_t() : rd{}, mt{rd()}, dist{0, 5} {}

    int roll() {
      return dist(mt);
    }

		int rollc(int a, int b) {
			uniform_int_distribution<int> now(a, b);
			return now(mt);
		}
		double rolld() {
			uniform_real_distribution<double> now(0.0, 1.0);
			return now(mt);
		}

};

thread_local dice_t gendice;

struct AI {
	virtual amove give_move(const vector<pii>& moves, const State& s) {
		(void)s;
		return amove(moves[0], 0);
	}
};
void print_state(State s, ostringstream& ss) {
	ss << "Turn " << s.turn << " ";
	for (int i = 0; i < s.nply; i++) {
		ss << "Player" << i << "[" << s.scores[i] << "]";
		if (i < s.nply - 1) {
			ss << ", ";
		}
	}
	ss << endl;
	ss << "Next to play : " << s.toplay << " has " << 3 - s.utokens << " tokens left" << endl;
	for (int i = 0; i < COLM; i++) {
		int dice = i + 2;
		if (s.finished[i]) {
			int p = s.finished[i]-1;
			ss << "Dice " << dice << " taken by player " << p << endl;						
		} else {
			for (int p = 0; p < s.nply; p++) {
				int x = token_per_num[i] - s.vtokens[p][i];
				int y = 0;
				if (p == s.toplay) {
					y = s.cur_tokens[i];
				}
				//ss << "Dice " << dice << " Player " << p << " is " << x << " away" << endl;
				ss << "Dice " << dice << "\t Player " << p << " ";
				for (int j = 0; j < x; j++) {
					if (j >= x-y) {
						ss << "x ";
					} else {
						ss << "* ";
					}
				}
				ss << "(" << x << ")" << endl;
			}
		}
	}
}
/*	
	int nply; // number of players
	int cont;
	int ntokens; // max token per turn
	int utokens; // current token on board
	int toplay; //
	vector<joli> vtokens;
	joli cur_tokens;
	joli scores; // scores of each player, win if score = 3
	joli finished;
*/

void print_amove(amove am, ostringstream& s) {
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
//void print_fvec(ostringstream& s, string (*printF)(amove), vector<amove> v) {
void print_fvec(ostringstream& s, vector<amove> v) {
	s << "[";
	int i = 0;
	for (amove am : v) {
		print_amove(am, s);
		if (++i < int(v.size()))
			s << ", ";
	}
	s << "]" << endl;
}



// are those pairs the same or not
bool distinc(pii& a, pii& b) {
	if (a.first == b.first and a.second == b.second)
		return false;
	if (a.first == b.second and a.second == b.first)
		return false;
	return true;
}

// return list of unique pairs from 4 dices
vector<pii> gen_pos() {
	int a = gendice.roll();
	int b = gendice.roll();
	int c = gendice.roll();
	int d = gendice.roll();
	pii p1 = pii(a+b, c+d);
	pii p2 = pii(a+c, b+d);
	pii p3 = pii(a+d, b+c);
	vector<pii> v;

	v.push_back(p1);
	if (distinc(p1, p2)) {
		v.push_back(p2);
	}
	if (distinc(p3, p1) and distinc(p3, p2))
		v.push_back(p3);
	return v;
}

int playable(int k, const State& s) {
	if (s.finished[k])  // the column is already taken
		return -1;
	if (s.cur_tokens[k] >= token_per_num[k]) // there is already enough token on that column
		return -1;
	return k;
}

int max(int a, int b) {
	if (a > b)
		return a;
	return b;
}
int min(int a, int b) {
	if (a < b)
		return a;
	return b;
}



// one more move : can playonly one element of the pair, or both but separately
vector<pii> possible_moves(const State& s) { // tokens
	vector<pii> v = gen_pos();

	vector<pii> p;
	for (pii k : v) { // for each unique pair of dice
		int a = playable(k.first, s);
		int b = playable(k.second, s);


		if (a == -1 and b == -1)
			continue;

		if (a == -1) {
			a = b;
			b = -1;
		}

		int tokens = 3 - s.utokens; // token left

		if (tokens <= 1) { // one or less token left
			int ac = s.cur_tokens[a] > 0;
			int bc = 0;
			if (b != -1)
				bc = s.cur_tokens[b] > 0;

			if ((ac && bc) || (a == b)) { // both numbers are already being played
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
	/*vector<amove> m;
	for (pii k : p) {
		m.push_back(amove(k, true));
		m.push_back(amove(k, false));
	}*/
	return p;
}


void place_tokens(State& s, int a) {
	if (a != -1) {
		if (s.cur_tokens[a] > 0) {
			s.cur_tokens[a] = min(s.cur_tokens[a] + 1, token_per_num[a]);
		} else {
			s.utokens++;
			s.cur_tokens[a] = s.vtokens[s.toplay][a] + 1;
		}
	}
}
void clean(State& s) {
	s.utokens = 0;
	for (int i = 0; i < COLM; i++) {
		s.cur_tokens[i] = 0;
	}
}
void nextply(State& s) {
		s.toplay = (s.toplay + 1)%s.nply;
}


void save_pos(State& s) {
	for (int i = 0; i < COLM; i++) {
		// take max from cur_tokens and our saved tokens
		s.vtokens[s.toplay][i] =	min(max(s.vtokens[s.toplay][i], s.cur_tokens[i]), token_per_num[i]);
	}
	for (int i = 0; i < COLM; i++) {
		if ((s.vtokens[s.toplay][i] == token_per_num[i]) && s.finished[i] == 0) {
			s.scores[s.toplay]++;
			s.finished[i] = s.toplay + 1;
		}
	}
	clean(s);
}

int step(State& s, amove m) {
	place_tokens(s, m.first.first);
	place_tokens(s, m.first.second);
	if (not m.second) { // player decided to stop after placing tokens
		save_pos(s);
		nextply(s);
		clean(s);
		return false;
	}
	return true;
}

void big_step(State& s, vector<unique_ptr<AI>>& players) {
	s.turn++;
	auto pm = possible_moves(s);
	if (pm.size() == 0) {
		nextply(s);
		clean(s);
		return;
	} 
	auto m = players[s.toplay]->give_move(pm, s);
	step(s, m);
}

int finish(State& s) {
	for (int i = 0; i < s.nply; i++) {
		if (s.scores[i] >= 3)
			return i;
	}
	return -1;
}

int one_game(State& s, vector<unique_ptr<AI>>& players) {
	int i = 0;
	ostringstream ss;
	while(finish(s) == -1) {
		//cout << "ONE STEP" << endl;
		big_step(s, players);
		i++;
	}

	return finish(s);
}

joli few_games(vector<unique_ptr<AI>>& players, int n) {
	int nply = players.size();
	joli scores(nply+1, 0);
	int toplay = 0;
	for (int i = 0; i < n; i++) {
		State s = initial(nply);
		s.toplay = toplay % nply;
		toplay++;
		int j = one_game(s, players);
		scores[j]++;
	}
	return scores;
}


struct AIdebug : AI {
	amove give_move(const vector<pii>& moves, const State& s) override {
		int x = gendice.rollc(0, moves.size() - 1);
		int go = gendice.rollc(0, 1);
		ostringstream ss;
		ss << "\nTURN " << s.turn << endl;
		print_state(s, ss);
		//print_fvec(ss, moves);
		print_amove(amove(moves[x], go), ss);
		cout << ss.str() << endl;
		return amove(moves[x], go);
	} ;
};
struct AIrand : AI {
	amove give_move(const vector<pii>& moves, const State& s) override {
		(void)s;
		int x = gendice.rollc(0, moves.size() - 1);
		int go = gendice.rollc(0, 1);
		return amove(moves[x], go);
	};
};


struct AIcantstop : AI {
	amove give_move(const vector<pii>& moves, const State& s) override {
		int x = 0;
		int got = 0;
		for (int i = 0; i < (int)moves.size(); i++) {
			int a = moves[i].first;
			int b = moves[i].second;
			int fa = s.cur_tokens[a] + 1 == token_per_num[a];
			int fb = s.cur_tokens[b] + 1 == token_per_num[b];
			if (!got && (fa || fb)) {
				x = i;
				got = 1;
			}
		}
		int go = 0;
		if (!got) {
			x = gendice.rollc(0, moves.size() - 1);
			go = 1;
		}
			//go = gendice.rollc(0, 1);
		return amove(moves[x], go);
	};
};

// full retard monte carlo
struct AImonte : AI {
	amove give_move(const vector<pii>& moves, const State& s) override {
		int currentply = s.toplay;
		int n = 100000;

		int bgo = 0;
		int bmove = 0;
		int max = 0;
		cout << "\nTURN " << s.turn << endl;
		vector<unique_ptr<AI>> plys;
		for (int i = 0; i < s.nply; i++)
			plys.push_back(make_unique<AIrand>());

		for (int j = 0; j < (int)moves.size()*2; j++) {
			int go = 0;
			if (j >= (int)moves.size())
					go = 1;
			int i = j%moves.size();
			int score = 0;
			for (int j = 0; j < n; j++) {
				State ss = copy(s);

				step(ss, amove(moves[i], go));
				int win = one_game(ss, plys);
				if (win == currentply) {
					score++;
				}
			}
			ostringstream sx;
			sx << j << " > ";
			print_amove(amove(moves[i], go), sx);
			sx << " score\t" << (double)score/(double)n << endl;
			cout << sx.str();
			if (score >= max) {
				max = score;
				bgo = go;
				bmove = i;
			}
		}
		ostringstream ss;
		if (bgo == 0) {
			print_state(s, ss);
		}
		ss << "chosen";
		print_amove(amove(moves[bmove], bgo), ss);
		//return amove(moves[0], gendice.rollc(0, 1));

		cout << ss.str() << endl;
		return amove(moves[bmove], bgo);
	};
};


// attribute a value to each column, stop once thresold is reached
struct AIvalue : AI {
	amove give_move(const vector<pii>& moves, const State& s) override {
		double threshold = 4*(double)s.turn/5;
		double sum = 0;

		for (int i =0; i < COLM; i++) {
			if (s.cur_tokens[i] > 0) {
				int dif = s.cur_tokens[i] - s.vtokens[s.toplay][i] + 1; // how many token in col
				int sommet = token_per_num[i] - s.cur_tokens[i] + 1; // how many token left to finish
				sum += ((double)dif+1/(double)sommet)/(double)token_per_num[i]; // the less token needed, the more the token are valuable
			}
		}
		for (int i = 0; i < (int)moves.size(); i++) {
			int a = moves[i].first;
			int b = moves[i].second;
			int fa = s.cur_tokens[a] + 1 == token_per_num[a];
			int fb = s.cur_tokens[b] + 1 == token_per_num[b];
			if (fa || fb) {
				return amove(moves[i], 0);
			}
		}
		int go = 1;
		if (sum > threshold) {
			go = 0;
		}
		int x = gendice.rollc(0, moves.size() - 1);
		return amove(moves[x], go);
	};
};


struct AIstop : AI {
	amove give_move(const vector<pii>& moves, const State& s) override {
		//int x = gendice.rollc(0, moves.size() - 1);
		int mx = 0;
		int m = -1;
		int mj = -1;
		for (int i = 0; i < (int)moves.size(); i++) {
			int a = moves[i].first;
			int b = moves[i].second;
			int fa = s.cur_tokens[a] > 0;
			int fb = s.cur_tokens[b] > 0;
			int da = s.cur_tokens[a] + 1 == token_per_num[a];
			int db = s.cur_tokens[b] + 1 == token_per_num[b];
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
		if (s.utokens > 2 && gendice.rolld() > 0.1) {
			go = 0;
		}
		return amove(moves[mx], go);
	};
};

int main() {
	State s = initial(2);
	//s.finished[0] = 1;
	//s.finished[4] = 2;
	//s.vtokens[0][1] = 3;


	vector<unique_ptr<AI>> plys;
	plys.push_back(make_unique<AImonte>());
	plys.push_back(make_unique<AIcantstop>());
	//plys.push_back(make_unique<AIcantstop>());
	//step(s, truc.give_move(pm, s));
	//ostringstream ss;
	//print_state(s, ss);
	//cout << ss.str() << endl;
	//
	//cout << "Won : " << one_game(s, plys) << " in " << s.turn << endl;
	int n = 1;
	chrono::steady_clock::time_point begin = chrono::steady_clock::now();
	joli scores = few_games(plys, n);
	chrono::steady_clock::time_point end = chrono::steady_clock::now();
	cout << (float)scores[0]/(float)n*100 << " % winrate of player 0" << endl;

	cout << "In " << (float)chrono::duration_cast<chrono::microseconds>(end - begin).count()/1E6 << "s" << endl;

	return 0;
}
