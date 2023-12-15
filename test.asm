    .global main
    .text
OS  .EQU 0x04 ; offset para rellenar
THRESHOLD  .EQU 0x28

main

; 20 valores aleatorios a partir de 0x2000.0100
    MOVW R0, #0x0100
    MOVT R0, #0x2000
    MOVW R1, #0x1234
    MOVT R1, #0xABCD
    STR R1, [R0]
    STR R1, [R0, #OS]!
    STR R1, [R0, #OS]!
    STR R1, [R0, #OS]!
    STR R1, [R0, #OS]!

;INITIALIZE BOTH COUNTERS
    MOV  R10, #0 ; initialize the counter for the number of iterations
    MOV  R12, #0; counter for the "value part" if (X_0 > 28)

; Starting point where we can take values from
    MOVW R0, #0x00FF
    MOVT R0, #0x2000

; Starting point for -value-
    MOVW R5, #0x0114
    MOVT R5, #0x2000
; Starting point for the -order-
    MOVW R6, #0x0115
    MOVT R6, #0x2000

; ACCESS TO MEMORY

label
    ADD  R10, R10, #1 ; counter, N=1
	CMP R10, #20 ; make sure to do this max 20 times
	BEQ fin ; if counter R10=20, stop and finish

    ; store in R2 the -VALUE- which points R0: (XXXX.XXXX)
    ; + offset and update the memory location (per byte)
    LDRBT R2, [R0, #0x1]! ; Guardo en R2 los LSB

    CMP R2, #THRESHOLD ; R2 - THRESHOLD
    ; if not the case, up to the next value in memory
    BMI label ; BMI stands for a negative result

    ;checar la localidad 0x2000.0116
    ; ahi tiene que estar el valor que es mayor a 0x28
    STRBT R2, [R5, #0x2]!; mete el R2 a donde apunta R5
    ADD R12, #0x1 ; the byte next to -value- contains -order-
    STRBT R12, [R6, #0x2]!
    B label ; see if we're done with the 20 values

fin B fin
	.end

