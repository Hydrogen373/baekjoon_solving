#include<iostream>
#include<tuple>

int H, W;
int R, C, D;
bool cleaning;
int tr, tc, td;

int dr[]{ -1,0,1,0 };
int dc[]{ 0,1,0,-1 };

char** A;
char** B;
bool** cleaned;

void rotate(char c) {
	D = (D + c) % 4;
}
unsigned long long int counter = 0;
void move(bool count) {
	static unsigned long long buf = 0;
	R += dr[D];
	C += dc[D];
	if (count) {
		counter += buf + 1;
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
	cleaned = new bool* [H];
	for (int i = 0; i < H; i++) {
		cleaned[i] = new bool[W] {0, };
	}

	while (true) {
		if (cleaned[R][C]) {
			if (cleaning == true) {
				cleaning = false;
				tr = R;
				tc = C;
				td = D;
			}
			else if (tr == R && tc == C && td == D) {
				break;
			}
			rotate(B[R][C]);
			move(false);
		}
		else {
			cleaning = true;
			cleaned[R][C] = true;
			rotate(A[R][C]);
			move(true);
		}

		if (R < 0 || R >= H || C < 0 || C >= W)
			break;
	}
	std::cout << counter;

	for (int i = 0; i < H; i++) {
		delete[] A[i], B[i];
		delete[] cleaned[i];
	}
	delete[] A, B;
	delete[] cleaned;

	return 0;
}