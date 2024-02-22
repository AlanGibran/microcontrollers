  .global main
main
  ; Adición no signada-> Interpretamos a los números cómo positivos
  MOVW R1, #0xF000 ;El operando basicos será R1=FFFFF000
  MOVT R1, #0xFFFF ; "
  ADD R0, R1, #10 ;La operación no activa el registro de estado
  ADDS R0, R1, #10 ;Registro de estado activado, se puede detectar acarreo
  ADDS R0, R1, #0xFF ; "
  ADDS R0, R1, #0xFF00 ; "
    
  ; Substracción no signada
  SUBS R2, R1,#0x0F
  SUBS R2, R1,#0xFF00
  SUBS R2, R1,#0XFFFFFFFF
  ;-------------------------------------------------------------------------------
    
  ; Adición signadas -> Interpretamos a los números cómo positivos
  MOVW R5, #0xF000 ;El operando basicos será R5=7FFFF000
  MOVT R5, #0x7FFF ; "
  ADD R0, R5, #10
  ADDS R2, R5, #10
  ADDS R2, R5, #0xFF
  ADDS R2, R5, #0xFF00
    
  ; Substracción signada
  SUBS R2, R5,#0x0F
  SUBS R2, R5,#0xFF00
  MOV R2,#0XAA
  SUBS R2, R2,#0xFF00
  SUBS R2,R2,R5
fin NOP
  B fin
  .end 
