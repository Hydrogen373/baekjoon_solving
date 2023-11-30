#include<iostream>

unsigned long long int x, y;
unsigned long long int result;
unsigned long long int W, S;
int main() {
	using namespace std;
	cin >> x >> y >> W >> S;
	if (x > y) {
		swap(x, y);
	}
	if (S<W) {
		unsigned long long t = y - x;
		result = S * (x + ((t >> 1) << 1)) + (t % 2 == 0 ? 0 : W);
	}
	else if (S < 2 * W) {
		result = S * x + W * (y - x);
	}
	else {
		result = W * (x + y);
	}

	cout << result;

	return 0;
}
