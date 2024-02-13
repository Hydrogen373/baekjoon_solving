#include<iostream>
#include<map>

int H, W;
int R, C, D;

int dr[]{ -1,0,1,0 };
int dc[]{ 0,1,0,-1 };

char** A;
char** B;

struct Status {
	unsigned short r, c;
	unsigned char d;
	Status(unsigned short r, unsigned short c, unsigned char d)
		:r(r), c(c), d(d) {}
	void rotate(char c) {
		d = (d + c) % 4;
	}
	void move() {
		r += dr[d];
		c += dc[d];
	}
	bool operator==(const Status& other) const {
		return r == other.r && c == other.c && d == other.d;
	}
	bool operator<(const Status& other) const {
		if (r != other.r) return r < other.r;
		else if (c != other.c) return c < other.c;
		else return d < other.d;
	}
};
struct TreeNode {
	Status stat;
	TreeNode* head;
	long long distance;
	long long size;
	TreeNode(const Status& s)
		:stat(s), head(nullptr), distance(0), size(1) {}
	void setHead(TreeNode* h, long long distance) {
		if (head != nullptr) {
			return;
		}
		h->size += this->size;
		head = h;
		this->distance = distance;
	}
	void deHead() {
		if (head == nullptr)
			return;
		head->size -= this->size;
		head = nullptr;
		distance = 0;
	}
	void compressPath() {
		if (head == nullptr || head->head == nullptr)
			return;
		head->compressPath();
		auto h = head->head;
		auto d = head->distance;
		d += this->distance;
		deHead();
		setHead(h, d);
		return;
	}
	TreeNode* getRoot() {
		compressPath();
		TreeNode* result = this;
		while (result->head != nullptr) {
			result = result->head;
		}
		return result;
	}
};
std::map<Status, TreeNode*> treeNodes;
std::map<Status, TreeNode*> destinations;
TreeNode endPoint(Status(0xffff, 0xffff, 0xff));
long long getDistance(const Status& first, const Status& last) {
	if (first == last) return 0;
	if (first == endPoint.stat || last == endPoint.stat)
		return INT64_MAX;
	if (treeNodes.find(first) == treeNodes.end()
		|| treeNodes.find(last) == treeNodes.end()) {
		return INT64_MAX;
	}
	auto firstNode = treeNodes[first];
	auto lastNode = treeNodes[last];
	{
		auto firstRoot = firstNode->getRoot();
		auto lastRoot = lastNode->getRoot();
		if (firstRoot != lastRoot) return INT64_MAX;
		if (firstNode->head == nullptr)
			return -lastNode->distance;
		else if (lastNode->head == nullptr)
			return firstNode->distance;
	}

	return firstNode->distance - lastNode->distance;
}
Status getDestination(const Status& s) {
	if (treeNodes.find(s) == treeNodes.end()) {
		return s;
	}
	return destinations[treeNodes[s]->getRoot()->stat]->stat;
}
void clean(unsigned short r, unsigned short c) {
	for (int i = 0; i < 4; i++) {
		Status prev(r, c, i);
		Status next = prev;
		next.rotate(B[r][c]);
		next.move();

		if (treeNodes.find(prev) == treeNodes.end()) {
			TreeNode* node = new TreeNode(prev);
			treeNodes.insert(std::make_pair(prev, node));
			destinations.insert(std::make_pair(prev, node));
		}
		auto np = treeNodes[prev];
		if (next.r < 0 || next.r >= H 
			|| next.c < 0 || next.c >= W)
		{
			destinations[np->getRoot()->stat] 
				= &endPoint;
			continue;
		}

		if (treeNodes.find(next) == treeNodes.end()) {
			TreeNode* node = new TreeNode(next);
			treeNodes.insert(std::make_pair(next, node));
			destinations.insert(std::make_pair(next, node));
		}
		auto nn = treeNodes[next];
		auto npr = np->getRoot();
		auto nnr = nn->getRoot();
		if (nnr == npr) {
			destinations[npr->stat] = &endPoint;
			continue;
		}

		long long distance
			= getDistance(npr->stat, np->stat)
			+ 1
			+ getDistance(nn->stat, nnr->stat);
		if (npr->size < nnr->size) {
			npr->setHead(nnr, distance);
			destinations.erase(npr->stat);
		}
		else {
			nnr->setHead(npr, -distance);
			destinations[npr->stat] = destinations[nnr->stat];
			destinations.erase(nnr->stat);
		}
		continue;
	}
}

Status aris(0, 0, 0);
long long arisMovingCounter = 0;
int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	std::cout.tie(0);

	std::cin >> H >> W;

	std::cin >> R >> C >> D;
	aris = Status(R, C, D);

	A = new char* [H];
	for (int i = 0; i < H; i++) {
		A[i] = new char[W];
		for (int j = 0; j < W; j++) {
			std::cin >> A[i][j];
			A[i][j] -= '0';
		}
	}
	B = new char* [H];
	for (int i = 0; i < H; i++) {
		B[i] = new char[W];
		for (int j = 0; j < W; j++) {
			std::cin >> B[i][j];
			B[i][j] -= '0';
		}
	}

	while (0 <= aris.r && aris.r < H
		&& 0 <= aris.c && aris.c < W) {
		auto next = getDestination(aris);
		if (next == aris) {
			clean(aris.r, aris.c);
			aris.rotate(A[aris.r][aris.c]);
			aris.move();
			arisMovingCounter += 1;
		}
		else if (next == endPoint.stat) {
			break;
		}
		else {
			auto d = getDistance(aris, next);
			aris = next;
			arisMovingCounter += d;
		}

	}
	std::cout << arisMovingCounter;

	for (int i = 0; i < H; i++) {
		delete[] A[i], B[i];
	}
	delete[] A, B;

	return 0;
}