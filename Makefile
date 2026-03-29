# Compiler
C_COMPILER = gcc

# Flags (Compiler Settings)
CFLAGS = -Wall -Wextra -Iinclude

# The Target Executable
VM_TARGET = vm 

# The Source Files inside the src directory
VM_SRCS = src/main.c src/vm.c src/Memory.c
# VM_SRCS = $(wildcard src/*.c)

# Default target
all: $(VM_TARGET)

# Recipe to build the VM
$(VM_TARGET): $(VM_SRCS)
	$(CC) $(CFLAGS) -o $(VM_TARGET) $(VM_SRCS)

# Recipe to clean the directory
clean:
	rm -f $(VM_TARGET)