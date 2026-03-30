#ifndef VM_H
#define VM_H

#include <stdbool.h>
#include <stdint.h>

#define MEMORY_SIZE 65536

typedef struct {
    uint8_t memory[MEMORY_SIZE];
    uint16_t registers[4];
    uint32_t pc;// changed pc type ,as we want the pc to actually be able to reach or exceed the memory limit to trigger a stop,
    uint16_t sp;
    bool zf;
    bool nf;
    bool is_running;
} VirtualMachine;

// "Function Prototypes" - The Contract
void init_vm(VirtualMachine *vm);
void test_cpu_logic(); // <-- ADD THIS LINE
#endif