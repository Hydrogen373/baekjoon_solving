#include<iostream>
#include<stack>
#include<vector>

using Line = std::stack<int>;
std::vector<Line> horizontalLines;
std::vector<Line> verticalLines;
int N;
std::vector<std::pair<int, char>> outputs;

void moveTank(int tankNum, char c) {
	outputs.push_back(std::make_pair(tankNum, c));
}

class Tester {
	std::vector<std::pair<int,int>> coor;
	std::vector<std::vector<int>> mp;
	int N;

public:
	Tester(int N)
		:coor(1, std::make_pair(0, 0)), mp(N, std::vector<int>(N, 0)), N(N) {}

	void push(int i, int r, int c) {
		coor.push_back(std::make_pair(r, c));
		mp[r][c] = i;
	}

	void print() {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				std::cout << mp[i][j] << ' ';
			}
			std::cout << '\n';
		}
	}

	void move(std::pair<int, char> p) {
		int r = coor[p.first].first;
		int c = coor[p.first].second;
		int nr, nc;
		switch (p.second)
		{
		case 'U':
			nr = r - 1;
			nc = c;
			break;
		case 'L':
			nr = r;
			nc = c - 1;
			break;
		case 'D':
			nr = r + 1;
			nc = c;
			break;
		case 'R':
			nr = r;
			nc = c + 1;
			break;
		default:
			break;
		}

		if (mp[nr][nc] != 0) {
			return;
		}
		else {
			coor[p.first] = std::make_pair(nr, nc);
			std::swap(mp[r][c], mp[nr][nc]);
		}
	}

	void run(const std::vector<std::pair<int, char>>& outputs) {
		for (auto iter : outputs) {
			move(iter);
		}
	}


};

void fill(std::vector<Line>& lines, int linenum, char c) {
	if (c == 'U' || c == 'L') {
		if (lines[linenum + 1].size() == 0) {
			fill(lines, linenum + 1, c);
		}
		auto num = lines[linenum + 1].top();
		lines[linenum + 1].pop();
		lines[linenum].push(num);
		moveTank(num, c);
	}
	else {
		auto num = lines[linenum - 1].top();
		lines[linenum - 1].pop();
		lines[linenum].push(num);
		moveTank(num, c);
		if (lines[linenum - 1].size() == 0) {
			fill(lines, linenum - 1, c);
		}
	}
}

void run(std::vector<Line>& lines, char a, char b) {
	int stock = 0;
	for (int i = 0; i < N; i++) {
		if (lines[i].size() == 1) {
			continue;
		}
		else if (lines[i].size() == 0) {
			if (stock > 0) {
				fill(lines, i, b);
				stock -= 1;
			}
			else {
				fill(lines, i, a);
			}
		}
		else {
			stock += lines[i].size() - 1;
		}
	}
}

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0); std::cout.tie(0);
	std::cin >> N;
	horizontalLines = std::vector<Line>(N);
	verticalLines = std::vector<Line>(N);

	//Tester tester(N);

	for (int i = 1; i <= N; i++) {
		int r, c;
		std::cin >> r >> c;
		r -= 1;
		c -= 1;
		horizontalLines[r].push(i);
		verticalLines[c].push(i);

		//tester.push(i, r, c);
	}

	run(horizontalLines, 'U', 'D');
	run(verticalLines, 'L', 'R');

	//tester.run(outputs);
	//tester.print();
	std::cout << outputs.size() << '\n';
	for (auto iter : outputs) {
		std::cout << iter.first << ' ' << iter.second << '\n';
	}
	std::cout << std::endl;

	return 0;
}