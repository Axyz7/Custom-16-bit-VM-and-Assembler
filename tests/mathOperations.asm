; Basic Arithmetic operations
; add subtract multiply divide
; Written by me

;----- ADD -----
; 25 + 17 should give 42
LOAD_VAL R0, 25      ; first number
LOAD_VAL R1, 17      ; second number
ADD R0, R1           ; do the add, result in r0
PRINT R0             ; 42


;----- SUBTRACT -----
; 50 - 18 = 32
LOAD_VAL R0, 50
LOAD_VAL R1, 18      ; number to subtract
SUB R0, R1           ; r0 = r0 - r1
PRINT R0             ; should print 32


;----MULTIPLY-----
; no MUL opcode so doing it with a loop (repeated add)
; 6 x 7 = 42
LOAD_VAL R0, 6       ; multiplicand
LOAD_VAL R1, 7       ; how many times to add (counter)
LOAD_VAL R2, 0       ; accumulator, starts 0
LOAD_VAL R3, 1       ; const for decrementing

MUL_LOOP:
ADD R2, R0           ; acc += 6
SUB R1, R3           ; counter--
JNE MUL_LOOP         ; loop until counter hits zero
PRINT R2             ; print result (42)


;----- DIVIDE -----
;49 / 7
; again no DIV so using repeated subtraction
; keep subtracting divisor and count how many times

LOAD_VAL R0, 49      ; dividend
LOAD_VAL R1, 7       ; divisor
LOAD_VAL R2, 0       ; quotient counter
LOAD_VAL R3, 1       ; need this for quotient increment

DIV_LOOP:
CMP R0, R1
JLT END_DIV          ; jump out once dividend is smaller than divisor
SUB R0, R1           ; dividend -= divisor
ADD R2, R3           ; quotient++
JMP DIV_LOOP

END_DIV:
PRINT R2             ; quotient = 7
PRINT R0             ; remainder = 0 (divides evenly)

HALT