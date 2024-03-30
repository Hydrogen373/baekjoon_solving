#include<iostream>
#include<stack>

using lli = long long int;
using Vec2D = std::pair<lli, lli>;
Vec2D operator-(const Vec2D& v1, const Vec2D& v2) {
	return std::make_pair(v1.first - v2.first, v1.second - v2.second);
}
lli outerProduct(const Vec2D& v1, const Vec2D& v2) {
	return v1.first * v2.second - v1.second * v2.first;
}

int N;
std::stack<Vec2D> points;
int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0); std::cout.tie(0);
	lli result = 0;

	std::cin >> N;

	lli x, y;
	std::cin >> x >> y;
	Vec2D first = std::make_pair(x, y);
	points.push(first);

	for (int i = 1; i < N; i++) {
		std::cin >> x >> y;
		points.push(std::make_pair(x, y));
	}

	points.push(first);
	//

	while (points.empty() == false) {
		Vec2D p1, p2, p3;
		p3 = points.top();
		points.pop();
		p2 = points.top();
		points.pop();
		p1 = points.top();

		if (p1 == p3)
			break;


		auto prod = outerProduct(p2 - p1, p3 - p2);
		result += prod;

		points.push(p3);
	}
	result = std::max(-result, result);

	//
	std::cout << result / 2 << '.' << (result % 2 == 0 ? 0 : 5) << std::endl;
	return 0;
}


