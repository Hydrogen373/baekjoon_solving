#include<iostream>
#include<string>
#include<algorithm>
#include<vector>

struct comp {
	bool operator()(const std::string& a, const std::string& b) {
		return a.compare(b) > 0;
	}
};

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	std::cout.tie(0);

	int N;
	std::cin >> N;

	std::vector<std::string> words;
	std::string result;

	for (int i = 0; i < N; i++) {
		std::string tmp;
		std::cin >> tmp;
		words.push_back(tmp);
		std::push_heap(words.begin(), words.end(), comp());
	}

	while (words.empty() == false)
	{
		std::string first = *(words.begin());
		std::pop_heap(words.begin(), words.end(), comp());
		words.pop_back();

		result.push_back(first.front());

		first = first.substr(1, first.length() - 1);

		if (first.empty() == false) {
			words.push_back(first);
			std::push_heap(words.begin(), words.end(), comp());
		}
	}

	std::cout << result;
	return 0;
}