#ifndef VM_H
#define VM_H

#include <stdbool.h>
#include <stdint.h>

#define MEMORY_SIZE 65536

typedef struct {
    uint8_t memory[MEMORY_SIZE];
    uint16_t registers[4];
    uint16_t pc;
    uint16_t sp;
    bool zf;
    bool nf;
    bool is_running;
} VirtualMachine;

// "Function Prototypes" - The Contract
void init_vm(VirtualMachine *vm);


#endif