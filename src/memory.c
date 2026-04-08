#include "../include/memory.h"

#include "../include/vm.h"

#include <stdbool.h>  // For true/false
#include <stdint.h>   // For uint8_t and uint16_t
#include <stdio.h>    // For printf (error messages)
#include <stdlib.h>   // For exit() if the VM crashes

void write_memory(VirtualMachine *vm, uint32_t address, uint8_t value) {
    if (address >= MEMORY_SIZE) {
        printf("[FATAL ERROR]: Segmentation Fault! Attempted write at 0x%04X\n", address);
        exit(1);
    }
    vm->memory[address] = value;
}

uint8_t read_memory(VirtualMachine *vm, uint32_t address) {
    if (address >= MEMORY_SIZE) {
        printf("[FATAL ERROR]: Segmentation Fault! Attempted write at 0x%04X\n", address);
        exit(1);
    }
    return vm->memory[address];
}

void stack_push(VirtualMachine *vm, uint16_t value) {
    uint32_t next_sp = vm->sp - 2;
    if (next_sp < STACK_LIMIT || next_sp > 0xFFFF) {
        printf("[FATAL ERROR]: Stack Overflow");
        exit(1);
    }
    // we have to convert it into 32 bits beacuse suppose sp is 1 and subracting 2 will make it a very large number causing crash but if it is 32 bit the condition above will still turn to true and support our safe feature
    vm->sp = (uint16_t) next_sp;  // though the calculation is done in 32 bits for data type safety
    // if we try to fit the 32 bit number directly to a 16 bit number then there is possiblity of error by compiler : possible loss of data , by writing uint16t we are telling the compiler that thisa number fits within 16 bits so you dont have to worry about the data loss
    write_mem16(vm, vm->sp, value);
}

uint16_t stack_pop(VirtualMachine *vm) {
    if (vm->sp >= 0xFFFD) {
        printf("[FATAL ERROR]: Stack Underflow");
        exit(1);
    }
    uint16_t value = read_mem16(vm, vm->sp);
    vm->sp += 2;
    return value;
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

void dump_memory(VirtualMachine *vm, uint16_t start_address, uint16_t size) {
    for (uint32_t i = 0; i < size; i++) {
        uint32_t current_addr = (uint32_t) start_address + i;  // making i a 32 bit integer is also a safety feature, the uint32 explicit coversion for start address wasnt neccesary as the ocmputer would have promoted it even without it cause i is a 32 bit integer
        if (i % 8 == 0) {
            printf("\n0x%04X: ", (uint16_t) current_addr);  // the conversion specification expects a hexadecimal , this conversion will not lead to data loss as the intial bits were only leading zeroes
        }
        printf("%02X ", read_memory(vm, current_addr));
    }
    printf("\n\n---------------------------------------\n\n");
}

/***1> why the 16 bit clock fails :
cause suppose the next_sp is 1 and then we subracted 2 from it then it wraps around and turns to 0xFFFF and furhter on again push now it will pass the test and lets the crash happen.
2. the 32 bit safet check . we added another safety check which says if the next_sp is > 0xFFFF , cause imagine the same case as above , now when it wraps the next_sp becomes very large as 4.2 billion and the condition that is newly added  make it true and then the crash doesnt happen
3. when we add the uint16_t start_address to i which is 32 bit the computer automatically adds leading zero to the start_address and this ensures no errors while maintaining the same number .
4.suppose if we have i a 16 bit integer then and lets say we start the address from 0 then when i reaches = size it wraps itself to zero and the loop continues forever , so converting i into 32 bit makes i = 0x00010000 when i = size so now when current addr is passed to the read memory it will be safe. ***/