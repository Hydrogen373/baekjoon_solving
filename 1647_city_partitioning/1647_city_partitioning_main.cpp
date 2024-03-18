#include<iostream>
#include<vector>
#include<algorithm>

int N, M;
struct Road {
	int cost;
	int A;
	int B;
	Road(int A, int B, int C) :A(A), B(B), cost(C) {}
};

struct {
	bool operator()(const Road& a, const Road& b) {
		return a.cost < b.cost;
	}
} ExpensiveFirst;
struct {
	bool operator()(const Road& a, const Road& b) {
		return a.cost > b.cost;
	}
} CheapFirst;
std::vector<Road> V;
std::vector<Road> U;

int roots[100'002]{ 0, };
int find(int i) {
	if (roots[i] == 0) {
		return i;
	}
	else
		return roots[i] = find(roots[i]);
}
void unite(int A, int B) {
	roots[find(A)] = find(B);
}


int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);
	std::cout.tie(0);
	std::cin >> N >> M;
	int mc = 0;
	int res = 0;

	// body
	for (int i = 0; i < M; i++) {
		int A, B, C;
		std::cin >> A >> B >> C;
		V.push_back(Road(A, B, C));
		std::push_heap(V.begin(), V.end(), CheapFirst);
	}

	while (V.empty() == false) {
		Road r = V.front();
		std::pop_heap(V.begin(), V.end(), CheapFirst);
		V.pop_back();

		if (find(r.A) == find(r.B)) {
			continue;
		}
		else {
			unite(r.A, r.B);

			res += r.cost;
			mc = std::max(r.cost, mc);
		}
	}

	std::cout << res - mc;


	// body end
	return 0;

}