#include<iostream>

int H, W;
int R, C, D;

int dr[]{0, 1, 0, -1};
int dc[]{1, 0, -1, 0};
int** A, ** B;

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);

	std::cin >> H >> W;
	std::cin >> R >> C >> D;

	A = new int* [H];
	B = new int* [H];
	for (int i = 0; i < H; i++) {
		A[i] = new int[W];
		B[i] = new int[W];
	}

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
		}
	}
}