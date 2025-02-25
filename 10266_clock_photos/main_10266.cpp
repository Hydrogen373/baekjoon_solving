#define _CRT_SECURE_NO_WARNINGS	
#include<iostream>
#include<vector>
#include<algorithm>

int failFunctions[200'002]{ 0, };

int N;

std::vector<int> input() {
	std::vector<int> ret;
	for (int i = 0; i < N; i++) {
		int tmp;
		std::cin >> tmp;
		ret.push_back(tmp);
	}
	std::sort(ret.begin(), ret.end());
	int first = ret[0];
	for (int i = 0; i < N - 1; i++) {
		ret[i] = ret[i + 1] - ret[i];
	}
	ret[N - 1] = 360'000 + first - ret[N - 1];
	return ret;
}

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);

	std::cin >> N;
	std::vector<int> t = input();
	std::vector<int> p = input();

	{
		int i = 1;
		int j = 0;
		while (i < N - 1) {
			if (p[i] == p[j]) {
				failFunctions[i] = j + 1;
				i++;
				j++;
			}
			else if (j == 0) {
				i++;
			}
			else {
				j = failFunctions[j - 1];
			}
		}
	}

	//// dbg
	//for (int i = 0; i <= N; i++) {
	//	char tmp[2048];
	//	sprintf(tmp, "f(%d) : %d\n", i, failFunctions[i]);
	//	std::cout << tmp;
	//}

	int i = 0;
	int j = 0;

	while (i < 2 * N && j < N) {
		if (t[i % N] == p[j]) {
			i++;
			j++;
		}
		else if (j == 0) {
			i++;
		}
		else {
			j = failFunctions[j - 1];
		}
	}

	std::cout << (j == N ? "possible" : "impossible");



	return 0;
}