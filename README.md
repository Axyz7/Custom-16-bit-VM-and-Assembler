# 16-Bit Custom Virtual Machine & Assembler

> A custom 16-bit CPU emulator and a two-pass assembler built entirely from scratch — demonstrating bottom-up systems engineering, memory management, and compiler theory.

![VM: C](https://img.shields.io/badge/VM-C-blue?style=flat-square&logo=c)
![Assembler: C++](https://img.shields.io/badge/Assembler-C%2B%2B-00599C?style=flat-square&logo=cplusplus)
![Architecture: 16-bit](https://img.shields.io/badge/Architecture-16--bit-green?style=flat-square)
![Build: Makefile](https://img.shields.io/badge/Build-Makefile-lightgrey?style=flat-square)

---

## Table of Contents

- [Overview](#overview)
- [System Architecture](#system-architecture)
- [Project Structure](#project-structure)
- [Build Instructions](#build-instructions)
- [Running a Program](#running-a-program)
- [Instruction Set Architecture (ISA)](#instruction-set-architecture-isa)
- [Assembly Language Guide](#assembly-language-guide)
- [How the Two-Pass Assembler Works](#how-the-two-pass-assembler-works)
- [Development Team](#development-team)

---

## Overview

This project is a fully functional simulation of a 16-bit computer system, comprising two core components:

- **Virtual Machine (`vm.c`, `memory.c`):** Written in pure **C** to emphasize low-level memory manipulation and direct pointer arithmetic. It implements a fetch-decode-execute loop that reads raw binary programs from disk, loads them into a 64 KB simulated RAM, and executes them instruction by instruction.

- **Two-Pass Assembler (`assembler.cc`):** Written in **C++**, leveraging the STL (`std::map`, `std::vector`, `std::stringstream`) for lexical analysis, symbol table construction, and binary code emission. It translates human-readable `.asm` source files into raw binary programs the VM can execute directly.

Together, they form a complete toolchain — write assembly, assemble to binary, run on the VM — built from scratch with no external libraries.

---

## System Architecture

| Component | Detail |
| :--- | :--- |
| **Memory** | 64 KB (65,536 bytes) of byte-addressable RAM (`uint8_t memory[65536]`) |
| **Registers** | 4 general-purpose 16-bit registers: `R0`, `R1`, `R2`, `R3` |
| **Program Counter** | `uint32_t pc` — intentionally 32-bit so it can exceed `0xFFFF` to signal end-of-program naturally |
| **Stack Pointer** | `uint16_t sp` — initialized to `0xFFFD`, grows downward |
| **Stack Limit** | `0x4000` — pushing below this address triggers a Stack Overflow halt |
| **Endianness** | Little-endian (low byte stored at the lower address) |
| **Zero Flag (`zf`)** | Set when the result of `ADD`, `SUB`, or `CMP` is zero |
| **Negative Flag (`nf`)** | Set when bit 15 of the result of `ADD`, `SUB`, or `CMP` is `1` |

### Why `pc` is `uint32_t`

The program counter is deliberately typed as `uint32_t`. A `uint16_t` PC would wrap around to `0x0000` after reaching `0xFFFF`, causing an infinite loop on programs that run to the end of memory. By using 32 bits, the PC can exceed `0xFFFF`, and the run loop detects this as a natural stop condition — no explicit `HALT` required in that edge case.

### Stack Layout

```
0xFFFF  ← top of addressable RAM
  ...   ← stack grows downward
0xFFFD  ← initial SP (word-aligned; two bytes reserved at top)
  ...
0x4000  ← STACK_LIMIT (stack overflow boundary)
  ...
0x0000  ← program binary loaded here
```

The SP starts at `0xFFFD` rather than `0xFFFF` to keep the stack word-aligned on even boundaries (`0xFFFD`, `0xFFFB`, …). This prevents the SP from wrapping to `0x0000` on an erroneous `POP` when the stack is empty.

---

## Project Structure

```
Custom-16-bit-VM-and-Assembler/
│
├── include/
│   ├── vm.h            # VirtualMachine struct, all opcode #defines, function prototypes
│   ├── memory.h        # Memory read/write, stack op prototypes, STACK_LIMIT define
│   └── assembler.h     # Assembler function prototypes
│
├── src/
│   ├── main.c          # VM entry point: parses CLI args, loads .bin, boots VM
│   ├── vm.c            # fetch_byte/word, execute_instruction, run_vm loop
│   ├── memory.c        # read/write_memory, stack_push/pop, read/write_mem16, dump_memory
│   └── assembler.cc    # Full two-pass assembler: cleanup, tokenize, symbol table, binary emit
│
├── input/
│   └── basic_sum.asm   # Included example program
│
├── Makefile            # Builds the VM via gcc
└── README.md
```

---

## Build Instructions

### Prerequisites

- `gcc` — to compile the VM
- `g++` — to compile the Assembler
- `make` — to use the Makefile

### Build the VM

```bash
git clone https://github.com/Axyz7/Custom-16-bit-VM-and-Assembler.git
cd Custom-16-bit-VM-and-Assembler

make all        # compiles src/main.c, src/vm.c, src/memory.c → ./vm
make clean      # removes the ./vm binary
```

### Build the Assembler

The assembler is compiled separately with `g++`:

```bash
g++ -Wall -Wextra -Iinclude -o assembler src/assembler.cc
```

---

## Running a Program

The **assembler** takes two arguments: the `.asm` source file and the desired output `.bin` filename.

```bash
./assembler input/basic_sum.asm program.bin
```

The **VM** takes the `run` subcommand followed by the binary path:

```bash
./vm run program.bin
```

### Example Output

Running the included `basic_sum.asm` (loads 10 into R1, loads 20 into R2, adds them, prints R1):

```
[SYSTEM]: Launching RUN MODE...
[SYSTEM]: Successfully loaded 11 bytes.
---Booting Virtual Machine---
PRINT OUTPUT: 30 (0x001E)
---VM Shut Down---
```

---

## Instruction Set Architecture (ISA)

The CPU supports **15 instructions**. All multi-byte values are encoded in little-endian order.

### Instruction Encoding Formats

| Format | Total Size | Example |
| :--- | :--- | :--- |
| `opcode` | 1 byte | `HALT` |
| `opcode` + `reg` | 2 bytes | `PUSH R0` |
| `opcode` + `regA` + `regB` | 3 bytes | `ADD R0, R1` |
| `opcode` + `addr16` | 3 bytes | `JMP loop` |
| `opcode` + `reg` + `imm16` | 4 bytes | `LOAD_VAL R0, 42` |
| `opcode` + `addr16` + `reg` | 4 bytes | `STORE_MEM 0x1000, R2` |

Registers are encoded as single bytes: `R0 = 0x00`, `R1 = 0x01`, `R2 = 0x02`, `R3 = 0x03`.

---

### Data Transfer

| Opcode | Mnemonic | Arguments | Size | Description |
| :--- | :--- | :--- | :--- | :--- |
| `0x01` | `LOAD_VAL` | `reg, imm16` | 4 bytes | Loads a 16-bit immediate value into a register. |
| `0x02` | `LOAD_MEM` | `reg, addr16` | 4 bytes | Reads the 16-bit value at `addr` from RAM into a register. |
| `0x03` | `STORE_MEM` | `addr16, reg` | 4 bytes | Writes a register's 16-bit value into RAM at `addr`. |
| `0x04` | `MOV_REG` | `regDst, regSrc` | 3 bytes | Copies the value of `regSrc` into `regDst`. |

### Arithmetic & Logic

| Opcode | Mnemonic | Arguments | Size | Description |
| :--- | :--- | :--- | :--- | :--- |
| `0x10` | `ADD` | `regA, regB` | 3 bytes | `regA = regA + regB`. Updates `zf` and `nf`. |
| `0x11` | `SUB` | `regA, regB` | 3 bytes | `regA = regA - regB`. Updates `zf` and `nf`. |
| `0x12` | `CMP` | `regA, regB` | 3 bytes | Computes `regA - regB`, updates `zf`/`nf`, discards result. |

### Control Flow

| Opcode | Mnemonic | Arguments | Size | Description |
| :--- | :--- | :--- | :--- | :--- |
| `0x20` | `JMP` | `addr16` | 3 bytes | Unconditional jump: `pc = addr`. |
| `0x21` | `JEQ` | `addr16` | 3 bytes | Jump if `zf == true` (result was zero / operands were equal). |
| `0x22` | `JNE` | `addr16` | 3 bytes | Jump if `zf == false` (result was non-zero / operands not equal). |
| `0x23` | `JLT` | `addr16` | 3 bytes | Jump if `nf == true` (result was negative / `regA < regB`). |

### Stack Operations

| Opcode | Mnemonic | Arguments | Size | Description |
| :--- | :--- | :--- | :--- | :--- |
| `0x30` | `PUSH` | `reg` | 2 bytes | Decrements `sp` by 2, writes register's 16-bit value to `[sp]`. |
| `0x31` | `POP` | `reg` | 2 bytes | Reads 16-bit value from `[sp]` into register, increments `sp` by 2. |

### System

| Opcode | Mnemonic | Arguments | Size | Description |
| :--- | :--- | :--- | :--- | :--- |
| `0xF0` | `PRINT` | `reg` | 2 bytes | Prints `PRINT OUTPUT: <decimal> (0x<hex>)` to stdout. |
| `0xFF` | `HALT` | *(none)* | 1 byte | Sets `is_running = false`, cleanly exits the run loop. |

---

## Assembly Language Guide

### Syntax Rules

| Element | Syntax | Example |
| :--- | :--- | :--- |
| Instruction | `MNEMONIC [operands]` | `SUB R1, R0` |
| Register | `R0` – `R3` | `PUSH R2` |
| Immediate (decimal) | bare integer | `LOAD_VAL R0, 100` |
| Immediate (hex) | `0x` prefix | `LOAD_VAL R0, 0x64` |
| Label definition | `name:` on its own line | `loop:` |
| Label reference | bare label name | `JNE loop` |
| Comment | `;` — extends to end of line | `; initialize counter` |
| Operand separator | `,` (optional) | `ADD R0, R1` or `ADD R0 R1` |

The assembler's `cleanup()` function strips comments and trims whitespace before tokenizing, so blank lines and comment-only lines are safely ignored.

---

### Example 1 — Basic Arithmetic (`input/basic_sum.asm`)

```asm
LOAD_VAL R1, 10      ; R1 = 10
LOAD_VAL R2, 20      ; R2 = 20
ADD R1, R2           ; R1 = R1 + R2  →  30
PRINT R1             ; PRINT OUTPUT: 30 (0x001E)
HALT
```

Assembled binary (11 bytes, little-endian):

```
01 01 0A 00   ; LOAD_VAL R1, 10
01 02 14 00   ; LOAD_VAL R2, 20
10 01 02      ; ADD R1, R2
F0 01         ; PRINT R1
FF            ; HALT
```

---

### Example 2 — Countdown Loop with `JNE`

```asm
LOAD_VAL R0, 5       ; R0 = counter = 5
LOAD_VAL R1, 1       ; R1 = step

loop:
    PRINT R0         ; print current counter
    SUB   R0, R1     ; R0 -= 1  (sets zf when R0 hits 0)
    JNE   loop       ; if zf == false, keep looping

HALT
```

---

### Example 3 — Stack LIFO Behaviour

```asm
LOAD_VAL R0, 0xABCD
LOAD_VAL R1, 0x1234

PUSH R0              ; stack: [0xABCD]
PUSH R1              ; stack: [0xABCD, 0x1234]

POP R2               ; R2 = 0x1234  (last in, first out)
POP R3               ; R3 = 0xABCD

PRINT R2             ; PRINT OUTPUT: 4660 (0x1234)
PRINT R3             ; PRINT OUTPUT: 43981 (0xABCD)
HALT
```

---

## How the Two-Pass Assembler Works

A single-pass assembler cannot handle **forward references** — a `JNE loop` that appears before `loop:` is defined. The two-pass design solves this cleanly.

### Pass 1 — Symbol Table Construction

`buildSymbolTable()` iterates every cleaned, non-empty line and maintains an `address_counter` that tracks how many bytes the current instruction will occupy (via `getInstructionSize()`). When a label definition is found (a token ending in `:`), the mapping `label → address_counter` is inserted into a `std::map<string, uint16_t>`. No binary is produced.

```
Line              address_counter   Symbol Table
────────────────  ───────────────   ────────────────────
LOAD_VAL R0, 5    0 → 4
LOAD_VAL R1, 1    4 → 8
loop:             8  (no advance)   loop = 0x0008
PRINT R0          8 → 10
SUB R0, R1        10 → 13
JNE loop          13 → 16
HALT              16 → 17
```

### Pass 2 — Binary Code Emission

`assembleLine()` reads each line again and appends encoded bytes to a `std::vector<uint8_t>`. Jump instructions look up label addresses from the symbol table built in Pass 1 and emit them as little-endian 16-bit values via `push16bits()`. Label lines are skipped with a `continue`.

### Writing the Output

`saveBinary()` opens the output file with `std::ios::binary` and writes the completed byte vector in one call. On success it prints:

```
Successfully wrote 17 bytes to program.bin
```

### Error Handling

The assembler exits with `[Fatal Error]` and a descriptive message on:

- Unknown mnemonic (`getInstructionSize` returns 0 in Pass 1)
- Missing operands for any instruction
- Invalid register name (e.g. `R4`, `RX` — only `R0`–`R3` are valid)
- Failure to open the input or output file

---

## Development Team

| Member | Role |
| :--- | :--- |
| **Aryan** | System Architect & Core VM Engine (`vm.c`, `execute_instruction`, fetch logic) |
| **Dhananjay** | Assembler Lexer & Binary Generator (`assembler.cc`) |
| **Guru** | Memory Management & Stack Logic (`memory.c`, overflow/underflow guards) |
| **Hamesh** | Build Systems & File I/O (`Makefile`, `main.c`) |
| **Aditya** | Assembly Programming & QA Testing (`.asm` programs, end-to-end validation) |