#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <cstdint>
#include <string>
#include <vector>

std::string trim(const std::string &str);
std::vector<std::string> tokenize(std::string &line);
uint16_t getInstructionSize(const std::string &mnemonic);
void buildSymbolTable(std::vector<std::string> lines);
uint8_t parseRegister(std::string regStr);
void push16bits(std::vector<uint8_t> &binary, uint16_t value);

#endif  // ASSEMBLER_H