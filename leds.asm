;** EL PROGRAMA INICIALIZA EL PUERTO N COMO SALIDA y

;** 4 leds

	.global main

	.text ;define codigo del programa y lo ubica en flash

SYSCTL_RCGCGPIO_R .field 0x400FE608,32 ; REGISTRO DEL RELOJ

GPIO_PORTN_DIR_N .field 0x40064400, 32 ; Registro de Dirección N (input or output)
GPIO_PORTF_DIR_F .field 0x4005D400, 32 ; F

; Tristate, enable the use of the pin as a digital input or output
; (either GPIO or alternate function)
GPIO_PORTN_DEN_N .field 0x4006451C, 32
GPIO_PORTF_DEN_F .field 0x4005D51C, 32

GPIO_PORTN_DATA_N .field 0x4006400C,32 ; Registro de Datos N (mask)
GPIO_PORTF_DATA_F .field 0x4005D044,32

LEDS .EQU 0x03 ; se asigna el valor a el simbolo LEDS
LEDS_OFF .EQU 0X00 ; se asigna el valor 0x00 a el simbolo LEDS2

LEDS_F .EQU 0x11 ; 1001 PF[4] y PF[0]

loop_delay; (1)
	SUBS R10, #0x1 ; Decrementar el contador (2)
    BNE loop_delay ; Repetir si el contador no cero (3)
    BX LR ; Regresa al programa principal

main

	LDR R1,SYSCTL_RCGCGPIO_R
	LDR R0, [R1]
	ORR R0, R0, #0x1020 ; se valida el bit 12(puerto n) y 5(puerto f)
	STR R0, [R1]
	NOP
	NOP

;-----------------------------------------------------------------
	LDR R1,GPIO_PORTN_DIR_N ; Determina la dirección del puerto
	MOV R0, #0xFF ; PN0 PN1 salidas
	STR R0,[R1]

	LDR R1, GPIO_PORTF_DIR_F ; Determina la dirección del puerto
	MOV R0, #0xFF ; PF0 PF1 salidas
	STR R0, [R1] ; PF4 y PF0
;-----------------------------------------------------------------

	LDR R1, GPIO_PORTN_DEN_N ; Habilita al puerto digital N
	MOV R0, #0xFF
	STR R0, [R1]

	LDR R1, GPIO_PORTF_DEN_F ; Habilita al puerto digital F
	MOV R0, #0xFF
	STR R0, [R1]

	MOV R5, #0xFF
;-----------------------------------------------------------------

	LDR R1, GPIO_PORTN_DATA_N ; apunta al Puerto de datos N
	AND R0, #LEDS ; (FF AND LEDS) = LEDS

	LDR R3, GPIO_PORTF_DATA_F ; apunta al Puerto de datos N
	AND R5, #LEDS_F ; (FF AND LEDS) = LEDS

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
