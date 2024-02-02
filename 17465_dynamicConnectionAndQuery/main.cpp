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
using std::swap;

struct SeriesNode {
private:
	Vertex a, b;
	int level;
	int size;
	SeriesNode* l, * p, * r;
	SeriesNode* leftmostFlagged;
	bool flag;
public:
	SeriesNode(Vertex a, Vertex b, int level) :
		a(a),
		b(b),
		level(level),
		size(1),
		l(nullptr),
		p(nullptr),
		r(nullptr),
		leftmostFlagged(nullptr),
		flag(false) 
	{}

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
				//SeriesNode* uncle;
				//SeriesNode* child;
				//if (this == p->l)
				//	child = r;
				//else
				//	child = l;
				//if (p->p->l == p)
				//	uncle = p->p->r;
				//else
				//	uncle = p->p->l;
				//unsigned int c, u;
				//if (child != nullptr)
				//	c = child->size;
				//else
				//	c = 0;
				//if (uncle != nullptr)
				//	u = uncle->size;
				//else
				//	u = 0;
				//if (c < u)
				//	this->rotate();
				//else
				//	p->rotate();
				if ((p->p->l == p) == (p->l == this))
					p->rotate();
				else
					rotate();
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
		splay();
		return leftmostFlagged;
	}
};

struct Graph {
private:
	map<std::tuple<Vertex, Vertex, int>, SeriesNode*> mp;
	struct Information
	{
		bool mst;
		int level;
		Information() :Information(false, 0) {}
		Information(bool mst, int level) :mst(mst), level(level) {}
	};
	map<std::pair<Vertex, Vertex>, Information> info;

	set<Vertex> conn[20][100'000];
	int degrees[20][100'000]{ 0, };
	int numComponent;

	SeriesNode* makeNode(Vertex a, Vertex b, int level) {
		SeriesNode* node = new SeriesNode(a, b, level);
		mp.insert(make_pair(std::make_tuple(a, b, level), node));
		return node;
	}

	void _insertMST(Vertex a, Vertex b, int level) {
		if (a > b)
			swap(a, b);
		assert(isSameComponent(a, b, level) == false);

		auto edge = make_pair(a, b);
		if (info.find(edge) == info.end()) {
			info.insert(make_pair(edge, Information(true, level)));
		}
		else if (info[edge].level < level)
			info[edge].level = level;

		SeriesNode* aa, * bb;
		aa = find(a, a, level);
		bb = find(b, b, level);
		if (aa == nullptr)
			aa = makeNode(a, a, level);
		if (bb == nullptr)
			bb = makeNode(b, b, level);

		aa->setHead(); bb->setHead();
		aa = aa->getRoot(); bb = bb->getRoot();

		SeriesNode* ab = makeNode(a, b, level);
		SeriesNode* ba = makeNode(b, a, level);

		SeriesNode::joinSeries(aa, ab);
		SeriesNode::joinSeries(bb, ba);
		SeriesNode::joinSeries(aa, bb);
	}

	void _removeMST(Vertex a, Vertex b) {
		if (a > b) 
			swap(a, b);

		for (int i = 0; i <= info[make_pair(a, b)].level; i++) {
			auto ab = find(a, b, i);
			auto ba = find(b, a, i);
			SeriesNode* lp, * rp;

			ab->setHead();
			ab->split(lp, rp);
			ba->split(lp, rp);

			mp.erase(make_tuple(a, b, i));
			mp.erase(make_tuple(b, a, i));

			delete[] ab;
			delete[] ba;
		}

		info.erase(make_pair(a, b));
	}

	void _insertExtra(Vertex a, Vertex b, int level) {
		if (a > b)
			swap(a, b);
		info.insert(make_pair(make_pair(a, b), Information(false, level)));

		conn[level][a].insert(b);
		conn[level][b].insert(a);
		degrees[level][a] += 1;
		degrees[level][b] += 1;
		if (degrees[level][a] == 1)
			find(a, a, level)->flagUp(true);
		if (degrees[level][b] == 1)
			find(b, b, level)->flagUp(true);
	}

	void _removeExtra(Vertex a, Vertex b) {
		if (a > b)
			swap(a, b);

		int level = info[make_pair(a, b)].level;
		info.erase(make_pair(a, b));

		conn[level][a].erase(b);
		conn[level][b].erase(a);
		degrees[level][a] -= 1;
		degrees[level][b] -= 1;
		if (degrees[level][a] == 0)
			find(a, a, level)->flagUp(false);
		if (degrees[level][b] == 0)
			find(b, b, level)->flagUp(false);
	}

	std::pair<Vertex, Vertex> _findAlterEdge(Vertex a, Vertex b, int level) {
		if (a > b)
			swap(a, b);

		auto aa = find(a, a, level);
		auto bb = find(b, b, level);

		aa = aa->getRoot();
		bb = bb->getRoot();
		auto smallerMST = aa->getSize() < bb->getSize() ? aa : bb;
		auto iter = smallerMST->getFirstFlagged();

		while (iter != nullptr) {
			if (iter->getA() < iter->getB()) {
				iter->flagUp(false);
				_insertMST(iter->getA(), iter->getB(), level + 1);
				find(iter->getA(), iter->getB(), level + 1)->flagUp(true);
			}
			iter = iter->nextFlagged();
		}

		iter = smallerMST->getFirstFlagged();
		while (iter != nullptr)
		{
			assert(iter->getA() == iter->getB());

			Vertex u = iter->getA();
			while (conn[level][u].empty() == false)
			{
				Vertex v = *(conn[level][u].begin());
				_removeExtra(u, v);

				if (isSameComponent(u, v, level + 1)) {
					_insertExtra(u, v, level + 1);
				}
				else {
					if (u > v) swap(u, v);
					return make_pair(u, v);
				}
			}

			iter = iter->nextFlagged();
		}
		return make_pair(-1, -1);
	}
public:
	Graph(int N) :numComponent(N) {}

	int getNumComponent() {
		return numComponent;
	}

	SeriesNode* find(Vertex a, Vertex b, int level) {
		auto iter = mp.find(make_tuple(a, b, level));
		if (iter == mp.end())
			return nullptr;
		else
			return iter->second;
	}

	bool isContaining(Vertex a, Vertex b) {
		if (a > b)
			swap(a, b);
		return info.find(make_pair(a, b)) != info.end();
	}

	bool isSameComponent(Vertex a, Vertex b, int level) {
		if (a > b)
			swap(a, b);

		auto aa = find(a, a, level);
		auto bb = find(b, b, level);

		if (aa == nullptr || bb == nullptr)
			return false;
		return aa->getRoot() == bb->getRoot();
	}

	void remove(Vertex a, Vertex b) {
		if (a > b) swap(a, b);
		if (info[make_pair(a, b)].mst == false) {
			_removeExtra(a, b);
			return;
		}
		int lv = info[make_pair(a, b)].level;
		_removeMST(a, b);
		std::pair<Vertex, Vertex> alter;
		bool found = false;
		for (; lv >= 0; lv--) {
			alter = _findAlterEdge(a, b, lv);

			if (alter.first != alter.second) {
				found = true;
				break;
			}
		}
		if (found) {
			for (int i = 0; i <= lv; i++) {
				_insertMST(alter.first, alter.second, i);
			}
			find(alter.first, alter.second, lv)->flagUp(true);
		}
		else
			numComponent++;
	}

	void insert(Vertex a, Vertex b) {
		if (a > b) swap(a, b);
		if (isSameComponent(a, b, 0)) {
			_insertExtra(a, b, 0);
		}
		else {
			_insertMST(a, b, 0);
			find(a, b, 0)->flagUp(true);
			numComponent--;
		}
	}
};

int N, Q;
unsigned long long int F = 0;

int main() {
	using namespace std;
	ios::sync_with_stdio(false);
	cin.tie(0);
	cin >> N >> Q;
	Graph* graph = new Graph(N);
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
			if (graph->isContaining(x, y)) {
				graph->remove(x, y);
			}
			else {
				graph->insert(x, y);
			}
		}
		else {
			cout << (graph->isContaining(y, x) 
				|| graph->isSameComponent(y, x, 0)) << '\n';
#ifdef DEBUG
			fo << graph->isSameComponent(x, y, 0) << endl;
#endif // DEBUG
		}

		F += graph->getNumComponent();
	}
#ifdef DEBUG
	fo.close();
#endif
	return 0;
}
