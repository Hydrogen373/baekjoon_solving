#include<iostream>
#include<tuple>

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
Status*** jumpTable;
Status aris;
Status endPoint(0xffff, 0xffff, 0xff, 0);
Status find(const Status& s) {
	if (jumpTable[s.r][s.c][s.d] == s || jumpTable[s.r][s.c][s.d] == endPoint) {
		return jumpTable[s.r][s.c][s.d];
	}
	else {
		//unsigned int tmp = jumpTable[s.r][s.c][s.d].distance;
		//jumpTable[s.r][s.c][s.d] = find(jumpTable[s.r][s.c][s.d]);
		//jumpTable[s.r][s.c][s.d].distance += tmp;
		jumpTable[s.r][s.c][s.d].jump(find(jumpTable[s.r][s.c][s.d]));
		return jumpTable[s.r][s.c][s.d];
	}
}
void initJumpTable(const Status& a) {
	if (jumpTable[a.r][a.c][a.d].distance != 0)
		return;
	auto r = a.r;
	auto c = a.c;

	for (int i = 0; i < 4; i++) {
		Status tmp = Status(r, c, i, 0);
		tmp.rotate(B[r][c]);
		tmp.move();
		if (tmp.r < 0 || tmp.r >= H || tmp.c < 0 || tmp.c >= W || find(tmp) == a) {
			tmp.jump(endPoint);
		}
		else {
			tmp.jump(find(tmp));
		}
		jumpTable[r][c][i] = tmp;
	}
}

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);

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

	jumpTable = new Status * *[H];
	for (int i = 0; i < H; i++) {
		jumpTable[i] = new Status * [W];
		for (int j = 0; j < W; j++) {
			jumpTable[i][j] = new Status[4];
			for (int k = 0; k < 4; k++) {
				jumpTable[i][j][k] = Status(i, j, k, 0);
			}
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
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			delete[] jumpTable[i][j];
		}
		delete[] jumpTable[i];
	}
	delete[] jumpTable;

	return 0;
}