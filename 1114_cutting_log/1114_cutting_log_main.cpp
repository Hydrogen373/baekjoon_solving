#include<iostream>
#include<vector>
#include<algorithm>
#define lli long long
lli L, K, C;
std::vector<lli> vec;

lli bisearch(lli i) {
	// equal to or greater than
	lli low = 0;
	lli high = vec.size();
	lli ret = high;
	while (low < high) {
		lli mid = (low + high) / 2;
		if (vec[mid] >= i) {
			ret = high = mid;
		}
		else {
			low = mid + 1;
		}
	}
	return ret;
}
std::pair<lli, lli> count(lli len) {
	// (cutting count, first location)
	std::pair<lli, lli> ret = std::make_pair(0, L); 
	while (true) {
		lli ind = bisearch(ret.second - len);
		if (ind == 0) {
			if (ret.first < C) {
				ret.second = vec[1];
			}
			return ret;
		}
		else if (vec.size() <= ind || vec[ind] == ret.second) {
			return std::make_pair(INT32_MAX, INT32_MAX);
		}
		else {
			ret.first += 1;
			ret.second = vec[ind];
		}
	}
}
int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0); std::cout.tie(0);
	std::cin >> L >> K >> C;
	vec.push_back(0);
	for (lli i = 0; i < K; i++) {
		lli v;
		std::cin >> v;
		vec.push_back(v);
	}
	vec.push_back(L);
	std::sort(vec.begin(), vec.end());

	//
	lli high = L;
	lli low = 0;
	std::pair<lli, lli> res;
	while (low < high) {
		lli mid = (high + low) / 2;
		auto tmp = count(mid);
		
		if (tmp.first <= C) {
			high = mid;
			res = tmp;
		}
		else {
			low = mid + 1;
		}
	}
	std::cout << high << ' ' << res.second;

	//
	return 0;
}