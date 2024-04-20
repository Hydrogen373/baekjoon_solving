#include<iostream>
#include<vector>
#include<map>
#include<algorithm>
#include<queue>


using us = unsigned short;
using uli = unsigned long;
using NodeNum = us;

struct Node {
	std::vector<NodeNum> predecessors;
	std::vector<std::pair<NodeNum, uli>> successors;
	uli mCriticalPathTime = 0;
	us counter = 0;

};

std::vector<Node> nodes;
std::vector<NodeNum> nextVisitNodes;
uli N, M;

void addRoute(NodeNum a, NodeNum b, us weight) {
	nodes[a].successors.push_back(std::make_pair(b, weight));
	nodes[b].counter += 1;
}

void renewCriticalPathTime(NodeNum a, NodeNum b, uli criticalPathTime) {
	if (criticalPathTime > nodes[b].mCriticalPathTime) {
		nodes[b].mCriticalPathTime = criticalPathTime;
		nodes[b].predecessors.clear();
		nodes[b].predecessors.push_back(a);
	}
	else if (criticalPathTime == nodes[b].mCriticalPathTime) {
		nodes[b].predecessors.push_back(a);
	}

	nodes[b].counter -= 1;
}

void visit(NodeNum n) {
	NodeNum& a = n;
	uli& mCriticalPathTime = nodes[a].mCriticalPathTime;
	for (auto iter : nodes[n].successors) {
		NodeNum& b = iter.first;
		uli criticalPathTime = mCriticalPathTime + iter.second;

		renewCriticalPathTime(a, b, criticalPathTime);
		if (nodes[b].counter == 0) {
			nextVisitNodes.push_back(b);
		}
	}
}

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);
	std::cout.tie(0);
	std::cin >> N >> M;
	nodes = std::vector<Node>(N + 2);

	for (int i = 0; i < M; i++) {
		NodeNum a, b;
		us weight;
		std::cin >> a >> b >> weight;
		addRoute(a, b, weight);
	}
	NodeNum s, e;
	std::cin >> s >> e;

	nextVisitNodes.push_back(s);

	//
	while (nextVisitNodes.empty() == false)
	{
		NodeNum n = nextVisitNodes.back();
		nextVisitNodes.pop_back();

		visit(n);
	}

	std::queue<NodeNum> que;
	std::vector<bool> check(N + 2, 0);
	que.push(e);
	uli result = 0;
	while (que.empty() == false) {
		auto n = que.front();
		que.pop();

		for (auto iter : nodes[n].predecessors) {
			if (check[iter] == false) {
				que.push(iter);
				check[iter] = true;
			}
			result += 1;
		}
	}

	std::cout << nodes[e].mCriticalPathTime << '\n';
	std::cout << result;

	//
	return 0;
}



