; ==========================================================
; ALGORITHM: Software Multiplication (13 x 5)
; Highlights: Accumulators, Zero Flag hardware triggers
; ==========================================================

LOAD_VAL R0, 13     ; R0 = Multiplicand
LOAD_VAL R1, 5      ; R1 = Multiplier (Used as our countdown)
LOAD_VAL R2, 0      ; R2 = Product (Starts at 0)
LOAD_VAL R3, 1      ; R3 = Constant 1

MUL_LOOP:
    ADD R2, R0      ; Product = Product + Multiplicand
    SUB R1, R3      ; Countdown = Countdown - 1
    
    ; Because SUB updates the hardware Zero Flag (ZF):
    JNE MUL_LOOP    ; If Countdown is not 0, jump back up

    PRINT R2        ; Print Final Product (Expected Output: 65, Hex: 0x0041)
    HALT