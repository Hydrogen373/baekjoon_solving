#include<iostream>
#include<map>
#include<queue>
#define SAME_COMPONENT 1

struct Edge {
	int first;
	int second;
	int level;
};

bool operator<(const Edge& a, const Edge& b) {
	/*if (a.first < b.first) {
		return true;
	}
	else if (a.first == b.first) {
		if (a.second < b.second) {
			return true;
		}
		else if (a.second == b.second) {
			return a.level < b.level;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}*/
	if (a.level > b.level) {
		return true;
	}
	else if (a.level == b.level) {
		if (a.first < b.first) {
			return true;
		}
		else {
			return a.first == b.first && a.second < b.second;
		}
	}
	else {
		return false;
	}
}

//bool operator<(const std::pair<int, int>& a, const std::pair<int, int>& b) {
//	return a.first < b.first || a.first == b.first && a.second == b.second;
//}

struct MSTNode {
	Edge edge;
	MSTNode* prev;
	MSTNode* next;
};

int N, Q;
unsigned long long F;
std::map<std::pair<int, int>, int> levels;
std::map<Edge, MSTNode*> mstnodes;
int numComponent;

void rooting(MSTNode* node) {
	if (node->prev == nullptr) {
		return;
	}

	MSTNode* start = node->prev;
	while (start->prev != nullptr) {
		start = start->prev;
	}

	MSTNode* end = node;
	while (end->next != nullptr) {
		end = end->next;
	}

	node->prev->next = nullptr;
	node->prev = nullptr;
	end->next = start;
	start->prev = end;
	return;
}

int query1(int x, int y) {
	// insertion
	if (levels.find({ x,y }) == levels.end()) {
		levels.insert({ { x, y }, 0 });
		levels.insert({ { y, x }, 0 });
		// same component
		if (query2(x, y) == true) {
			return;
		}
		// not same component
		MSTNode* nodeX, * nodeY, * xy, * yx;
		auto iter = mstnodes.find({ x,x,0 });
		if (iter == mstnodes.end()) {
			nodeX = new MSTNode{ {x,x,0},nullptr,nullptr };
			mstnodes.insert({ {x,x,0},nodeX });
		}
		else {
			nodeX = iter->second;
			rooting(nodeX);
		}
		iter = mstnodes.find({ y,y,0 });
		if (iter == mstnodes.end()) {
			nodeY = new MSTNode{ {y,y,0},nullptr,nullptr };
			mstnodes.insert({ {y,y,0},nodeY });
		}
		else {
			nodeY = iter->second;
			rooting(nodeY);
		}
		xy = new MSTNode{ {x,y,0},nodeX,nodeY };
		nodeY->prev = xy;
		while (nodeY->next != nullptr) {
			nodeY = nodeY->next;
		}
		nodeY->next = yx;
		yx = new MSTNode{ {y,x,0},nodeY,nodeX->next };
		nodeX->next->prev = yx;
		nodeX->next = xy;

		mstnodes.insert({ {x,y,0}, xy });
		mstnodes.insert({ {y,x,0}, yx });
		return;
	}

	// deletion
	// when mst doesn't have edge(x,y,0)

	// TODO
	//if (mstnodes.find({ x,y,0 }) == mstnodes.end()) {
	//	levels.erase({ x,y });
	//}
	//else {
	//	// count nodes
	//	MSTNode* xy, * yx;
	//	MSTNode* alter;

	//	int level = levels[{x, y}];
	//	for (; level >= 0; level--) {
	//		xy = mstnodes[{x, y, level}];
	//		yx = mstnodes[{y, x, level}];

	//		// find alternative edge
	//	}


	//}

	


}
int query2(int x, int y) {
	auto iter = mstnodes.find({ x,x,0 });
	MSTNode* nodeX, * nodeY;
	if (iter != mstnodes.end()) {
		nodeX = iter->second;
	}
	else {
		return 0;
	}
	iter = mstnodes.find({ y,y,0 });
	if (iter != mstnodes.end()) {
		nodeY = iter->second;
	}
	else {
		return 0;
	}

	rooting(nodeX);

	while (nodeY->prev != nullptr) {
		nodeY = nodeY->prev;
	}

	return nodeX == nodeY;
}

int main() {
	using namespace std;
	cin >> N >> Q;
	numComponent = N;

	for (int i = 0; i < Q; i++) {
		int a, b, x, y;
		cin >> a >> b;

		x = (a ^ F) % N;
		y = (b ^ F) % N;

		if (x < y) {
			query1(x,y);
		}
		else {
			//cout << (getRoot(x) == getRoot(y) ? 1 : 0) << endl;
			cout << query2(x, y);
		}

		F += numComponent;
	}


	return 0;
}

