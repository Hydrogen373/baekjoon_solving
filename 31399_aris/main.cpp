#include<iostream>
#include<map>
#include<cassert>
#include<string>

int H, W;
int R, C, D;

int dr[]{ -1,0,1,0 };
int dc[]{ 0,1,0,-1 };

char** A;
char** B;

struct Status {
	unsigned short r, c;
	unsigned char d;
	unsigned int distance;
	Status(unsigned short r, unsigned short c, char d, unsigned int distance) :
		r(r), c(c), d(d), distance(distance) {}
	Status() :Status(0, 0, 0, 0) {}
	void rotate(char c) {
		d = (d + c) % 4;
	}
	void move() {
		r += dr[d];
		c += dc[d];
		distance += 1;
	}
	bool operator==(const Status& other) {
		return r == other.r && c == other.c && d == other.d;
	}
	void jump(const Status& after) {
		r = after.r;
		c = after.c;
		d = after.d;
		distance += after.distance;
	}
};
bool operator<(const Status& a, const Status& b) {
	if (a.r != b.r)return a.r < b.r;
	else if (a.c != b.c)return a.c < b.c;
	else return a.d < b.d;
}
std::map<Status, Status> jumpTable;
Status aris;
Status endPoint(0xffff, 0xffff, 0xff, 0);
Status find(const Status& s) {
	if (jumpTable.find(s) == jumpTable.end()) {
		Status result = s;
		result.distance = 0;
		return result;
	}
	else {
		jumpTable[s].jump(find(jumpTable[s]));
		return jumpTable[s];
	}
}
void initJumpTable(const Status& a) {
	assert(jumpTable.find(a) == jumpTable.end());
	auto r = a.r;
	auto c = a.c;

	for (int i = 0; i < 4; i++) {
		Status tmp = Status(r, c, i, 0);
		tmp.rotate(B[r][c]);
		tmp.move();
		if (tmp.r < 0 || tmp.r >= H || tmp.c < 0 
			|| tmp.c >= W 
			|| find(tmp) == Status(r, c, i, 0)) 
		{
			tmp.jump(endPoint);
		}
		jumpTable.insert(
			std::make_pair(
				Status(r, c, i, 0),
				tmp
			)
		);
	}
}

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	std::cout.tie(0);

	std::cin >> H >> W;

	std::cin >> R >> C >> D;
	aris = Status(R, C, D, 0);

	A = new char* [H];
	for (int i = 0; i < H; i++) {
		A[i] = new char[W];
		for (int j = 0; j < W; j++) {
			std::cin >> A[i][j];
			A[i][j] -= '0';
		}
	}
	B = new char* [H];
	for (int i = 0; i < H; i++) {
		B[i] = new char[W];
		for (int j = 0; j < W; j++) {
			std::cin >> B[i][j];
			B[i][j] -= '0';
		}
	}

	while (true) {
		auto next = find(aris);
		if (next == endPoint)
			break;

		if (next == aris) {
			initJumpTable(next);
			aris.rotate(A[aris.r][aris.c]);
			aris.move();
		}
		else {
			aris.jump(next);
		}

		if (aris.r < 0 || aris.r >= H || aris.c < 0 || aris.c >= W)
			break;

	}
	std::cout << aris.distance;

	for (int i = 0; i < H; i++) {
		delete[] A[i], B[i];
	}
	delete[] A, B;

	return 0;
}