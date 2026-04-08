; ==========================================================
; ALGORITHM: Basic Arithmetic — Add, Subtract, Multiply, Divide
; Operations:
;   ADD  :  25 + 17 = 42   → PRINT OUTPUT: 42  (0x002A)
;   SUB  :  50 - 18 = 32   → PRINT OUTPUT: 32  (0x0020)
;   MUL  :  6  × 7  = 42   → PRINT OUTPUT: 42  (0x002A)
;   DIV  :  49 / 7  = 7 (remainder 0)
;                           → PRINT OUTPUT: 7   (0x0007)
;                           → PRINT OUTPUT: 0   (0x0000)
; Note: MUL and DIV are implemented via repeated ADD/SUB loops
;       since the ISA has no native MUL/DIV opcode.
; ==========================================================


; ----------------------------------------------------------
; ADDITION:  25 + 17 = 42
; ----------------------------------------------------------
LOAD_VAL R0, 25         ; R0 = 25
LOAD_VAL R1, 17         ; R1 = 17
ADD R0, R1              ; R0 = R0 + R1  →  42
PRINT R0                ; PRINT OUTPUT: 42 (0x002A)


; ----------------------------------------------------------
; SUBTRACTION:  50 - 18 = 32
; ----------------------------------------------------------
LOAD_VAL R0, 50         ; R0 = 50
LOAD_VAL R1, 18         ; R1 = 18
SUB R0, R1              ; R0 = R0 - R1  →  32
PRINT R0                ; PRINT OUTPUT: 32 (0x0020)


; ----------------------------------------------------------
; MULTIPLICATION:  6 × 7 = 42  (repeated addition)
; Strategy: add the multiplicand 'multiplier' times
; ----------------------------------------------------------
LOAD_VAL R0, 6          ; R0 = multiplicand
LOAD_VAL R1, 7          ; R1 = multiplier (used as countdown)
LOAD_VAL R2, 0          ; R2 = product accumulator (starts at 0)
LOAD_VAL R3, 1          ; R3 = constant 1

MUL_LOOP:
    ADD R2, R0          ; product += multiplicand
    SUB R1, R3          ; countdown--  (SUB sets Zero Flag when result = 0)
    JNE MUL_LOOP        ; if countdown != 0, keep looping

PRINT R2                ; PRINT OUTPUT: 42 (0x002A)


; ----------------------------------------------------------
; DIVISION:  49 / 7 = 7, remainder 0  (repeated subtraction)
; Strategy: subtract divisor from dividend until dividend < divisor;
;           count how many times we subtracted → that is the quotient.
; ----------------------------------------------------------
LOAD_VAL R0, 49         ; R0 = dividend (will become remainder after loop)
LOAD_VAL R1, 7          ; R1 = divisor
LOAD_VAL R2, 0          ; R2 = quotient accumulator
LOAD_VAL R3, 1          ; R3 = constant 1

DIV_LOOP:
    CMP R0, R1          ; compare dividend vs divisor
    JLT END_DIV         ; if dividend < divisor → we are done
    SUB R0, R1          ; dividend -= divisor
    ADD R2, R3          ; quotient++
    JMP DIV_LOOP

END_DIV:
    PRINT R2            ; PRINT OUTPUT: 7  (0x0007)  — the quotient
    PRINT R0            ; PRINT OUTPUT: 0  (0x0000)  — the remainder

HALT
