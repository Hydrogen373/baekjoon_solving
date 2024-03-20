#include<iostream>
#include<vector>
std::vector<int> sumA, sumB;
int T;
int n;
int m;

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
	int l, r;
	int nr = 0;
	for (int l= 0; l< n; l++) {
		


	}


	//
	return 0;

}