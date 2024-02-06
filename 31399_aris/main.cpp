#include<iostream>
#include<tuple>

int H, W;
int R, C, D;

int dr[]{ -1,0,1,0 };
int dc[]{ 0,1,0,-1 };
char A[1024][1024];
char B[1024][1024];
std::tuple<int, int, int> loopPoint;
bool looping = false;
bool dust[1024][1024];

void rotate(char c) {
	D = (D + c) % 4;
}
bool loopTest() {
	return looping && std::make_tuple(R, C, D) == loopPoint;
}
void setLoop() {
	looping = true;
	loopPoint = std::make_tuple(R, C, D);
}

unsigned long long int moveCount = 0;
void move(bool count) {
	static unsigned long long int buf = 0;
	R += dr[D];
	C += dc[D];
	if (count) {
		moveCount += buf + 1;
		buf = 0;
	}
	else {
		buf++;
	}
}

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);

	std::cin >> H >> W;
	std::cin >> R >> C >> D;

	setLoop();

	int dustCt = H * W;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			std::cin >> A[i][j];
		}
	}
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			std::cin >> B[i][j];
		}
	}
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			dust[i][j] = true;
		}
	}

	while (dustCt >= 0) {
		if (dust[R][C]) {
			dust[R][C] = false;
			dustCt--;
			if (A[R][C] != B[R][C])
				looping = false;
			else if (looping == false) {
				setLoop();
			}
			rotate(A[R][C]);
			move(true);
		}
		else {
			if (looping == false) {
				setLoop();
			}
			rotate(B[R][C]);
			move(false);
		}

		if (R < 0 || R >= H || C < 0 || C >= W)
			break;

		if (loopTest() == true)
			break;
	}
	std::cout << moveCount;

	return 0;
}