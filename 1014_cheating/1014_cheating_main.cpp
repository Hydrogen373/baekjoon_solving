#include<iostream>

class TestCase {
	int M = 0;
	int N = 0;
	unsigned long long int pow = 0;

	int** mp;
	int* mask;

	void solve();
	void print();
	void init();
	void solveLastColumn(int depth = 0, int value = 0, int count = 0);
public:
	void run();
	~TestCase();
};

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);
	std::cout.tie(0);

	int C;
	std::cin >> C;

	for (int i = 0; i < C; i++) {
		TestCase tc;
		tc.run();
	}
}

void TestCase::init() {
	std::cin >> N >> M;
	pow =(unsigned long long) 1 << N;

	mp = new int* [M];
	for (int i = 0; i < M; i++) {
		mp[i] = new int [pow] {0, };
	}

	mask = new int[M] {0, };

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			char c;
			std::cin >> c;

			if (c == 'x') {
				mask[j] |= (1 << i);
			}
		}
	}

}

void TestCase::solve() {
	solveLastColumn();

	for (int i = M - 2; i >= 0; i--) {
		for (int value = 0; value < pow; value++) {
			if (value & this->mask[i])
				continue;

			int mask = 0;
			for (int j = 0; j < N; j++) {
				if ((value >> j) & 1) {
					if (j - 1 >= 0)
						mask |= (1 << (j - 1));
					
					mask |= (1 << j);
					if (j + 1 < N) {
						mask |= (1 << (j + 1));
					}

				}
			}

			for (int j = 0; j < pow; j++) {
				if (j & mask)
					continue;

				mp[i][value] = std::max(mp[i][value], mp[i + 1][j]);
			}

			int count = 0;
			for (int k = 0; k < N; k++) {
				if ((value >> k) & 1)
					count += 1;
			}
			mp[i][value] += count;


		}
	}

	//// debug
	//for (int j = 0; j < pow; j++) {
	//	for (int i = 0; i < M; i++) {
	//		std::cout << mp[i][j] << ' ';
	//	}
	//	std::cout << '\n';
	//}

}

void TestCase::print() {
	int max = 0;
	for (int i = 0; i < pow; i++) {
		max = std::max(mp[0][i], max);
	}

	std::cout << max << '\n';
}

void TestCase::run() {
	init();
	solve();
	print();
}

TestCase::~TestCase() {
	for (int i = 0; i < M; i++) {
		delete[] mp[i];
	}
	delete[] mp;
	delete[] this->mask;
}

void TestCase::solveLastColumn(int depth, int value, int count) {
	if (depth == N) {
		if (value & mask[M - 1])
			return;
		mp[M - 1][value] = count;
		return;
	}
	solveLastColumn(depth + 1, value + (1 << depth), count + 1);
	solveLastColumn(depth + 1, value, count);
}
