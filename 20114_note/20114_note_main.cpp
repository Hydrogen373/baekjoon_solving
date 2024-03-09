#include<iostream>
#include<vector>
std::vector<char> str;
int N, H, W;
int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0); std::cout.tie(0);
	std::cin >> N >> H >> W;
	str = std::vector<char>(N, '?');

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < W; k++) {
				char c;
				std::cin >> c;
				if (c != '?')
					str[j] = c;
			}
		}
	}
	for (auto c : str) {
		std::cout << c;
	}
	std::cout << std::endl;

}

