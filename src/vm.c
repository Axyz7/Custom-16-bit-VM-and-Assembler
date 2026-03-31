#include "../include/vm.h"

#include "../include/memory.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void init_vm(VirtualMachine *vm) {
    for (int i = 0; i < MEMORY_SIZE; i++)
        vm->memory[i] = 0;
    for (int i = 0; i < 4; i++)
        vm->registers[i] = 0;
    vm->pc = 0x0000;
    vm->sp = 0xFFFD;  // stack grows downwards, changed the sp intial to down by 2 bytes because so that the stack pointer doesnt wraps around and always keep the pointer at the odd bits, such as 0xFFFD OR 0xFFFB , as suppose there was a pop func called when the sp was at 0xFFFE then the sp willl wrap to 0x0000
    vm->sp = 0xFFFD;  // stack grows downwards, changed the sp intial to down by 2 bytes because so that the stack pointer doesnt wraps around and always keep the pointer at the odd bits, such as 0xFFFD OR 0xFFFB , as suppose there was a pop func called when the sp was at 0xFFFE then the sp willl wrap to 0x0000
    vm->zf = false;
    vm->is_running = true;
}

uint8_t fetch_byte(VirtualMachine *vm) {
    return read_memory(vm,vm->pc++);
}

uint16_t fetch_word(VirtualMachine *vm) {
    uint8_t low_byte = fetch_byte(vm);
    uint8_t high_byte = fetch_byte(vm);
    return (high_byte << 8) | low_byte;  // little-endian
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
            stack_push(vm, vm->registers[reg]);
            stack_push(vm, vm->registers[reg]);
            break;
        }
        case OP_POP: {
            uint8_t reg = fetch_byte(vm);
            vm->registers[reg] = stack_pop(vm);
            vm->registers[reg] = stack_pop(vm);
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
    while (vm->is_running) {
        uint8_t opcode = fetch_byte(vm);
        execute_instruction(vm, opcode);
    }
    printf("---VM Shut Down---\n");
}