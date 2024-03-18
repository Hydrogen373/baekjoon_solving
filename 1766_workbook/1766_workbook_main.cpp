#include<iostream>
#include<vector>
#include<algorithm>
struct Node {
	int num = 0;
	int pointing = 0;
	std::vector<int> children;
};

Node nodes[32'002];
std::vector<int> heap;
int N, M;
struct {
	bool operator()(const Node& a, const Node& b) {
		return a.num > b.num;
	}
	bool operator()(const int& a, const int& b) {
		return a > b;
	}
} EasyFirst;

void set(int p, int c) {
	nodes[p].children.push_back(c);
	nodes[c].pointing += 1;
}
void solve(int i) {
	for (auto c : nodes[i].children) {
		nodes[c].pointing -= 1;
		if (nodes[c].pointing == 0) {
			heap.push_back(c);
			std::push_heap(heap.begin(), heap.end(), EasyFirst);
		}
	}
}

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0); std::cout.tie(0);

	std::cin >> N >> M;

	for (int i = 0; i < M; i++) {
		int A, B;
		std::cin >> A >> B;
		set(A, B);
	}
	for (int i = 1; i <= N; i++) {
		nodes[i].num = i;
		if (nodes[i].pointing == 0) {
			heap.push_back(i);
			std::push_heap(heap.begin(), heap.end(), EasyFirst);
		}
	}
	// body

	while (heap.empty() == false) {
		int num = heap.front();
		std::pop_heap(heap.begin(), heap.end(), EasyFirst);
		heap.pop_back();

		solve(num);
		std::cout << num << ' ';
	}
	std::cout << std::endl;

	// body end
	// 
	return 0;
}