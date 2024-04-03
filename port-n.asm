;** EL PROGRAMA INICIALIZA EL PUERTO N COMO SALIDA y

;** CONMUTA SU VALOR

	.global main

	.text ;define codigo del programa y lo ubica en flash

SYSCTL_RCGCGPIO_R .field 0x400FE608,32 ; REGISTRO DEL RELOJ
GPIO_PORTN_DIR_N .field 0x40064400,32 ; Registro de Dirección N
GPIO_PORTN_DEN_N .field 0x4006451C,32 ; Registro de habilitación N
GPIO_PORTN_DATA_N .field 0x4006400C,32 ; Registro de Datos N

LEDS .EQU 0x03 ; se asigna el valor 0x20 a el simbolo LEDS
LEDS2 .EQU 0X00 ; se asigna el valor 0x00 a el simbolo LEDS2


loop_delay; (1)
	SUBS R10, #0x1 ; Decrementar el contador (2)
    BNE loop_delay ; Repetir si el contador no cero (3)
    BX LR ; Regresa al programa principal

main

	LDR R1,SYSCTL_RCGCGPIO_R ; 1) activar el reloj del puerto N
	LDR R0, [R1]
	ORR R0, R0, #0x1000 ; se valida el bit 12 para habilitar el reloj
	STR R0, [R1]
	NOP
	NOP ; se da tiempo para que el reloj se habilite
	LDR R1,GPIO_PORTN_DIR_N ; Determina la dirección del puerto
	MOV R0, #0xFF ; PN0 PN1 salidas
	STR R0,[R1]
	LDR R1,GPIO_PORTN_DEN_N ; Habilita al puerto digital N
	MOV R0, #0xFF
	STR R0,[R1]
	LDR R1,GPIO_PORTN_DATA_N ; apunta al Puerto de datos N
	AND R0,#LEDS

; salto
salto STR R0,[R1] ; Escribe en el registro del datos del Puerto N
	MOVW R10, #0xB0A9 ; 0x0028.B0A9 = 2.6 millones veces se repite el proceso
	MOVT R10, #0x0028 ; aquí se consumen 0.5[s] y son 3 instrucciones por ciclo
	; 2.6 millones de veces con T = 62.5[ns] por 3 instrucciones consumen 0.5s
	BL loop_delay ; #valor del contador = 0.5[s]/(3*T) = 2.6E6 =  0x0028.B0A9

	MOV R2, #LEDS2
	STR R2,[R1] ;apaga
	MOVW R10, #0xB0A9 ; mismo proceso
	MOVT R10, #0x0028 ;
	BL loop_delay
	B salto


; -----------------

.end
