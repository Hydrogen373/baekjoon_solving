#include<iostream>
#include<map>
#include<queue>
#define INSERTION 0
#define DELETION 1
#define SAME_COMPONENT 0
#define NOT_SAME_COMPONENT 1

int N, Q;
unsigned long long F;
//std::map<std::pair<int, int>, bool> edges;
std::map<int, std::vector<int>> adjacentList;
int* roots = nullptr;
int numComponent;

int getRoot(int x) {
	if (roots[x] == x) {
		return x;
	}
	return roots[x] = getRoot(roots[x]);
}
int unionSet(int x, int y) {
	if (getRoot(x) == getRoot(y)) {
		return SAME_COMPONENT;
	}
	roots[getRoot(y)] = getRoot(x);
	return NOT_SAME_COMPONENT;
}
int setRootBFS(int x) {
	std::queue<int> que;
	roots[x] = x;
	que.push(x);
	while (!que.empty()) {
		int node = que.front();
		que.pop();

		for (int i = 0; i < N; i++) {
			if ((edges.find({ (i < node ? i : node), (i < node ? node : i) }) != edges.end())
				&& roots[i] != x) 
			{
				roots[i] = x;
				que.push(i);
			}
		}
	}
	return 0;
}

int query1(int x, int y) {
	//if (edges.find({ x,y }) != edges.end()) {
	//	edges.erase({ x,y });
	//	return DELETION;
	//}
	//else {
	//	edges[{x, y}] = true;
	//	return INSERTION;
	//}
	for (int i = 0; i < 2;i++) {
		if (adjacentList.find(x) != adjacentList.end()) {
			// XXX
			// deletion or insertion

		}
		else {
			adjacentList[x].push_back(y);
		}
		std::swap(x, y);
	}
}

int main() {
	using namespace std;
	cin >> N >> Q;
	numComponent = N;
	roots = new int[N];
	for (int i = 0; i < N; i++) {
		roots[i] = i;
	}
	for (int i = 0; i < Q; i++) {
		int a, b, x, y;
		cin >> a >> b;

		x = (a ^ F) % N;
		y = (b ^ F) % N;

		if (x < y) {
			switch (query1(x, y)) {
			case INSERTION:
				if (unionSet(x, y) == NOT_SAME_COMPONENT) {
					--numComponent;
				}
				break;
			case DELETION:
				int rootNew = y,
					rootOld = getRoot(x);
				if (rootOld == y) {
					rootOld = y;
					rootNew = x;
				}
				setRootBFS(x);
				if (getRoot(x) != getRoot(y)) {
					// no longer same component
					++numComponent;
				}
				break;
			}
		}
		else {
			cout << (getRoot(x) == getRoot(y) ? 1 : 0) << endl;
		}

		F += numComponent;
	}


	delete[] roots;
	return 0;
}

