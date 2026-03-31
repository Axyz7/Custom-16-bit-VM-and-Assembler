#include <cstdlib>
#include <iostream>
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

uint16_t getInstructionSize(const string &mnemonic) {
    std::map<string, uint16_t> inst_addr = {
        {"HALT", 1},
        {"PUSH", 2},
        {"POP", 2},
        {"PRINT", 2},
        {"MOV_REG", 3},
        {"ADD", 3},
        {"SUB", 3},
        {"CMP", 3},
        {"JMP", 3},
        {"JEQ", 3},
        {"JNE", 3},
        {"LOAD_VAL", 4},
        {"LOAD_MEM", 4},
        {"STORE_MEM", 4},
    };
    if (inst_addr.find(mnemonic) != inst_addr.end()) {
        return inst_addr[mnemonic];
    }
    return 0;  // Error case
}

void buildSymbolTable(vector<string> lines) {
    size_t total_lines = lines.size();
    uint16_t address_counter = 0;
    std::map<string, uint16_t> symbol_table;
    vector<string> tokens;

    for (auto &line : lines) {
        uint16_t line_no = 1;
        tokens = tokenize(line);
        size_t first = tokens[0].find_first_of(":");
        if (first != string::npos) {
            symbol_table[tokens[0].substr(0, (first))] = address_counter;
        }
        uint16_t inst_addr = getInstructionSize(tokens[0]);
        if (inst_addr == 0) {
            std::cerr
                << "[Fatal Error]: Unknown instruction "
                << tokens[0]
                << " was found at line "
                << line_no << "."
                << std::endl;
            exit(0);
        }
        address_counter += inst_addr;
    }
}

uint8_t parseRegister(string regStr) {
    uint8_t regcode = regStr[1] - '0';
    if (regcode > 3) {
        std::cerr
            << "[Fatal Error]: Unknown register "
            << std::hex
            << regcode
            << " was found."
            << std::endl;
        exit(0);
    }
    return regcode;  // returns int, unnecessary to change to hex
}