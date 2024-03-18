#include<iostream>
#include<vector>

int slash[19]{ 0, };
int rslash[19]{ 0, };
int N;
std::vector<std::pair<short,short>> posBlack; // even
std::vector<std::pair<short, short>> posWhite; // odd
int opt;
bool check(int r, int c) {
	return slash[r + c] == 0 && rslash[c - r + N - 1] == 0;
}
void set(int r, int c) {
	slash[r + c] += 1;
	rslash[c - r + N - 1] += 1;
}
void unset(int r, int c) {
	slash[r + c] -= 1;
	rslash[c - r + N - 1] -= 1;
	return;
}

int step(int ind, int ct, const std::vector<std::pair<short,short>>& pos) {
	if (ind >= pos.size()) {
		return ct;
	}
	int r = pos[ind].first;
	int c = pos[ind].second;
	int ret = ct;
	if (check(r,c)) {
		set(r, c);
		ret = step(ind + 1, ct + 1, pos);
		unset(r, c);
	}
	ret = std::max(ret, step(ind + 1, ct, pos));
	return ret;
}

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0); std::cout.tie(0);
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			int b;
			std::cin >> b;
			if (b == 0)
				continue;

			if ((i + j) % 2 == 0)
				posBlack.push_back(std::make_pair(i, j));
			else
				posWhite.push_back(std::make_pair(i, j));
		}
	}

	// body
	std::cout << step(0, 0, posBlack) + step(0, 0, posWhite);

	//
	return 0;

}
