#include "../include/memory.h"
#include "../include/vm.h"

#include <stdio.h>
#include <string.h>

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
    // Run it!
    run_vm(&my_vm);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Error: Please type 'run' or 'debug'.\n");
        return 1;
    }

    char *mode = argv[1];

    // Simple Router
    if (strcmp(mode, "debug") == 0) {
        printf("DEBUG MODE: Visual Debugger will go here in Phase 3.\n");
    } else if (strcmp(mode, "run") == 0) {
        printf("[SYSTEM]: Launching RUN MODE...\n");
        test_cpu_logic();  // <-- THIS IS WHERE WE CONNECT TO ARYAN'S CODE
    } else {
        printf("Error: Unknown command.\n");
        return 1;
    }

    return 0;
}