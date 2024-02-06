#include<iostream>
#include<map>
#include<algorithm>

int N, M, K;
std::map<int, int> gaps;

struct compSecond {
	bool operator() (const std::pair<int, int>& a, const std::pair<int, int>& b) {
		int gapA = a.second - a.first;
		int gapB = b.second - b.first;
		return gapA < gapB || gapA == gapB && a.first > b.first;

	}
};

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);

	std::cin >> N >> M >> K;

	int* locs = new int[K];
	for (int i = 0; i < K; i++) {
		std::cin >> locs[i];
	}
	for (int i = 0; i < K - 1; i++) {
		gaps.insert(std::make_pair(locs[i], locs[i + 1]));
	}

	for (int i = 0; i < K - M; i++) {
		auto iter = std::min_element(gaps.begin(), gaps.end(), compSecond());

		auto lg = iter;
		auto rg = iter;
		if (iter != gaps.begin() && iter != --(gaps.end())) {
			auto tmp = iter;
			auto ttmp = iter;
			tmp--; ttmp++;
			if (compSecond()(*tmp, *ttmp)) {
				lg--;
			}
			else {
				rg++;
			}
		}
		else if (iter == gaps.begin()) {
			rg++;
		}
		else {
			lg--;
		}

		lg->second = rg->second;
		gaps.erase(rg);
	}

	for (auto iter = gaps.begin(); iter != gaps.end(); iter++) {
		std::cout << iter->first << ' ' << iter->second << '\n';
	}

	auto iter = std::min_element(gaps.begin(), gaps.end(), compSecond());

	int gp = iter->second - iter->first;

	int counter = 1;
	int last = locs[0];
	std::cout << 1;
	for (int i = 1; i < K; i++) {
		if (locs[i] - last >= gp && counter < M) {
			std::cout << 1;
			last = locs[i];
			counter++;
		}
		else
			std::cout << 0;

	}
	delete[] locs;
}