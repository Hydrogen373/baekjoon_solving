#include<iostream>
#include<vector>
#include<algorithm>
#define ulli unsigned long long int
std::vector<int> vec;
std::vector<int> get(int l, int r) {
	std::vector<int> ret;
	if (l == r) {
		ret.push_back(vec[l]);
		return ret;
	}
	int mid = (l + r) / 2;
	auto v1 = get(l, mid);
	auto v2 = get(mid + 1, r);
	for (auto i : v1) {
		for (auto j : v2) {
			ret.push_back(i + j);
		}
		ret.push_back(i);
	}
	for (auto j : v2) {
		ret.push_back(j);
	}
	return ret;
}

ulli bisearch(const std::vector<int>& vec, int value) {
	int l = 0;
	int r = vec.size();
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
ulli count(const std::vector<int>& vec, int value) {
	return bisearch(vec, value + 1) - bisearch(vec, value);
}
std::vector<int> v1, v2;
int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0); std::cout.tie(0);
	int N;
	int S;
	std::cin >> N >> S;
	for (int i = 0; i < N; i++) {
		int t;
		std::cin >> t;
		vec.push_back(t);
	}

	if (N == 1) {
		std::cout << (vec[0] == S ? 1 : 0);
		return 0;
	}
	int mid = (N - 1) / 2;
	v1 = get(0, mid);
	v2 = get(mid + 1, N - 1);
	std::sort(v1.begin(), v1.end());
	std::sort(v2.begin(), v2.end());


	ulli ct = 0;

	ct += count(v1, S) + count(v2, S);
	
	int value = v1[0];
	while (true) {
		int ind = bisearch(v1, value);
		if (ind == v1.size())
			break;
		value = v1[ind];
		ct += count(v1, value) * count(v2, S - value);
		value += 1;
	}

	std::cout << ct;
	return 0;

}
