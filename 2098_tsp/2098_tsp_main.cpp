#include<iostream>
#include<vector>
#include<algorithm>
#include<cassert>

using ulli = unsigned long long int;
using ui = unsigned int;

ui mp[16][16]{ 0, };
struct Node;
std::vector<Node*> nodes;
Node* best = nullptr;
int N;

struct Node {
	ulli lowerBound = 0;
	ulli cost = 0;
	unsigned char last = 0;
	unsigned char size = 0;
	unsigned short visited = 0x0000;

	Node() {}

	void setInitialValue() {
		lowerBound = 0;
		cost = 0;
		last = 0;
		size = 1;
		visited = 0x0001;
	}
	void setDummyBest() {
		lowerBound = UINT64_MAX;
		cost = UINT64_MAX;
		last = 0;
		size = N;
		visited = 0xffff;
	}

	Node(const Node* prev, unsigned char next) :
		last(next),
		visited(prev->visited),
		size(prev->size),
		cost(prev->cost)
	{
		visited = visited | (1 << next);
		cost += mp[prev->last][next];
		size += 1;
		if (size == N) {
			size += 1;
			cost += mp[next][0];
		}
		_setLowerBound();
	}
private: 
	void _setLowerBound() {
		lowerBound = cost;
		if (size >= N) {
			return;
		}

		ui val[16]{ 0, };
		bool cannotCycle = true;

		for (int i = 0; i < N; i++) {
			if ((visited >> i) & 0x0001) {
				val[i] = 0;
			}
			else {
				val[i] = mp[i][0];
				if (mp[i][0] != INT32_MAX) {
					cannotCycle = false;
				}
				for (int j = 1; j < N; j++) {
					if ((visited >> j) & 0x0001)
						continue;
					val[i] = std::min(val[i], mp[i][j]);
				}
			}
		}

		if (cannotCycle) {
			lowerBound = UINT64_MAX;
			return;
		}


		val[last] = INT32_MAX;
		for (int j = 1; j < N; j++) {
			if ((visited >> j) & 0x0001)
				continue;
			val[last] = std::min(val[last], mp[last][j]);
		}
		
		for (int i = 0; i < N; i++) {
			lowerBound += val[i];
		}
	}
};

struct {
	bool operator()(const Node& a, const Node& b) {
		return a.lowerBound > b.lowerBound
			|| (a.lowerBound == b.lowerBound && a.size < b.size);
	}
	bool operator()(Node* a, Node* b) {
		return a->lowerBound > b->lowerBound
			|| (a->lowerBound == b->lowerBound && a->size < b->size);
	}
} LessLowerBoundFirst;

void visit(Node* nd) {
	assert(nd->size != N);
	if (nd->lowerBound >= best->lowerBound) {
		return;
	}
	else if (nd->size == N + 1) {
		delete best;
		best = nd;
		return;
	}
	else {
		for (int i = 0; i < N; i++) {
			if ((nd->visited >> i) & 0x0001)
				continue;
			auto next = new Node(nd, i);
			if (next->lowerBound > best->lowerBound)
				delete next;
			else {
				nodes.push_back(next);
				std::push_heap(nodes.begin(), nodes.end(), LessLowerBoundFirst);
			}
		}
		delete nd;
		return;
	}
}

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0); std::cout.tie(0);
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			std::cin >> mp[i][j];
			if (mp[i][j] == 0)
				mp[i][j] = INT32_MAX;
		}
	}

	best = new Node;
	best->setDummyBest();

	auto nd = new Node;
	nd->setInitialValue();
	nodes.push_back(nd);

	//

	while (nodes.empty() == false) {
		auto nd = nodes.front();
		std::pop_heap(nodes.begin(), nodes.end(), LessLowerBoundFirst);
		nodes.pop_back();

		if (nd->lowerBound >= best->lowerBound) {
			delete nd;
			break;
		}

		visit(nd);
	}

	std::cout << best->lowerBound;
	//
	for (auto nd : nodes) {
		delete nd;
	}
	return 0;
}