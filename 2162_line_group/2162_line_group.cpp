#include<iostream>

struct Point {
	int x, y;
	Point() :Point(0, 0) {}
	Point(int x, int y) :x(x), y(y) {}
	Point operator-(const Point& other) const {
		return Point(x - other.x, y - other.y);
	}
	Point operator+(const Point& other) const {
		return Point(x + other.x, y + other.y);
	}
	Point operator*(int c) {
		return Point(c * x, c * y);
	}
};
int prod(const Point& v1, const Point& v2) {
	return v1.x * v2.x + v1.y * v2.y;
}
struct Line {
	Point p1, p2;
	Line(const Point& p1, const Point& p2) :p1(p1), p2(p2) {}

	Point getVec(const Point& p) {
		Point v1 = p2 - p1;
		Point v2 = p - p1;

		return v2 * prod(v1, v1) + v1 * prod(v1, v2);
	}
	
	void isCrossing(const Line& other) {
		auto v1 = getVec(other.p1);
		auto v2 = getVec(other.p2);

		// todo

	}
};


int main() {
	
}
