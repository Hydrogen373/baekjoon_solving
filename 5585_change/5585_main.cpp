#include<iostream>

int main() {
	using namespace std;
	int result{ 0 };
	int change;
	cin >> change;

	change = 1000 - change;

	int v[]{ 500,100,50,10,5,1 };

	for (int i = 0; i < 6; i++) {
		int tmp = change / (v[i]);
		result += tmp;
		change -= v[i] * tmp;
	}
	cout << result;
	return 0;
}