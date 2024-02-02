#include<iostream>
#include<algorithm>
#include<cassert>

using namespace std;
using Station = pair<int, int>;

int N;
unsigned long long int L, P;
unsigned long long int fuel;
Station* stations;
int arrSize;
int maxInd;
int result{ 0 };
bool comp(const Station& small, const Station& big) {
	if ((small.first <= fuel) == (big.first <= fuel))
		return small.second < big.second;
	else return small.first > fuel;
}

int main() {
	using namespace std;
	ios::sync_with_stdio(false);
	cin.tie(0);

	cin >> N;
	stations = new Station[N];
	arrSize = N;
	for (int i = 0; i < N; i++) {
		int a, b;
		cin >> a >> b;

		stations[i] = make_pair(a, b);
	}

	cin >> L >> P;
	fuel = P;

	make_heap(stations, stations + arrSize, comp);

	while (fuel < L) {
		if (stations[0].first > fuel || arrSize <= 0) {
			result = -1;
			break;
		}

		fuel += stations[0].second;
		pop_heap(stations, stations + arrSize, comp);
		arrSize--;
		result++;
		make_heap(stations, stations + arrSize, comp);
	}
	cout << result;

	delete[] stations;
	return 0;
}