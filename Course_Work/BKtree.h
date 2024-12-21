#pragma once
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

int levenshtein_distance(const string& a, const string& b);


struct BKnode {
	string word;
	map<int, BKnode*> children;

	explicit BKnode(const string& w);
};


class BKtree {
private:
	BKnode* root;

	void insert(BKnode* node, const string& new_word);

	void search(BKnode* node, const string& query, size_t max_dist, multimap<size_t, string>& results);

public:
	BKtree();

	void insert(const string& word);

	multimap<size_t, string> search(const string& query, size_t max_dist);
};