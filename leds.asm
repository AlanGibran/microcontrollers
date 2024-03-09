;** 4 leds

	.global main

	.text 
SYSCTL_RCGCGPIO_R .field 0x400FE608,32 ; REGISTRO DEL RELOJ


GPIO_PORTN_DIR_N .field 0x40064400, 32 ; N (input or output)
GPIO_PORTF_DIR_F .field 0x4005D400, 32 ; F


; Tristate, enable the use of the pin as a digital input or output
; (either GPIO or alternate function)
GPIO_PORTN_DEN_N .field 0x4006451C, 32
GPIO_PORTF_DEN_F .field 0x4005D51C, 32


GPIO_PORTN_DATA_N .field 0x4006400C,32 ; 0x0C bc 000.11[00]: PN1 y PN0 with mask
GPIO_PORTF_DATA_F .field 0x4005D044,32 ; 0x44 bc 0100.01[00]: PF4 y PF0 with mask


LEDS .EQU 0x03 ; 0000.0011b: PN[1] & PN[0]. No mask here
LEDS_F .EQU 0x11 ; 00001.0001b: PF[4] y PF[0]
LEDS_OFF .EQU 0X00 ; OFF



loop_delay; (1)
    SUBS R10, #0x1 ; Count -= 1 (2)
    BNE loop_delay ; Repeat if counter not zero  (3)
    BX LR ; back to LR

main

	LDR R1,SYSCTL_RCGCGPIO_R
	LDR R0, [R1]
	ORR R0, R0, #0x1020 ; PN=12 & PF=5; 1.0000.0010.0000
	STR R0, [R1]
	NOP
	NOP

;-----------------------------------------------------------------
	LDR R1,GPIO_PORTN_DIR_N ; input or output
	MOV R0, #0x03 PN1 PN0
	STR R0,[R1] ; write 0000.0011b onto DIR register

	LDR R1, GPIO_PORTF_DIR_F ; input or output
	MOV R0, #0x11 ; PF4 PF0
	STR R0, [R1] ; write 0001.0001b onto DIR register
;-----------------------------------------------------------------

	LDR R1, GPIO_PORTN_DEN_N ; Digital Enable, same logic
	MOV R0, #0x03
	STR R0, [R1]

	LDR R1, GPIO_PORTF_DEN_F ; 
	MOV R5, #0x11
	STR R5, [R1]

;-----------------------------------------------------------------

	LDR R1, GPIO_PORTN_DATA_N ; Gotta write ON/OFF here

	LDR R3, GPIO_PORTF_DATA_F 

salto
	STR R0, [R1] ; Escribe en el registro del datos del Puerto N
	STR R5, [R3]
	MOVW R10, #0xB0A9 ; 0x0028.B0A9 = 2.6 millones veces se repite el proceso
	MOVT R10, #0x0058 ; aquí se consumen 0.5[s] y son 3 instrucciones por ciclo
	; 2.6 millones de veces con T = 62.5[ns] por 3 instrucciones consumen 0.5s
	BL loop_delay ; #valor del contador = 0.5[s]/(3*T) = 2.6E6 =  0x0028.B0A9

	MOV R2, #LEDS_OFF
	STR R2,[R1] ;apaga
	STR R2,[R3] ;apaga
	MOVW R10, #0xB0A9 ; mismo proceso
	MOVT R10, #0x0058 ;
	BL loop_delay
	B salto


; -----------------

.end
