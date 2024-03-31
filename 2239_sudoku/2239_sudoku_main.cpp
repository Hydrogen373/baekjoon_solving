#include<iostream>
#include<algorithm>
#include<vector>
#include<stack>
#include<cassert>

unsigned char board[9][9]{ 0, };
std::vector<int> X;
std::vector<int> Y;

bool isAbleToFillWith(int r, int c, int v) {
	for (int i = 0; i < 9; i++) {
		if (board[r][i] == v || board[i][c] == v) {
			return false;
		}
	}
	int sr = (r / 3) * 3;
	int sc = (c / 3) * 3;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (board[sr + i][sc + j] == v)
				return false;
		}
	}
	return true;
}

bool dfs(int i) {
	if (i >= X.size())
		return true;
	int r = X[i];
	int c = Y[i];

	for (int v = 1; v <= 9; v++) {
		if (isAbleToFillWith(r, c, v) == false)
			continue;
		board[r][c] = v;
		bool res = dfs(i + 1);
		if (res == false)
			board[r][c] = 0;
		else 
			return true;
	}
	return false;
}



int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);
	std::cout.tie(0);

	//
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			char c;
			std::cin >> c;
			if (c == '0') {
				X.push_back(i);
				Y.push_back(j);
			}
			else {
				board[i][j] = c - '0';
			}
		}
	}

	dfs(0);


	//
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			std::cout << (int)board[i][j];
		}
		std::cout << '\n';
	}
	return 0;
}
