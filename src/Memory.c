#include "../include/vm.h"
#include "../include/Memory.h"
#include <stdbool.h>  // For true/false
#include <stdint.h>   // For uint8_t and uint16_t
#include <stdio.h>    // For printf (error messages)
#include <stdlib.h>   // For exit() if the VM crashes

void write_memory(VirtualMachine *vm, uint32_t address, uint8_t value);
uint8_t read_memory(VirtualMachine *vm, uint32_t address);
void write_mem16(VirtualMachine *vm, uint16_t addr, uint16_t val) ;
uint16_t read_mem16(VirtualMachine *vm, uint16_t addr);

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

void stack_push(VirtualMachine*vm, uint16_t value){
    uint32_t next_sp = vm->sp -2;
    if(next_sp<400){
        printf("[FATAL ERROR]: Stack Overflow");
        exit(1);
    }
    vm->sp = next_sp;
    write_mem16(vm,vm->sp,value);
}

uint16_t stack_pop(VirtualMachine*vm){
   
    if(vm->sp>=0xFFFD){
        printf("[FATAL ERROR]: Stack Underflow");
        exit(1);
    }
    uint16_t value= read_mem16(vm, vm->sp);
    vm->sp +=2;
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