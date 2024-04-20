#include<iostream>
#include<vector>

using ulli = unsigned long long int;
ulli N;
std::vector<ulli> primes;

void find_primes() {
	if (N <= 1)
		return;
	ulli i = 2;
	while (i * i <= N) {
		if (N % i == 0) {
			primes.push_back(i);
		}
		while (N % i == 0) {
			N /= i;
		}
		i += 1;
	}
	if (N > 1) {
		primes.push_back(N);
		N = 1;
	}

}

int main() {
	std::cin >> N;
	ulli result = N;

	//
	find_primes();
	for (auto p : primes) {
		result -= result / p;
	}
	//

	std::cout << result;
	return 0;
}
