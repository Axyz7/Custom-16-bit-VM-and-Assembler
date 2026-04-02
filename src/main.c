#include "../include/memory.h"
#include "../include/vm.h"
#include <stdio.h>
#include <string.h>

void load_program(VirtualMachine *vm, const char *filepath) {
    FILE *file = fopen(filepath, "rb");
    if (!file) {
        printf("[FATAL ERROR]: Could not open file: %s\n", filepath);
        exit(1);
    }
    uint16_t address = 0x0000;
    int current_byte;
    while ((current_byte = fgetc(file)) != EOF) {
        write_memory(vm, address++, (uint8_t)current_byte);
    }
    fclose(file);
    printf("[SYSTEM]: Successfully loaded %d bytes.\n", address);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: ./vm run program.bin\n");
        return 1;
    }
    char *mode = argv[1];

    if (strcmp(mode, "run") == 0) {
        printf("[SYSTEM]: Launching RUN MODE...\n");
        VirtualMachine my_vm;
        init_vm(&my_vm);
        load_program(&my_vm, argv[2]); // Load the .bin file
        run_vm(&my_vm);                // Boot the CPU
    } else {
        printf("Error: Unknown command.\n");
    }
    return 0;
}