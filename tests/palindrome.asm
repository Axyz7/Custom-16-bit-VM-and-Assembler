; 3-digit palindrome checker
; a number is a palindrome if first and last digit are the same
; ex: 121 -> yes, 123 -> no
; prints 1 if palindrome, 0 if not

; how it works:
; divide by 100 to get hundreds digit
; then get units digit from whats left
; compare the two

; mem 0x2000 = hundreds digit stored here

LOAD_VAL R0, 121     ; number to check (change to 123 to test false case)
LOAD_VAL R3, 1       ; constant 1, reused everywhere


; step 1 - get the hundreds digit
; just subtract 100 repeatedly and count how many times

LOAD_VAL R1, 100
LOAD_VAL R2, 0       ; counter starts at 0

HDIV_LOOP:
CMP R0, R1           ; is whats left >= 100?
JLT HDIV_DONE        ; nope, done
SUB R0, R1           ; take away 100
ADD R2, R3           ; count it
JMP HDIV_LOOP

HDIV_DONE:
STORE_MEM 0x2000, R2  ; save hundreds digit
; R0 has remainder now (like 21 if input was 121)


; step 2 - get the units digit
; subtract 10 until less than 10, whatever is left = units digit

LOAD_VAL R1, 10
LOAD_VAL R2, 0       ; dont actually need R2 here but using for tens count anyway

UDIV_LOOP:
CMP R0, R1           ; less than 10 yet?
JLT UDIV_DONE
SUB R0, R1           ; remove 10
ADD R2, R3           ; tens++ (not used but whatever)
JMP UDIV_LOOP

UDIV_DONE:
; R0 = units digit now


; step 3 - compare hundreds vs units
; if equal its a palindrome

LOAD_MEM R1, 0x2000   ; get the hundreds digit back
CMP R0, R1            ; compare
JEQ IS_PALINDROME     ; if same -> palindrome


NOT_PALINDROME:
LOAD_VAL R0, 0
PRINT R0              ; prints 0
HALT


IS_PALINDROME:
LOAD_VAL R0, 1
PRINT R0              ; prints 1
HALT