#include <map>
#include <sstream>

using std::string;
using std::vector;

string trim(const string &str) {
    size_t first = str.find_first_not_of(" \t");
    // fully empty (containing whitespace) string will be trimmed fully
    if (first == string::npos) return "";

    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, (last - first + 1));
}

vector<string> tokenize(string &line) {
    vector<string> tokens;
    if (line.empty()) return tokens;
    size_t first = line.find_first_of(",");
    while (first != string::npos) {
        line.erase(first, 1);
        first = line.find_first_of(",");
    }

    std::stringstream ss(line);
    string token;
    while (getline(ss, token, ' ')) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

void buildSymbolTable(vector<string> lines) {
    size_t total_lines = lines.size();
    uint16_t address_counter = 0;
    std::map<string, uint16_t> symbol_table;
    vector<string> tokens;

    for (auto &line : lines) {
        tokens = tokenize(line);
        size_t first = tokens[0].find_first_of(":");
        if (first != string::npos) {
            symbol_table[tokens[0].substr(0, (first))] = address_counter;
        }
        address_counter += 4;  // 4 bytes per instruction
    }
}