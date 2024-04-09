// Interrupción por flanco de bajada en PJ0 = SW1
// La Rutina de Servicio de Interrupción incrementa a un contador.

#include <stdint.h>


/*--------------------------------------------------------------------------------------------
 * Registro de reloj para los puertos
 * -----------------------------------------------------------------------------------------*/
#define SYSCTL_RCGCGPIO_R       (*((volatile uint32_t *)0x400FE608))
#define SYSCTL_PRGPIO_R         (*((volatile unsigned long *)0x400FEA08))

#define NVIC_ST_CTRL_R          (*((volatile unsigned long *)0xE000E010)) //apuntador del registro que permite configurar las acciones del temporizador
#define NVIC_ST_RELOAD_R        (*((volatile unsigned long *)0xE000E014)) //apuntador del registro que contiene el valor de inicio del contador
#define NVIC_ST_CURRENT_R       (*((volatile unsigned long *)0xE000E018)) //apuntador del registro que presenta el estado de la cuenta actual

#define NVIC_ST_CTRL_COUNT     0x00010000   //Bandera que se levanta cuando llega a 0 la cuenta
#define NVIC_ST_CTRL_CLK_SRC   0x00000004 // Clock Source
#define NVIC_ST_CTRL_INTEN     0x00000002 // Habilitador de interrupción
#define NVIC_ST_CTRL_ENABLE    0x00000001 // Modo del contador
#define NVIC_ST_RELOAD_M       0x00FFFFFF // Valor de carga del contador

/*--------------------------------------------------------------------------------------------
 * Registros del puerto J
 * -----------------------------------------------------------------------------------------*/
#define GPIO_PORTJ_DIR_R (*((volatile uint32_t *)0x40060400)) //Registro de Dirección PJ
#define GPIO_PORTJ_DEN_R (*((volatile uint32_t *)0x4006051C)) //Registro de habilitación PJ
#define GPIO_PORTJ_PUR_R (*((volatile uint32_t *)0x40060510)) //Registro de pull-up PJ
#define GPIO_PORTJ_DATA_R (*((volatile uint32_t *)0x40060004)) //Registro de Datos J

#define GPIO_PORTJ_IS_R (*((volatile uint32_t *)0x40060404)) //Registro de configuración de detección de nivel o flanco
#define GPIO_PORTJ_IBE_R (*((volatile uint32_t *)0x40060408)) //Registro de configuración de interrupción por ambos flancos
#define GPIO_PORTJ_IEV_R (*((volatile uint32_t *)0x4006040C)) //Registro de configuración de interrupción por un flanco
#define GPIO_PORTJ_ICR_R (*((volatile uint32_t *)0x4006041C)) //Registro de limpieza de interrupcion de flanco en PJ
#define GPIO_PORTJ_IM_R (*((volatile uint32_t *)0x40060410)) //Registro de mascara de interrupcion PJ (p.764)

#define NVIC_EN1_R (*((volatile uint32_t *)0xE000E104)) // Registro de habilitación de interrupción PJ, interrupción X
#define NVIC_PRI12_R (*((volatile uint32_t *)0xE000E430))//Registro de prioridad de interrupción



/*--------------------------------------------------------------------------------------------
 * Registros del puerto E 0x4005.C000
 * -----------------------------------------------------------------------------------------*/
#define GPIO_PORTE_DIR_R (*((volatile uint32_t *)0x4005C400)) //Registro de Dirección PE
#define GPIO_PORTE_DEN_R (*((volatile uint32_t *)0x4005C51C)) //Registro de habilitación PE
#define GPIO_PORTE_PUR_R (*((volatile uint32_t *)0x4005C510)) //Registro de pull-up PE
#define GPIO_PORTE_DATA_R (*((volatile uint32_t *)0x4005C004)) //Registro de Datos PE

#define GPIO_PORTE_IS_R (*((volatile uint32_t *)0x4005C404)) //Registro de configuración de detección de nivel o flanco
#define GPIO_PORTE_IBE_R (*((volatile uint32_t *)0x4005C408)) //Registro de configuración de interrupción por ambos flancos
#define GPIO_PORTE_IEV_R (*((volatile uint32_t *)0x4005C40C)) //Registro de configuración de interrupción por un flanco
#define GPIO_PORTE_ICR_R (*((volatile uint32_t *)0x4005C41C)) //Registro de limpieza de interrupcion de flanco en PJ
#define GPIO_PORTE_IM_R (*((volatile uint32_t *)0x4005C410)) //Registro de mascara de interrupcion PE (p.764)

#define NVIC_EN0_R (*((volatile uint32_t *)0xE000E100)) // Registro de habilitación de interrupción PE, interrupcion 4
#define NVIC_PRI1_R (*((volatile uint32_t *)0xE000E404))//Registro de prioridad de interrupción



//------------------------------------------------------------------------
//                              Puerto N
//------------------------------------------------------------------------
#define GPIO_PORTN_DATA_R       (*((volatile unsigned long *)0x4006400C)) // Registro de Datos Puerto N
#define GPIO_PORTN_DIR_R        (*((volatile unsigned long *)0x40064400)) // Registro de Dirección Puerto N
#define GPIO_PORTN_DEN_R        (*((volatile unsigned long *)0x4006451C)) // Registro de Habilitación Puerto N




int i = 0;
// Incrementa la variable una vez cada que se presiona SW1 -Interrupcion PJ =&gt; #51 (p. 115)
volatile uint32_t Flancosdebajada = 0;


void EdgeCounter_Init(void){
SYSCTL_RCGCGPIO_R |= 0x00001110; // (a) activa el reloj para el puerto J, E y N
Flancosdebajada = 0; // (b) inicializa el contador

//Puerto N
GPIO_PORTN_DIR_R |= 0x03;    // puerto N de salida
GPIO_PORTN_DEN_R |= 0x03;    // habilita el puerto N

//Puerto J
GPIO_PORTJ_DIR_R &= ~0x03; // (c) PJ0 dirección entrada - boton SW1
GPIO_PORTJ_DEN_R |= 0x03; // PJ0 se habilita
GPIO_PORTJ_PUR_R |= 0x03; // habilita weak pull-up on PJ1
GPIO_PORTJ_IS_R &= ~0x03; // (d) PJ1 es sensible por flanco (p.761)
GPIO_PORTJ_IBE_R &= ~0x03; // PJ1 no es sensible a dos flancos (p. 762)
GPIO_PORTJ_IEV_R &= ~0x03; // PJ1 detecta eventos de flanco de bajada (p.763)
GPIO_PORTJ_ICR_R = 0x03; // (e) limpia la bandera 0 (p.769)
GPIO_PORTJ_IM_R |= 0x03; // (f) Se desenmascara la interrupcion PJ0 y PJ1 y se envia al
//controlador de interrupciones (p.764)
NVIC_PRI12_R = (NVIC_PRI12_R&0x00FFFFFF)|0x20000000; // (g) prioridad 0 (p. 159)
NVIC_EN1_R= 1<<(51-32); //(h) habilita la interrupción 51 en NVIC (p. 154) se realiza el corrimiento

//Puerto E
GPIO_PORTE_DIR_R &= ~0x01; // (c) PJ0 dirección entrada - boton SW1
GPIO_PORTE_DEN_R |= 0x01; // PJ0 se habilita
GPIO_PORTE_PUR_R |= 0x01; // habilita weak pull-up on PJ1
GPIO_PORTE_IS_R &= ~0x01; // (d) PJ1 es sensible por flanco (p.761)
GPIO_PORTE_IBE_R &= ~0x01; // PJ1 no es sensible a dos flancos (p. 762)
GPIO_PORTE_IEV_R &= ~0x01; // PJ1 detecta eventos de flanco de bajada (p.763)
GPIO_PORTE_ICR_R = 0x01; // (e) limpia la bandera 0 (p.769)
GPIO_PORTE_IM_R |= 0x01; // (f) Se desenmascara la interrupcion PJ0 y PJ1 y se envia al
//controlador de interrupciones (p.764)

NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFFFF00)|0x00000000; // (g) prioridad 0 (p. 159)
NVIC_EN0_R= 1<<(4-0); //(h) habilita la interrupción 4 en NVIC (p. 154) se realiza el corrimiento
}



void Temp(void);

void Temp(void){
    NVIC_ST_CTRL_R = 0; // Desahabilita el SysTick durante la configuración
    NVIC_ST_RELOAD_R = NVIC_ST_RELOAD_M; // Se establece el valor de cuenta deseado en RELOAD_R
    NVIC_ST_CURRENT_R = 0; // Se escribe al registro current para limpiarlo
    NVIC_ST_CTRL_R = 0x00000001; // Se Habilita el SysTick y se selecciona la fuente de reloj
}

void SysTick_Wait (uint32_t retardo){
    NVIC_ST_RELOAD_R = retardo - 1; //número de cuentas por esperar
    NVIC_ST_CURRENT_R = 0;
    while((NVIC_ST_CTRL_R & 0x00010000) == 0){//espera hasta que la bandera COUNT sea valida
    }
}

void SysTick_Wait_2s(int retardo){
    int i = 0;
    for(i = 0; i < retardo; i++){
    SysTick_Wait(800000); // Espera 1 s (asume reloj de 16)
    }
}



void GPIOPortJ_Handler(void){
    GPIO_PORTJ_ICR_R = 0x03; // bandera de confirmación
    Flancosdebajada = Flancosdebajada + 1;
    int L;
    for(L = 0; L < 5; L++){
        GPIO_PORTN_DATA_R = 0x03;
        SysTick_Wait_2s(5);
        GPIO_PORTN_DATA_R = 0x00;
        SysTick_Wait_2s(5);
    }
}

void GPIOPortE_Handler(void){
    GPIO_PORTE_ICR_R = 0x01;
    Flancosdebajada = Flancosdebajada + 1;
    int L;
    for(L = 0; L < 10; L++){
        GPIO_PORTN_DATA_R = 0x03;
        SysTick_Wait_2s(1);
        GPIO_PORTN_DATA_R = 0x00;
        SysTick_Wait_2s(1);
    }
}



//Programa principal
int main(void){
    EdgeCounter_Init(); // inicializa la interrupción en el puerto GPIO J
    Temp();

    while(1);
}
