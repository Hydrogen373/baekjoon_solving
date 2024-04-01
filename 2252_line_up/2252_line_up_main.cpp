#include<iostream>
#include<vector>
#include<queue>

int N, M;
struct Node {
	std::vector<int> pointing;
	int numPointed = 0;
};

std::vector<Node> nodes;
std::queue<int> que;

void point(int a, int b) {
	nodes[a].pointing.push_back(b);
	nodes[b].numPointed += 1;
}

void func(int a) {
	for (auto iter : nodes[a].pointing) {
		nodes[iter].numPointed -= 1;
		if (nodes[iter].numPointed <= 0) {
			que.push(iter);
		}
	}
}

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0); std::cout.tie(0);
	std::cin >> N >> M;
	nodes = std::vector<Node>(N + 1);

	for (int i = 0; i < M; i++) {
		int a, b;
		std::cin >> a >> b;
		point(a, b);
	}

	for (int i = 1; i <= N; i++) {
		if (nodes[i].numPointed > 0)
			continue;
		que.push(i);
	}
	//

	while (que.empty() == false) {
		auto a = que.front();
		que.pop();
		std::cout << a << ' ';
		func(a);
	}
	//
	return 0;
}