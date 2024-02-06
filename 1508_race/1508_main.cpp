#include<iostream>

int N, M, K;
int* locs;

int count(int gp) {
	int lastInd = 0;
	int result = 1;
	for (int i = 1; i < K; i++) {
		if (locs[i] - locs[lastInd] >= gp) {
			lastInd = i;
			result++;
		}
	}
	return result;
}

int main() {
	std::cin >> N >> M >> K;

	locs = new int[K];
	for (int i = 0; i < K; i++) {
		std::cin >> locs[i];
	}

	int low = 0;
	int high = N;
	int mid = (high + low) / 2;
	while (high >= low) {
		mid = (high + low) / 2;
		int ct = count(mid);
		if (ct < M) {
			high = mid - 1;
		}
		else {
			low = mid + 1;
		}
	}
	
	int last = 0;
	int ct = 1;
	std::cout << 1;
	for (int i = 1; i < K; i++) {
		if (locs[i] - locs[last] >= high && ct < M) {
			std::cout << 1;
			last = i;
			ct++;
		}
		else {
			std::cout << 0;
		}
	}
	delete[] locs;

}