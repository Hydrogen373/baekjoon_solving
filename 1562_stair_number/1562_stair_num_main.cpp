#include<iostream>
#define uli unsigned long int
#define VALUE_MAX 1'000'000'000

uli count[102][10][10][10]{ 0, };
int N;
static uli& get(int len, int end, int min, int max) {
	return count[len][end][min][max];
}

void func(int len, int end, int min, int max) {
	if (end == 0) {
		get(len, end, min, max) = get(len - 1, 1, 1, max) + get(len - 1, 1, 0, max);

	}
	else if (end == 9) {
		get(len, end, min, max) = get(len - 1, 8, min, 9) + get(len - 1, 8, min, 8);
	}
	else {
		get(len, end, min, max) = get(len-1, end - 1, min, max) + get(len-1, end + 1, min, max);
		get(len, end, min, max) %= VALUE_MAX;
		if (end == min) {
			get(len, end, min, max) += get(len - 1, end + 1, end + 1, max);
		}
		else if (end == max) {
			get(len, end, min, max) += get(len - 1, end - 1, min, end - 1);
		}
	}
	get(len, end, min, max) %= VALUE_MAX;
}

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0); std::cout.tie(0);
	//

	std::cin >> N;

	for (int i = 1; i <= 9; i++) {
		get(1, i, i, i) = 1;
	}

	// body
	for (int l = 2; l <= N; l++) {
		for (int e = 0; e <= 9; e++) {
			for (int mn = 0; mn <= e; mn++) {
				for (int mx = e; mx <= 9; mx++) {
					if (mn == mx)
						continue;
					func(l, e, mn, mx);
				}
			}
		}
	}

	
	uli res = 0;
	for (int e = 0; e <= 9; e++) {
		res += get(N, e, 0, 9);
		res %= VALUE_MAX;
	}
	std::cout << res << std::endl;
	// body end

	// debug
	//for (int l = 2; l <= 40; l++) {
	//	for (int e = 0; e <= 9; e++) {
	//		for (int mn = 0; mn <= e; mn++) {
	//			for (int mx = e; mx <= 9; mx++) {
	//				func(l, e, mn, mx);
	//			}
	//		}
	//	}
	//}
	//uli res = 0;
	//for (int l = 1; l <= 40; l++) {
	//	for (int e = 0; e <= 9; e++) {
	//		res += get(l, e, 0, 9);
	//		res %= VALUE_MAX;
	//	}
	//}
	//std::cout << res;
	// debug end



	//
	return 0;
}