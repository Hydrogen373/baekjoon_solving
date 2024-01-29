#define DEBUG
#include <iostream>
#include <set>
#include <map>


#ifdef DEBUG
#include <cassert>
#include <fstream>
#endif // DEBUG
#ifndef DEBUG
#define assert(x)
#endif // !DEBUG



using std::set;
using std::map;
using Vertex = unsigned int;
using std::make_pair;

struct SeriesNode {
private:
	Vertex a, b;
	int size;
	SeriesNode* l, * p, * r;
	SeriesNode* leftmostFlagged;
	bool flag;
public:
	SeriesNode(Vertex a, Vertex b, bool flag) :
		a(a),
		b(b),
		size(1),
		l(nullptr),
		p(nullptr),
		r(nullptr),
		leftmostFlagged(nullptr),
		flag(flag) 
	{
		if (this->flag) leftmostFlagged = this;
	}

	void update() {
		this->size = 1;
		if (l != nullptr) size += l->size;
		if (r != nullptr) size += r->size;

		if (l != nullptr && l->leftmostFlagged != nullptr)
			leftmostFlagged = l->leftmostFlagged;
		else if (this->flag)
			leftmostFlagged = this;
		else if (r != nullptr && r->leftmostFlagged != nullptr)
			leftmostFlagged = r->leftmostFlagged;
		else
			leftmostFlagged = nullptr;
	}

	void flagUp(bool flag) {
		splay();
		this->flag = flag;
		update();
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
		while (this->p!=nullptr)
		{
			if (p->p != nullptr) {
				SeriesNode* uncle;
				SeriesNode* child;
				if (this == p->l)
					child = r;
				else
					child = l;
				if (p->p->l == p)
					uncle = p->p->r;
				else
					uncle = p->p->l;
				unsigned int c, u;
				if (child != nullptr)
					c = child->size;
				else
					c = 0;
				if (uncle != nullptr)
					u = uncle->size;
				else
					u = 0;
				if (c < u)
					this->rotate();
				else
					p->rotate();
			}
			this->rotate();
		}
		return;
	}

	void split(SeriesNode*& lp, SeriesNode*& rp) {
		splay();
		lp = l;
		rp = r;
		if (l != nullptr) {
			l->p = nullptr;
			l = nullptr;
		}
		if (r != nullptr) {
			r->p = nullptr;
			r = nullptr;
		}
		update();
	}

	SeriesNode* getRoot() {
		SeriesNode* x = this;
		while (x->p != nullptr) {
			x = x->p;
		}
		return x;
	}

	static void joinSeries(SeriesNode* a, SeriesNode* b) {
		if (a == nullptr || b == nullptr) return;
		a = a->getRoot();
		b = b->getRoot();
		if (a->size > b->size) {
			// a is parent
			while (a->r!=nullptr)
			{
				a = a->r;
			}
			a->splay();
			a->r = b;
			b->p = a;
			a->update();
		}
		else {
			// b is parent
			while (b->l != nullptr)
			{
				b = b->l;
			}
			b->splay();
			b->l = a;
			a->p = b;
			b->update();
		}
		return;
	}

	void setHead() {
		splay();
		SeriesNode* lp, *rp;
		this->split(lp, rp);
		joinSeries(this, rp);
		if (lp != nullptr)
			joinSeries(this, lp);
		return;
	}

	int getSize() {
		return size;
	}

	Vertex getA() {
		return a;
	}
	Vertex getB() {
		return b;
	}

	SeriesNode* nextFlagged() {
		splay();
		if (r != nullptr)
			return r->leftmostFlagged;
		else
			return nullptr;
	}

	SeriesNode* getLeftmostFlagged() {
		return leftmostFlagged;
	}
};

struct NoEdge {};

struct MST {
private:
	map<std::pair<Vertex, Vertex>, SeriesNode*> mp;

	SeriesNode* makeNode(Vertex a, Vertex b, bool flag) {
		SeriesNode* node = new SeriesNode(a, b, flag);
		mp.insert(make_pair(make_pair(a, b), node));
		return node;
	}

public:
	~MST() {
		for (auto iter : mp) {
			delete iter.second;
		}
	}

	SeriesNode* find(Vertex a, Vertex b) {
		auto iter = mp.find(make_pair(a, b));
		if (iter == mp.end()) {
			return nullptr;
		}
		else return iter->second;
	}

	bool isContaining(Vertex a, Vertex b) {
		return mp.find(make_pair(a, b)) != mp.end();
	}

	bool isSameComponent(Vertex a, Vertex b) {
		auto aa = find(a, a);
		auto bb = find(b, b);
		if (aa == nullptr || bb == nullptr)
			return false;
		else return (aa->getRoot() == bb->getRoot());
	}

	void insert(Vertex a, Vertex b, bool highest) {
		if (a > b) 
			std::swap(a, b);
		assert(find(a, b) == nullptr);
		assert(find(b, a) == nullptr);
		assert(isSameComponent(a, b) == false);

		SeriesNode* aa, * bb;
		aa = find(a, a);
		bb = find(b, b);
		if (aa == nullptr)
			aa = makeNode(a, a, false);
		if (bb == nullptr)
			bb = makeNode(b, b, false);

		aa->setHead(); bb->setHead();
		aa = aa->getRoot(); bb = bb->getRoot();

		SeriesNode* ab = makeNode(a, b, highest);
		SeriesNode* ba = makeNode(b, a, false);

		SeriesNode::joinSeries(aa, ab);
		SeriesNode::joinSeries(bb, ba);
		SeriesNode::joinSeries(aa, bb);
	}

	void remove(Vertex a, Vertex b) {
		if (a > b) 
			std::swap(a, b);
		assert(find(a, b) != nullptr && find(b, a) != nullptr);

		auto ab = find(a, b);
		auto ba = find(b, a);
		SeriesNode* lp, * rp;

		ab->setHead();
		ab->split(lp, rp);

		ba->split(lp, rp);

		mp.erase(make_pair(a, b));
		mp.erase(make_pair(b, a));
	
		delete[] ab, ba;
	}
};

struct ExtraEdgeHolder {
private:
	set<Vertex> conn[100'000];
	int degrees[100'000]{ 0, };
public:
	MST* mst;

	bool isContaining(Vertex a, Vertex b) {
		return conn[a].find(b) != conn[a].end();
	}

	void insert(Vertex a, Vertex b) {
		conn[a].insert(b);
		conn[b].insert(a);
		degrees[a] += 1;
		degrees[b] += 1;
		if (degrees[a] == 1)
			mst->find(a, a)->flagUp(true);
		if (degrees[b] == 1)
			mst->find(b, b)->flagUp(true);
	}

	void remove(Vertex a, Vertex b) {
		conn[a].erase(b);
		conn[b].erase(a);
		degrees[a] -= 1;
		degrees[b] -= 1;
		if (degrees[a] == 0)
			mst->find(a, a)->flagUp(false);
		if (degrees[b] == 0)
			mst->find(b, b)->flagUp(false);
	}

	Vertex getConnectedVertex(Vertex a) {
		if (degrees[a] != 0)
			return *conn[a].begin();
		else
			throw NoEdge();
	}
};

struct Graph {
private:
	MST mst[20];
	ExtraEdgeHolder extraEdges[20];
	struct EdgeInformation {
		bool mst;
		int level;
		EdgeInformation(bool mst, int level) :mst(mst), level(level) {}
		EdgeInformation() :EdgeInformation(false, 0) {}
	};
	map<std::pair<Vertex, Vertex>, EdgeInformation> edgeInformations;
public:
	void flagUp(int level, Vertex v, bool flag) {
		mst[level].find(v, v)->flagUp(flag);
	}
	Graph() {
		for (int i = 0; i < 20; i++) {
			extraEdges[i].mst = &(mst[i]);
		}
	}


	unsigned long long numComponent;

	bool isContaining(Vertex a, Vertex b) {
		if (a > b) {
			std::swap(a, b);
		}

		return edgeInformations.find(make_pair(a, b)) 
			!= edgeInformations.end();
	}

	bool isSameComponent(Vertex a, Vertex b) {
		if (a > b) std::swap(a, b);
		return isContaining(a, b) 
			|| mst[0].isSameComponent(a, b);
	}

	void insert(Vertex a, Vertex b) {
		if (a > b) std::swap(a, b);
		assert(isContaining(a, b) == false);

		bool insertIntoMst = !mst[0].isSameComponent(a, b);
		if (insertIntoMst) { 
			mst[0].insert(a, b, true);
			numComponent--;
		}
		else {
			extraEdges[0].insert(a, b);
		}
		edgeInformations.insert(
			make_pair(
				make_pair(a, b),
				EdgeInformation(insertIntoMst, 0)
			)
		);
	}

	void remove(Vertex a, Vertex b) {
		if (a > b) std::swap(a, b);
		assert(isContaining(a, b) == true);
		
		EdgeInformation info = edgeInformations[make_pair(a, b)];
		edgeInformations.erase(make_pair(a, b));

		// remove extra edge
		if (info.mst == false) {
			extraEdges[info.level].remove(a, b);

			return;
		}

		// remove mst edge
		for (int i = info.level; i >= 0; i--) {
			mst[i].remove(a, b);
		}

		bool found = false;
		std::pair<Vertex, Vertex> alterEdge;
		for (int i = info.level; i >= 0; i--) {
			auto aa = mst[i].find(a, a);
			auto bb = mst[i].find(b, b);

			aa->splay(); bb->splay();
			auto start = (aa->getSize() < bb->getSize() ? aa : bb);

			auto iter = start->getRoot()->getLeftmostFlagged();
			while (iter != nullptr)
			{
				// mst levelup
				if (iter->getA() < iter->getB()) {
					iter->flagUp(false);
					Vertex a = iter->getA();
					Vertex b = iter->getB();
					mst[i + 1].insert(a, b, true);
					edgeInformations[make_pair(a, b)].level += 1;
				}
				iter = iter->nextFlagged();
			}

			iter = start->getRoot()->getLeftmostFlagged();
			while (iter != nullptr) {
				if (!(iter->getA() == iter->getB()))
					iter = iter->nextFlagged();

				Vertex u = iter->getA();
				Vertex v;
				try {
					v = extraEdges[i].getConnectedVertex(u);
				}
				catch (NoEdge&) {
					iter = iter->nextFlagged();
					continue;
				}
				if (u > v) std::swap(u, v);
				extraEdges[i].remove(u, v);

				// (u, v) is not alternative edge
				if (mst[i + 1].isSameComponent(u, v)) {
					extraEdges[i + 1].insert(u, v);
					edgeInformations[make_pair(u, v)].level += 1;
					continue;
				}

				// found
				alterEdge = make_pair(u, v);
				found = true;
				break;
			}
			if (found)
				break;

		}
		if (found) {
			auto alterLevel = edgeInformations[alterEdge].level;
			Vertex& u = alterEdge.first;
			Vertex& v = alterEdge.second;

			for (int i = alterLevel; i >= 0; i--) {
				assert(mst[i].isSameComponent(u, v) == false);
				mst[i].insert(u, v, i == alterLevel);
			}
			edgeInformations[alterEdge].mst = true;
		}
		else {
			numComponent++;
		}
	}

};

int N, Q;
unsigned long long int F = 0;
Graph graph;

int main() {
//	using namespace std;
//	cin >> N >> Q;
//	graph.numComponent = N;
//#ifdef DEBUG
//	ofstream fo;
//	fo.open("output.txt");
//#endif // DEBUG
//
//
//	for (int i = 0; i < Q; i++) {
//		unsigned long long int a, b, x, y;
//		std::cin >> a >> b;
//		x = (a ^ F) % N;
//		y = (b ^ F) % N;
//
//		if (x < y) {
//			if (graph.isContaining(x, y)) {
//				graph.remove(x, y);
//			}
//			else {
//				graph.insert(x, y);
//			}
//		}
//		else {
//			cout << graph.isSameComponent(y, x) << endl;
//#ifdef DEBUG
//			fo << graph.isSameComponent(x, y) << endl;
//#endif // DEBUG
//		}
//
//		F += graph.numComponent;
//	}
//#ifdef DEBUG
//	fo.close();
//#endif // DEBUG

	using namespace std;
	cin >> N;
	graph.numComponent = N;


	while (true) {
		unsigned long long int a, b, x, y;
		std::cin >> a >> b;
		x = a;
		y = b;

		if (x < y) {
			if (graph.isContaining(x, y)) {
				graph.remove(x, y);
			}
			else {
				graph.insert(x, y);
			}
		}
		else {
			cout << graph.isSameComponent(y, x) << endl;
		}

		F += graph.numComponent;
	}

	return 0;
}
