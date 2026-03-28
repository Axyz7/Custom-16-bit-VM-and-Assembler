#include <stdio.h>
#include <string.h>
#include "vm.h"
#include "Memory.h"

int main(int argc, char *argv[]) {
    // 1. Check if the user provided enough arguments
    if (argc < 3) {
        printf("Usage: ./vm <mode> <filename>\n");
        printf("Modes: run, debug\n");
        printf("Example: ./vm debug program.bin\n");
        return 1;
    }

    // 2. Capture the mode and the file name
    char *mode = argv[1];
    char *filename = argv[2];

    // 3. Route to the correct logic using string comparison (strcmp)
    if (strcmp(mode, "debug") == 0) {
        printf("[SYSTEM]: Launching VM in DEBUG mode with file: %s\n", filename);
        // We will call your step-by-step visual debugger function here later
    } 
    else if (strcmp(mode, "run") == 0) {
        printf("[SYSTEM]: Launching VM in RUN mode with file: %s\n", filename);
        // We will call the standard execution loop here later
    } 
    else {
        printf("[ERROR]: Unknown mode '%s'. Use 'run' or 'debug'.\n", mode);
        return 1;
    }

    return 0;
}