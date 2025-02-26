#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// Function to remove left recursion
void eliminateLeftRecursion(vector<pair<string, vector<string>>>& productions) {
    vector<pair<string, vector<string>>> result; // Store the updated productions

    for (auto& production : productions) {
        string nonTerminal = production.first;
        vector<string> prod = production.second;

        // Separate left-recursive and non-left-recursive productions
        vector<string> leftRecursive;
        vector<string> nonLeftRecursive;

        for (const string& rule : prod) {
            if (rule[0] == nonTerminal[0]) { // Left-recursive if rule starts with non-terminal
                leftRecursive.push_back(rule.substr(1)); // Remove the first character (non-terminal)
            } else {
                nonLeftRecursive.push_back(rule);
            }
        }

        if (leftRecursive.empty()) {
            // No left recursion, just copy the original production
            result.push_back(production);
        } else {
            // Create new non-terminal (A') for handling recursion
            string newNonTerminal = nonTerminal + "'";

            // Add the non-left-recursive productions with new non-terminal
            for (const string& rule : nonLeftRecursive) {
                result.push_back({nonTerminal, {rule + newNonTerminal}});
            }

            // Add the left-recursive productions with the new non-terminal
            vector<string> newRules;
            for (const string& rule : leftRecursive) {
                newRules.push_back(rule + newNonTerminal);
            }

            // Add the ε production for the new non-terminal
            newRules.push_back("ε");

            // Create new production for the non-terminal A'
            result.push_back({newNonTerminal, newRules});
        }
    }

    // Print the updated grammar
    cout << "Updated Grammar (Without Left Recursion):\n";
    for (const auto& production : result) {
        cout << production.first << " -> ";
        for (const string& rule : production.second) {
            cout << rule << " | ";
        }
        cout << endl;
    }
}

int main() {
    int numNonTerminals;
    cout << "Enter the number of non-terminals: ";
    cin >> numNonTerminals;
    cin.ignore(); // To ignore the newline character after the integer input

    vector<pair<string, vector<string>>> productions;

    // Input the grammar
    for (int i = 0; i < numNonTerminals; ++i) {
        string nonTerminal;
        cout << "Enter the non-terminal name: ";
        getline(cin, nonTerminal);

        int numProductions;
        cout << "Enter the number of productions for " << nonTerminal << ": ";
        cin >> numProductions;
        cin.ignore(); // To ignore the newline character after the integer input

        vector<string> prod;
        cout << "Enter the productions for " << nonTerminal << " (separated by spaces): ";
        for (int j = 0; j < numProductions; ++j) {
            string production;
            getline(cin, production);
            prod.push_back(production);
        }

        productions.push_back({nonTerminal, prod});
    }

    // Display the original grammar
    cout << "\nOriginal Grammar:\n";
    for (const auto& production : productions) {
        cout << production.first << " -> ";
        for (const string& rule : production.second) {
            cout << rule << " | ";
        }
        cout << endl;
    }

    // Eliminate left recursion
    eliminateLeftRecursion(productions);

    return 0;
}