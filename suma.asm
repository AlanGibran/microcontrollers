; Project 2: Arithmetic using the Assembly Language
    .global main
OS  .EQU 0x04 ; offset para rellenar
main

; 20 valores aleatorios a partir de 0x2000.0000
    MOVW R0, #0x0000
    MOVT R0, #0x2000

    MOVW R1, #0xBEEF
    MOVT R1, #0xDEAD
; Store 0xDEADBEEF five times, i.e. (4 bytes)*5 = 20 1-byte values
; starting from 0x2000.0000
    STR R1, [R0]
    STR R1, [R0, #OS]!
    STR R1, [R0, #OS]!
    STR R1, [R0, #OS]!
	  STR R1, [R0, #OS]!

; Starting point where we can take values from
; Rest one so we can use the updated pointer.
    MOVW R0, #0xFFFF ;[2000.0000 - 1] = 1FFF.FFFF
    MOVT R0, #0x1FFF

	MOV R1, #0 ; Guardar cada iteracion en R1 de la suma
;SUM
	LDR R1, [R0, #1]!
	LDR R1, [R0, #OS]!
	LDR R1, [R0, #OS]!
	LDR R1, [R0, #OS]!
	LDR R1, [R0, #OS]!
	LDR R1, [R0, #OS]!




fin B fin
	.end
