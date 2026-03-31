#include "../include/memory.h"
#include "../include/vm.h"

#include <stdio.h>
#include <string.h>
#include "vm.h"
#include "Memory.h"

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
    } else if (strcmp(mode, "run") == 0) {
        printf("[SYSTEM]: Launching RUN MODE...\n");
        test_cpu_logic();  // <-- THIS IS WHERE WE CONNECT TO ARYAN'S CODE
    } else {
        test_cpu_logic();  // <-- THIS IS WHERE WE CONNECT TO ARYAN'S CODE
    } else {
        printf("Error: Unknown command.\n");
        return 1;
    }

    return 0;
}