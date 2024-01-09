#include<iostream>
#include<fstream>

unsigned int N, Q, F{ 0 }, numComponent;
bool** adjacent = nullptr;
int* roots;

bool* check = nullptr;
void _dfs(int node) {
	if (check[node]) {
		return;
	}
	check[node] = true;
	for (unsigned i = 0; i < N; i++) {
		if (adjacent[node][i] && check[i] == false) {
			_dfs(i);
		}
	}

}

int findRoot(int a) {
	if (roots[a] == a) return a;
	else {
		return roots[a] = findRoot(roots[a]);
	}
}
void unionSet(int a, int b) {
	roots[findRoot(a)] = findRoot(b);
}

void dfs(int newroot) {
	check = new bool[N] {false, };
	_dfs(newroot);
	for (unsigned int i = 0; i < N; i++) {
		if (check[i]) {
			roots[i] = newroot;
		}
	}
	delete[] check;
}

int main() {
	using namespace std;
	ofstream input, output, log;
	input.open("input.txt");
	log.open("log.txt");
	output.open("output.txt");

	cin >> N >> Q;
	numComponent = N;
	input << N << ' ' << Q << endl;
	log << N << ' ' << Q << endl;
	log << "#i\tF\tnC\ta\tb\tx\ty\tout\n";


	roots = new int[N];
	adjacent = new bool* [N];
	for (unsigned int i = 0; i < N; i++) {
		roots[i] = i;
		adjacent[i] = new bool[N] {false, };
	}

	for (unsigned int i = 0; i < Q; i++) {
		unsigned int a, b, x, y;
		a = rand() << 1 >> 1;
		b = rand() << 1 >> 1;
		x = (a ^ F) % N;
		y = (b ^ F) % N;
		if (x == y) {
			i--;
			continue;
		}
		input << a << ' ' << b << endl;
		log << i << '\t' << F << '\t' << numComponent << '\t' << a << '\t' << b << '\t' << x << '\t' << y << '\t';

		if (x < y) {
			bool taskIsRemoving = adjacent[x][y];
			if (taskIsRemoving) {
				adjacent[x][y] = adjacent[y][x] = false;
				int oldRoot = findRoot(x);
				int newRoot = oldRoot == x ? y : x;
				dfs(newRoot);
				if (findRoot(x) != findRoot(y)) {
					numComponent++;
				}
			}
			else {
				if (findRoot(x) != findRoot(y)) {
					unionSet(x, y);
					numComponent--;
				}
				adjacent[x][y] = adjacent[y][x] = true;
			}
			log << endl;
		}
		else {
			bool result = (findRoot(x) == findRoot(y));
			cout << result << endl;
			log << result << endl;
			output << result << endl;
		}

		F += numComponent;
	}
	// terminate
	for (unsigned int i = 0; i < N; i++) {
		delete[] adjacent[i];
	}
	delete[] adjacent;
	delete[] roots;
	input.close();
	log.close();
	output.close();
	return 0;
}
