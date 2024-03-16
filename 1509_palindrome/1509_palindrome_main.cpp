#include<iostream>
constexpr int MAX_INPUT_SIZE = 2502;

char str[MAX_INPUT_SIZE]{ 0, };
int count[MAX_INPUT_SIZE];

bool isPalindrome(int left, int right) {
	bool ret = true;
	while (left < right) {
		if (str[left] == str[right]) {
			left++;
			right--;
		}
		else {
			ret = false;
			break;
		}
	}
	return ret;
}

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);
	std::cout.tie(0);

	std::cin >> str;

	count[0] = 1;
	int N = MAX_INPUT_SIZE;
	for (int right = 1; right < MAX_INPUT_SIZE; right++) {
		if (str[right] == 0) {
			N = right;
			break;
		}
		else {
			count[right] = count[right - 1] + 1;
		}

		if (isPalindrome(0, right)) {
			count[right] = 1;
			continue;
		}

		for (int left = 1; left < right; left++) {
			if (isPalindrome(left, right)) {
				count[right] = std::min(count[left - 1] + 1, count[right]);
			}
		}
	}

	std::cout << count[N - 1];
	return 0;
}