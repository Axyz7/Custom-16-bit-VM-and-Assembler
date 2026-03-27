#ifndef Memory_H
#define Memory_H

#include <stdint.h>
#include <stdbool.h>
#include "vm.h"
uint8_t read_memory(VirtualMachine *vm,uint32_t address);
void write_memory(VirtualMachine *vm,uint32_t address,uint8_t value);


#endif