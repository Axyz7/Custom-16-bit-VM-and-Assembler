; ==========================================================
; ALGORITHM: Division with Remainder (25 / 7)
; Highlights: Custom JLT instruction, Registers, Math
; ==========================================================

LOAD_VAL R0, 25     ; R0 = Dividend (Will become the Remainder)
LOAD_VAL R1, 7      ; R1 = Divisor
LOAD_VAL R2, 0      ; R2 = Quotient (Answer) Accumulator
LOAD_VAL R3, 1      ; R3 = Constant 1 (for incrementing)

DIV_LOOP:
    CMP R0, R1      ; Compare current Dividend with Divisor
    JLT END_DIV     ; If Dividend < Divisor, we are done!
    
    SUB R0, R1      ; Dividend = Dividend - Divisor
    ADD R2, R3      ; Quotient = Quotient + 1
    JMP DIV_LOOP    ; Unconditional jump back to start of loop

END_DIV:
    PRINT R2        ; Print the Quotient (Expected Output: 3)
    PRINT R0        ; Print the Remainder (Expected Output: 4)
    HALT            ; Shut down VM