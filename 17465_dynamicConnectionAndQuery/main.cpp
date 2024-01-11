#include<iostream>
#include<map>
#include<set>
#include<vector>
#include<fstream>
#include<cassert>

using namespace std;

using Node = int;

class Ark {
public:
	Node a;
	Node b;
	Ark();
	Ark(Node, Node);
	bool operator<(const Ark& other);
};
class Edge {
public:
	Node a;
	Node b;
	Edge();
	Edge(Node, Node);
	Edge(const Ark&);
	bool operator<(const Edge& other);
};
class SeriesNode {
public:
	SeriesNode* p, * l, * r;
	Ark ark;
	int size; 
	bool isVertex; 
	SeriesNode* leftmostVertex;

	SeriesNode(const Ark&);
	static void join(SeriesNode*, SeriesNode*);
	void splay();
	void rotate();
	void makeHead();
	void update();
	SeriesNode* getRoot();
	bool operator<(const SeriesNode& other);
	void print(int);

};
class NoEdge :std::exception {};
class MST {
private:
	void levelupMST(SeriesNode*);
	SeriesNode* addNode(Ark);
public:
	int level = 0;
	~MST();
	set<Node> extraEdges[100'000];
	map<Ark, SeriesNode*> mp;
	void insert(Edge);
	void remove(Edge);
	bool isSameComponent(Node a, Node b);
	Edge findAlterEdge(Edge);
};
struct EdgeInfo{
	bool mst;
	int level;
	EdgeInfo();
	EdgeInfo(bool, int);
};

bool operator<(const Ark& a, const Ark& b) {
	return a.a < b.a || a.a == b.a && a.b < b.b;
}
bool operator<(const Edge& a, const Edge& b) {
	return a.a < b.a || a.a == b.a && a.b < b.b;
}

// variables
unsigned int N, Q, numComponent, F{ 0 };
map<Edge, EdgeInfo> info;
MST mst[20];

int main() {
	cin >> N >> Q;
	numComponent = N;

	for (int i = 0; i < 20; i++) {
		mst[i].level = i;
	}

	for (unsigned int i = 0; i < Q; i++) {
		unsigned int a, b, x, y;
		std::cin >> a >> b;
		x = (a ^ F) % N;
		y = (b ^ F) % N;

		if (x < y) {
			Edge edge = Edge(x, y);
			auto edgeinfo = info.find(edge);
			bool taskIsRemoving = (edgeinfo != info.end());
			bool edgeIsMST;
			if (taskIsRemoving) {
				int L = edgeinfo->second.level;
				edgeIsMST = edgeinfo->second.mst;
				info.erase(edgeinfo);

				if (edgeIsMST) {
					for (int i = L; i >= 0; i--) {
						mst[i].remove(edge);
					}

					// find alterEdge
					bool found = false;
					int alterLevel;
					Edge alterEdge;
					for (int i = L; i >= 0; i--) {
						try {
							alterEdge = mst[i].findAlterEdge(edge);
							alterLevel = i;
							found = true;
							break;
						}
						catch (const NoEdge&) {
							continue;
						}
					}

					if (found) {
						info[alterEdge].mst = true;
						for (int i = 0; i < alterLevel; i++) {
							mst[i].insert(alterEdge);
						}
					}
					else {
						numComponent++;
					}
				}
				else {
					mst[L].extraEdges[x].erase(y);
					mst[L].extraEdges[y].erase(x);
				}
				
			}
			else {
				edgeIsMST = !(mst[0].isSameComponent(x, y));
				info.insert(make_pair(edge, EdgeInfo(edgeIsMST, 0)));
				if (edgeIsMST) {
					mst[0].insert(edge);
					numComponent--;
				}
				else {
					mst[0].extraEdges[x].insert(y);
					mst[0].extraEdges[y].insert(x);
				}

				// debug
				auto tmp = mst[0].mp[Ark(x, x)];
				tmp->splay();
				tmp->print(0);

			}


		}
		else {
			cout << mst[0].isSameComponent(x, y) << endl;
		}

		F += numComponent;
	}
	// terminate
	return 0;
}

Ark::Ark() :Ark(0, 0)
{
}

Ark::Ark(Node a, Node b) :
	a(a), b(b)
{
}

bool Ark::operator<(const Ark& other)
{
	return this->a < other.a || this->a == other.a && this->b < other.b;
}

Edge::Edge() :Edge(0, 0)
{
}

Edge::Edge(Node a, Node b) :a(a), b(b)
{
	if (a > b) swap(a, b);
}

Edge::Edge(const Ark& ark) :Edge(ark.a, ark.b)
{
}

bool Edge::operator<(const Edge& other)
{
	return this->a < other.a || this->a == other.b && this->b < other.b;

}

SeriesNode::SeriesNode(const Ark& ark) :
	p(nullptr), l(nullptr), r(nullptr),
	ark(ark),
	size(1),
	isVertex(false),
	leftmostVertex(nullptr)
{
	if (ark.a == ark.b) {
		isVertex = true;
		leftmostVertex = this;
	}
}

void SeriesNode::join(SeriesNode* a, SeriesNode* b)
{
	a = a->getRoot();
	b = b->getRoot();
	while (a->r != nullptr)
	{
		a = a->r;
	}
	a->splay();
	a->r = b;
	b->p = a;
	a->update();
}

void SeriesNode::splay()
{
	if (this->p == nullptr) {
		return;
	}
	SeriesNode* parent = this->p;
	SeriesNode* grandpa = this->p->p;
	if (this->p->p != nullptr) {
		if ((grandpa->l == p) == (parent->l == this)) {
			parent->rotate();
		}
		else {
			this->rotate();
		}
	}
	this->rotate();
	this->splay();
}

void SeriesNode::rotate()
{
	if (this->p == nullptr) {
		return;
	}
	SeriesNode* child;
	SeriesNode* parent = this->p;
	SeriesNode* grandpa = this->p->p;

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
	child->p = parent;
	parent->p = this;
	this->p = grandpa;

	parent->update();
	this->update();
	return;
}

void SeriesNode::makeHead()
{
	// debug
	//this->getRoot()->print(0);

	splay();

	// debug
	//cout << "this!!";
	//this->getRoot()->print(0);

	if (this->l == nullptr) {
		return;
	}
	SeriesNode* left = this->l;
	this->l = nullptr;
	left->p = nullptr;
	this->update();

	//// debug
	//this->getRoot()->print(0);

	SeriesNode* right = this;
	while (right->r != nullptr)
	{
		right = right->r;
	}
	right->splay();

	//// debug
	//this->getRoot()->print(0);


	assert(right->r == nullptr);
	
	right->r = left;
	left->p = right;

	right->update();
}

void SeriesNode::update()
{
	this->size = 1;
	this->leftmostVertex = nullptr;
	if (l != nullptr) {
		size += l->size;
		this->leftmostVertex = l->leftmostVertex;
	}
	if (leftmostVertex == nullptr && this->isVertex) {
		leftmostVertex = this;
	}
	if (r != nullptr) {
		size += r->size;
		if (leftmostVertex == nullptr) {
			leftmostVertex = r->leftmostVertex;
		}
	}
}

SeriesNode* SeriesNode::getRoot()
{
	SeriesNode* result = this;
	while (result->p != nullptr) {
		result = result->p;
	}
	return result;
}

void SeriesNode::print(int i = 0)
{
	if (i == 0) {
		cout << "############################";
		cout << endl;
	}
	if (this->r != nullptr) {
		this->r->print(i + 1);
	}
	else {
		cout << endl;
	}
	for (int j = 0; j < i; j++) {
		cout << '=';
	}
	cout << this->ark.a << this->ark.b << endl;
	if (this->l != nullptr) {
		this->l->print(i + 1);
	}
	else {
		cout << endl;
	}
	if (i == 0) {
		cout << "############################";
		cout << endl;
	}
	
}

bool SeriesNode::operator<(const SeriesNode& other)
{
	return this->ark < other.ark;
}

void MST::levelupMST(SeriesNode* series)
{
	if (series == nullptr) return;
	levelupMST(series->l);
	Edge edge(series->ark.a, series->ark.b);
	if (info[edge].level == this->level && edge.a != edge.b) {
		info[edge].level++;
		mst[level + 1].insert(edge);
	}
	levelupMST(series->r);
}

SeriesNode* MST::addNode(Ark ark) {
	SeriesNode* node = new SeriesNode(ark);
	mp.insert(make_pair(ark, node));
	return node;
}

MST::~MST()
{
	for (auto iter : mp) {
		delete iter.second;
	}
}

void MST::insert(Edge edge)
{
	/*
	* This method assumes that
	* (a,a) and (b,b) are disjoint.
	* mst doesn't contain (a,b) and (b,a).
	*/
	SeriesNode* na, * nb;
	SeriesNode* ab, * ba;

	{
		Ark tmp(edge.a, edge.a);
		auto iter = mp.find(tmp);
		if (iter == mp.end()) {
			na = addNode(tmp);
		}
		else {
			na = iter->second;
		}
		tmp = Ark(edge.b, edge.b);
		iter = mp.find(tmp);
		if (iter == mp.end()) {
			nb = addNode(tmp);
		}
		else {
			nb = iter->second;
		}
	}

	ab = addNode(Ark(edge.a, edge.b));
	ba = addNode(Ark(edge.b, edge.a));

	na->makeHead();
	nb->makeHead();

	na->splay();
	nb->splay();

	//SeriesNode::join(na, ab);
	//SeriesNode::join(ab, nb);
	//SeriesNode::join(nb, ba);

	ab->l = na;
	na->p = ab;
	ab->update();

	nb->l = ab;
	ab->p = nb;
	nb->update();

	ba->l = nb;
	nb->p = ba;
	ba->update();
}

void MST::remove(Edge edge)
{
	Node a = edge.a, b = edge.b;
	SeriesNode* ab, * ba;
	ab = mp[Ark(a, b)];
	ba = mp[Ark(b, a)];


	ab->makeHead();
	ab->splay();
	ab->r->p = nullptr;
	ab->r = nullptr;
	ab->update();

	ba->splay();
	ba->l->p = nullptr;
	ba->l = nullptr;
	ba->r->p = nullptr;
	ba->r = nullptr;
	ba->update();

	mp.erase(Ark(a, b));
	mp.erase(Ark(b, a));

	delete ab, ba;
}

bool MST::isSameComponent(Node a, Node b)
{
	SeriesNode* aa, * bb;
	if (mp.find(Ark(a, a)) == mp.end() || mp.find(Ark(b, b)) == mp.end()) {
		return false;
	}

	aa = mp[Ark(a, a)];
	bb = mp[Ark(b, b)];
	return aa->getRoot() == bb->getRoot();
}

Edge MST::findAlterEdge(Edge edge)
{
	Node a = edge.a, b = edge.b;

	SeriesNode* aa, * bb;
	aa = mp[Ark(a, a)];
	bb = mp[Ark(b, b)];
	aa->splay();
	bb->splay();
	SeriesNode* smallerSeries;
	smallerSeries = aa->size < bb->size ? aa : bb;

	levelupMST(aa);

	SeriesNode* iter = smallerSeries;
	iter->splay();
	iter->leftmostVertex;

	while (iter != nullptr)
	{
		Node u = iter->ark.a;
		auto tmp = extraEdges[u];

		for (Node v : tmp) {
			extraEdges[u].erase(v);
			extraEdges[v].erase(u);
			if (mst[level + 1].isSameComponent(u, v)) {
				mst[level + 1].extraEdges[u].insert(v);
				mst[level + 1].extraEdges[v].insert(u);
				info[Edge(u, v)].level++;
				continue;
			}
			else {
				return Edge(u, v);
			}

		}
		iter->splay();
		iter = iter->r->leftmostVertex;
	}

	throw NoEdge();
}

EdgeInfo::EdgeInfo() :EdgeInfo(0, 0) {}

EdgeInfo::EdgeInfo(bool mst, int level) :
	mst(mst), level(level)
{
}
