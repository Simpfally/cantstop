// return probability distribution of being able to play each of the 11 numbers
vector<double> stat_one_num(int n) {
	joli count(13, 0);
	for (int i = 0; i < n; i++) {
		vector<pii> pos = gen_pos();
		for (pii p : pos) {
			count[p.first]++;
			if (p.first != p.second)
				count[p.second]++;
		}
	}
	int sum = 0;
	for (int i : count)
		sum += i;
	vector<double> s(13);
	
	for (int i = 0; i < 13; i++) {
		s[i] = count[i]/(double)sum;
	}


	return s;
}

vector<double> stat_one_num_true(int n) {
	joli count(13, 0);
	for (int i = 0; i < n; i++) {
		joli ccount(13, 0);
		vector<pii> pos = gen_pos();
		for (pii p : pos) {
			ccount[p.first]++;
			ccount[p.second]++;
		}
		for (int i = 0; i < 13; i++) {
			if (ccount[i] > 0) {
				count[i]++;
			}
		}

	}
	int sum = 0;
	for (int i : count)
		sum += i;
	vector<double> s(13);
	
	for (int i = 0; i < 13; i++) {
		s[i] = count[i]/(double)n;
	}


	return s;
}

vector<double> stat_one_num_test(int n) {
	joli count(13, 0);
	for (int i = 0; i < n; i++) {
		int a = dice.roll();
		int b = dice.roll();
		count[a+b]++;
	}
	int sum = 0;
	for (int i : count)
		sum += i;
	vector<double> s(13);
	for (int i = 0; i < 13; i++) {
		s[i] = count[i]/(double)sum;
	}


	return s;
}

void printo(vector<double> v) {
	int i = -1;
	for (auto k : v) {
		i++;
		if (k <= 0.00001)
			continue;
		cout << fixed << setprecision(5) << i << " : " << k*100 << endl;
	}
	return;
}

void print_single_num_stat(int n) {
	auto v = stat_one_num(n);
	auto vv = stat_one_num_test(n);
	vector<double> diff(13);
	for (int x = 2; x < 13; x++)
		diff[x] = v[x] - vv[x];
	auto vvv = stat_one_num_true(n);
	printo(v);
	printo(vv);
	cout << "diff" << endl;
	printo(diff);
	cout << "true" << endl;
	printo(vvv);
	return;
}
