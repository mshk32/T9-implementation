#include "BKtree.h"

using namespace std;

int levenshtein_distance(const string& a, const string& b) {
	size_t n = a.length(), m = b.length();

	vector<vector<int>> dm(n + 1, vector<int>(m + 1));

	for (int i = 0; i <= n; ++i) dm[i][0] = i;
	for (int j = 0; j <= m; ++j) dm[0][j] = j;

	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= m; ++j) {
			int cost = (a[i - 1] == b[j - 1]) ? 0 : 1;
			dm[i][j] = min({ dm[i - 1][j] + 1,				//deletion
							 dm[i][j - 1] + 1,				//insertion
							 dm[i - 1][j - 1] + cost });    //substitution
		}
	}
	return dm[n][m]; //return bottom-right element of levenshtein matrix
}


BKnode::BKnode(const string& w) { word = w; }

void BKtree::insert(BKnode* node, const string& new_word) {
	if (node->word == new_word) return;                       //don't add duplicates 
	int dist = levenshtein_distance(node->word, new_word);
	if (node->children.find(dist) == node->children.end()) {
		node->children[dist] = new BKnode(new_word);
	}
	else {
		insert(node->children[dist], new_word);
	}
}

void BKtree::search(BKnode* node, const string& query, size_t max_dist, multimap<size_t, string>& results) {
	size_t dist = levenshtein_distance(node->word, query);
	if (dist <= max_dist) {
		results.insert({ dist, node->word });
	}

	for (auto& key : node->children) {
		if (key.first >= dist - max_dist && key.first <= dist + max_dist) {
			search(key.second, query, max_dist, results);
		}
	}
}

BKtree::BKtree() { root = nullptr; }

void BKtree::insert(const string& word) {
	if (!root) root = new BKnode(word);
	else insert(root, word);
}

multimap<size_t, string> BKtree::search(const string& query, size_t max_dist) {
	multimap<size_t, string> results;
	if (query.empty()) return results;
	if (root) {
		search(root, query, max_dist, results);
	}

	return results;
}