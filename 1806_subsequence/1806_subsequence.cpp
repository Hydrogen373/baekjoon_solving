#include<iostream>
#define ulli unsigned long long int

unsigned short sequence[100'002]{ 0, };
int S;
int N;

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);
	std::cout.tie(0);
	std::cin >> N >> S;
	for (int i = 0; i < N; i++) {
		std::cin >> sequence[i];
	}
	int l, r;
	l = r = 0;
	int res = INT32_MAX;
	ulli sum = sequence[l];

	while (l <= r && r < N) {
		if (sum >= S) {
			res = std::min(res, 1 + r - l);
			sum -= sequence[l];
			l += 1;
		}
		else {
			r += 1;
			sum += sequence[r];
		}
	}

	if (res == INT32_MAX)
		std::cout << 0;
	else
		std::cout << res;

	return 0;

}
