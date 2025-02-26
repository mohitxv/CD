#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

map<char, set<char>> firstSets, followSets;
map<char, vector<string>> productions;
set<char> nonTerminals, terminals;
char startSymbol;

// Function to check if a character is a terminal
bool isTerminal(char c) {
    return !isupper(c) && c != 'ε';
}

// Compute FIRST set for a non-terminal
set<char> computeFirst(char symbol) {
    if (firstSets.count(symbol)) return firstSets[symbol];

    set<char> &first = firstSets[symbol];

    for (auto &prod : productions[symbol]) {
        if (prod == "ε") {
            first.insert('ε');
            continue;
        }
        for (char c : prod) {
            if (isTerminal(c)) {
                first.insert(c);
                break;
            } else {
                set<char> firstOfC = computeFirst(c);
                first.insert(firstOfC.begin(), firstOfC.end());
                first.erase('ε');
                if (firstOfC.find('ε') == firstOfC.end()) break;
            }
        }
    }
    return first;
}

// Compute FOLLOW set for a non-terminal
set<char> computeFollow(char symbol) {
    if (followSets.count(symbol)) return followSets[symbol];

    set<char> &follow = followSets[symbol];
    if (symbol == startSymbol) follow.insert('$');

    for (auto &p : productions) {
        char lhs = p.first;
        for (auto &rhs : p.second) {
            for (size_t i = 0; i < rhs.size(); i++) {
                if (rhs[i] == symbol) {
                    size_t j = i + 1;
                    while (j < rhs.size()) {
                        if (isTerminal(rhs[j])) {
                            follow.insert(rhs[j]);
                            break;
                        } else {
                            set<char> firstOfNext = computeFirst(rhs[j]);
                            follow.insert(firstOfNext.begin(), firstOfNext.end());
                            follow.erase('ε');
                            if (firstOfNext.find('ε') == firstOfNext.end()) break;
                        }
                        j++;
                    }
                    if (j == rhs.size() && lhs != symbol) {
                        set<char> followOfLhs = computeFollow(lhs);
                        follow.insert(followOfLhs.begin(), followOfLhs.end());
                    }
                }
            }
        }
    }
    return follow;
}

void displaySets(const map<char, set<char>> &sets, const string &setName) {
    cout << "\n" << setName << " sets:\n";
    for (auto &entry : sets) {
        cout << setName << "(" << entry.first << ") = { ";
        for (char c : entry.second) cout << c << " ";
        cout << "}\n";
    }
}

void constructParsingTable() {
    cout << "\nPredictive Parsing Table:\n";
    for (auto &p : productions) {
        char lhs = p.first;
        for (auto &rhs : p.second) {
            set<char> firstSet;
            if (rhs == "ε") firstSet = followSets[lhs];
            else {
                for (char c : rhs) {
                    if (isTerminal(c)) {
                        firstSet.insert(c);
                        break;
                    } else {
                        set<char> temp = firstSets[c];
                        firstSet.insert(temp.begin(), temp.end());
                        if (temp.find('ε') == temp.end()) break;
                    }
                }
            }
            for (char terminal : firstSet) {
                if (terminal != 'ε')
                    cout << "M[" << lhs << ", " << terminal << "] = " << lhs << " -> " << rhs << "\n";
            }
            if (firstSet.find('ε') != firstSet.end()) {
                for (char followSymbol : followSets[lhs]) {
                    cout << "M[" << lhs << ", " << followSymbol << "] = " << lhs << " -> ε\n";
                }
            }
        }
    }
}

int main() {
    int n;
    cout << "Enter the number of productions: ";
    cin >> n;
    cout << "Enter productions (e.g., E->TR):\n";

    for (int i = 0; i < n; i++) {
        string input;
        cin >> input;
        char lhs = input[0];
        string rhs = input.substr(3); // Skip "->"

        if (i == 0) startSymbol = lhs;
        productions[lhs].push_back(rhs);
        nonTerminals.insert(lhs);

        for (char c : rhs) {
            if (isTerminal(c) && c != 'ε') terminals.insert(c);
            else if (isupper(c)) nonTerminals.insert(c);
        }
    }

    // Compute FIRST and FOLLOW sets
    for (char nt : nonTerminals) computeFirst(nt);
    for (char nt : nonTerminals) computeFollow(nt);

    displaySets(firstSets, "FIRST");
    displaySets(followSets, "FOLLOW");
    constructParsingTable();

    return 0;
}