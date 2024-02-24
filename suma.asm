; Project 2: Arithmetic using the Assembly Language
    .global main
; Offset that updates the pointer when assigning values to RAM
OS  .EQU 0x04 
ITER .EQU 10 ; Counter in decimal

main
; Starting from 0x2000.0000
    MOVW R0, #0x0000
    MOVT R0, #0x2000
; Store 20 random values (not really random)
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

	MOV R1, #0 ; Fetches each byte from RAM
; R2 stores the 8-bit value from RAM 
	MOV R2, #0 ; R2 = sum_0^9 X_i for each iteration

	MOV  R10, #0; Counter. if R10 = 10, exit SUM loop 

SUM
	ADD  R10, R10, #1 ; Start with the first iteration
	LDRB R1, [R0, #1]!; {B} Just one byte is needed for each sum
	ADDS R2, R1; {s} activates flags. Store the updated result in R2
	CMP R10, #ITER ; Make sure to do this 10x
	BNE SUM ; While not equal, go back and sum again

; Store the sum result in a 32-bit format @0x2000.0020
	MOVW R0, #0x0020
	MOVT R0, #0x2000
	STR R2, [R0]

fin B fin
	.end
