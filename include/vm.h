#ifndef VM_H
#define VM_H

#include <stdbool.h>
#include <stdint.h>

#define MEMORY_SIZE 65536

typedef struct {
    uint8_t memory[MEMORY_SIZE];
    uint16_t registers[4];
    uint32_t pc;  // changed pc type ,as we want the pc to actually be able to reach or exceed the memory limit to trigger a stop,
    uint16_t sp;
    bool zf;
    bool nf;
    bool is_running;
} VirtualMachine;

// opcodes from ISA spec V1
#define OP_LOAD_VAL 0x01
#define OP_LOAD_MEM 0x02
#define OP_STORE_MEM 0x03
#define OP_MOV_REG 0x04
#define OP_ADD 0x10
#define OP_SUB 0x11
#define OP_CMP 0x12
#define OP_JMP 0x20
#define OP_JEQ 0x21
#define OP_JNE 0x22
#define OP_PUSH 0x30
#define OP_POP 0x31
#define OP_PRINT 0xF0
#define OP_HALT 0xFF

// "Function Prototypes" - The Contract
void init_vm(VirtualMachine *vm);

uint8_t fetch_byte(VirtualMachine *vm);
uint16_t fetch_word(VirtualMachine *vm);
void write_mem16(VirtualMachine *vm, uint16_t addr, uint16_t val) ;
uint16_t read_mem16(VirtualMachine *vm, uint16_t addr);
void execute_instruction(VirtualMachine *vm, uint8_t opcode);
void run_vm(VirtualMachine *vm);

#endif