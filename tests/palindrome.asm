; ==========================================================
; ALGORITHM: 3-Digit Palindrome Checker
;
; A 3-digit number is a palindrome if its HUNDREDS digit
; equals its UNITS digit.  (e.g. 121 → 1 == 1 ✓)
;
; Example used: 121  → IS a palindrome  → PRINT OUTPUT: 1
; Change the first LOAD_VAL to 123 to test a non-palindrome
;                             → NOT a palindrome → PRINT OUTPUT: 0
;
; Method:
;   Step 1 – Divide N by 100 via repeated subtraction
;            → quotient = hundreds digit, remainder = last 2 digits
;   Step 2 – Divide remainder by 10 via repeated subtraction
;            → what is left over = units digit
;   Step 3 – Compare hundreds digit with units digit
;
; Memory used:
;   0x2000 = saved hundreds digit
; ==========================================================

LOAD_VAL R0, 121        ; R0 = number to test (try 123 for non-palindrome)
LOAD_VAL R3, 1          ; R3 = constant 1 (kept throughout)


; ----------------------------------------------------------
; STEP 1: Extract hundreds digit  (N / 100)
;   R2 accumulates how many times we can subtract 100 from R0
;   After the loop R0 holds the remainder (last 2 digits)
; ----------------------------------------------------------
LOAD_VAL R1, 100        ; R1 = 100
LOAD_VAL R2, 0          ; R2 = hundreds-digit counter

HDIV_LOOP:
    CMP R0, R1          ; remaining - 100
    JLT HDIV_DONE       ; remaining < 100  → done extracting hundreds
    SUB R0, R1          ; remaining -= 100
    ADD R2, R3          ; hundreds++
    JMP HDIV_LOOP

HDIV_DONE:
STORE_MEM 0x2000, R2    ; save hundreds digit to RAM
; R0 now = last-two-digit remainder  (e.g. 21 for 121)


; ----------------------------------------------------------
; STEP 2: Extract units digit  (last-two-digit remainder % 10)
;   Keep subtracting 10 until what is left is < 10;
;   that leftover is the units digit.
; ----------------------------------------------------------
LOAD_VAL R1, 10         ; R1 = 10
LOAD_VAL R2, 0          ; R2 = tens accumulator (result discarded)

UDIV_LOOP:
    CMP R0, R1          ; remaining - 10
    JLT UDIV_DONE       ; remaining < 10  → R0 = units digit
    SUB R0, R1          ; remaining -= 10
    ADD R2, R3          ; tens++ (counted but not needed)
    JMP UDIV_LOOP

UDIV_DONE:
; R0 = units digit of original number  (e.g. 1 for 121)


; ----------------------------------------------------------
; STEP 3: Compare hundreds digit == units digit
; ----------------------------------------------------------
LOAD_MEM R1, 0x2000     ; R1 = hundreds digit (retrieved from RAM)
CMP R0, R1              ; units - hundreds
JEQ IS_PALINDROME       ; result = 0 → digits match → palindrome

NOT_PALINDROME:
    LOAD_VAL R0, 0
    PRINT R0            ; PRINT OUTPUT: 0 (0x0000) — NOT a palindrome
    HALT

IS_PALINDROME:
    LOAD_VAL R0, 1
    PRINT R0            ; PRINT OUTPUT: 1 (0x0001) — IS a palindrome
    HALT
