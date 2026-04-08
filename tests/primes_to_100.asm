; ==========================================================
; ALGORITHM: Count Primes from 2 to 100
; Expected PRINT OUTPUT: 25 (0x0019)
; Memory (decimal addresses — assembler uses std::stoi base-10):
;   8192 = N  (current candidate)
;   8194 = d  (trial divisor)
;   8196 = prime_count
; ==========================================================

LOAD_VAL R3, 1
LOAD_VAL R0, 2
STORE_MEM 8192, R0          ; N = 2
LOAD_VAL R0, 0
STORE_MEM 8196, R0          ; prime_count = 0

OUTER_LOOP:
    LOAD_MEM R0, 8192        ; R0 = N
    LOAD_VAL R1, 101
    CMP R0, R1               ; N - 101
    JLT OUTER_BODY
    JMP DONE

OUTER_BODY:
    LOAD_VAL R1, 2
    STORE_MEM 8194, R1       ; d = 2

INNER_LOOP:
    LOAD_MEM R0, 8192        ; R0 = N
    LOAD_MEM R1, 8194        ; R1 = d
    CMP R1, R0               ; d - N
    JLT DO_MOD               ; d < N → test divisor
    JMP IS_PRIME             ; d >= N → no factor → prime

DO_MOD:
    LOAD_MEM R2, 8192        ; R2 = N (working copy for mod)

MOD_LOOP:
    CMP R2, R1               ; temp - d
    JLT MOD_DONE             ; temp < d → R2 = N mod d
    SUB R2, R1               ; temp -= d
    JMP MOD_LOOP

MOD_DONE:
    CMP R2, R3               ; (N mod d) - 1
    JLT NOT_PRIME            ; mod < 1 means mod == 0 → composite
    ADD R1, R3               ; d++
    STORE_MEM 8194, R1
    JMP INNER_LOOP

IS_PRIME:
    LOAD_MEM R0, 8196        ; R0 = prime_count
    ADD R0, R3
    STORE_MEM 8196, R0       ; prime_count++
    LOAD_MEM R0, 8192
    ADD R0, R3
    STORE_MEM 8192, R0       ; N++
    JMP OUTER_LOOP

NOT_PRIME:
    LOAD_MEM R0, 8192        ; R0 = N
    ADD R0, R3
    STORE_MEM 8192, R0       ; N++
    JMP OUTER_LOOP

DONE:
    LOAD_MEM R0, 8196        ; R0 = prime_count
    PRINT R0                 ; PRINT OUTPUT: 25 (0x0019)
    HALT