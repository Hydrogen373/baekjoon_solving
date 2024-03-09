#include<iostream>
#include<vector>
int N, R, Q;
struct Node {
	std::vector<int> connected;
	int subTreeSize = 1;
};
std::vector<Node> nodes;
std::vector<bool> visited;
void dfs(int i) {
	visited[i] = true;
	for (auto next : nodes[i].connected) {
		if (visited[next]) continue;
		else dfs(next);
		nodes[i].subTreeSize += nodes[next].subTreeSize;
	}
}

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0); std::cout.tie(0);
	std::cin >> N >> R >> Q;
	nodes = std::vector<Node>(N + 1);
	visited = std::vector<bool>(N + 1, false);

	for (int i = 0; i < N - 1; i++) {
		int u, v;
		std::cin >> u >> v;
		nodes[u].connected.push_back(v);
		nodes[v].connected.push_back(u);
	}
	dfs(R);
	for (int i = 0; i < Q; i++) {
		int q;
		std::cin >> q;
		std::cout << nodes[q].subTreeSize << '\n';
	}

	return 0;
}