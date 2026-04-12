; count primes from 2 to 100
; answer should be 25

; memory layout:
; 8192 = current number being tested (N)
; 8194 = trial divisor (d)
; 8196 = prime count

LOAD_VAL R3, 1
LOAD_VAL R0, 2
STORE_MEM 8192, R0     ; N starts at 2
LOAD_VAL R0, 0
STORE_MEM 8196, R0     ; prime count = 0


OUTER_LOOP:
LOAD_MEM R0, 8192      ; grab N
LOAD_VAL R1, 101
CMP R0, R1             ; if N >= 101 we're done
JLT OUTER_BODY
JMP DONE


OUTER_BODY:
LOAD_VAL R1, 2
STORE_MEM 8194, R1     ; reset d to 2 for each new N


INNER_LOOP:
LOAD_MEM R0, 8192
LOAD_MEM R1, 8194      ; d
CMP R1, R0             ; d vs N
JLT DO_MOD             ; d < N, keep checking
JMP IS_PRIME           ; d reached N without finding factor, its prime


DO_MOD:
LOAD_MEM R2, 8192      ; working copy of N for the mod

MOD_LOOP:
CMP R2, R1             ; temp < d?
JLT MOD_DONE
SUB R2, R1             ; temp -= d
JMP MOD_LOOP

MOD_DONE:
CMP R2, R3             ; is remainder 0? (mod < 1 means == 0)
JLT NOT_PRIME          ; yep, composite
ADD R1, R3             ; d++
STORE_MEM 8194, R1
JMP INNER_LOOP


IS_PRIME:
LOAD_MEM R0, 8196
ADD R0, R3
STORE_MEM 8196, R0     ; prime_count++
LOAD_MEM R0, 8192
ADD R0, R3
STORE_MEM 8192, R0     ; N++
JMP OUTER_LOOP


NOT_PRIME:
LOAD_MEM R0, 8192      ; just increment N and move on
ADD R0, R3
STORE_MEM 8192, R0
JMP OUTER_LOOP


DONE:
LOAD_MEM R0, 8196
PRINT R0               ; should be 25
HALT