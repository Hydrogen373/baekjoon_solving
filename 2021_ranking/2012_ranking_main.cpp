#include<iostream>
#include<algorithm>
#include<vector>
int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0); std::cout.tie(0);
	int N;
	std::vector<int> vec;
	vec.push_back(0);
	unsigned long long int result = 0;
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		int tmp;
		std::cin >> tmp;
		vec.push_back(tmp);
	}
	std::sort(vec.begin(), vec.end());
	for (int i = 0; i <= N; i++) {
		int tmp = vec[i] - i;
		tmp = tmp > 0 ? tmp : -tmp;
		result += tmp;
	}
	std::cout << result;
	return 0;
}