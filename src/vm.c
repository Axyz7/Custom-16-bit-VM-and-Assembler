#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define MEMORY_SIZE 65536 //64kb RAM

//opcodes from ISA spec V1
#define OP_LOAD_VAL 0x01
#define OP_LOAD_MEM 0x02
#define OP_STORE_MEM 0x03
#define OP_MOV_REG  0x04

#define OP_ADD 0x10
#define OP_SUB 0x11
#define OP_CMP 0x12

#define OP_JMP 0x20
#define OP_JEQ 0x21
#define OP_JNE 0x22

#define OP_PUSH 0x30
#define OP_POP  0x31

#define OP_PRINT 0xF0
#define OP_HALT  0xFF

typedef struct{
    uint8_t memory[MEMORY_SIZE];
    uint16_t registers[4];
    uint16_t pc;
    uint16_t sp;
    bool zf;
    bool nf;
    bool is_running;
} VirtualMachine;

void init_vm(VirtualMachine* vm){
    for(int i=0;i<MEMORY_SIZE;i++) vm->memory[i]=0;
    for(int i=0;i<4;i++) vm->registers[i]=0;
    vm->pc=0x0000;
    vm->sp=0xFFFF;
    vm->zf=false;
    vm->is_running=true;
}

uint8_t fetch_byte(VirtualMachine* vm){
    uint8_t data=vm->memory[vm->pc];
    vm->pc++;
    return data;
}

uint16_t fetch_word(VirtualMachine* vm){
    uint8_t low_byte=fetch_byte(vm);
    uint8_t high_byte=fetch_byte(vm);
    return (high_byte<<8)|low_byte;
}

void execute_instruction(VirtualMachine* vm, uint8_t opcode){
    switch(opcode){
        case OP_LOAD_VAL:{
            uint8_t reg=fetch_byte(vm);
            uint16_t val=fetch_word(vm);
            vm->registers[reg]=val;
            break;
        }
        case OP_ADD:{
            uint8_t regA=fetch_byte(vm);
            uint8_t regB=fetch_byte(vm);

            uint16_t result=vm->registers[regA]+vm->registers[regB];
            vm->registers[regA]=result;

            vm->zf=(result==0);
            vm->nf=(result & 0x8000)!=0; //check if highest bit is 1
            break;
        }
        case OP_PRINT:{
            uint8_t reg=fetch_byte(vm);
            printf("PRINT OUTPUT: %d (0x%04X)\n",vm->registers[reg],vm->registers[reg]);
            break;
        }
        case OP_HALT:{
            uint8_t reg=fetch_byte(vm);
            vm->is_running=false;
            break;
        }
        default:{
            printf("ERROR: Unknown Opcode 0x%02X at PC 0x%04X\n", opcode, vm->pc-1);
            vm->is_running=false;
            break;
        }
    }
}

void run_vm(VirtualMachine* vm){
    printf("---Booting Virtual Machine---\n");
    while(vm->is_running && vm->pc<MEMORY_SIZE){
        uint8_t opcode=fetch_byte(vm);
        execute_instruction(vm,opcode);
    }
    printf("---VM Shut Down---\n");
}

