#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <conio.h>
#include "Trie.h"
#include "BKtree.h"

using namespace std;

const string appropriate_chars = "abcdefghijklmnopqrstuvwxyz\r\n\b\t -_(),.:;'!?";
const string punctuation = " -_(),.;:!?";

int main() {
    srand(time(0));

    BKtree bktree;
    Trie trie;

    ifstream input("words.txt");
    while (!input.eof()) {
        string s;
        input >> s;
        trie.insert_many(s, rand() % 100);
        bktree.insert(s);
    }

    string all_input = "";
    string inputPrefix;
    char ch;
    multimap<double, string> final_completes;

    int selectedIndex = -1; // Индекс для выделения текущего предложения

    cout << "Start typing: ";

    while (true) {
        ch = _getch();
        system("cls");

        if (appropriate_chars.find(ch) == -1 && ch != 0 && ch != -32) {
            cout << "Inappropriate char: " << ch << "\nIt was automatically deleted.\n\n";
        }
        else {
            if (ch == '\r' || ch == '\n') { // Enter завершает программу
                cout << "\nEnd line: " << all_input << inputPrefix << '\n';
                break;
            }
            else if (ch == '\b') { // Backspace удаляет символ
                if (!inputPrefix.empty()) inputPrefix.pop_back();
                else if (!all_input.empty()) all_input.pop_back();
                selectedIndex = -1; // Сброс индекса
            }
            else if (ch == '\t') { // TAB выбирает слово
                if (!final_completes.empty() && selectedIndex >= 0) {
                    auto it = final_completes.rbegin();
                    int currentIndex = 0;

                    // Ищем элемент с индексом selectedIndex
                    while (currentIndex < selectedIndex && it != final_completes.rend()) {
                        ++it;
                        ++currentIndex;
                    }

                    if (it != final_completes.rend()) {
                        all_input += it->second + " ";
                        trie.insert(it->second);
                        bktree.insert(it->second);
                        inputPrefix = "";
                        selectedIndex = -1; // Сброс индекса
                    }
                }


            }
            else if (ch == 0 || ch == -32) { // Обработка стрелок
                ch = _getch();
                if (ch == 72) { // Стрелка вверх
                    if (selectedIndex > 0) --selectedIndex;
                }
                else if (ch == 80) { // Стрелка вниз
                    if (selectedIndex < (int)final_completes.size() - 1) ++selectedIndex;
                }
            }
            else if (punctuation.find(ch) != -1) {
                all_input += inputPrefix + ch;
                
                trie.insert(inputPrefix);
                bktree.insert(inputPrefix);
                inputPrefix = "";
                
                selectedIndex = -1;
            }
            else { // Добавление нового символа
                inputPrefix += ch;
                selectedIndex = -1; // Сброс индекса при вводе нового символа
            }
        }

        if (!all_input.empty() && punctuation.find(all_input.back()) == -1) {
            inputPrefix = all_input.substr(all_input.find_last_of(' ') + 1);
            all_input = all_input.substr(0, all_input.find_last_of(' ') + 1);
        }


        cout << "Current input: " << all_input << inputPrefix << '\n';


        // Получаем результаты автодополнения
        multimap<size_t, string> fuzzy_completes = bktree.search(inputPrefix, 1);
        multimap<size_t, string> autocompletes = trie.autocomplete(inputPrefix);

        final_completes.clear();
        double w1 = 1., w2 = 5., avg_frequency = 50.;

        //score = w1 * (frequency / avg_frequency) + w2 * (1 / (distance + 1))

        for (auto it = autocompletes.begin(); it != autocompletes.end(); ++it) {
            int dist = levenshtein_distance(inputPrefix, (*it).second);
            if (dist == 0) continue;
            double score = w1 * ((*it).first / avg_frequency) + w2 * (1. / (dist + 1));
            final_completes.insert({ score, (*it).second });
        }

        for (auto it = fuzzy_completes.begin(); it != fuzzy_completes.end(); ++it) {
            if ((*it).first == 0) continue;
            size_t frequency = trie.search((*it).second);
            double score = w1 * (frequency / avg_frequency) + w2 * (1. / ((*it).first + 1));
            final_completes.insert({ score, (*it).second });
        }

        // Вывод результатов с подсветкой
        size_t i = 0;
        string prev = "";
        int displayIndex = 0;
        for (auto it = final_completes.rbegin(); it != final_completes.rend(); ++it) {
            if ((*it).second == prev) continue;
            prev = (*it).second;

            if (displayIndex == selectedIndex) {
                cout << ">> " << (*it).second << " (" << (*it).first << ")\n";
            }
            else {
                cout << "   " << (*it).second << " (" << (*it).first << ")\n";
            }

            if (++i >= 5) break;
            ++displayIndex;
        }
    }

    trie.save_to_file("tree.txt");
    return 0;
}
