#include<iostream>
#include<algorithm>
#include<vector>

using std::min;
class Solver {
	int N, W;
	int** mp;
	bool isAbleToMerge(int i, int j) {
		if (j == 0) {
			return false;
		}
		return mp[i][j - 1] + mp[i][j] <= W;
	}
	void _solve(int i, int** solution) {
		if (i == 0) {
			solution[1][0] = solution[0][0] = 1;
			solution[2][0] = 
				(mp[0][0] + mp[1][0] <= W) ? 1 : 2;
			return;
		}
		
		// solve solution[0][i]
		if (isAbleToMerge(0, i)) {
			solution[0][i]
				= min(
					solution[1][i - 1], 
					solution[2][i - 1]
				) + 1;
		}
		else {
			solution[0][i] = solution[2][i - 1] + 1;
		}

		// solve solution[1][i]
		if (isAbleToMerge(1, i)) {
			solution[1][i]
				= min(
					solution[0][i - 1],
					solution[2][i - 1]
				) + 1;
		}
		else {
			solution[1][i] = solution[2][i - 1] + 1;
		}
		// solve solution[2][i]
		bool tmp = mp[0][i] + mp[1][i] <= W;
		solution[2][i] = min({
			solution[0][i] + 1,
			solution[1][i] + 1,
			solution[2][i - 1] + (tmp ? 1 : 2)
			});
		if (isAbleToMerge(0, i) && isAbleToMerge(1, i)) {
			if (i == 1)
				solution[2][i] = 2;
			else {
				solution[2][i] =
					min(
						solution[2][i], 
						solution[2][i - 2] + 2
					);
			}
		}
	}
public:
	bool flag;
	Solver(int N, int W, int** mp) 
		:N(N), W(W), mp(mp), flag(false) {}
	int solve() {
		auto solution = new int* [3];
		for (int i = 0; i < 3; i++) {
			solution[i] = new int[N];
			for (int j = 0; j < N; j++) {
				solution[i][j] = INT32_MAX;
			}
		}

		for (int i = 0; i < N; i++) {
			_solve(i, solution);
		}

		//// debug
		//for (int i = 0; i < 3; i++) {
		//	for (int j = 0; j < N; j++) {
		//		std::cout << solution[i][j] << ' ';
		//	}
		//	std::cout << '\n';
		//}

		auto result = solution[2][N - 1];
		if (flag)
			result = solution[1][N - 1];
		delete[] solution[0];
		delete[] solution[1];
		delete[] solution[2];
		delete[] solution;
		return result;
	}
};
class TestCase {
	int N, W;
	int** mp;

public:
	TestCase() :N(0), W(0), mp(nullptr) {}
	~TestCase() {
		delete[] mp[0];
		delete[] mp[1];
		delete[] mp;
		mp = nullptr;
	}
	void init() {
		std::cin >> N >> W;
		mp = new int* [2];
		for (int i = 0; i < 2; i++) {
			mp[i] = new int[N];
			for (int j = 0; j < N; j++) {
				std::cin >> mp[i][j];
			}
		}
		return;
	}
	int preprocessing() {
		if (N <= 1)
			return 0;
		std::vector<std::vector<bool>> check(2, std::vector<bool>(N));


		for (int i = 0; i < N; i++) {
			if (i == N - 1) {
				check[0][i] = mp[0][0] + mp[0][N - 1] <= W;
				check[1][i] = mp[1][0] + mp[1][N - 1] <= W;
			}
			else {
				check[0][i] = mp[0][i] + mp[0][i + 1] <= W;
				check[1][i] = mp[1][i] + mp[1][i + 1] <= W;
			}
			if (check[0][i] || check[1][i]) {
				continue;
			}
			if (i == N - 1)
				return 0;
			auto tmp = new int* [2];
			tmp[0] = new int[N];
			tmp[1] = new int[N];
			for (int j = 0; j < N; j++) {
				tmp[0][j] = mp[0][(i + 1 + j) % N];
				tmp[1][j] = mp[1][(i + 1 + j) % N];
			}
			std::swap(mp, tmp);
			delete[] tmp[0];
			delete[] tmp[1];
			delete[] tmp;

			return 0;
		}

		for (int i = 0; i < N; i++) {
			if (check[0][i] && check[1][i]) {
				continue;
			}
			if (i != N - 1) {
				auto tmp = new int* [2];
				tmp[0] = new int[N];
				tmp[1] = new int[N];
				for (int j = 0; j < N; j++) {
					tmp[0][j] = mp[0][(i + 1 + j) % N];
					tmp[1][j] = mp[1][(i + 1 + j) % N];
				}
				std::swap(mp, tmp);
				delete[] tmp[0];
				delete[] tmp[1];
				delete[] tmp;
			}
			if (mp[1][0] + mp[1][N - 1] <= W) {
				std::swap(mp[0], mp[1]);
			}
			return 1;
		}

		return 2;
	}
	int solve() {
		auto s = preprocessing();
		//// debug
		//for (int i = 0; i < 2; i++) {
		//	for (int j = 0; j < N; j++) {
		//		std::cout << mp[i][j] << ' ';
		//	}
		//	std::cout << '\n';
		//}
		if (s == 2)
			return N;
		Solver solver(N, W, mp);
		int result = solver.solve();

		if (s == 1) {
			mp[0][0] = mp[0][N - 1] = W;
			Solver tmp(N, W, mp);
			tmp.flag = true;
			int r_ = tmp.solve();
			result = std::min(result, r_);
		}
		return result;
	}
	
	
};
int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	std::cout.tie(0);
	int T;
	std::cin >> T;
	for (int i = 0; i < T; i++) {
		TestCase tc;
		tc.init();
		int result = tc.solve();
		std::cout << result << '\n';
	}


	// termination
	return 0;
}