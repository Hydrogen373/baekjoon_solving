#include<iostream>
#include<map>
#include<queue>
#include<set>


using std::map;
using std::set;
using std::queue;
using Node = int;

struct Edge {
	int a;
	int b;
};

bool operator<(const Edge& e1, const Edge& e2) {
	return e1.a < e2.a || e1.a == e2.a && e1.b < e2.b;
}

class MST {
	struct MSTNode {
		MSTNode* prev = nullptr;
		Edge value;
		MSTNode* next = nullptr;
	};
	map<Edge, MSTNode*> mst;

	void addNode(Node node) {
		MSTNode* mstnode = new MSTNode{ nullptr, {node,node}, nullptr };
		mst.insert({ { node,node }, mstnode });
	}

public:
	void rooting(const Edge& e) {
		MSTNode* edge = mst[e];
		if (edge->prev == nullptr) {
			return;
		}

		MSTNode* n1, * n2, * n3, * n4;
		n1 = edge;
		while (n1->prev != nullptr) {
			n1 = n1->prev;
		}
		n2 = edge->prev;

		n3 = edge;
		n4 = edge;
		while (n4->next == nullptr)
		{
			n4 = n4->next;
		}
		n2->next = nullptr;
		n3->prev = nullptr;

		n4->next = n1;
		n1->prev = n4;
	}

	void addEdge(const Edge& e) {
		if (mst.find({ e.a,e.a }) == mst.end()) {
			addNode(e.a);
		}
		if (mst.find({ e.b,e.b }) == mst.end()) {
			addNode(e.b);
		}

		rooting({ e.a,e.a });
		rooting({ e.b,e.b });
		MSTNode* n1, * n2, * n3, * n4;
		n1 = mst[{e.a, e.a}];
		n2 = n1->next;
		n3 = mst[{e.b, e.b}];
		n4 = n3;
		while (n4->next != nullptr) {
			n4 = n4->next;
		}

		MSTNode* e1, * e2;
		e1 = new MSTNode{ n1, {e.a, e.b}, n3 };
		e2 = new MSTNode{ n4, {e.b, e.a}, n2 };

		n1->next = e1;
		if (n2 != nullptr) {
			n2->prev = e2;
		}
		n3->prev = e1;
		n4->next = e2;
	}
	
	void removeEdge(const Edge& e);

};

class Forest {
	class NoEdge :std::exception {};
	set<Edge> extraEdges;
	MST mst;
	map<Node, Forest*> subforest;

public:
	Forest(MST& mst) :mst(mst) {}

	~Forest() {
		for (auto iter : subforest) {
			delete iter.second;
		}
		for (auto iter : mst) {
			delete iter.second;
		}
	}

	void addMSTEdge(const Edge& e) {
		rooting({ e.a, e.a });
		rooting({ e.b, e.b });
		
	}
	


	Edge findAlterEdge(Edge& e) {
		rooting(e);
		
		

	}

	



	
	
};

int* roots;

bool findSet();
void unionSet();


