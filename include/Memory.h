#ifndef Memory_H
#define Memory_H

#include <stdint.h>
#include <stdbool.h>
#include "vm.h"
uint8_t read_memory(VirtualMachine *vm,uint32_t address);
void write_memory(VirtualMachine *vm,uint32_t address,uint8_t value);

void stack_push(VirtualMachine*vm, uint16_t value);
uint16_t stack_pop(VirtualMachine*vm);
void write_mem16(VirtualMachine *vm, uint16_t addr, uint16_t val) ;
uint16_t read_mem16(VirtualMachine *vm, uint16_t addr);

#endif