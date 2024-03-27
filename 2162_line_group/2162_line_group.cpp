#include<iostream>
#include<vector>
#define lli long long int

using MyVector = std::pair<lli, lli>;
struct Line {
	MyVector coef;
	MyVector bias;
	Line(int x1, int y1, int x2, int y2) {
		coef = std::make_pair(x2 - x1, y2 - y1);
		bias = std::make_pair(x1, y1);
	}
};
lli outerProduct(const MyVector& v1, const MyVector& v2) {
	return v1.first* v2.second - v1.second * v2.first;
}
lli innerProduct(const MyVector& v1, const MyVector& v2) {
	return v1.first * v2.first + v1.second * v2.second;
}
MyVector operator+(const MyVector& v1, const MyVector& v2) {
	return std::make_pair(v1.first + v2.first, v1.second + v2.second);
}
MyVector operator-(const MyVector& v) {
	return std::make_pair(-v.first, -v.second);
}
MyVector operator-(const MyVector& v1, const MyVector& v2) {
	return v1 + (-v2);
}
MyVector operator*(lli c, const MyVector& v) {
	return std::make_pair(v.first * c, v.second * c);
}

bool isOn(const Line& line, const MyVector& point) {
	auto u = point - line.bias;
	auto v = line.coef;

	auto r = (innerProduct(v, v) * u) - (innerProduct(v, u) * v);
	return innerProduct(r, r) == 0 && (innerProduct(-u, v - u) <= 0);
}


bool isCrossing(const Line& l1, const Line& l2) {
	auto coef = outerProduct(l1.coef, l2.coef);
	if (coef == 0) {
		auto len1 = innerProduct(l1.coef, l1.coef);
		auto len2 = innerProduct(l2.coef, l2.coef);

		if (len1 == 0 && len2 == 0) {
			return l1.bias.first == l2.bias.first 
				&& l1.bias.second == l2.bias.second;
		}
		else if (len1 == 0) {
			return isOn(l2, l1.bias);
		}
		else if (len2 == 0) {
			return isOn(l1, l2.bias);
		}
		else {
			return isOn(l1, l2.bias) || isOn(l1, l2.bias + l2.coef)
				|| isOn(l2, l1.bias) || isOn(l2, l1.bias + l1.coef);
		}
	}


	// x
	auto mn = std::min(0ll, coef);
	auto mx = std::max(0ll, coef);
	auto bias = outerProduct(l2.coef, l1.bias - l2.bias);
	if (bias < mn || mx < bias)
		return false;
	
	// y
	mn = std::min(0ll, -coef);
	mx = std::max(0ll, -coef);
	bias = outerProduct(l1.coef, l2.bias - l1.bias);
	return mn <= bias && bias <= mx;
}
struct UnionFind {
	int* roots;
	int* sizes;
	int mxsize = 1;
	int numGroup;
	UnionFind(int N) :roots(new int[N]),sizes(new int[N]),numGroup(N) {
		for (int i = 0; i < N; i++) {
			roots[i] = i;
			sizes[i] = 1;
		}
	}
	~UnionFind() {
		delete[] roots;
		delete[] sizes;
	}

	int find(int x) {
		if (roots[x] == x)
			return x;
		else return roots[x] = find(roots[x]);
	}

	void unite(int a, int b) {
		auto r1 = find(a);
		auto r2 = find(b);
		if (r1 == r2)
			return;

		if (sizes[r1] < sizes[r2]) {
			roots[r1] = r2;
			sizes[r2]+=sizes[r1];
			mxsize = std::max(mxsize, sizes[r2]);
		}
		else {
			roots[r2] = r1;
			sizes[r1] += sizes[r2];
			mxsize = std::max(mxsize, sizes[r1]);
		}
		numGroup--;
	}
};

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0); std::cout.tie(0);
	
	int N;
	std::cin >> N;

	std::vector<Line> lines;
	UnionFind uf(N);

	for (int i = 0; i < N; i++) {
		int x1, y1, x2, y2;
		std::cin >> x1 >> y1 >> x2 >> y2;
		lines.push_back(Line(x1, y1, x2, y2));
	}

	//
	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			if (isCrossing(lines[i], lines[j])) {
				uf.unite(i, j);
			}
		}
	}

	//
	std::cout << uf.numGroup << '\n' << uf.mxsize << std::endl;
	return 0;
}
