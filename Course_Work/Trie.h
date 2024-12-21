#pragma once
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>

using namespace std;

class TrieNode {
private:
    vector<TrieNode*> children;
    size_t end_of_word_value;
public:
    TrieNode();
    friend class Trie;
};


class Trie {
private:
    TrieNode* root;

    void FindAllWordsWithPrefix(TrieNode* node, string prefix, multimap<size_t, string>& words);

public:
    Trie();

    multimap<size_t, string> autocomplete(string prefix);

    multimap<size_t, string> traverse();

    void save_to_file(string fname);

    void load_from_file(string fname);

    void insert(string word);

    void insert_many(string word, size_t amount);

    size_t search(string word);

};