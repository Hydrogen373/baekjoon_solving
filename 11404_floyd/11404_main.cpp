#include<iostream>

int N;
int M;

int cost[102][102];


int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);

	std::cin >> N >> M;

	for (int i = 0; i < 102; i++) {
		for (int j = 0; j < 102; j++) {
			if (i == j) cost[i][j] = 0;
			else cost[i][j] = INT32_MAX;
		}
	}

	for (int i = 0; i < M; i++) {
		int a, b, c;
		std::cin >> a >> b >> c;
		cost[a][b] = std::min(cost[a][b], c);
	}

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			for (int k = 1; k <= N; k++) {
				if (cost[j][i] == INT32_MAX || cost[i][k] == INT32_MAX) continue;
				cost[j][k] = std::min(cost[j][k], cost[j][i] + cost[i][k]);
			}
		}
	}

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			std::cout << (cost[i][j] == INT32_MAX ? 0 : cost[i][j]) << ' ';
		}
		std::cout << '\n';
	}




	return 0;
}