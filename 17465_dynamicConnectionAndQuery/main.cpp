#include<iostream>
#include<map>
#include<set>

// structure
using std::map;
using std::set;
using Node = unsigned int;
class Ark {
public:
	Node a;
	Node b;
	Ark() : Ark(0, 0) {
	}
	Ark(Node a, Node b) : a(a), b(b) {
	}
	bool operator<(const Ark& other) {
		return this->a < other.a || this->a == other.a && this->b < other.b;
	}
	//bool operator>(const Ark& other) {
	//	return !(*this < other);
	//}
	//bool operator==(const Ark& other) {
	//	return !((*this < other) && (*this < other));
	//}

};
class Edge :Ark {
public:
	Edge() :Edge(0, 0) {
	}
	Edge(Node a, Node b) :Ark(a, b) {
		if (a > b) std::swap(a, b);
	}
	Edge(const Ark& ark) {
		this->a = ark.a;
		this->b = ark.b;
		if (a > b) std::swap(a, b);
	}
};
class LeveledArk :Ark {
public:
	int level;
	LeveledArk() :LeveledArk(0, 0, 0) {
	}
	LeveledArk(Node a, Node b, int level) :Ark(a, b), level(level) {
	}
	bool operator<(const LeveledArk& other) {
		if (this->level != other.level) return this->level < other.level;
		else if (this->a != other.a) return this->a < other.a;
		else return this->b < other.b;
	}
};
class LeveledEdge :LeveledArk {
public:
	int level;
	LeveledEdge() :LeveledEdge(0, 0, 0) {
	}
	LeveledEdge(Node a, Node b, int level) :LeveledArk(a, b, level) {
		if (a > b) std::swap(a, b);
	}
};
class NoEdge :std::exception {};
class SeriesNode {
	LeveledArk ark;
	SeriesNode* p, * l, * r;
	int size; 
	SeriesNode* head; // leftmost vertex
	bool isVertex;
	
	void update() {
		// update size
		this->size = 1;
		if (this->l != nullptr)
		{
			this->size += l->size;
		}
		if (this->r != nullptr)
		{
			this->size += r->size;
		}
		// update head
		if (l != nullptr && l->head != nullptr)
		{
			this->head = l->head;
		}
		else if (this->isVertex)
		{
			this->head = this;
		}
		else if (r != nullptr)
		{
			this->head = r->head;
		}
	}

	void rotate() {
		if (this->p == nullptr) {
			return;
		}
		SeriesNode* child;
		SeriesNode* grandpa = p->p;
		SeriesNode* parent = this->p;
		if (p->l == this) {
			child = this->r;
			this->r = parent;
			this->p = grandpa;
			parent->l = child;
			parent->p = this;
		}
		else {
			child = this->l;
			this->l = parent;
			this->p = grandpa;
			parent->r = child;
			parent->p = this;
		}
		if (grandpa->l == parent) {
			grandpa->l = this;
		}
		else {
			grandpa->r = this;
		}
		parent->update();
		this->update();
		grandpa->update();
	}

	void splay() {
		if (this->p == nullptr) {
			return;
		}

		SeriesNode* parent = this->p;
		SeriesNode* grandpa = this->p->p;
		if (grandpa != nullptr) {
			if ((grandpa->l == parent) == (parent->l == this)) {
				parent->rotate();
			}
			else {
				this->rotate();
			}
		}
		this->rotate();
		splay();
	}

	void rooting() {
		splay();
		SeriesNode* left = this->l;
		this->l = nullptr;
		update();
		left->p = nullptr;
		SeriesNode* right = this->r;
		while (right->r != nullptr)
		{
			right = right->r;
		}

		right->splay();
		right->r = left;
		right->update();
	}
};

// global variables
unsigned int N, Q, numComponent, F{ 0 };
map<Edge, int> levels;

// function 
bool isMST(LeveledEdge);
bool isExtraEdge(Edge);
void removeMST(LeveledEdge);
void insertMST(LeveledEdge);
LeveledEdge findAlterEdge(LeveledEdge);
bool isSameComponent(Node a, Node b);

int main() {
	using namespace std;
	cin >> N >> Q;
	numComponent = N;

	for (unsigned int i = 0; i < Q; i++) {
		unsigned int a, b, x, y;
		std::cin >> a >> b;
		x = (a ^ F) % N;
		y = (b ^ F) % N;

		if (x < y) {
			// remove edge
			//if (mst.find(LevelArk(x, y, 0)) != mst.end()) {
			if (isMST(LeveledEdge(x, y, 0))) {
				int level = levels[Edge(x, y)];
				levels.erase(Edge(x, y));
				for (int i = level; i >= 0; i--) {
					removeMST(LeveledEdge(x, y, 0));
				}

				LeveledEdge alterEdge;
				bool found = false;
				for (int i = level; i >= 0; i--) {
					LeveledEdge le(x, y, i);
					try {
						alterEdge = findAlterEdge(le);
						found = true;
						break;
					}
					catch (NoEdge ne) {
						continue;
					}
				}

				if (found) {
					while (alterEdge.level >= 0) {
						insertMST(alterEdge);
						--alterEdge.level;
					}
				}
				else {
					++numComponent;
				}
			}
			else if (isExtraEdge(Edge(x, y))) {
				// remove extra edge
				// TODO
			}

			// add edge
			if (isSameComponent(x, y)) {
				// add extra edge
				// TODO
			}
			else {
				insertMST(LeveledEdge(x, y, 0));
				--numComponent;
			}
		}
		else {
			cout << isSameComponent(x, y) << endl;
		}

		F += numComponent;
	}
	// terminate
	return 0;
}

// definition
