// Interrupción por flanco de bajada en PJ0 = SW1
// La Rutina de Servicio de Interrupción incrementa a un  contador.


#include <stdint.h>

#define SYSCTL_RCGCGPIO_R       (*((volatile uint32_t *)0x400FE608))
#define SYSCTL_PRGPIO_R         (*((volatile unsigned long *)0x400FEA08))
#define GPIO_PORTJ_DIR_R        (*((volatile uint32_t *)0x40060400)) //Registro de Dirección PJ
#define GPIO_PORTJ_DEN_R        (*((volatile uint32_t *)0x4006051C)) //Registro de habilitación PJ
#define GPIO_PORTJ_PUR_R        (*((volatile uint32_t *)0x40060510)) //Registro de pull-up PJ
#define GPIO_PORTJ_DATA_R       (*((volatile uint32_t *)0x40060004)) //Registro de Datos J
#define GPIO_PORTJ_IS_R         (*((volatile uint32_t *)0x40060404)) //Registro de configuración de detección de nivel o flanco
#define GPIO_PORTJ_IBE_R        (*((volatile uint32_t *)0x40060408)) //Registro de configuración de interrupción por ambos flancos
#define GPIO_PORTJ_IEV_R        (*((volatile uint32_t *)0x4006040C)) //Registro de configuración de interrupción por un flanco
#define GPIO_PORTJ_ICR_R        (*((volatile uint32_t *)0x4006041C)) //Registro de limpieza de interrupcion de flanco en PJ
#define GPIO_PORTJ_IM_R         (*((volatile uint32_t *)0x40060410)) //Registro de mascara de interrupcion PJ (p.764)
#define NVIC_EN1_R              (*((volatile uint32_t *)0xE000E104)) // Registro de habilitación de interrupción PJ
#define NVIC_PRI12_R             (*((volatile uint32_t *)0xE000E430))//Registro de prioridad de interrupción
int i=0;


// Incrementa la variable una vez cada que se presiona SW1 -Interrupcion PJ => #51 (p. 115)
volatile uint32_t Flancosdebajada = 0;
void EdgeCounter_Init(void){
  SYSCTL_RCGCGPIO_R |= 0x00000100; // (a) activa el reloj para el puerto J
  Flancosdebajada = 0;                // (b) inicializa el contador
  GPIO_PORTJ_DIR_R &= ~0x03;       // (c) PJ0 dirección entrada - boton SW1
  GPIO_PORTJ_DEN_R |= 0x03;        //     PJ0 se habilita
  GPIO_PORTJ_PUR_R |= 0x03;        //     habilita weak pull-up on PJ1
  GPIO_PORTJ_IS_R &= ~0x03;        // (d) PJ1 es sensible por flanco (p.761)
  GPIO_PORTJ_IBE_R &= ~0x03;       //     PJ1 no es sensible a dos flancos (p. 762)
  GPIO_PORTJ_IEV_R &= ~0x03;       //     PJ1 detecta eventos de flanco de bajada (p.763)
  GPIO_PORTJ_ICR_R = 0x03;         // (e) limpia la bandera 0 (p.769)
  GPIO_PORTJ_IM_R |= 0x03;         // (f) Se desenmascara la interrupcion PJ0 y se envia al controlador de interrupciones (p.764)
  NVIC_PRI12_R = (NVIC_PRI12_R&0x00FFFFFF)|0x20000000; // (g) prioridad 0  (p. 159)
  NVIC_EN1_R= 1<<(51-32);          //(h) habilita la interrupción 51 en NVIC (p. 154)

}
void GPIOPortJ_Handler(void)
{
  GPIO_PORTJ_ICR_R = 0x03;      // bandera de confirmación
  Flancosdebajada = Flancosdebajada + 1;
  for(i=0;i<80000;i++)
  {

  }
}

//Programa principal
int main(void){
  EdgeCounter_Init();           // inicializa la interrupción en el puerto GPIO J
  while(1);
}

