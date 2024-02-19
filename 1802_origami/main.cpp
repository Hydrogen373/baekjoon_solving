#include<iostream>

struct TestCase {
	char* str;
	unsigned long long int len;
	TestCase() :str(nullptr), len(0) {}
	~TestCase() {
		if (str != nullptr) {
			delete[] str;
			str = nullptr;
		}
	}
	void init() {
		str = new char[3000];
		while (true) {
			str[len] = std::cin.get();
			if (str[len] == '\0' || str[len] == '\n') {
				str[len] = '\0';
				++len;
				break;
			}
			else
				++len;
		}
	}
	bool run() {
		while (len >= 2) {
			auto pivot = len / 2;
			for (int i = 0; i < pivot - 1; i++) {
				if ((str[i] ^ str[len - 2 - i]) == 0) {
					return false;
				}
			}
			len = len / 2;
		}
		return true;
	}
};

int main() {
	std::cin.sync_with_stdio(false);
	std::cin.tie(0); std::cout.tie(0);
	int T;
	std::cin >> T;
	std::cin.get();
	for (int i = 0; i < T; i++) {
		TestCase tc;
		tc.init();
		std::cout << (tc.run() ? "YES" : "NO") << '\n';
	}
	return 0;
}