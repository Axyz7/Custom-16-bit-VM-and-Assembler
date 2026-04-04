#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <cstdint>
#include <map>
#include <string>
#include <vector>

std::string trim(const std::string &str);
std::vector<std::string> tokenize(std::string &line);
uint16_t getInstructionSize(const std::string &mnemonic);
std::map<std::string, uint16_t> buildSymbolTable(std::vector<std::string> lines);
uint8_t parseRegister(std::string regStr);
void push16bits(std::vector<uint8_t> &binary, uint16_t value);
std::vector<uint8_t> assembleLine(std::vector<std::string> tokens, std::map<std::string, uint8_t> symbols);

#endif  // ASSEMBLER_H