#include<iostream>
#include<vector>
#define NOT_READY 0xFFFFFFFF;
struct Building {
	unsigned int required = 0;
	unsigned int accumulated = NOT_READY;
	std::vector<Building*> parents;

	Building(unsigned int time) :required(time) {}
	bool ready() {
		return accumulated != NOT_READY;
	}
	void build() {
		if (ready()) return;
		int max = 0;
		for (auto iter : parents) {
			if (iter->ready() == false) {
				iter->build();
			}
			max = max > iter->accumulated ? max : iter->accumulated;
		}
		accumulated = max + required;
	}
};
class TestCase {
	int N = 0;
	int K = 0;
	std::vector<Building> buildings;
public:
	void run() {
		std::cin >> N >> K;
		buildings.push_back(Building(0));
		for (int i = 0; i < N; i++) {
			int d;
			std::cin >> d;
			buildings.push_back(Building(d));
		}
		for (int i = 0; i < K; i++) {
			int x, y;
			std::cin >> x >> y;
			buildings[y].parents.push_back(&(buildings[x]));
		}

		int W;
		std::cin >> W;
		buildings[W].build();
		std::cout << buildings[W].accumulated << '\n';
	}


};

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0); std::cout.tie(0);
	int T;
	std::cin >> T;

	for (int i = 0; i < T; i++) {
		TestCase tc;
		tc.run();
	}

	return 0;

}