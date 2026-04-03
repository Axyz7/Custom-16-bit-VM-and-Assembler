#include "../include/assembler.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

using std::map;
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
    std::map<string, uint16_t> inst_size = {
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
        {"JLT", 3},
        {"LOAD_VAL", 4},
        {"LOAD_MEM", 4},
        {"STORE_MEM", 4},
    };
    
    if (inst_size.find(mnemonic) != inst_size.end()) {
        return inst_size[mnemonic];
    }
    return 0;  // Error case
}

map<string, uint16_t> buildSymbolTable(vector<string> lines) {
    uint16_t address_counter = 0;
    std::map<string, uint16_t> symbol_table;
    vector<string> tokens;

    uint16_t line_no = 1;
    for (auto &line : lines) {
        tokens = tokenize(line);
        size_t first = tokens[0].find_first_of(":");
        if (first != string::npos) {
            symbol_table[tokens[0].substr(0, (first))] = address_counter;
            continue;
        }
        uint16_t inst_addr = getInstructionSize(tokens[0]);
        if (inst_addr == 0) {
            std::cerr
                << "[Fatal Error]: Unknown instruction "
                << tokens[0]
                << " was found at line "
                << line_no << "."
                << std::endl;
            exit(1);
        }
        address_counter += inst_addr;
        line_no++;
    }
    return symbol_table;
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
        exit(1);
    }
    return regcode;  // returns int, unnecessary to change to hex
}

void push16bits(vector<uint8_t> &binary, uint16_t value) {
    // pushes low byte and high byte of a 16 bit value perfectly
    // little endian byte order
    binary.push_back(value & 0xFF);
    binary.push_back((value >> 8) & 0xFF);
}

void assembleLine(vector<uint8_t> binary, vector<string> tokens, map<string, uint16_t> symbols) {
    if (tokens[0] == "HALT") {
        binary.push_back(0xff);  // HALT opcode
    } else if (tokens[0] == "PUSH") {
        binary.push_back(0x30);  // PUSH opcode
        if (tokens.size() <= 1) {
            std::cerr
                << "[Fatal Error]: Expected register after PUSH instruction."
                << std::endl;
            exit(1);
        }
        uint8_t value = std::stoi(tokens[1]);
        binary.push_back(value);
    } else if (tokens[0] == "POP") {
        binary.push_back(0x31);  // POP opcode
        if (tokens.size() <= 1) {
            std::cerr
                << "[Fatal Error]: Expected register after POP instruction."
                << std::endl;
            exit(1);
        }
        uint8_t value = std::stoi(tokens[1]);
        binary.push_back(value);
    } else if (tokens[0] == "PRINT") {
        binary.push_back(0xf0);  // PRINT opcode
        if (tokens.size() <= 1) {
            std::cerr
                << "[Fatal Error]: Expected register after PRINT instruction."
                << std::endl;
            exit(1);
        }
        uint8_t value = std::stoi(tokens[1]);
        binary.push_back(value);
    } else if (tokens[0] == "MOV_REG") {
        binary.push_back(0x04);  // MOV_REG opcode
        if (tokens.size() <= 2) {
            std::cerr
                << "[Fatal Error]: Expected two registers after MOV_REG instruction."
                << std::endl;
            exit(1);
        }
        uint8_t reg = std::stoi(tokens[1]);
        binary.push_back(reg);
        reg = std::stoi(tokens[2]);
        binary.push_back(reg);
    } else if (tokens[0] == "ADD") {
        binary.push_back(0x10);  // ADD opcode
        if (tokens.size() <= 2) {
            std::cerr
                << "[Fatal Error]: Expected register after ADD instruction."
                << std::endl;
            exit(1);
        }
        uint8_t reg = std::stoi(tokens[1]);
        binary.push_back(reg);
        reg = std::stoi(tokens[2]);
        binary.push_back(reg);
    } else if (tokens[0] == "SUB") {
        binary.push_back(0x11);  // SUB opcode
        if (tokens.size() <= 2) {
            std::cerr
                << "[Fatal Error]: Expected register after SUB instruction."
                << std::endl;
            exit(1);
        }
        uint8_t reg = std::stoi(tokens[1]);
        binary.push_back(reg);
        reg = std::stoi(tokens[2]);
        binary.push_back(reg);
    } else if (tokens[0] == "CMP") {
        binary.push_back(0x12);  // CMP opcode
        if (tokens.size() <= 2) {
            std::cerr
                << "[Fatal Error]: Expected register after CMP instruction."
                << std::endl;
            exit(1);
        }
        uint8_t reg = std::stoi(tokens[1]);
        binary.push_back(reg);
        reg = std::stoi(tokens[2]);
        binary.push_back(reg);
    } else if (tokens[0] == "JMP") {
        binary.push_back(0x20);  // JMP opcode
        if (tokens.size() <= 1) {
            std::cerr
                << "[Fatal Error]: Expected 16-bit address after JMP instruction."
                << std::endl;
            exit(1);
        }

        uint16_t addr = symbols[tokens[1]];  // 16-bit address
        push16bits(binary, addr);
    } else if (tokens[0] == "JEQ") {
        binary.push_back(0x21);  // JEQ opcode
        if (tokens.size() <= 1) {
            std::cerr
                << "[Fatal Error]: Expected 16-bit address after JEQ instruction."
                << std::endl;
            exit(1);
        }
        uint16_t addr = symbols[tokens[1]];  // 16-bit address
        push16bits(binary, addr);
    } else if (tokens[0] == "JNE") {
        binary.push_back(0x22);  // JNE opcode
        if (tokens.size() <= 1) {
            std::cerr
                << "[Fatal Error]: Expected 16-bit address after JNE instruction."
                << std::endl;
            exit(1);
        }
        uint16_t addr = symbols[tokens[1]];  // 16-bit address
        push16bits(binary, addr);
    } else if (tokens[0] == "LOAD_VAL") {
        binary.push_back(0x01);  // LOAD_VAL opcode
        if (tokens.size() <= 2) {
            std::cerr
                << "[Fatal Error]: Expected register and value after LOAD_VAL instruction."
                << std::endl;
            exit(1);
        }
        uint8_t reg = std::stoi(tokens[1]);
        binary.push_back(reg);
        uint16_t addr = std::stoi(tokens[2]);  // 16-bit address
        push16bits(binary, addr);
    } else if (tokens[0] == "LOAD_MEM") {
        binary.push_back(0x02);  // LOAD_MEM opcode
        if (tokens.size() <= 2) {
            std::cerr
                << "[Fatal Error]: Expected register and value after LOAD_MEM instruction."
                << std::endl;
            exit(1);
        }
        uint8_t reg = std::stoi(tokens[1]);
        binary.push_back(reg);
        uint16_t addr = std::stoi(tokens[2]);  // 16-bit address
        push16bits(binary, addr);
    } else if (tokens[0] == "STORE_MEM") {
        binary.push_back(0x03);  // STORE_MEM opcode
        if (tokens.size() <= 2) {
            std::cerr
                << "[Fatal Error]: Expected register and value after STORE_MEM instruction."
                << std::endl;
            exit(1);
        }
        uint16_t addr = std::stoi(tokens[1]);  // 16-bit address
        push16bits(binary, addr);
        uint8_t reg = std::stoi(tokens[2]);
        binary.push_back(reg);
    }
}

void saveBinary(string &filename, vector<uint8_t> &binary) {
    // std::ios::binary is CRITICAL here
    std::ofstream outFile(filename, std::ios::out | std::ios::binary);

    if (!outFile) {
        std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
        exit(1);
    }
    outFile.write(reinterpret_cast<const char *>(binary.data()), binary.size());
    outFile.close();
    std::cout << "Successfully wrote " << binary.size() << " bytes to " << filename << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr
            << "[Fatal Error]: Missing source and output file path."
            << std::endl;
        exit(1);
    } else if (argc < 3) {
        std::cerr
            << "[Fatal Error]: Missing output file path."
            << std::endl;
        exit(1);
    }

    std::ifstream is(argv[1]);
    if (!is.is_open()) {
        std::cerr << "Error: " << std::strerror(errno) << std::endl;
        return 1;
    }

    std::string line;
    vector<string> lines;
    while (std::getline(is, line)) {
        if (line.empty()) continue;
        lines.push_back(line);
    }
    map<string, uint16_t> symbols = buildSymbolTable(lines);
    vector<uint8_t> binary;
    for (auto &line : lines) {
        vector<string> tokens = tokenize(line);
        assembleLine(binary, tokens, symbols);
    }
    std::string filename = argv[2];
    saveBinary(filename, binary);
    return 0;
}
