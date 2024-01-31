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
using std::make_tuple;

struct SeriesNode {
private:
	Vertex a, b;
	int level;
	int size;
	SeriesNode* l, * p, * r;
	SeriesNode* leftmostFlagged;
	bool flag;
public:
	SeriesNode(Vertex a, Vertex b, int level, bool flag) :
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

	SeriesNode* getFirstFlagged() {
		return leftmostFlagged;
	}
};

struct NoEdge {};

struct MST {
private:
	map<std::tuple<Vertex, Vertex, int>, SeriesNode*> mp;
	map<std::pair<Vertex, Vertex>, int> levels;

	SeriesNode* makeNode(Vertex a, Vertex b, int level, bool flag) {
		SeriesNode* node = new SeriesNode(a, b, level, flag);
		mp.insert(make_pair(std::make_tuple(a, b, level), node));
		return node;
	}

public:
	~MST() {
		for (auto iter : mp) {
			delete iter.second;
		}
	}

	SeriesNode* find(Vertex a, Vertex b, int level) {
		auto iter = mp.find(make_tuple(a, b, level));
		if (iter == mp.end()) {
			return nullptr;
		}
		else return iter->second;
	}


	int getLevel(Vertex a, Vertex b) {
		if (a > b) std::swap(a, b);
		return levels[make_pair(a, b)];
	}

	bool isSameComponent(Vertex a, Vertex b, int level) {
		auto aa = find(a, a, level);
		auto bb = find(b, b, level);
		if (aa == nullptr || bb == nullptr)
			return false;
		else return (aa->getRoot() == bb->getRoot());
	}

	void insert(Vertex a, Vertex b, int level, bool highest) {
		if (a > b) std::swap(a, b);
		assert(find(a, b, level) == nullptr);
		assert(find(b, a, level) == nullptr);
		assert(isSameComponent(a, b, level) == false);

		auto edge = make_pair(a, b);
		if (levels.find(edge) == levels.end()) {
			levels.insert(make_pair(edge, level));
		}
		else if (levels[edge] < level) {
			levels[edge] = level;
		}

		SeriesNode* aa, * bb;
		aa = find(a, a, level);
		bb = find(b, b, level);
		if (aa == nullptr)
			aa = makeNode(a, a, level, false);
		if (bb == nullptr)
			bb = makeNode(b, b, level, false);

		aa->setHead(); bb->setHead();
		aa = aa->getRoot(); bb = bb->getRoot();

		SeriesNode* ab = makeNode(a, b, level, highest);
		SeriesNode* ba = makeNode(b, a, level, false);

		SeriesNode::joinSeries(aa, ab);
		SeriesNode::joinSeries(bb, ba);
		SeriesNode::joinSeries(aa, bb);
	}

	void remove(Vertex a, Vertex b) {
		if (a > b) std::swap(a, b);
		assert(find(a, b, 0) != nullptr && find(b, a, 0) != nullptr);

		for (int i = 0; i <= levels[make_pair(a, b)]; i++) {
			auto ab = find(a, b, i);
			auto ba = find(b, a, i);
			SeriesNode* lp, * rp;

			ab->setHead();
			ab->split(lp, rp);

			ba->split(lp, rp);

			mp.erase(make_tuple(a, b, i));
			mp.erase(make_tuple(b, a, i));
			delete[] ab, ba;
		}

		levels.erase(make_pair(a, b));
	}
};

struct ExtraEdgeHolder {
private:
	set<Vertex> conn[20][100'000];
	int degrees[20][100'000];
	map<std::pair<Vertex, Vertex>, int> levels;
public:
	MST* mst = nullptr;
	bool isContaining(Vertex a, Vertex b) {
		if (a > b) std::swap(a, b);
		return levels.find(make_pair(a, b)) != levels.end();
	}

	void levelup(Vertex a, Vertex b) {
		if (a > b) std::swap(a, b);

		int lv = levels[make_pair(a, b)];

		remove(a, b);
		insert(a, b, lv + 1);
	}

	void insert(Vertex a, Vertex b, int lv) {
		if (a > b) std::swap(a, b);
		levels.insert(make_pair(make_pair(a, b), lv));
		conn[lv][a].insert(b);
		conn[lv][b].insert(a);
		degrees[lv][a] += 1;
		degrees[lv][b] += 1;
		if (degrees[lv][a] == 1)
			mst->find(a, a, lv)->flagUp(true);
		if (degrees[lv][b] == 1)
			mst->find(b, b, lv)->flagUp(true);
	}

	void remove(Vertex a, Vertex b) {
		if (a > b) std::swap(a, b);
		int lv = levels[make_pair(a, b)];
		levels.erase(make_pair(a, b));
		for (int i = 0; i <= lv; i++) {
			conn[i][a].erase(b);
			conn[i][b].erase(a);
			degrees[i][a] -= 1;
			degrees[i][b] -= 1;
			if (degrees[i][a] == 0)
				mst->find(a, a, i)->flagUp(false);
			if (degrees[i][b] == 0)
				mst->find(b, b, i)->flagUp(false);
		}
	}

	Vertex getConnectedVertex(Vertex a, int lv) {
		if (degrees[lv][a] != 0)
			return *conn[lv][a].begin();
		else
			throw NoEdge();
	}
};

enum class ResultSign
{
	NONE, MST, EXTRA
};
struct Graph {
private:
	MST mst;
	ExtraEdgeHolder extraEdges;
	unsigned long long numComponent;
public:
	Graph() :numComponent(0) {
		extraEdges.mst = &mst;
	}

	ResultSign isContaining(Vertex a, Vertex b) {
		if (a > b) {
			std::swap(a, b);
		}

		if (mst.find(a, b, 0) != nullptr)
			return ResultSign::MST;
		else if (extraEdges.isContaining(a, b))
			return ResultSign::EXTRA;
		else return ResultSign::NONE;
	}

	bool isSameComponent(Vertex a, Vertex b) {
		if (a > b) std::swap(a, b);
		return (isContaining(a, b) != ResultSign::NONE) || mst.isSameComponent(a, b, 0);
	}

	void insert(Vertex a, Vertex b) {
		if (a > b) std::swap(a, b);
		assert(isContaining(a, b) == ResultSign::NONE);

		bool insertIntoMst = !(mst.isSameComponent(a, b, 0));
		if (insertIntoMst) { 
			mst.insert(a, b, 0, true);
			numComponent--;
		}
		else {
			extraEdges.insert(a, b, 0);
		}
	}

	void remove(Vertex a, Vertex b) {
		if (a > b) std::swap(a, b);
		assert(isContaining(a, b) != ResultSign::NONE);
		bool isInMST = (isContaining(a, b) == ResultSign::MST);
		
		// remove extra edge
		if (isInMST == false) {
			extraEdges.remove(a, b);
			return;
		}

		// remove mst edge
		int lv = mst.getLevel(a, b);
		mst.remove(a, b);
		for (int i = lv; i >= 0; i--) {
			if (findAndInsertAlterEdge(a, b, i)) {
				return;;
			}
		}

		numComponent++;
	}

	unsigned long long int getNumComponent() { return numComponent; }

	bool findAndInsertAlterEdge(Vertex a, Vertex b, int lv) {
		if (a > b) std::swap(a, b);

		auto aa = mst.find(a, a, lv);
		auto bb = mst.find(b, b, lv);

		aa->getRoot();
		bb->getRoot();

		auto start = aa->getSize() < bb->getSize() ? aa : bb;
		auto iter = start->getRoot()->getFirstFlagged();
		while (iter != nullptr)
		{
			if (iter->getA() < iter->getB()) {
				iter->flagUp(false);
				mst.insert(iter->getA(), iter->getB(), lv + 1, true);
			}
			iter = iter->nextFlagged();
		}

		iter = start->getRoot()->getFirstFlagged();
		while (iter != nullptr)
		{
			assert(iter->getA() == iter->getB());
			Vertex u = iter->getA();
			while (true)
			{
				Vertex v;
				try {
					v = extraEdges.getConnectedVertex(u, lv);
				}
				catch (NoEdge&) {
					iter = iter->nextFlagged();
					break;
				}

				if (mst.isSameComponent(u, v, lv + 1)) {
					extraEdges.levelup(u, v);
				}
				else {
					extraEdges.remove(u, v);
					if (u > v) std::swap(u, v);
					mst.insert(u, v, lv, true);
					for (int i = 0; i < lv; i++) {
						mst.insert(u, v, i, false);
					}
					return true;
				}
			}
		}
		return false;
	}
};

int N, Q;
unsigned long long int F = 0;

int main() {
	using namespace std;
	cin >> N >> Q;
	Graph* graph = new Graph;
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
			if (graph->isContaining(x, y) != ResultSign::NONE) {
				graph->remove(x, y);
			}
			else {
				graph->insert(x, y);
			}
		}
		else {
			cout << graph->isSameComponent(y, x) << endl;
#ifdef DEBUG
			fo << graph->isSameComponent(x, y) << endl;
#endif // DEBUG
		}

		F += graph->getNumComponent();
	}
#ifdef DEBUG
	fo.close();
#endif // DEBUG

	//using namespace std;
	//cin >> N;
	//Graph* graph = new Graph(N);

	//while (true) {
	//	unsigned long long int a, b, x, y;
	//	std::cin >> a >> b;
	//	x = a;
	//	y = b;

	//	if (x < y) {
	//		if (graph->isContaining(x, y)) {
	//			graph->remove(x, y);
	//		}
	//		else {
	//			graph->insert(x, y);
	//		}
	//	}
	//	else {
	//		cout << graph->isSameComponent(y, x) << endl;
	//	}

	//	F += graph->getNumComponent();
	//}

	//delete graph;
	//return 0;
}
