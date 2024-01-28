#define DEBUG
#include<iostream>
#include<map>
#include<set>

#ifdef DEBUG
#include<cassert>
#include<fstream>
#endif // DEBUG
#ifndef DEBUG
#define assert(x)
#endif // !DEBUG




using std::map;
using std::set;
using std::make_pair;
using Vertex = unsigned int;

class Edge {
public:
	Vertex a, b;
	Edge(Vertex a, Vertex b):a(a),b(b) {
		if (this->a > this->b) {
			std::swap(this->a, this->b);
		}
	}
	bool operator<(const Edge& other) const {
		return a < other.a || (a == other.a && b < other.b);
	}

};

class Ark {
public:
	Vertex a, b;
	//int level;
	Ark(Vertex a, Vertex b) :a(a), b(b) {}
	bool operator<(const Ark& other) const {
		return a < other.a || (a == other.a && b < other.b);
	}
};

class SeriesNode {
public:
	Ark value;
	int size;
	SeriesNode* l, * p, * r;
	// SeriesNode* leftmostVertex;
	SeriesNode* leftmostHighestNode;
	SeriesNode* leftmostNode;
	bool highest;

	SeriesNode(const Ark& ark, bool highest = false) :
		value(ark),
		size(1),
		l(nullptr),
		p(nullptr),
		r(nullptr),
		leftmostHighestNode(nullptr),
		leftmostNode(this),
		highest(highest)
	{
		if (highest)
			leftmostHighestNode = this;
	}

	void update() {
		this->size = 1;
		if (l != nullptr) size += l->size;
		if (r != nullptr) size += r->size;

		if (l != nullptr && l->leftmostHighestNode != nullptr)
			leftmostHighestNode = l->leftmostHighestNode;
		else if (this->highest)
			leftmostHighestNode = this;
		else if (r != nullptr && r->leftmostHighestNode != nullptr)
			leftmostHighestNode = r->leftmostHighestNode;
		else
			leftmostHighestNode = nullptr;

		if (l != nullptr)
			leftmostNode = l->leftmostNode;
		else leftmostNode = this;
		assert(leftmostNode != nullptr);
	}

	void rotate() {
		if (p == nullptr) return;

		SeriesNode* grandpa = p->p;
		SeriesNode* parent = p;
		SeriesNode* child = nullptr;

		if (grandpa != nullptr) {
			if (grandpa->l == parent) {
				grandpa->l = this;
			}
			else {
				grandpa->r = this;
			}
		}

		if (parent->l == this) {
			parent->l = child = this->r;
			this->r = parent;
		}
		else {
			parent->r = child = this->l;
			this->l = parent;
		}
		parent->p = this;	
		this->p = grandpa;

		if (child != nullptr) {
			child->p = parent;
		}

		parent->update();
		this->update();
		return;
	}

	void splay() {
		while (this->p != nullptr) {
			if (p->p != nullptr) {
				if ((p->p->l == p) == (p->l == this))
					p->rotate();
				else
					this->rotate();
			}
			this->rotate();
		}
		return;
	}

	SeriesNode* detachLeftSeries() {
		splay();
		if (l == nullptr) return nullptr;
		SeriesNode* result = l;
		l->p = nullptr;
		l = nullptr;
		update();
		assert(result->p == nullptr);
		assert(leftmostNode == this);
		return result;
	}

	SeriesNode* detachRightSeries() {
		splay();
		if (r == nullptr) return nullptr;
		SeriesNode* result = r;
		r->p = nullptr;
		r = nullptr;
		update();
		assert(result->p == nullptr);
		return result;
	}

	static void joinSeries(SeriesNode* a, SeriesNode* b) {
		assert(a != nullptr);
		a = a->getRoot();
		assert(b != nullptr);
		b = b->getRoot()->leftmostNode;
		assert(b != nullptr);
		b->splay();

		a->p = b;
		assert(b->l == nullptr);
		b->l = a;
		b->update();
	}

	void setHead() {
		splay();
		if (l == nullptr) return;

		SeriesNode* leftSeries = detachLeftSeries();
		assert(leftSeries != nullptr);
		joinSeries(this, leftSeries);
		return;
	}

	SeriesNode* getRoot() {
		SeriesNode* result = this;
		while (result->p != nullptr)
			result = result->p;
		return result;
	}

	bool operator<(const SeriesNode& other) {
		return value < other.value;
	}

};

class NoEdge :std::exception {};
class MST {
	map<Ark, SeriesNode*> mp;

	SeriesNode* makeNode(const Ark& ark, bool highest = false) {
		SeriesNode* node = new SeriesNode(ark, highest);
		mp.insert(make_pair(ark, node));
		return node;
	}

public:
	~MST() {
		for (auto u : mp) {
			delete[] u.second;
		}
	}
	SeriesNode* find(const Ark& ark) {
		auto iter = mp.find(ark);
		if (iter == mp.end()) {
			return nullptr;
		}
		else {
			return iter->second;
		}
	}

	bool isContaining(const Edge& edge) {
		return find(Ark(edge.a, edge.b)) != nullptr;
	}

	bool isSameComponent(Vertex a, Vertex b) {
		auto aa = find(Ark(a, a));
		auto bb = find(Ark(b, b));
		if (aa == nullptr || bb == nullptr) return false;
		return aa->getRoot() == bb->getRoot();
	}

	void insert(const Edge& edge, bool highest = true) {
		Vertex a = edge.a;
		Vertex b = edge.b;
		assert(a < b);
		assert(find(Ark(a,b)) == nullptr);

		SeriesNode* aa, * bb;
		aa = find(Ark(a, a));
		bb = find(Ark(b, b));
		if (aa == nullptr) {
			aa = makeNode(Ark(a, a));
		}
		if (bb == nullptr) {
			bb = makeNode(Ark(b, b));
		}

		SeriesNode* ab = makeNode(Ark(a, b), true);
		SeriesNode* ba = makeNode(Ark(b, a));

		aa->setHead();
		bb->setHead();

		SeriesNode::joinSeries(aa, ab);
		SeriesNode::joinSeries(ab, bb);
		SeriesNode::joinSeries(bb, ba);
	}

	void remove(const Edge& edge, int level = 0) {
		Vertex a = edge.a;
		Vertex b = edge.b;

		SeriesNode* ab = find(Ark(a, b));
		SeriesNode* ba = find(Ark(b, a));
		assert(ab != nullptr);
		assert(ba != nullptr);

		ab->setHead();

		ab->detachRightSeries();

		ba->detachLeftSeries();
		ba->detachRightSeries();

		mp.erase(Ark(a, b));
		mp.erase(Ark(b, a));

		delete[] ab, ba;
	}
};

class ExtraEdgeHolder {
	set<Vertex> conn[100'000];

public:
	bool isContaining(const Edge& edge) {
		return conn[edge.a].find(edge.b) != conn[edge.a].end();
	}
	void insert(const Edge& edge, int level = 0) {
		conn[edge.a].insert(edge.b);
		conn[edge.b].insert(edge.a);
	}

	void remove(const Edge& edge) {
		assert(conn[edge.a].find(edge.b) != conn[edge.a].end());
		assert(conn[edge.b].find(edge.a) != conn[edge.b].end());
		conn[edge.a].erase(edge.b);
		conn[edge.b].erase(edge.a);
	}

	Vertex findAlterEdge(Vertex u) {
		if (conn[u].empty()) {
			throw NoEdge();
		}
		return *(conn[u].begin());
	}
};

class Graph {
	MST mst[20];
	ExtraEdgeHolder extraEdges[20];
	struct EdgeInformation {
		bool isMST;
		int level;
		EdgeInformation() :EdgeInformation(0, 0) {}
		EdgeInformation(bool isMST, int level) :isMST(isMST), level(level) {}
		bool operator<(const EdgeInformation& other) {
			if (isMST == other.isMST) return level < other.level;
			else return isMST;
		}
	};
	map<Edge, EdgeInformation> edgeInfo;
	
	void _inorder(SeriesNode* node, int level, set<Vertex>& vertexes) {
		if (node == nullptr) return;
		_inorder(node->l, level, vertexes);
		if (node->highest) { // node is not vertex
			Edge edge(node->value.a, node->value.b);
			mst[level + 1].insert(edge);
			edgeInfo[edge].level += 1;
			node->highest = false;
		}
		else if (node->value.a == node->value.b)
		{
			vertexes.insert(node->value.a);
		}
		_inorder(node->r, level, vertexes);
	}

	void levelUpMST(SeriesNode *node, int level, set<Vertex> &vertexes)
	{
		if (node == nullptr)
			return;
		else
		{
			node->splay();
			node = node->leftmostHighestNode;
		}
		while (node != nullptr)
		{
			mst[level + 1].insert(Edge(node->value.a, node->value.b));
			vertexes.insert(node->value.a);
			vertexes.insert(node->value.b);

			node->splay();
			node->highest = false;
			node->update();
			node = node->leftmostHighestNode;
		}
	}

public:
	unsigned int numComponent = 0;
	bool isContaining(const Edge& edge) {
		return edgeInfo.find(edge) != edgeInfo.end();
	}

	void insert(const Edge& edge) {
		assert(edge.a < edge.b);
		bool insertIntoMST = !(mst[0].isSameComponent(edge.a, edge.b));
		if (insertIntoMST) {
			mst[0].insert(edge);
			numComponent -= 1;
		}
		else {
			extraEdges[0].insert(edge);
		}
		edgeInfo.insert(make_pair(edge, EdgeInformation(insertIntoMST, 0)));
	}

	void remove(const Edge& edge) {
		assert(edge.a < edge.b);
		bool isMST = edgeInfo[edge].isMST;
		int level = edgeInfo[edge].level;
		edgeInfo.erase(edge);
		if (isMST == false) {
			extraEdges[level].remove(edge);
			return;
		}

		Vertex a = edge.a;
		Vertex b = edge.b;
		for (int i = 0; i <= level; i++) {
			mst[i].remove(edge);
		}

		bool found = false;
		Edge alterEdge(0, 0);
		// levelup and find alternative edge
		for (int i = level; i >= 0; i--) {
			auto aa=mst[i].find(Ark(a, a));
			auto bb=mst[i].find(Ark(b, b));
			assert(aa != nullptr);
			assert(bb != nullptr);

			aa->splay();
			bb->splay();

			auto snode = (aa->size < bb->size ? aa : bb);

			// levelup mst
			set<Vertex> vertexes;
			levelUpMST(snode, i, vertexes);

			for (auto u = vertexes.begin(); u != vertexes.end();)
			{
				try {
					Vertex v = extraEdges[i].findAlterEdge(*u);
					Edge edge(*u, v);
					extraEdges[i].remove(edge);
					// edge is alternative edge
					if (vertexes.find(v) == vertexes.end()) {
						found = true;
						alterEdge = edge;
						break;
					}
					// level up extra edge
					else { 
						extraEdges[i + 1].insert(edge);
						edgeInfo[edge].level += 1;
						continue;
					}
				}
				catch (const NoEdge&) {
					u++;
				}
			}
			if (found) break;
		}

		if (found) {
			int alterLevel = edgeInfo[alterEdge].level;
			for (int i = 0; i <= alterLevel; i++) {
				bool &&highest = (i == alterLevel);
				mst[i].insert(alterEdge, highest);
			}
			edgeInfo[alterEdge].isMST = true;
		}
		else {
			numComponent += 1;
		}
		return;
	}

	bool isSameComponent(Vertex a, Vertex b) {
		return isContaining(Edge(a, b)) ||
			   mst[0].isSameComponent(a, b);
	}
	

};

int N, Q;
unsigned long long int F = 0;
Graph graph;
int main() {
	using namespace std;
	cin >> N >> Q;
	graph.numComponent = N;
#ifdef DEBUG
	ofstream fo;
	fo.open("output.txt");
#endif // DEBUG


	for (int i = 0; i < Q; i++) {
		unsigned long long int a, b, x, y;
		std::cin >> a >> b;
		x = (a ^ F) % N;
		y = (b ^ F) % N;

		if (x < y) {
			Edge edge(x, y);
			assert(edge.a < edge.b);
			if (graph.isContaining(edge)) {
				graph.remove(edge);
			}
			else {
				graph.insert(edge);
			}
		}
		else {
			cout << graph.isSameComponent(x, y) << endl;
#ifdef DEBUG
			fo << graph.isSameComponent(x, y) << endl;
#endif // DEBUG
		}

		F += graph.numComponent;
	}
#ifdef DEBUG
	fo.close();
#endif // DEBUG

	return 0;
}
