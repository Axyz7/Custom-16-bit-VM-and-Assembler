# **16-Bit Custom Virtual Machine & Assembler**

A custom 16-bit Virtual Machine (CPU emulator) and a corresponding Two-Pass Assembler built entirely from scratch. This project demonstrates bottom-up systems engineering, memory management, and compiler theory.  
The Virtual Machine is written in pure **C** to emphasize low-level memory manipulation, while the Assembler is written in **C++** to leverage the Standard Template Library (STL) for fast lexical analysis and symbol table generation.

## **🧠 System Architecture**

* **Memory Structure:** 64 KB (65,536 bytes) of simulated byte-addressable RAM.  
* **Registers:** 4 General Purpose 16-bit Registers (R0, R1, R2, R3).  
* **Endianness:** Little-Endian data storage.  
* **Hardware Flags:** Zero Flag (ZF) and Negative Flag (NF) for conditional branching.  
* **Stack:** 16-bit stack that grows downwards, originating at 0xFFFF.

## **⚙️ Build Instructions**

This project uses a standard Makefile for streamlined compilation.  
\# Clone the repository  
git clone \[https://github.com/Axyz7/Custom-16-bit-VM-and-Assembler.git\](https://github.com/Axyz7/Custom-16-bit-VM-and-Assembler.git)  
cd Custom-16-bit-VM-and-Assembler

\# Compile both the VM and the Assembler  
make all

\# Clean compiled binaries  
make clean

## **🚀 Running a Program**

1. Write your assembly code in a .asm file.  
2. Compile the text file into raw binary using the assembler.  
3. Execute the binary using the virtual machine.

./assembler test.asm   \# Generates program.bin  
./vm program.bin       \# Boots the VM and runs the binary

## **📜 Instruction Set Architecture (ISA)**

Our custom CPU supports 13 distinct instructions.

| Hex | Mnemonic | Arguments | Description |
| :---- | :---- | :---- | :---- |
| 0x01 | LOAD\_VAL | reg, val | Loads a 16-bit literal value into a register. |
| 0x02 | LOAD\_MEM | reg, addr | Loads a 16-bit value from a memory address into a register. |
| 0x03 | STORE\_MEM | addr, reg | Stores a 16-bit register value into a memory address. |
| 0x04 | MOV\_REG | regA, regB | Copies the value from regB into regA. |
| 0x10 | ADD | regA, regB | Adds regA and regB, stores result in regA. Updates ZF/NF. |
| 0x11 | SUB | regA, regB | Subtracts regB from regA, stores result in regA. Updates ZF/NF. |
| 0x12 | CMP | regA, regB | Compares regA and regB. Updates ZF/NF without storing result. |
| 0x20 | JMP | addr | Unconditional jump to a memory address. |
| 0x21 | JEQ | addr | Jumps to address if the Zero Flag (ZF) is true (Equal). |
| 0x22 | JNE | addr | Jumps to address if the Zero Flag (ZF) is false (Not Equal). |
| 0x30 | PUSH | reg | Pushes a 16-bit register value onto the stack. |
| 0x31 | POP | reg | Pops a 16-bit value off the stack into a register. |
| 0xF0 | PRINT | reg | Prints the integer and hex value of a register to standard output. |
| 0xFF | HALT | *none* | Safely halts the CPU execution. |

## **👨‍💻 Development Team**

* **Aryan** \- System Architect & Core VM Engine  
* **Dhananjay** \- Assembler Lexer & Binary Generator  
* **Guru** \- Memory Management & Stack Logic  
* **Hamesh** \- Build Systems & File I/O  
* **Aditya** \- Assembly Programming & QA Testing