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
	int level;
	Ark(int a, int b, int level) :a(a), b(b), level(level) {}
	bool operator<(const Ark& other) const {
		if (a != other.a) return a < other.a;
		else if (b != other.a) return b < other.b;
		else return level < other.level;
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
		return result;
	}

	static void joinSeries(SeriesNode* a, SeriesNode* b) {
		a = a->getRoot();
		while (a->r != nullptr)
		{
			a = a->r;
		}
		a->splay();
		b->splay();

		a->r = b;
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
};

class MST {
	map<Ark, SeriesNode*> mp;
	map<Edge, int> levels;

	SeriesNode* makeNode(const Ark& ark) {
		SeriesNode* node = new SeriesNode(ark);
		mp.insert(make_pair(ark, node));
		return node;
	}

public:
	SeriesNode* find(const Ark& ark) {
		auto iter = mp.find(ark);
		if (iter == mp.end()) {
			return nullptr;
		}
		else {
			return iter->second;
		}
	}

	bool isSameComponent(int a, int b, int level) {
		auto aa = find(Ark(a, a, level));
		auto bb = find(Ark(b, b, level));
		if (aa == nullptr || bb == nullptr) return false;
		return aa->getRoot() == bb->getRoot();
	}

	void insert(const Edge& edge) {
		int a = edge.a;
		int b = edge.b;
		levels.insert(make_pair(edge, 0));

		SeriesNode* aa, * bb;
		aa = find(Ark(a, a, 0));
		bb = find(Ark(b, b, 0));
		if (aa == nullptr) {
			aa = makeNode(Ark(a, a, 0));
		}
		if (bb == nullptr) {
			bb = makeNode(Ark(b, b, 0));
		}

		SeriesNode* ab = makeNode(Ark(a, b, 0));
		SeriesNode* ba = makeNode(Ark(b, a, 0));

		aa->setHead();
		bb->setHead();

		SeriesNode::joinSeries(aa, ab);
		SeriesNode::joinSeries(ab, bb);
		SeriesNode::joinSeries(bb, ba);
	}

	int remove(const Edge& edge) {
		int a = edge.a;
		int b = edge.b;

		int level = levels[edge];

		for (int i = 0; i <= level; i++) {
			SeriesNode* ab = find(Ark(a, b, i));
			SeriesNode* ba = find(Ark(b, a, i));

			ab->setHead();

			ab->splay();
			ab->r->detachLeftSeries();

			ba->splay();
			ba->r->detachLeftSeries();
			ba->detachLeftSeries();

			mp.erase(Ark(a, b, i));
			mp.erase(Ark(b, a, i));

			delete[] ab, ba;
		}

		levels.erase(edge);
		return level;
	}
};

class ExtraEdgeHolder {
	map<Edge, int> levels;
	set<int> conn[20][100'000];

public:
	bool isContaining(const Edge& edge) {
		return levels.find(edge) != levels.end();
	}
	void insert(const Edge& edge) {
		levels.insert(make_pair(edge, 0));
		conn[0][edge.a].insert(edge.b);
		conn[0][edge.b].insert(edge.a);
	}

	void remove(const Edge& edge) {
		int level = levels[edge];
		levels.erase(edge);
		conn[level][edge.a].erase(edge.b);
		conn[level][edge.b].erase(edge.a);
	}

	void levelUp(const Edge& edge) {
		int level = levels[edge];
		levels[edge] += 1;
		conn[level][edge.a].erase(edge.b);
		conn[level][edge.b].erase(edge.a);
		conn[level + 1][edge.a].insert(edge.b);
		conn[level + 1][edge.b].insert(edge.a);
	}

	int findAlterEdge(int node, int level) {
		if (conn[level][node].empty()) return -1;
		return *(conn[level][node].begin());
	}
};

class Graph {
	MST mst;
	ExtraEdgeHolder extraEdges;
public:
	void qry(int a, int b) {
		if (a < b) {
			if (mst.find(Ark(a, b, 0)) != nullptr) {
				// remove from mst
				int level = mst.remove(Edge(a, b));

				for (int l = level; l >= 0; l--) {
					auto node = mst.find(Ark(a, a, l));

					
					// levelup mst
					// TODO

					// find alter edge(u,v)
					// TODO
					

				}


			}
			else if (extraEdges.isContaining(Edge(a, b))) {
				extraEdges.remove(Edge(a,b));
			}
			
			// insertion
			if (mst.isSameComponent(a, b, 0)) {
				extraEdges.insert(Edge(a, b));
			}
			else {
				mst.insert(Edge(a, b));
			}
		}
		else {
			std::cout << mst.isSameComponent(a, b, 0) << std::endl;
		}

	}
	
};
