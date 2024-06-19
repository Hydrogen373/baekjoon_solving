#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<vector>
#include<algorithm>

using Index = std::vector<int>;
using Dictionary = std::vector<std::string>;

struct Distributer {
	std::vector<Index> prefixToIndex;
	bool endString = false;
	int endStringIndex;
	char lastPrefix = 0;
	bool prefixDifferent = false;
public:
	Distributer() :prefixToIndex(26) {
	}
	void push(char c, int n) {
		if (c == 0) {
			endString = true;
			endStringIndex = n;
			return;
		}
		if (lastPrefix == 0) {
			lastPrefix = c;
		}
		if (lastPrefix != c) {
			prefixDifferent = true;
		}
		prefixToIndex[c - 'a'].push_back(n);
	}
	bool isEndString() {
		return endString;
	}
	bool isPrefixDifferent() {
		return prefixDifferent;
	}
	const std::vector<Index>& getPrefixToIndex() {
		return prefixToIndex;
	}
};

class TestCase {
	Dictionary dict;
	long long int result;

public:
	TestCase(const Dictionary& dict) 
		:dict(dict), result(0) {}

	void run() {
		if (dict.size() == 1) {
			result = 1;
		}
		else {
			Distributer distributer;
			for (int i = 0; i < dict.size(); i++) {
				distributer.push(dict[i].at(0), i);
			}
			for (auto index : distributer.prefixToIndex) {
				if (index.empty()) {
					continue;
				}
				distribute(index, 1, 1);
			}
		}
		char buf[1024];
		auto tmp = result * 1000 / dict.size();

		sprintf(buf, "%ld.%02ld\n", tmp / 1000, (tmp % 1000) / 10 + (tmp % 10 >= 5 ? 1 : 0));
		std::cout << buf;
	}

	void main() {

	}

	void distribute(const Index& index, int depth, int cursor) {
		if (index.size() == 1) {
			result += depth;
			return;
		}
		while (true) {
			Distributer distributer;
			for (auto ind : index) {
				if (dict[ind].size() <= cursor) {
					distributer.push(0, ind);
					result += depth;
				}
				else {
					distributer.push(dict[ind].at(cursor), ind);
				}
			}
			if (distributer.isEndString() 
				|| distributer.isPrefixDifferent()) {
				for (auto index : distributer.getPrefixToIndex()) {
					if (index.empty()) {
						continue;
					}
					distribute(index, depth + 1, cursor + 1);
				}
				break;
			}
			else {
				cursor += 1;
			}
		}
	}
};



int main() {
	//
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);
	//
	while (true) {
		int N;
		std::cin >> N;
		if (std::cin.eof()) {
			break;
		}
		Dictionary dict;
		for (int i = 0; i < N; i++) {
			std::string str;
			std::cin >> str;
			dict.push_back(str);
		}
		TestCase tc(dict);
		tc.run();
	}


	//



	//

	return 0;
}
