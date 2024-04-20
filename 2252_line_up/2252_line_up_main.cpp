#include<iostream>
#include<vector>
#include<queue>


int N, M;
struct Node {
	std::vector<short> predecessors;
	bool visited = false;
};

Node* nodes;

void point(int a, int b) {
	nodes[b].predecessors.push_back(a);
}

void visit(int a) {
	if (nodes[a].visited)
		return;
	for (auto iter : nodes[a].predecessors) {
		visit(iter);
	}
	nodes[a].visited = true;
	std::cout << a << ' ';
}

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0); std::cout.tie(0);
	std::cin >> N >> M;
	nodes = new Node[N + 2];

	for (int i = 0; i < M; i++) {
		int a, b;
		std::cin >> a >> b;
		point(a, b);
	}
	//

	for (int i = 1; i <= N; i++) {
		visit(i);
	}

	//
	return 0;
}