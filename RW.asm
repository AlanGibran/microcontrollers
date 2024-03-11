; PROGRAMA LECTURA Y ESCRITURA DE PUERTOS PARALELOS
;  Lee PJ0 y PJ1 y escribe el dato leido en PN0 y PN1

  .global main


       .text                 ;define codigo del programa y lo ubica en flash

SYSCTL_RCGCGPIO_R  .field 0x400FE608,32    ; REGISTRO DEL RELOJ (p.382)

GPIO_PORTN_DIR_N     .field 0x40064400,32    ; Registro de Dirección N (p.760)
GPIO_PORTN_DEN_N     .field 0x4006451C,32    ; Registro de habilitación N (p.781)
GPIO_PORTN_DATA_N    .field 0x4006400C,32    ; Registro de Datos N (p. 759 + direccionamiento de cada bit)


GPIO_PORTN_DIR_J     .field 0x40060400,32    ; Registro de Dirección J (p.760)
GPIO_PORTN_PUR_J     .field 0x40060510,32    ; Registro de pull-up J
GPIO_PORTN_DEN_J     .field 0x4006051C,32    ; Registro de habilitación J (p.781)
GPIO_PORTN_DATA_J    .field 0x4006000C,32    ; Registro de Datos J (p. 759 + direccionamiento de cada bit)



SW     .EQU 0x03

main
      LDR R1,SYSCTL_RCGCGPIO_R      ; 1) activar el reloj del puerto N y J (p.382)
      LDR R0, [R1]
      ORR R0, R0, #0x1100           ; se valida el bit 12 para habilitar el reloj N y J
      STR R0, [R1]
      NOP
      NOP                           ; se da tiempo para que el reloj se habilite
;---------------------------------------------------------------
;                    CONFIGURACION PUERTO J
;---------------------------------------------------------------

      LDR R1,GPIO_PORTN_DIR_J   ; Configura la dirección del registro J (p.760)
      MOV R0, #0X00             ; PJ1 PJ2 entradas
      STR R0,[R1]

      LDR R1,GPIO_PORTN_PUR_J   ; Configura los resistores pull_up J
      MOV R0, #0X03             ; habilita weak pull up para PJ0 PJ1
      STR R0,[R1]


      LDR R1,GPIO_PORTN_DEN_J   ; Habilita al puerto digital J (p.781)
      MOV R0, #0XFF             ; 1 significa que habilita E/S
      STR R0,[R1]

;---------------------------------------------------------------
;                    CONFIGURACION PUERTO N
;---------------------------------------------------------------


      LDR R1,GPIO_PORTN_DIR_N       ; Configura la dirección del registro (p.760)
      MOV R0, #0X03                ; PN0 PN1 salidas
      STR R0,[R1]

      LDR R1,GPIO_PORTN_DEN_N       ; Habilita al puerto digital N (p.781)
      MOV R0, #0XFF                 ; 1 significa que habilita E/S
      STR R0,[R1]

; nunca especificamos qué dato iba por defecto en el data de N

;---------------------------------------------------------------
;                    RUTINA DE LECTURA ESCRITURA
;---------------------------------------------------------------

loop  LDR R1,GPIO_PORTN_DATA_J   ; apunta al Puerto de datos J (p. 759* + direccionamiento de cada bit )
      LDR R0,[R1] ; R0 = 0x3 cuándo le metimos 3 data al data_j? Nunca, ya estaba w
      MOV R5,R0; R5 = R0 = 0x3; Siempre tiene ese valor a lo largo del programa. neta?
      CMP R0,#SW ; 3-3 qué activa? al parecer C=1
; BNE se activa cuando Z=0, PERO
      BNE sal; Z=1, entonces no se cumple la condición
;, entonces no sal
      B loop

; solo se va a salir del loop cuando R0 no sea 3.
; Y esto yo lo controlo cuando presiono un boton.
; si boton izquierda, entonces en DATA_N hay un 02
; que es igual a 0000.0010
;si presiono el derecho se escribe un 1.
; 0000.0001

sal   LDR R1,GPIO_PORTN_DATA_N    ; apunta al Puerto de datos N  (p.759 + direccionamiento de cada bit)
      STR R5,[R1]; Prende led ; Escribe en el registro del datos del Puerto N el valor de J
      B loop
;Con 0x1 prende PN0 y con 0x2 prende PN1
;con 0x3 prenden ambos
; -----------------


      .end
