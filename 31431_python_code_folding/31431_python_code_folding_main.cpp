#include<iostream>
#include<vector>
#include<stack>
#include<map>

class SplayTree;
void toggle(int);
void _show(SplayTree*, int);
class SplayTree {
protected:
	SplayTree* p = nullptr;
	int left;
	int right;
	int sum = 1;
	bool folded = false;
	SplayTree* lt = nullptr;
	SplayTree* rt = nullptr;
	SplayTree* ft = nullptr;
	friend void toggle(int);
	friend void _show(SplayTree*, int);
public:
	SplayTree(int i) :left(i), right(i) {}
	SplayTree(int l, int r) :left(l), right(r) {}
	void fold(SplayTree* t, int r) {
		ft = t;
		folded = true;
		right = r;
	}
	SplayTree* unfold() {
		auto res = ft;
		ft = nullptr;
		folded = false;
		right = left;
		return res;
	}
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
		sum = 1;
		if (lt != nullptr) {
			sum += lt->sum;
		}
		if (rt != nullptr) {
			sum += rt->sum;
		}
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
		this->p = grandpa;
		parent->update();
		this->update();
	}
	void splay() {
		if (p == nullptr) {
			return;
		}
		if (p->p != nullptr) {
			if ((p->p->lt == p) == (p->lt == this)) {
				p->rotate();
			}
			else {
				rotate();
			}
		}

		rotate();
		splay();
	}
	SplayTree* splitR() {
		splay();
		auto res = rt;
		if (rt != nullptr)
			rt->p = nullptr;
		rt = nullptr;
		update();
		return res;
	}
	SplayTree* splitL() {
		splay();
		auto res = lt;
		if (lt != nullptr)
			lt->p = nullptr;
		lt = nullptr;
		update();
		return res;
	}
	SplayTree* getRoot() {
		auto res = this;
		while (res->p != nullptr) {
			res = res->p;
		}
		return res;
	}
	static void conn(SplayTree* l, SplayTree* r) {
		if (l == nullptr || r == nullptr || l == r) {
			return;
		}
		l = l->getRoot();
		r = r->getRoot();
		while (l->rt != nullptr) {
			l = l->rt;
		}
		l->splay();

		l->rt = r;
		r->p = l;
		l->update();
	}

	//SplayTree* _find(int i) {
	//	if (left <= i && i <= right) {
	//		splay();
	//		return this;
	//	}
	//	else if (i < left)
	//		return lt->_find(i);
	//	else
	//		return rt->_find(i);
	//}

	int getSum() {
		return sum;
	}
};

struct Block {
	bool folded = false;
	int header = 0;
	int end = 0;
	Block(int h, int e) :header(h), end(e) {}
};
std::vector<Block> blocks;
int lineNum = 0;
std::vector<SplayTree*> vec;
void toggle(int i) {
	SplayTree* t1, * t2, * t3;
	if (blocks[i].folded == false) {
		// folding
		t1 = vec[blocks[i].header];
		t3 = vec[blocks[i].end];

		t2 = t1->splitR();
		if (t3 != nullptr) {
			t2 = t3->splitL();
		}
		t1->fold(t2, blocks[i].end - 1);
		SplayTree::conn(t1, t3);
		t1->splay();
		lineNum = t1->getSum();
	}
	else {
		t1 = vec[blocks[i].header];
		t3 = t1->splitR();

		t2 = t1->unfold();
		
		SplayTree::conn(t1, t2);
		SplayTree::conn(t2, t3);
		t1->splay();
		lineNum = t1->getSum();
	}
	blocks[i].folded = !blocks[i].folded;
}

void _show(SplayTree* t, int i){
	if (t->rt != nullptr) {
		_show(t->rt, i + 1);
	}
	for (int j = 0; j < i; j++) {
		std::cout << '\t';
	}
	if (t->folded) {
		std::cout << "(" << t->left << ", " << t->right << ")";
	}
	else {
		std::cout << "(" << t->left << ")";
	}
	std::cout << "[" << t->sum << "]" << '\n';
	if (t->lt != nullptr) {
		_show(t->lt, i + 1);
	}
}
void show() {
	std::cout << "vvvvvvvvvvvvvvvvvvvvvvvvv\n";
	_show(vec[1]->getRoot(), 0);
	std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^\n";

}

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0); std::cout.tie(0);

	blocks.push_back(Block(0, 0));
	vec.push_back(nullptr);

	int N, Q;
	std::cin >> N >> Q;
	lineNum = N;

	std::stack<std::pair<int, int>> blockStack;
	SplayTree* node = nullptr;
	SplayTree* root = nullptr;
	for (int i = 1; i <= N; i++) {
		int li;
		char ti;
		std::cin >> li >> ti;

		while (blockStack.empty() == false)
		{
			if (blockStack.top().first >= li) {
				blocks[blockStack.top().second].end = i;
				blockStack.pop();
			}
			else {
				break;
			}
		}

		if (ti == 'h') {
			blocks.push_back(Block(i, i));
			blockStack.push(std::make_pair(li, blocks.size() - 1));
		}

		node = new SplayTree(i);
		vec.push_back(node);

		SplayTree::conn(root, node);
		std::swap(node, root);
		root->splay();
	}
	while (blockStack.empty() == false) {
		blocks[blockStack.top().second].end = N + 1;
		blockStack.pop();
	}
	vec.push_back(nullptr);

	for (int i = 0; i < Q; i++) {
		char q;
		std::cin >> q;
		if (q == 't') {
			int x;
			std::cin >> x;
			toggle(x);
			//// debug
			//show();
		}
		else {
			std::cout << lineNum << '\n';
		}
	}


	return 0;
}

