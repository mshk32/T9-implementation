#include "Trie.h"

TrieNode::TrieNode() {
    this->end_of_word_value = 0;
    children.resize(27, NULL);
}



void Trie::FindAllWordsWithPrefix(TrieNode* node, string prefix, multimap<size_t, string>& words) {
    if (node->end_of_word_value != 0) words.insert({ node->end_of_word_value, prefix });

    for (size_t i = 0; i < 26; ++i) {
        if (node->children[i] != NULL)
            FindAllWordsWithPrefix(node->children[i], (prefix + static_cast<char>('a' + i)), words);
    }
    if (node->children[26] != NULL)
        FindAllWordsWithPrefix(node->children[26], (prefix + "'"), words);
}

Trie::Trie() { root = new TrieNode(); }

multimap<size_t, string> Trie::autocomplete(string prefix) {
    multimap<size_t, string> words;
    if (prefix.empty()) return words;

    TrieNode* node = root;
    for (auto ch : prefix) {
        size_t ch_index;
        if (ch == 39) ch_index = 26;       // symbol '
        else ch_index = ch - 'a';
        if (node->children[ch_index] == NULL)
            return words;
        node = node->children[ch_index];
    }

    FindAllWordsWithPrefix(node, prefix, words);
    return words;
}

multimap<size_t, string> Trie::traverse() {
    multimap<size_t, string> words;
    TrieNode* node = root;
    FindAllWordsWithPrefix(node, "", words);
    return words;
}

void Trie::save_to_file(string fname) {
    ofstream output(fname);
    multimap<size_t, string> words = this->traverse();
    for (auto element : words) {
        output << element.second << " " << element.first << '\n';
    }
}

void Trie::load_from_file(string fname) {
    ifstream input(fname);
    while (!input.eof()) {
        string word;
        size_t freq;
        input >> word >> freq;
        this->insert_many(word, freq);
    }
}

void Trie::insert(string word) {
    TrieNode* node = root;
    for (auto ch : word) {
        size_t ch_index;
        if (ch == 39) ch_index = 26;       // symbol '
        else ch_index = ch - 'a';
        if (node->children[ch_index] == NULL)
            node->children[ch_index] = new TrieNode();
        node = node->children[ch_index];
    }
    node->end_of_word_value++;
}

void Trie::insert_many(string word, size_t amount) {
    TrieNode* node = root;
    for (auto ch : word) {
        size_t ch_index;
        if (ch == 39) ch_index = 26;       // symbol '
        else ch_index = ch - 'a';
        if (node->children[ch_index] == NULL)
            node->children[ch_index] = new TrieNode();
        node = node->children[ch_index];
    }
    node->end_of_word_value += amount;
}

size_t Trie::search(string word) {
    TrieNode* node = root;
    for (auto ch : word) {
        size_t ch_index;
        if (ch == 39) ch_index = 26;       // symbol '
        else ch_index = ch - 'a';
        
        if (node->children[ch_index] == NULL)
            return 0;
        node = node->children[ch_index];
    }
    return node->end_of_word_value;
}