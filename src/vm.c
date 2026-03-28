// #include "../include/vm.h"
#include "vm.h"
#include "../include/Memory.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MEMORY_SIZE 65536  // 64kb RAM

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

void init_vm(VirtualMachine *vm) {
    for (int i = 0; i < MEMORY_SIZE; i++)
        vm->memory[i] = 0;
    for (int i = 0; i < 4; i++)
        vm->registers[i] = 0;
    vm->pc = 0x0000;
    vm->sp = 0xFFFF;  // stack grows downwards
    vm->zf = false;
    vm->is_running = true;
}

uint8_t fetch_byte(VirtualMachine *vm) {
    return vm->memory[vm->pc++];
}

uint16_t fetch_word(VirtualMachine *vm) {
    uint8_t low_byte = fetch_byte(vm);
    uint8_t high_byte = fetch_byte(vm);
    return (high_byte << 8) | low_byte;  // little-endian
}

void write_mem16(VirtualMachine *vm, uint16_t addr, uint16_t val) {
    vm->memory[addr] = val & 0xFF;      // low byte
    vm->memory[addr + 1] = (val >> 8);  // high byte
}

uint16_t read_mem16(VirtualMachine *vm, uint16_t addr) {
    uint8_t low = vm->memory[addr];
    uint8_t high = vm->memory[addr + 1];
    return (high << 8) | low;
}

void execute_instruction(VirtualMachine *vm, uint8_t opcode) {
    switch (opcode) {
        case OP_LOAD_VAL: {
            uint8_t reg = fetch_byte(vm);
            uint16_t val = fetch_word(vm);
            vm->registers[reg] = val;
            break;
        }
        case OP_LOAD_MEM: {
            uint8_t reg = fetch_byte(vm);
            uint16_t addr = fetch_word(vm);
            vm->registers[reg] = read_mem16(vm, addr);
            break;
        }
        case OP_STORE_MEM: {
            uint16_t addr = fetch_word(vm);
            uint8_t reg = fetch_byte(vm);
            write_mem16(vm, addr, vm->registers[reg]);
            break;
        }
        case OP_MOV_REG: {
            uint8_t regA = fetch_byte(vm);
            uint8_t regB = fetch_byte(vm);
            vm->registers[regA] = vm->registers[regB];
            break;
        }
        case OP_ADD: {
            uint8_t regA = fetch_byte(vm);
            uint8_t regB = fetch_byte(vm);

            uint16_t result = vm->registers[regA] + vm->registers[regB];
            vm->registers[regA] = result;

            vm->zf = (result == 0);
            vm->nf = (result & 0x8000) != 0;  // check if highest bit is 1
            break;
        }
        case OP_SUB: {
            uint8_t regA = fetch_byte(vm);
            uint8_t regB = fetch_byte(vm);
            uint16_t res = vm->registers[regA] - vm->registers[regB];
            vm->registers[regA] = res;
            vm->zf = (res == 0);
            vm->nf = (res & 0x8000) != 0;
            break;
        }
        case OP_CMP: {
            uint8_t regA = fetch_byte(vm);
            uint8_t regB = fetch_byte(vm);
            uint16_t res = vm->registers[regA] - vm->registers[regB];
            vm->zf = (res == 0);
            vm->nf = (res & 0x8000) != 0;
            break;
        }
        case OP_JMP: {
            uint16_t addr = fetch_word(vm);
            vm->pc = addr;
            break;
        }
        case OP_JEQ: {
            uint16_t addr = fetch_word(vm);
            if (vm->zf) vm->pc = addr;
            break;
        }
        case OP_JNE: {
            uint16_t addr = fetch_word(vm);
            if (!vm->zf) vm->pc = addr;
            break;
        }
        case OP_PUSH: {
            uint8_t reg = fetch_byte(vm);
            vm->sp -= 2;
            write_mem16(vm, vm->sp, vm->registers[reg]);
            break;
        }
        case OP_POP: {
            uint8_t reg = fetch_byte(vm);
            vm->registers[reg] = read_mem16(vm, vm->sp);
            vm->sp += 2;
            break;
        }
        case OP_PRINT: {
            uint8_t reg = fetch_byte(vm);
            printf("PRINT OUTPUT: %d (0x%04X)\n", vm->registers[reg], vm->registers[reg]);
            break;
        }
        case OP_HALT: {
            vm->is_running = false;
            break;
        }
        default: {
            printf("FATAL ERROR: Unknown Opcode 0x%02X at PC 0x%04X\n", opcode, vm->pc - 1);
            vm->is_running = false;
            break;
        }
    }
}

void run_vm(VirtualMachine *vm) {
    printf("---Booting Virtual Machine---\n");
    while (vm->is_running && vm->pc < MEMORY_SIZE) {
        uint8_t opcode = fetch_byte(vm);
        execute_instruction(vm, opcode);
    }
    printf("---VM Shut Down---\n");
}
// for test
// int main() 
void test_cpu_logic(){
    VirtualMachine my_vm;
    init_vm(&my_vm);

    // Hardcoding a test program into memory:
    // LOAD_VAL R1, 15 | LOAD_VAL R2, 10 | SUB R1, R2 | PRINT R1 | HALT

    // 1. LOAD_VAL R1, 15 (0x0F)
    my_vm.memory[0] = OP_LOAD_VAL;
    my_vm.memory[1] = 0x01;  // R1
    my_vm.memory[2] = 0x0F;  // Low byte of 15
    my_vm.memory[3] = 0x00;  // High byte of 15

    // 2. LOAD_VAL R2, 10 (0x0A)
    my_vm.memory[4] = OP_LOAD_VAL;
    my_vm.memory[5] = 0x02;  // R2
    my_vm.memory[6] = 0x0A;  // Low byte of 10
    my_vm.memory[7] = 0x00;  // High byte of 10

    // 3. SUB R1, R2
    my_vm.memory[8] = OP_SUB;
    my_vm.memory[9] = 0x01;   // R1
    my_vm.memory[10] = 0x02;  // R2

    // 4. PRINT R1
    my_vm.memory[11] = OP_PRINT;
    my_vm.memory[12] = 0x01;  // R1

    // 5. HALT
    my_vm.memory[13] = OP_HALT;

    VirtualMachine MyVm;
    init_vm(&MyVm);
    printf("-----VM--Memory--------\n");

    uint16_t test_adr = 0x4321;  // Decimal: 17185
    uint8_t test_val = 0xAB;     // Decimal: 171

    printf("Writing 0x%X to the address 0x%04X\n", test_val, test_adr);
    write_memory(&MyVm, test_adr, test_val);

    uint8_t retrieved = read_memory(&MyVm, test_adr);
    printf("retrieved value : 0x%X\n", retrieved);

    if (retrieved == test_val) {
        printf("SUCCESS: Memory Integrity Verified!\n");
    } else {
        printf("ERROR: Memory Mismatch!\n");
    }

    // Run it!
    run_vm(&my_vm);

    // return 0;
}