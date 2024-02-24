; Project 2: Arithmetic using the Assembly Language
    .global main
OS  .EQU 0x04 ; offset para rellenar
ITER .EQU 10
main

; 20 valores aleatorios a partir de 0x2000.0000
    MOVW R0, #0x0000
    MOVT R0, #0x2000

    MOVW R1, #0x0100
    MOVT R1, #0x0302
    STR R1, [R0]
    MOVW R1, #0x0504
    MOVT R1, #0x0706
    STR R1, [R0, #OS]!
    MOVW R1, #0x0908
    MOVT R1, #0x0B0A
    STR R1, [R0, #OS]!
    MOVW R1, #0x0D0C
    MOVT R1, #0x0F0E
    STR R1, [R0, #OS]!
    MOVW R1, #0x1110
    MOVT R1, #0x1312
	STR R1, [R0, #OS]!

; Starting point where we can take values from
    MOVW R0, #0xFFFF ;[2000.0000 - 1] = 1FFF.FFFF
    MOVT R0, #0x1FFF

	MOV R1, #0 ; Fetches each byte
	MOV R2, #0 ; Final result

	MOV  R10, #0; counter

SUM
	ADD  R10, R10, #1 ; start with first iteration

	LDRB R1, [R0, #1]!; {B} Just one byte is needed for each sum
	ADDS R2, R1; store the updated result in R2
	CMP R10, #ITER ; make sure to do this 10x
	BNE SUM ; while not equal, go back and sum again

; store the sum result in a 32-bit format @0x2000.0020
	MOVW R0, #0x0020
	MOVT R0, #0x2000
	STR R2, [R0]

fin B fin
	.end
