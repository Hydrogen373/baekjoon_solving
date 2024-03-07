#include<iostream>
#include<map>

class FoldedNode;
class SplayTree {
protected:
	SplayTree* p = nullptr;
	int key;
	int sum = 1;
	int size = 1;
	bool folded = false;
	SplayTree* lt = nullptr;
	SplayTree* rt = nullptr;
	SplayTree* ft = nullptr;
public:
	SplayTree(int i) :key(i) {}
	~SplayTree() {
		if (lt != nullptr)
			delete lt;
		if (rt != nullptr)
			delete rt;
		if (ft != nullptr)
			delete ft;
		ft = lt = rt = nullptr;
	}
	void update() {
		sum = (folded ? 0 : 1);
		size = 1;
		if (lt != nullptr) {
			if(lt->folded==false)
				sum += lt->sum;
			size += lt->size;
		}
		if (rt != nullptr) {
			if(rt->folded==false)
				sum += rt->sum;
			size += rt->size;
		}
		p->update();
	}
	void rotate() {
		if (p==nullptr)
			return;
		SplayTree* grandpa, * parent, * child;
		parent = p;
		grandpa = p->p;
		if (grandpa != nullptr) {
			if (grandpa->lt == parent) {
				grandpa->lt = this;
			}
			else {
				grandpa->rt = this;
			}
		}
		parent->p = this;
		if (p->lt == this) {
			parent->lt = child = this->rt;
			this->rt = parent;
		}
		else {
			parent->rt = child = this->lt;
			this->lt = parent;
		}
		if (child != nullptr) {
			child->p = parent;
		}
		parent->update();
		this->update();
	}
	void splay() {
		if (p == nullptr) {
			return;
		}
		if (p->p != nullptr) {
			SplayTree* u;
			SplayTree* c;
			if (p->p->lt = p) {
				u = p->p->rt;
			}
			else {
				u = p->p->lt;
			}
			if (p->lt = this) {
				c = rt;
			}
			else {
				c = lt;
			}

			if (u == nullptr
				|| (c != nullptr && u->size < c->size)) {
				p->rotate();
			}
			else {
				rotate();
			}
		}

		rotate();
		splay();
	}
	std::pair<SplayTree*, SplayTree*> split() {
		splay();
		auto r = std::make_pair(lt, rt);
		rt->p = lt->p = nullptr;
		lt = rt = nullptr;
		update();
		return r;
	}
	SplayTree* getRoot() {
		auto r = p;
		while (r->p != nullptr) {
			r = r->p;
		}
		return r;
	}
	static void conn(SplayTree* l, SplayTree* r) {
		l = l->getRoot();
		r = r->getRoot();
		while (l->rt == nullptr) {
			l = l->rt;
		}
		l->splay();
		while (r->lt == nullptr) {
			r = r->lt;
		}
		r->splay();

		if (l->size > r->size) {
			l->rt = r;
			r->p = l;
			l->update();
		}
		else {
			r->lt = l;
			l->p = r;
			r->update();
		}
	}
	static SplayTree* init() {
		auto res = new SplayTree(0);
		res->sum = 0;
		return res;
	}

	SplayTree* find(int i) {
		if (i == key)
			return this;
		else if (i < key)
			return lt->find(i);
		else
			return rt->find(i);
	}
};
class FoldedNode :SplayTree {
	SplayTree* t;
public:
	FoldedNode(SplayTree* t, int k) :t(t), SplayTree(k) {
		sum = 0;
		folded = true;
	}
};

void fold(SplayTree* s, int l, int r) {


}

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0); std::cout.tie(0);
	SplayTree base;
	std::vector<SplayTree*> blocks;
	blocks.push_back(&base);
	std::vector<SplayTree*> nodes;

	int N, Q;
	std::cin >> N >> Q;
	std::stack<std::pair<int,SplayTree*>> lastHeader;
	lastHeader.push(std::make_pair(-1,&base));
	for (int i = 0; i < N; i++) {
		int li;
		char ci;
		std::cin >> li >> ci;
		while (lastHeader.top().first >= li) {
			lastHeader.pop();
		}
		auto node = new SplayTree(li, ci=='h');
		nodes.push_back(node);
		node->point(lastHeader.top().second);
		if (ci == 'h') {
			blocks.push_back(node);
			lastHeader.push(std::make_pair(li, node));
		}
	}

	for (int i = 0; i < Q; i++) {
		char q;
		std::cin >> q;
		if (q == 't') {
			int j;
			std::cin >> j;
			blocks[j]->toggle();
		}
		else {
			std::cout << blocks[0]->getLineNum() << '\n';
		}
	}



	// end
	for (auto node : nodes) {
		delete node;
	}
	return 0;
}

