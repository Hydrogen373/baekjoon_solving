#include<iostream>
#include<vector>
#include<algorithm>
#include<cassert>

int* loc;
Referee* referees;
struct Referee {
	int* lg, *rg;
	int ind;
	void move(bool right) {
		int sign = right ? 1 : -1;
		assert(refrees[ind + sign] == nullptr);

		int var = loc[ind + sign] - loc[ind];
		if (lg != nullptr)
			(*lg) += var;
		if (rg != nullptr)
			(*rg) -= var;
		refrees[ind] = nullptr;
		ind += sign;
		refrees[ind] = this;
	}
};

int N{ 0 }, M{ 0 }, K{ 0 };

void test(int i, int j) {
	if ((refrees[i] == nullptr) == (refrees[j] == nullptr))
		return;
	bool direction = (refrees[i] != nullptr);
	refrees[i].move(direction);
}

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	std::cin >> N >> M >> K;

	referees = new Referee[K];
	for (int i = 0; i < K; i++) {
		int tmp;
		std::cin >> tmp;
		loc.push_back(tmp);
	}

	Case** dp = new Case*[M];
	for (int i = 0; i < M; i++) {
		dp[i] = new Case[K];
	}

	for (int i = 0; i < K; i++) {
		dp[0][i].mLoc.push_back(loc[0]);
		dp[0][i].R = INT32_MAX;
	}

	for (int i = 1; i < M; i++) {
		for (int j = 0; j <= i; j++) {
			dp[i][i].mLoc.push_back(loc[j]);
		}
		dp[i][i].R = std::min(dp[i - 1][i - 1].R, loc[i] - loc[i - 1]);
	}

	for (int _i = 0; _i < M - 1; _i++) {
		for (int _j = 0; _j < K - M; _j++) {
			int r = 1 + _i;
			int c = r + 1 + _j;

			int tmp = std::min(
				loc[c] - dp[r - 1][c - 1].mLoc.back(), 
				dp[r - 1][c - 1].R
			);

			bool insertion = tmp > dp[r][c - 1].R;
			if (tmp == dp[r][c - 1].R) {
				for (int k = 0; k < r; k++) {
					if (dp[r - 1][c - 1].mLoc[k] < dp[r][c - 1].mLoc[k]) {
						insertion = true;
						break;
					}
				}
			}

			if (insertion) {
				dp[r][c].mLoc = dp[r - 1][c - 1].mLoc;
				dp[r][c].mLoc.push_back(loc[c]);
				dp[r][c].R = tmp;
			}
			else {
				dp[r][c].mLoc = dp[r][c - 1].mLoc;
				dp[r][c].R = dp[r][c - 1].R;
			}
		}
	}

	auto j = dp[M - 1][K - 1].mLoc.begin();
	auto end = dp[M - 1][K - 1].mLoc.end();
	for (auto iter : loc) {
		if (j != end && iter == *j) {
			std::cout << 1;
			j++;
		}
		else {
			std::cout << 0;
		}
	}

	return 0;
}

