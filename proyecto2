; Project 2: Arithmetic using the Assembly Language

    .global main

; Offset that updates the pointer when assigning values to RAM
OS   .EQU 0x04
ITER .EQU 10 ; Counter in decimal


; Case RESULT > 0xFFFF.FFFF (C=1)

EXCEEDS_FFFF
    MOVW R1, #0x003C ; [40-4 = 3C]
    MOVT R1, #0x2000
    STR R2, [R1, #4]!

    B PROBLEM_3


main

;-------------------------------------------------------------
; PROBLEM 1
; \sum_{0}^{9} X_i
;-------------------------------------------------------------

; Starting from 0x2000.0000
    MOVW R0, #0x0000
    MOVT R0, #0x2000

; Store 20 values
	; X_i
    MOVW R1, #0xFFFF
    MOVT R1, #0xFFFF
    STR R1, [R0]
    MOVW R1, #0xFFFF
    MOVT R1, #0xFFFF
    STR R1, [R0, #OS]!
    MOVW R1, #0xFFFF

    ; Y_i
    MOVT R1, #0xFFFF
    STR R1, [R0, #OS]!
    MOVW R1, #0xFFFF
    MOVT R1, #0xFFFF
    STR R1, [R0, #OS]!
    MOVW R1, #0xFFFF
    MOVT R1, #0xFFFF
    STR R1, [R0, #OS]!

;-------------------------------------------------------------

; Starting point where we can take values from
    MOVW R0, #0xFFFF ; [2000.0000 - 1] = 1FFF.FFFF
    MOVT R0, #0x1FFF

; R1 retrieves each byte from RAM
    MOV R1, #0

; R2 holds the 8-bit value from RAM
    MOV R2, #0 ; R2 = sum_{0}^{9} X_i for each iteration

; Counter. If R10 = 10, exits SUM loop
    MOV  R10, #0

SUM
    LDRB R1, [R0, #1]! ; LDR{B} Just one byte is retrieved. Updates pointer (R0 = R0 + 1)
    ADDS R2, R1 ; {S} sets state. Stores the updated result in R2
    ADD  R10, R10, #1 ; counter = counter + 1
    CMP R10, #ITER ; Make sure to do this 10x
    BNE SUM ; While not equal, go back and SUM again

; Store the sum result in a 32-bit format @0x2000.0020
    MOVW R6, #0x0020
    MOVT R6, #0x2000
    STR R2, [R6]

;--------------------------------------------------------------------------------------------------------------------------


;-------------------------------------------------------------
; PROBLEM 2
; SUM-OF-SQUARED-PRODUCTS
; \sum_0^4 (X_i*Y_i)^2
;-------------------------------------------------------------


; (X_i*Y_i) section

; Starting point where we can take values from [X_i]
    MOVW R0, #0xFFFF ;[2000.0000 - 1] = 1FFF.FFFF
    MOVT R0, #0x1FFF

; R12 points to [Y_0] i.e. [2000.000A] minus one
    MOVW R12, #0x0009 ;[2000.000A - 1] = 2000.0009
    MOVT R12, #0x2000

    MOV  R10, #0 ; Counter

; Store product results in 2000.0100
    MOVW R9, #0x0100 ; just a random location in memory
    MOVT R9, #0x2000

PRODUCT
    LDRB R1, [R0, #1]! ; R1 = X_i; [0 =< i =< 4]
    LDRB R2, [R12, #1]! ; R2 = Y_i; [0 =< i =< 4]
    MUL R3, R1, R2 ; R3 = (X_i*Y_i)

; Store product results in 2000.0100 + 2 [MAX = (FF*FF) = FE01 => 2 bytes]]
    STRH R3, [R9, #2]! ; STR{H} i.e. 16-bit format, cannot be STRB

    ADD  R10, R10, #1 ; counter += counter
    CMP R10, #5 ; Make sure to do this 5x
    BNE PRODUCT ; While not equal, go back and multiply the next set of bytes

;-------------------------------------------------------------

; Square-the-product section
; (X_i*Y_i)^2

; Retrieve product results from 2000.0100 + 2
    MOVW R9, #0x0100
    MOVT R9, #0x2000

; Store each squared result in 2000.0150 + 4
    MOVW R11, #0x0150 ; could be up to 36 bits
    MOVT R11, #0x2000 ; maximum value is 3.F017.F004 (36 bits)

    MOV  R10, #0 ; Counter

SQUARED
    LDRH R1, [R9, #2]! ; R1 retrieves Z_i = X_i*Y_i
    MUL R2, R1, R1 ; R2 = Z_i^2

; Store each squared value in 2000.0150
; #4 because the power2 need 4 bytes
; Max value = (FF*FF) = [FC05.FC01]
    STR R2, [R11, #4]! ; R2 holds the squared term
    ADD  R10, R10, #1 ; counter += counter
    CMP R10, #5 ; Make sure to do this 5x
    BNE SQUARED ; While not equal, go back and ^2 again

;-------------------------------------------------------------

; sum_0^4 (X_i*Y_i)^2 section
; Retrieve squared results from 2000.0150
    MOVW R9, #0x0150
    MOVT R9, #0x2000

    MOV R2, #0 ; Store the result in R2

    MOV  R10, #0 ; Counter

SUM_SQ
    LDR R1, [R9, #4]! ; Load
    ADDS R2, R1 ; Updates the sum

; If R2 > 0xFFFF.FFFF (C=1), then store R2 @0x40
    BHS EXCEEDS_FFFF

    ADD  R10, R10, #1 ; Start with the first iteration
    CMP R10, #5 ; Make sure to do this 5x
    BNE SUM_SQ ; While not equal, go back and sum again

; else, store R2 @0x20
    MOVW R6, #0x0024
    MOVT R6, #0x2000
    STR R2, [R6]




;-------------------------------------------------------------
; PROBLEM 3
; \sum_0^9 (X_i + Y_i)
;-------------------------------------------------------------


PROBLEM_3
; Starting point where we can take values from [X_i]
    MOVW R0, #0xFFFF ;[2000.0000 - 1] = 1FFF.FFFF
    MOVT R0, #0x1FFF

; R12 points to [Y_0] i.e. [2000.000A] minus one
    MOVW R12, #0x0009 ; [2000.000A - 1] = 2000.0009
    MOVT R12, #0x2000

    MOV R10, #0 ; Counter

	MOV R3, #0 ; Store the final result here


SUM2_EX3
    LDRB R1, [R0, #1]! ;  R1 = X_i; [0 =< i =< 10]
    LDRB R2, [R12, #1]! ; R2 = Y_i; [0 =< i =< 10]
    ADDS R2, R1 ; R2 = X_i + Y_i
    ADDS R3, R2 ; Step needed R3 = R2 + R3, updated result
    ADDS  R10, R10, #1 ; Start with the first iteration
    CMP R10, #ITER ; Make sure to do this 10x

    BNE SUM2_EX3 ; While not equal, go back and sum again

; Store the result in the third place
    MOVW R6, #0x0028
    MOVT R6, #0x2000
    STR R3, [R6]




;-------------------------------------------------------------
; PROBLEM 4
; X ARITHMETIC MEAN
; \bar{X}
;-------------------------------------------------------------

; \sum_0^9 has been already calculated and stored
    MOVW R6, #0x0020
    MOVT R6, #0x2000

    LDR R2, [R6] ; Load the sum in R2
    MOV R4, #ITER ; Number of values

; Calculate the average using integer division
    SDIV R5, R2, R4 ; SDIV for signed numbers

;-------------------------------------------------------------

; Calculate remainder (decimal part)
    MOV R6, #100 ; Scaling factor (for 2 decimal places). Do not set more than 100, otherwise the error may increase
    MUL R7, R2, R6 ; Multiply the original sum by the scaling factor (scaled_quotient)
    SDIV R7, R7, R4 ; Perform the division

; Substract the (quotient*scaling_factor) from the scaled_quotient
    MUL R0, R5, R6 ; R0 = quotient*scaling_factor
    SUB R1, R7, R0 ; R1 = (scaled_quotient) - (quotient*scaling_factor)

; Store the integer and decimal parts
    MOVW R6, #0x002C ; Fourth place in memory
    MOVT R6, #0x2000

    STRB R1, [R6] ; Store decimal part here (just one byte is needed for a two-decimal representation)
    STR R5, [R6, #1] ; Integer part here goes here



;-------------------------------------------------------------
; PROBLEM 5
; Y ARITHMETIC MEAN
; \bar{Y}
;-------------------------------------------------------------

;-------------------------------------------------------------
; SUM
; First calculate the sum \sum^{0}^{9} Y_i

; R12 points to [Y_0] i.e. [2000.000A] minus one
    MOVW R12, #0x0009 ;[2000.000A - 1] = 2000.0009
    MOVT R12, #0x2000

; R1 retrieves each byte from RAM
    MOV R1, #0

; R2 holds the 8-bit value from RAM
    MOV R2, #0 ; R2 = sum_{0}^{9} Y_i for each iteration

; Counter. If R10 = 10, exits SUM loop
    MOV  R10, #0

SUM_Y
    LDRB R1, [R12, #1]!; LDR{B} Just one byte is retrieved. Updates pointer (R0 = R0 + 1)
    ADDS R2, R1; {S} sets state. Stores the updated result in R2
    ADD  R10, R10, #1 ; counter = counter + 1
    CMP R10, #ITER ; Make sure to do this 10x
    BNE SUM_Y ; While not equal, go back and SUM again

; Store the sum result in a 32-bit format
; Doesn't matter where this sum is placed, just make sure not to interfere with the rest of the program
    MOVW R6, #0x0090 ; R6 = \sum_x^9 Y_i
    MOVT R6, #0x2000
    STR R2, [R6]

;-------------------------------------------------------------

; AVERAGE
; Same process as Problem 4
    MOVW R6, #0x0090 ; Points to \sum_0^9 Y_i
    MOVT R6, #0x2000

    LDR R2, [R6] ; R2 = \sum_0^9 Y_i
    MOV R4, #ITER ; Number of values

    SDIV R5, R2, R4 ; Integer part

;-------------------------------------------------------------

; Calculate remainder (decimal part)
    MOV R6, #100 ; Scaling factor
    MUL R7, R2, R6
    SDIV R7, R7, R4 ; Scaled quotient

    MUL R0, R5, R6
    SUB R1, R7, R0 ; Decimal part result

; Store the integer and decimal parts
    MOVW R6, #0x0030 ; Fifth place
    MOVT R6, #0x2000

    STRB R1, [R6] ; Store decimal part
    STR R5, [R6, #1] ; Store integer part

end B end
