#include<iostream>

char str[1000]{ 0, };
int count[10]{ 0, };
int toErase[10]{ 0, };
int getMaxNum(int i, int max) {
	if (str[i] == 0)
		return -1;
	int val = str[i] - '0';
	if (count[val] != toErase[val]) {
		max = max > val ? max : val;
	}
	if (toErase[val] > 0 && max < 9) {
		count[val] -= 1;
		toErase[val] -= 1;
		max = getMaxNum(i + 1, max);
		count[val] += 1;
		toErase[val] += 1;
	}
	return max;
}

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0); std::cout.tie(0);

	std::cin >> str;
	for (int i = 0; i < 1000 && str[i] != 0; i++) {
		count[str[i] - '0'] += 1;
	}
	{
		char tmp[1000]{ 0, };
		std::cin >> tmp;
		int i = 0;
		while (tmp[i] != 0) {
			toErase[tmp[i] - '0'] += 1;
			i += 1;
		}
	}

	int i = 0;
	int m = getMaxNum(i, 0);
	m += '0';
	while (str[i] != 0) {
		count[str[i] - '0'] -= 1;
		if (str[i] != m) {
			toErase[str[i] - '0'] -= 1;
		}
		else {
			std::cout << str[i];
			m = getMaxNum(i + 1, 0);
			if (m == -1)
				break;
			m += '0';
		}
		i += 1;
	}



	return 0;
}
