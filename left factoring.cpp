#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <set>

// Function to eliminate left recursion
void eliminateLeftRecursion(std::string nonTerminal, std::vector<std::string>& productions) {
    std::vector<std::string> alpha, beta;

    // Separate the productions into alpha and beta
    for (auto& prod : productions) {
        if (prod[0] == nonTerminal[0]) {  // If the production starts with the non-terminal
            alpha.push_back(prod.substr(1));  // Remove the non-terminal
        } else {
            beta.push_back(prod);
        }
    }

    if (alpha.empty()) {
        return;  // No left recursion, no need to process
    }

    // Create new non-terminal name (e.g., A' for A)
    std::string newNonTerminal = nonTerminal + "'";

    // Modify existing productions
    productions.clear();
    for (auto& b : beta) {
        productions.push_back(b + newNonTerminal);
    }

    // Create new productions for the new non-terminal
    productions.push_back("");  // Add epsilon production for the new non-terminal
    for (auto& a : alpha) {
        productions.push_back(a + newNonTerminal);
    }

    // Print the updated grammar
    std::cout << "After Left Recursion Elimination for " << nonTerminal << ":\n";
    for (auto& prod : productions) {
        std::cout << nonTerminal << " -> " << prod << std::endl;
    }
}

// Function to perform Left Factoring
void leftFactor(std::string nonTerminal, std::vector<std::string>& productions) {
    // A map to store prefixes
    std::unordered_map<std::string, std::vector<std::string>> prefixMap;

    // Group productions by their common prefix
    for (auto& prod : productions) {
        std::string prefix = prod.substr(0, 1);  // Take the first character as the prefix
        prefixMap[prefix].push_back(prod);
    }

    // Apply left factoring
    std::vector<std::string> newProductions;
    for (auto& entry : prefixMap) {
        if (entry.second.size() > 1) {
            std::string commonPrefix = entry.first;
            std::string newNonTerminal = nonTerminal + "'";

            // Add the common prefix as the new production
            newProductions.push_back(nonTerminal + " -> " + commonPrefix + newNonTerminal);

            // Add remaining parts for each production
            for (auto& prod : entry.second) {
                if (prod != commonPrefix) {
                    newProductions.push_back(newNonTerminal + " -> " + prod.substr(commonPrefix.size()));
                }
            }
        } else {
            newProductions.push_back(nonTerminal + " -> " + entry.second[0]);
        }
    }

    // Print the updated grammar after left factoring
    std::cout << "\nAfter Left Factoring for " << nonTerminal << ":\n";
    for (auto& prod : newProductions) {
        std::cout << prod << std::endl;
    }
}

int main() {
    std::string nonTerminal;
    int numProductions;

    // Input: Non-terminal
    std::cout << "Enter the non-terminal (e.g., A): ";
    std::cin >> nonTerminal;

    // Input: Number of productions
    std::cout << "Enter the number of productions for " << nonTerminal << ": ";
    std::cin >> numProductions;

    // Input: Productions
    std::vector<std::string> productions;
    std::cout << "Enter the productions (one per line):\n";
    std::cin.ignore();  // To ignore the newline character left in the buffer
    for (int i = 0; i < numProductions; ++i) {
        std::string prod;
        std::getline(std::cin, prod);
        productions.push_back(prod);
    }

    // Print original productions
    std::cout << "\nOriginal productions for " << nonTerminal << ":\n";
    for (auto& prod : productions) {
        std::cout << nonTerminal << " -> " << prod << std::endl;
    }

    // Eliminate Left Recursion
    eliminateLeftRecursion(nonTerminal, productions);

    // Apply Left Factoring
    leftFactor(nonTerminal, productions);

    return 0;
}