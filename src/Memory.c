#include "vm.h"
#include <stdio.h>    // For printf (error messages)
#include <stdint.h>   // For uint8_t and uint16_t
#include <stdbool.h>  // For true/false
#include <stdlib.h>   // For exit() if the VM crashes

void write_memory(uint32_t address,uint8_t value);
uint8_t read_memory(uint32_t address);









int main(void){
    VirtualMachine MyVm;
    init_vm(&MyVm);
    printf("-----VM--Memory--------\n");

    uint16_t test_adr = 0x4321; //Decimal: 17185
    uint8_t test_val = 0xAB; //Decimal: 171

    printf("Writing 0x%X to the address 0x%04X\n",test_val,test_adr);
    write_memory(&MyVm,test_adr,test_val);

    uint8_t retrieved = read_memory(&MyVm,test_adr);
    printf("retrieved value : 0x%X\n",retrieved);

    if(retrieved == test_val){
        printf("SUCCESS: Memory Integrity Verified!\n");
    }
    else{
        printf("ERROR: Memory Mismatch!\n");
    }
    return 0;
}

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



