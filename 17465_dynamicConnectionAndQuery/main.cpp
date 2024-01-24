#include<iostream>
#include<map>
#include<set>
#include<cassert>

using std::map;
using std::set;
using std::make_pair;

class Edge {
public:
	int a, b;
	Edge(int a, int b) {
		if (this->a < this->b) {
			this->a = a;
			this->b = b;
		}
		else {
			this->a = b;
			this->b = a;
		}
	}
	bool operator<(const Edge& other) const {
		return a < other.a || (a == other.a && b < other.b);
	}

};

class Ark {
public:
	int a, b;
	//int level;
	Ark(int a, int b) :a(a), b(b) {}
	bool operator<(const Ark& other) const {
		return a < other.a || (a == other.a && b < other.b);
	}
};

class SeriesNode {
public:
	Ark value;
	int size;
	SeriesNode* l, * p, * r;
	SeriesNode* leftmostVertex;

	SeriesNode(const Ark& ark) : 
		value(ark), 
		size(1), 
		l(nullptr), 
		p(nullptr), 
		r(nullptr), 
		leftmostVertex(nullptr) 
	{}

	void update() {
		this->size = 1;
		if (l != nullptr) size += l->size;
		if (r != nullptr) size += r->size;

		if (l != nullptr && l->leftmostVertex != nullptr)
			leftmostVertex = l->leftmostVertex;
		else if (this->value.a == this->value.b)
			leftmostVertex = this;
		else if (r != nullptr && r->leftmostVertex != nullptr)
			leftmostVertex = r->leftmostVertex;
		else
			leftmostVertex = nullptr;
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
		return result;
	}

	static void joinSeries(SeriesNode* a, SeriesNode* b) {
		assert(a != nullptr);
		assert(b != nullptr);
		a = a->getRoot();
		while (a->r != nullptr)
		{
			a = a->r;
		}
		a->splay();
		b->splay();

		a->r = b;
		b->p = a;
		a->update();
	}

	void setHead() {
		splay();
		if (l == nullptr) return;

		SeriesNode* leftSeries = detachLeftSeries();
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

	SeriesNode* next() {
		splay();
		if (r == nullptr) return r;
		SeriesNode* result = r;
		while (result->l != nullptr)
		{
			result = result->l;
		}
		return result;
	}
};

class NoEdge :std::exception {};
class MST {
	map<Ark, SeriesNode*> mp;

	SeriesNode* makeNode(const Ark& ark) {
		SeriesNode* node = new SeriesNode(ark);
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

	bool isSameComponent(int a, int b) {
		auto aa = find(Ark(a, a));
		auto bb = find(Ark(b, b));
		if (aa == nullptr || bb == nullptr) return false;
		return aa->getRoot() == bb->getRoot();
	}

	void insert(const Edge& edge) {
		int a = edge.a;
		int b = edge.b;

		SeriesNode* aa, * bb;
		aa = find(Ark(a, a));
		bb = find(Ark(b, b));
		if (aa == nullptr) {
			aa = makeNode(Ark(a, a));
		}
		if (bb == nullptr) {
			bb = makeNode(Ark(b, b));
		}

		SeriesNode* ab = makeNode(Ark(a, b));
		SeriesNode* ba = makeNode(Ark(b, a));

		aa->setHead();
		bb->setHead();

		SeriesNode::joinSeries(aa, ab);
		SeriesNode::joinSeries(ab, bb);
		SeriesNode::joinSeries(bb, ba);
	}

	void remove(const Edge& edge, int level = 0) {
		int a = edge.a;
		int b = edge.b;

		SeriesNode* ab = find(Ark(a, b));
		SeriesNode* ba = find(Ark(b, a));
		assert(ab != nullptr);
		assert(ba != nullptr);

		ab->setHead();

		SeriesNode* nextTmp;
		nextTmp = ab->next();
		if (nextTmp != nullptr)
			nextTmp->detachLeftSeries();

		ba->detachLeftSeries();
		nextTmp = ba->next();
		if (nextTmp != nullptr)
			nextTmp->detachLeftSeries();

		mp.erase(Ark(a, b));
		mp.erase(Ark(b, a));

		delete[] ab, ba;
	}
};

class ExtraEdgeHolder {
	set<int> conn[100'000];

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

	Edge findAlterEdge(int u) {
		if (conn[u].empty()) {
			throw NoEdge();
		}
		int v = *(conn[u].begin());
		return Edge(u, v);
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
	
	void _inorder(SeriesNode* node, int level) {
		if (node == nullptr) return;
		_inorder(node->l, level);
		if (node->value.a != node->value.b) { // node is not vertex
			Edge edge(node->value.a, node->value.b);
			if (mst[level + 1].isContaining(edge) == false) {
				mst[level + 1].insert(edge);
				edgeInfo[edge].level += 1;
			}
		}
		_inorder(node->r, level);
	}
public:
	int numComponent = 0;
	bool isContaining(const Edge& edge) {
		return edgeInfo.find(edge) != edgeInfo.end();
	}

	void insert(const Edge& edge) {
		assert(edge.a != edge.b);
		bool insertIntoMST = (mst[0].isSameComponent(edge.a, edge.b) == false);
		if (mst[0].isSameComponent(edge.a, edge.b) == false) {
			mst[0].insert(edge);
			numComponent -= 1;
		}
		else {
			extraEdges[0].insert(edge);
		}
		edgeInfo.insert(make_pair(edge, EdgeInformation(insertIntoMST, 0)));
	}

	void remove(const Edge& edge) {
		assert(edge.a != edge.b);
		bool isMST = edgeInfo[edge].isMST;
		int level = edgeInfo[edge].level;
		edgeInfo.erase(edge);
		if (isMST == false) {
			extraEdges[level].remove(edge);
			return;
		}

		int a = edge.a;
		int b = edge.b;
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
			_inorder(snode, i);
			assert(snode != nullptr);
			snode->splay();
			assert(snode != nullptr);
			snode = snode->leftmostVertex;

			while (snode != nullptr)
			{
				int u = snode->value.a;
				assert(snode->value.a == snode->value.b);
				try {
					Edge edge = extraEdges[i].findAlterEdge(u);
					if (mst[i + 1].isSameComponent(edge.a, edge.b) == false) { // edge is alternative edge
						found = true;
						alterEdge = edge;
						extraEdges[i].remove(edge);
						break;
					}
					else { // level up extra edge
						extraEdges[i].remove(edge);
						extraEdges[i + 1].insert(edge);
						edgeInfo[edge].level += 1;
						continue;
					}
				}
				catch (const NoEdge&) {
					snode->splay();
					if (snode->r != nullptr)
						snode = snode->r->leftmostVertex;
					else
						break;
				}
			}
			if (found) break;
		}

		if (found) {
			for (int i = 0; i <= edgeInfo[alterEdge].level; i++) {
				mst[i].insert(alterEdge);
			}
			edgeInfo[alterEdge].isMST = true;
		}
		else {
			numComponent += 1;
		}
		return;
	}

	bool isSameComponent(int a, int b) {
		return mst[0].isSameComponent(a, b);
	}
	

};

int N, Q;
int F = 0;
Graph graph;
int main() {
	using namespace std;
	cin >> N >> Q;
	graph.numComponent = N;

	for (int i = 0; i < Q; i++) {
		unsigned int a, b, x, y;
		std::cin >> a >> b;
		x = (a ^ F) % N;
		y = (b ^ F) % N;

		if (x < y) {
			Edge edge(x, y);
			if (graph.isContaining(edge)) {
				graph.remove(edge);
			}
			else {
				graph.insert(edge);
			}
		}
		else {
			cout << graph.isSameComponent(x, y) << endl;
		}

		F += graph.numComponent;
	}
	return 0;
}
