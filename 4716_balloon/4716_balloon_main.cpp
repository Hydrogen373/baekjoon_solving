#include<iostream>
#include<vector>
#include<algorithm>
struct Team {
	int balloonNum;
	int Da;
	int Db;
	int var;
	Team(int bn, int da, int db) 
		:balloonNum(bn), Da(da), Db(db), var(Da > Db ? Da - Db : Db - Da) {}
	bool operator<(const Team& other) const {
		return var < other.var;
	}

};
class TestCase {
	int N = 0;
	int A = 0;
	int B = 0;

	std::vector<Team> vecA;
	std::vector<Team> vecB;
	int result = 0;
public:
	int init() {
		std::cin >> N >> A >> B;
		if (N == 0) {
			return 1;
		}
		return 0;
	}
	int run() {
		int bn, da, db;
		for (int i = 0; i < N; i++) {
			std::cin >> bn >> da >> db;
			if (da < db) {
				vecA.push_back(Team(bn, da, db));
				A -= bn;
				result += bn * da;
			}
			else {
				vecB.push_back(Team(bn, da, db));
				B -= bn;
				result += bn * db;
			}
		}

		if (A >= 0 && B >= 0)
			return result;

		std::vector<Team>* v;
		if (A < 0) {
			v = &vecA;
		}
		else {
			v = &vecB;
		}

		std::sort(v->begin(), v->end());
		int shortage = -std::min(A, B);

		auto iter = v->begin();
		while (shortage > 0) {
			int t = std::min(shortage, iter->balloonNum);
			shortage -= t;
			result += iter->var * t;
			iter++;
		}
		return result;
	}
};
int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0); std::cout.tie(0);

	//
	while (true) {
		TestCase tc;
		if (tc.init())
			break;
		std::cout << tc.run() << '\n';
	}
	//
	return 0;
}