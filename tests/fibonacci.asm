; ==========================================================
; ALGORITHM: Fibonacci Sequence Generator (First 8 Numbers)
; Highlights: Stack Memory (PUSH/POP), Data Swapping, Loops
; ==========================================================

LOAD_VAL R0, 0      ; R0 = 'A' (Previous number)
LOAD_VAL R1, 1      ; R1 = 'B' (Current number)
LOAD_VAL R2, 8      ; R2 = Loop Counter (How many to generate)
LOAD_VAL R3, 1      ; R3 = Constant 1 (For decrementing counter)

FIB_LOOP:
    PRINT R0        ; Print the current Fibonacci number

    ; --- The Swap & Add Logic ---
    PUSH R1         ; Save 'B' to the Stack (Memory bounds test!)
    ADD R1, R0      ; B = B + A (Calculate the NEXT number)
    POP R0          ; Old 'B' comes off the Stack and becomes new 'A'
    
    ; --- The Counter Logic ---
    SUB R2, R3      ; Counter = Counter - 1
    
    ; SUB automatically sets the Zero Flag if the result hits 0!
    JNE FIB_LOOP    ; If Counter != 0, keep generating
    
    HALT