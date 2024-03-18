#include<iostream>
#include<vector>
#define ulli unsigned long long int

constexpr bool Prime = false;
constexpr bool NotPrime = true;
bool nums[4'000'002]{ 0, };
int N;
std::vector<int> PrimeNumbers;

int main() {
	nums[0] = nums[1] = NotPrime;

	nums[2] = Prime;
	std::cin >> N;

	if (N < 2) {
		std::cout << 0;
		return 0;
	}
	else if (N == 2) {
		std::cout << 1;
		return 0;
	}

	// body
	for (ulli i = 2; i <= N; i++) {
		if (nums[i] == NotPrime)
			continue;
		else {
			PrimeNumbers.push_back(i);
		}
		for (ulli j = 1; i * j <= N; j++) {
			nums[i * j] = NotPrime;
		}
	}

	int res = 0;
	int left = 0;
	int right = 0;
	int sum = PrimeNumbers[left];
	while (left <= right && right < PrimeNumbers.size()) {
		if (sum == N) {
			res += 1;
			right += 1;
			if (right == PrimeNumbers.size())
				break;
			sum += PrimeNumbers[right];
		}
		else if (sum < N) {
			right += 1;
			if (right == PrimeNumbers.size())
				break;
			sum += PrimeNumbers[right];
		}
		else if (sum > N) {
			sum -= PrimeNumbers[left];
			left += 1;
		}
	}



	std::cout << res;

	// body end

	return 0;
}