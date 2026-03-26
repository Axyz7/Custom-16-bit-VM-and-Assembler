#include "../include/vm.h"
#include <stdio.h>    // For printf (error messages)
#include <stdint.h>   // For uint8_t and uint16_t
#include <stdbool.h>  // For true/false
#include <stdlib.h>   // For exit() if the VM crashes

void write_memory(VirtualMachine *vm, uint32_t address,uint8_t value);
uint8_t read_memory(VirtualMachine * vm ,uint32_t address);











void write_memory(VirtualMachine *vm,uint32_t address,uint8_t value){
    if(address>=MEMORY_SIZE){
        printf("[FATAL ERROR]: Segmentation Fault! Attempted write at 0x%04X\n",address);
        exit(1);
    }
    vm-> memory[address] = value;
}



uint8_t read_memory(VirtualMachine *vm,uint32_t address){
    if(address>=MEMORY_SIZE){
        printf("[FATAL ERROR]: Segmentation Fault! Attempted write at 0x%04X\n",address);
        exit(1);
    }
    return vm-> memory[address];
}



