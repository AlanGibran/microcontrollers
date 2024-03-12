// GPIO.c

#include<stdint.h>
// Definición de apuntadores para direccionar
#define GPIO_PORTN_DATA_R       (*((volatile unsigned long *)0x4006400C)) // Registro de Datos Puerto N
#define GPIO_PORTN_DIR_R        (*((volatile unsigned long *)0x40064400)) // Registro de Dirección Puerto N
#define GPIO_PORTN_DEN_R        (*((volatile unsigned long *)0x4006451C)) // Registro de Habilitación Puerto N
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE608)) // Registro de Habilitación de Reloj de Puertos
#define SYSCTL_PRGPIO_R         (*((volatile unsigned long *)0x400FEA08)) // Registro de estatus de Reloj de Puerto

// Definición de constantes para operaciones
#define SYSCTL_RCGC2_GPION      0x00001000  // bit de estado del reloj de puerto N


 main(void){
     uint32_t Num;
     Num = 0;

 SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPION; // activa reloj puerto N

  while ((SYSCTL_PRGPIO_R & 0X1000) == 0){};  // reloj listo?

  GPIO_PORTN_DIR_R |= 0x0F;    // puerto N de salida
  GPIO_PORTN_DEN_R |= 0x0F;    // habilita el puerto N

  GPIO_PORTN_DATA_R = 0x02;    // enciende 1 led

  while(1){

      if(Num < 675000){
          Num = Num + 1;
      }
      else{
          GPIO_PORTN_DATA_R ^= 0x03;  // conmuta encendido de leds
          Num = 0;
      }
  }
}
