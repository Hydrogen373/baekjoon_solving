#include<iostream>
#include<map>

using ll = long long;
using ull = unsigned long long;
using Pos = unsigned short;
using Direction = unsigned char;
using Rule = unsigned char;

int H, W;
Pos R, C; 
Direction D;
Pos dr[]{ -1,0,1,0 };
Pos dc[]{ 0,1,0,-1 };
Rule ** A, ** B;

class Status {
public:
	Pos r, c; 
	Direction d;
	Status(Pos r, Pos c, Direction d) :r(r), c(c), d(d) {}

	void rotate(Rule rl) {
		d = (d + rl) % 4;
	}

	void move() {
		r += dr[d];
		c += dc[d];
	}

	bool operator==(const Status& other) const {
		return r == other.r && c == other.c && d == other.d;
	}
};


class UnionFind {
public:
	class Node {
	public:
		Status stat;
		Node* root = nullptr;
		ll size = 1;
		Node* destination = nullptr;
		ll distance = INT64_MAX;



	};
	std::map<std::tuple<Pos, Pos, Direction>, Node*> nodes;


	Node Find(Pos r, Pos c, Direction d) {
		if (nodes.find(std::make_tuple(r, c, d)) == nodes.end()) {
			Node* nd = new Node;

		}
		return map[r][c][d];
	}

}

int main() {
	using namespace std;
	cin.tie(0);
	ios::sync_with_stdio(0);

	cin >> H >> W;
	cin >> R >> C >> D;

	A = new unsigned char* [H];
	for (int i = 0; i < H; i++) {
		A[i] = new unsigned char[W];
		for (int j = 0; j < W; j++) {
			char tmp;
			cin >> tmp; 
			A[i][j] = tmp - '0';
		}
	}
	B = new unsigned char* [H];
	for (int i = 0; i < H; i++) {
		B[i] = new unsigned char[W];
		for (int j = 0; j < W; j++) {
			char tmp;
			cin >> tmp; 
			B[i][j] = tmp - '0';
		}
	}
	//







	//
	for (int i = 0; i < H; i++) {
		delete[] A[i];
		delete[] B[i];
	}
	delete[] A;
	delete[] B;

	return 0;

}

