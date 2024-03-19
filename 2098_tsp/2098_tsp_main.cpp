#include<iostream>
#include<vector>
#include<algorithm>
#include<cassert>

using BitSet = unsigned short;
//using ui = unsigned int;
using ulli = unsigned long long int;

constexpr ulli BLOCKED = UINT64_MAX;
constexpr ulli NOT_INITILIZED = 0;

ulli mp[16][16]{ 0, };
ulli dp[0x80'00][15]{ 0, };
int N;

BitSet check(BitSet b, int val) {
	return b | (1 << val);
}
BitSet uncheck(BitSet b, int val) {
	return b & (~(1 << val));
}
bool isIncluding(BitSet b, int val) {
	return (b >> val) & 0x0001;
}

ulli get(BitSet visited, int s) {
	if (dp[visited][s] == NOT_INITILIZED) {
		auto bs = uncheck(visited, s);
		ulli mn = BLOCKED;
		for (int i = 0; i < N - 1; i++) {
			if (isIncluding(bs, i) 
				&& (mp[s][i] != BLOCKED)
				&& (get(bs, i) != BLOCKED))
			{
				mn = std::min(mn, mp[s][i] + get(bs, i));
			}
		}
		dp[visited][s] = mn;
	}
	return dp[visited][s];

}

ulli init(int i) {
	ulli ret;
	if (mp[N - 1][i] == BLOCKED || get((1 << N - 1) - 1, i) == BLOCKED)
		ret = BLOCKED;
	else
		ret = mp[N - 1][i] + get((1 << N - 1) - 1, i);
	assert(ret != 0);
	return ret;
}

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);
	std::cout.tie(0);

	std::cin >> N;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			std::cin >> mp[i][j];
			if (mp[i][j] == 0) {
				mp[i][j] = BLOCKED;
			}
		}
	}

	for (int i = 0; i < N - 1; i++) {
		for (int j = 0; j < N - 1; j++) {
			dp[check(0x0000, i)][j] = BLOCKED;
		}
		dp[check(0x0000, i)][i] = mp[i][N - 1];
	}

	//
	ulli res = BLOCKED;
	for (int i = 0; i < N - 1; i++) {
		res = std::min(res, init(i));
	}
	std::cout << res;
	//

	return 0;
}
