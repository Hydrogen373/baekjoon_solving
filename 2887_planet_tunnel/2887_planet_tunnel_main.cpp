#include<iostream>
#include<vector>
#include<algorithm>


class Planet {
public:
	int num = -1;
	int point[3]{ 0, };
	Planet(int num, int  x, int y, int z) :num(num), point{ x,y,z } {}
};

class Edge {
public:
	int a, b, distance;

	Edge(int a, int b, int distance) 
		:a(a), b(b), distance(distance) {}

	bool operator<(const Edge& other) const {
		//if (distance == other.distance) {
		//	return a > other.a || a == other.a && b > other.b;
		//}
		//else {
		//	return distance > other.distance;
		//}
		return distance > other.distance;
	}
};

class UnionFind {
	std::vector<int> pointers;
	std::vector<int> size;
public:
	UnionFind(int N) :pointers(N, -1), size(N, 0) {
	}
	
	int find(int a) {
		if (pointers[a] == -1) return a;
		else return pointers[a] = find(pointers[a]);
	}
	void unite(int a, int b) {
		a = find(a);
		b = find(b);

		if (a == b) return;

		if (size[a] < size[b]) {
			pointers[a] = b;
			size[b] += size[a];
		}
		else {
			pointers[b] = a;
			size[a] += size[b];
		}
		return;
	}


};

std::vector<Planet> planets;
int N;
std::vector<Edge> edges;
int count = 0;
unsigned long long int cost = 0;

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);


	std::cin >> N;
	for (int i = 0; i < N; i++) {
		int x, y, z;
		std::cin >> x >> y >> z;
		planets.push_back(Planet(i, x, y, z));
	}
	UnionFind uf(N);


	for (int i = 0; i < 3; i++) {
		std::sort(planets.begin(), planets.end(),
			[i](const Planet& p1, const Planet& p2)
			{
			//return p1.point[i] < p2.point[i]
			//|| p1.point[i] == p2.point[i] && p1.num < p2.num;
				return p1.point[i] < p2.point[i];
			}
		);

		for (int j = 0; j < N - 1; j++) {
			Edge edge(planets[j].num, planets[j + 1].num,
				planets[j + 1].point[i] - planets[j].point[i]);

			edges.push_back(edge);
			std::push_heap(edges.begin(), edges.end());
		}
	}

	while (count < N - 1) {
		Edge edge = edges[0];
		std::pop_heap(edges.begin(), edges.end());
		edges.pop_back();

		if (uf.find(edge.a) == uf.find(edge.b)) {
			continue;
		}

		uf.unite(edge.a, edge.b);
		count += 1;
		cost += edge.distance;

	}

	std::cout << cost;


	return 0;
}

