#include<iostream>
#include<vector>
#include<algorithm>
std::vector<int> sumA, sumB;
int T;
int n;
int m;

void init(std::vector<int>& vec) {
	int L = vec.size();
	for (int i = 0; i <= L - 2; i++) {
		int v = vec[i];
		for (int j = i + 1; j <= L - 1; j++) {
			v += vec[j];
			vec.push_back(v);
		}
	}
	std::sort(vec.begin(), vec.end());
	
	//// debug
	//for (auto iter : vec) {
	//	std::cout << iter << ' ';
	//}
	//std::cout << std::endl;
}

int bisearch(const std::vector<int>& vec, int value) {
	int r = vec.size();
	int l = 0;
	while (l < r) {
		int mid = (l + r) / 2;
		if (vec[mid] >= value) {
			r = mid;
		}
		else {
			l = mid + 1;
		}
	}
	return r;
}

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0); std::cout.tie(0);

	std::cin >> T >> n;
	for (int i = 0; i < n; i++) {
		int a;
		std::cin >> a;
		sumA.push_back(a);
	}
	std::cin >> m;
	for (int i = 0; i < m; i++) {
		int b;
		std::cin >> b;
		sumB.push_back(b);
	}
	//
	init(sumA);
	init(sumB);

	int a = 0;
	unsigned long long res = 0;
	while (a < sumA.size()) {
		int v = sumA[a];
		int n = bisearch(sumA, v + 1);
		unsigned long long c1 = n - a;
		a = n;

		unsigned long long c2 = bisearch(sumB, T - v + 1) - bisearch(sumB, T - v);

		res += c1 * c2;
	}

	std::cout << res;
	//
	return 0;

}