#include <iostream>
#include <set>
#include <map>
#include <cassert>

using std::set;
using std::map;
using Vertex = unsigned int;
using std::make_pair;

struct Edge {
public:
	Vertex a, b;
	Edge(Vertex a, Vertex b) :a(a), b(b) {
		if (this->a > this->b) std::swap(this->a, this->b);
	}
	bool operator<(const Edge& other) const {
		return a < other.a || (a == other.a && b < other.b);
	}
};

struct SeriesNode {
public:
	Vertex a, b;
	int size;
	SeriesNode* l, * p, * r;
	SeriesNode* leftmostHighest;
	bool highest;

	SeriesNode(Vertex a, Vertex b, bool highest) :
		a(a),
		b(b),
		size(1),
		l(nullptr),
		p(nullptr),
		r(nullptr),
		leftmostHighest(nullptr),
		highest(highest) {
		if (this->highest) leftmostHighest = this;
	}

	void update() {
		this->size = 1;
		if (l != nullptr) size += l->size;
		if (r != nullptr) size += r->size;

		if (l != nullptr && l->leftmostHighest != nullptr)
			leftmostHighest = l->leftmostHighest;
		else if (this->highest)
			leftmostHighest = this;
		else if (r != nullptr && r->leftmostHighest != nullptr)
			leftmostHighest = r->leftmostHighest;
		else
			leftmostHighest = nullptr;
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
		while (this->p==nullptr)
		{
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

	SeriesNode* detachRightSeries() {
		splay();
		if (r == nullptr) return nullptr;
		SeriesNode* result = r;
		r->p = nullptr;
		r = nullptr;
		update();
		return result;
	}

	SeriesNode* getRoot() {
		SeriesNode* x = this;
		while (x->p != nullptr) {
			x = x->p;
		}
		return x;
	}

	static void joinSeries(SeriesNode* a, SeriesNode* b) {
		a = a->getRoot();
		while (a->r!=nullptr)
		{
			a = a->r;
		}
		a->splay();
		b = b->getRoot();

		assert(a->r == nullptr && b->p == nullptr);
		a->r = b;
		b->p = a;
		a->update();
		return;
	}

	void setHead() {
		splay();
		if (l == nullptr) return;
		SeriesNode* leftSeriesRoot = detachLeftSeries();

		joinSeries(this, leftSeriesRoot);
		return;
	}
};

struct NoEdge {};

struct MST {
	map<std::pair<Vertex, Vertex>, SeriesNode*> mp;

	SeriesNode* makeNode(Vertex a, Vertex b, bool highest) {
		SeriesNode* node = new SeriesNode(a, b, highest);
		mp.insert(make_pair(make_pair(a, b), highest));

	}




};

