#include<iostream>
#include<map>
#include<vector>

int H, W;
int R, C, D;

int dr[]{ -1,0,1,0 };
int dc[]{ 0,1,0,-1 };

unsigned char** A;
unsigned char** B;

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
	bool operator!=(const Status& other) const {
		return !(*this == other);
	}
};
struct TreeNode {
	Status stat;
	TreeNode* head;
	long long distance;
	long long size;
	TreeNode* destination;
	TreeNode(const Status& s)
		:stat(s), head(nullptr), distance(INT64_MAX), size(1),
		destination(nullptr) {}
	TreeNode() :TreeNode(Status(0, 0, 0)) {}
	void init(const Status& s) {
		stat = s;
		head = nullptr;
		distance = INT64_MAX;
		size = 1;
		destination = this;
	}
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

TreeNode*** treeNodes;
TreeNode endPoint(Status(0xffff, 0xffff, 0xff));
TreeNode* getTreeNode(const Status& s) {
	if (s.r < 0 || s.r >= H || s.c < 0 || s.c >= W) {
		return &endPoint;
	}
	return &(treeNodes[s.r][s.c][s.d]);
}
long long getDistance(const Status& first, const Status& last) {
	if (first == last) return 0;
	auto firstNode = getTreeNode(first);
	auto lastNode = getTreeNode(last);
	if (firstNode == &endPoint || lastNode == &endPoint)
		return INT64_MAX;

	firstNode->compressPath(); lastNode->compressPath();
	{
		auto firstRoot = firstNode->getRoot();
		auto lastRoot = lastNode->getRoot();
		if (firstRoot != lastRoot) return INT64_MAX;
		else if (firstNode->head == nullptr)
			return -lastNode->distance;
		else if (lastNode->head == nullptr)
			return firstNode->distance;
	}
	return firstNode->distance - lastNode->distance;
}
Status getDestination(const Status& s) {
	return getTreeNode(s)->getRoot()->destination->stat;
}
void clean(unsigned short r, unsigned short c) {
	for (int i = 0; i < 4; i++) {
		Status prev(r, c, i);
		Status next = prev;
		next.rotate(B[r][c]);
		next.move();

		auto np = getTreeNode(prev);
		auto nn = getTreeNode(next);
		if (nn == &endPoint)
		{
			np->getRoot()->destination = &endPoint;
			continue;
		}

		auto npr = np->getRoot();
		auto nnr = nn->getRoot();
		if (nnr == npr) {
			npr->getRoot()->destination = &endPoint;
			continue;
		}

		long long distance
			= getDistance(npr->stat, np->stat)
			+ 1
			+ getDistance(nn->stat, nnr->stat);
		if (npr->size < nnr->size) {
			npr->setHead(nnr, distance);
		}
		else {
			nnr->setHead(npr, -distance);
			npr->destination = nnr->destination;
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

	A = new unsigned char* [H];
	for (int i = 0; i < H; i++) {
		A[i] = new unsigned char[W];
		for (int j = 0; j < W; j++) {
			std::cin >> A[i][j];
			A[i][j] -= '0';
		}
	}
	B = new unsigned char* [H];
	for (int i = 0; i < H; i++) {
		B[i] = new unsigned char[W];
		for (int j = 0; j < W; j++) {
			std::cin >> B[i][j];
			B[i][j] -= '0';
		}
	}
	treeNodes = new TreeNode * *[H];
	for (int i = 0; i < H; i++) {
		treeNodes[i] = new TreeNode * [W];
		for (int j = 0; j < W; j++) {
			treeNodes[i][j] = new TreeNode[4];
			for (int k = 0; k < 4; k++) {
				treeNodes[i][j][k].init(Status(i, j, k));
			}
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

	// termination
	for (int i = 0; i < H; i++) {
		delete[] A[i];
		delete[] B[i];
		for (int j = 0; j < W; j++) {
			delete[] treeNodes[i][j];
		}
		delete[] treeNodes[i];
	}
	delete[] A;
	delete[] B;
	delete[] treeNodes;

	return 0;
}