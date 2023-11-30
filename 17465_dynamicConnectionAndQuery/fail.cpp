#ifdef Fail

#include<iostream>
#include<queue>

unsigned int N, Q;
using Conn = unsigned char;
Conn** map;
using Node = unsigned int;
int* roots;
unsigned long long F = 0;
unsigned int numComponent;
unsigned int* numConnection;

Conn& findConnectionCell(Node p, Node q) {
	return map[q][p / 8];
}

Conn getKey(Node p) {
	return (unsigned char)1 << (p % 8);
}

bool isConnected(Node p, Node q) {
	return (findConnectionCell(p, q) & getKey(p)) != 0;
}

int getRoot(Node i) {
	if (i == roots[i]) {
		return i;
	}
	else {
		return roots[i] = getRoot(roots[i]);
	}
}

bool isSameSet(Node p, Node q) {
	return getRoot(p) == getRoot(q);
}

int unionSet(Node i, Node j) {
	Node r1 = getRoot(i);
	Node r2 = getRoot(j);
	if (r1 != r2) {
		roots[r2] = r1;
		--numComponent;
	}
	return 0;
}

int setRootBFS(Node root) {
	std::queue<Node> que;
	roots[root] = root;
	que.push(root);
	while (!que.empty()) {
		Node tmp = que.front();
		que.pop();
		for (Node i = 0; i < tmp; i++) {
			if (isConnected(i, tmp) && roots[i] != roots[tmp]) {
				roots[i] = root;
				if (numConnection[i] > 1) {
					que.push(i);
				}
			}
		}
		for (Node j = tmp + 1; j < N; j++) {
			if (isConnected(tmp, j) && roots[j] != roots[tmp]) {
				roots[j] = root;
				if (numConnection[j] > 1) {
					que.push(j);
				}
			}
		}
	}
	return 0;
}

int initRoots() {
	for (Node i = 0; i < N; i++) {
		roots[i] = i;
	}
	numComponent = 0;
	for (Node i = 0; i < N; i++) {
		if (roots[i] != i) {
			continue;
		}
		else {
			++numComponent;
			if (numConnection[i] != 0) {
				setRootBFS(i);
			}
		}
	}
	return 0;
}


int main() {
	using namespace std;
	cin >> N >> Q;
	map = new Conn * [N];
	roots = new int[N];
	numComponent = N;
	numConnection = new unsigned int[N] {0, };

	for (unsigned int i = 0; i < N; i++) {
		map[i] = new Conn[i / 8 + 1];
		roots[i] = i;
	}

	for (unsigned int i = 0; i < Q; i++) {
		unsigned int a, b, x, y;
		std::cin >> a >> b;
		x = (a ^ F) % N;
		y = (b ^ F) % N;

		if (x < y) {
			Conn& cell = findConnectionCell(x, y);
			Conn key = getKey(x);
			cell ^= key;
			if (isConnected(x, y)) {
				numConnection[x] += 1;
				numConnection[y] += 1;
				unionSet(x, y);
			}
			else {
				numConnection[x] -= 1;
				numConnection[y] -= 1;
				//initRoots();
				Node rootOld = getRoot(x);
				Node rootNew = y;
				if (rootOld == rootNew) {
					rootNew = x;
				}
				setRootBFS(rootNew);
				if (getRoot(rootOld) != getRoot(rootNew)) {
					++numComponent;
				}
			}
		}
		else {
			cout << isSameSet(x, y) << endl;
		}



		F += numComponent;
	}
	//terminate
	for (unsigned int i = 0; i < N; i++) {
		delete[] map[i];
	}
	delete[] map;
	delete[] roots;
	delete[] numConnection;
	return 0;
}




#endif // !Fail
