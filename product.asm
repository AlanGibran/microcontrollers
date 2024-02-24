; Project 2: Arithmetic using the Assembly Language
    .global main
; Offset that updates the pointer when assigning values to RAM
OS  .EQU 0x04
ITER .EQU 3 ; Counter in decimal

main
; Starting from 0x2000.0000
    MOVW R0, #0x0000
    MOVT R0, #0x2000
; Store 20 random values (not really random)
    MOVW R1, #0x0111
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

; Starting point where we can take values from [X_i]
    MOVW R0, #0xFFFF ;[2000.0000 - 1] = 1FFF.FFFF
    MOVT R0, #0x1FFF; Second time we use R0

; R12 points to [Y_0] i.e. [2000.000A] minus one
    MOVW R12, #0x0009 ;[2000.000A - 1] = 2000.0009
    MOVT R12, #0x2000

	MOV  R10, #0; Counter. if R10 = 10, exit product loop


; Store product results in 2000.0100
	MOVW R9, #0x008E; [90-2 = 8E]
	MOVT R9, #0x2000; The offset here is two
 ; because FF*FF = FE01 i.e. 2 bytes or 16 bits

PRODUCT
    LDRB R1, [R0, #1]!; R1 = X_i; [0 =< i =< 4]
    LDRB R2, [R12, #1]!; R2 = Y_i; [0 =< i =< 4]
	MUL R3, R1, R2

	STRH R3, [R9, #2]! ; STR{H} i.e. 16-bit format

	ADD  R10, R10, #1 ; Start with the first iteration
    CMP R10, #4 ; Make sure to do this 10x
    BNE PRODUCT ; While not equal, go back and sum again




fin B fin
	.end