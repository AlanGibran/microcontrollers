// USB button - 0.5 s on, boosterpack2 button, 2s on
/*PROGRAMA EN C PARA CONTROLAR LA DIRECCION DE UN MOTOR DE DC
USANDO EL INTEGRADO L293D*/
/*PUERTOS:
DIRECCION DEL MOTOR
PK0 - INPUT 1
PK1 - INPUT 2
PN0 - LED INDICADOR
PJ0 - BOTON DE DIRECCION (DERECHA)
PJ1 - BOTON DE DIRECCION (IZQUIERDA)*/
//LIBRERIAS
#include <stdbool.h>// Librería para operaciones lógicas*************************************************************************
#include <stdint.h> // Libreria para aritmetica
//DEFINICION DE MACROS DE LOS REGISTROS QUE SE VAN A UTILIZAR


//REGISTROS DE CONTROL Y ESTATUS
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE608)) // Registro de Habilitaciï¿½n de Reloj de Puertos
#define SYSCTL_PRGPIO_R         (*((volatile unsigned long *)0x400FEA08)) // Registro de estatus de Reloj de Puerto

//PUERTO K
#define GPIO_PORTK_DATA_R       (*((volatile unsigned long *)0x4006100C)) // Registro de Datos Puerto K
#define GPIO_PORTK_DIR_R        (*((volatile unsigned long *)0x40061400)) // Registro de Direccion Puerto K
#define GPIO_PORTK_DEN_R        (*((volatile unsigned long *)0x4006151C)) // Registro de Habilitacion Puerto K

//PUERTO N

#define GPIO_PORTN_DATA_R       (*((volatile unsigned long *)0x4006400C)) // Registro de Datos Puerto N
#define GPIO_PORTN_DIR_R        (*((volatile unsigned long *)0x40064400)) // Registro de Direccion Puerto N
#define GPIO_PORTN_DEN_R        (*((volatile unsigned long *)0x4006451C)) // Registro de Habilitacion Puerto N

//PUERTO J

#define GPIO_PORTJ_DIR_R        (*((volatile uint32_t *)0x40060400)) //Registro de DirecciÃ³n PJ
#define GPIO_PORTJ_DEN_R        (*((volatile uint32_t *)0x4006051C)) //Registro de habilitaciÃ³n PJ
#define GPIO_PORTJ_PUR_R        (*((volatile uint32_t *)0x40060510)) //Registro de pull-up PJ
#define GPIO_PORTJ_DATA_R       (*((volatile uint32_t *)0x4006000C)) //Registro de Datos J
//DEFINICION DE CONSTANTES PARA OPERACIONES
// Definiciï¿½n de constantes para operaciones
#define SYSCTL_RCGC2_GPION      0x00001000  // bit de estado del reloj de puerto N
#define SYSCTL_RCGC2_GPIOK      0x00000200  // bit de estado del reloj de puerto K
#define SYSCTL_RCGC2_GPIOJ      0x00000100  // bit de estado del reloj de puerto J

//DECLARACION DE VARIABLES
//***************************************************************************************************************************************
// Definición de apuntadores para direccionar
#define NVIC_ST_CTRL_R          (*((volatile unsigned long *)0xE000E010))  //apuntador del registro que permite configurar las acciones del temporizador
#define NVIC_ST_RELOAD_R        (*((volatile unsigned long *)0xE000E014))  //apuntador del registro que contiene el valor de inicio del contador
#define NVIC_ST_CURRENT_R       (*((volatile unsigned long *)0xE000E018))  //apuntador del registro que presenta el estado de la cuenta actual


// Definición de constantes para operaciones
#define NVIC_ST_CTRL_COUNT      0x00010000  // bandera de cuenta flag
#define NVIC_ST_CTRL_CLK_SRC    0x00000004  // Clock Source
#define NVIC_ST_CTRL_INTEN      0x00000002  // Habilitador de interrupción
#define NVIC_ST_CTRL_ENABLE     0x00000001  // Modo del contador
#define NVIC_ST_RELOAD_M        0x00FFFFFF  // Valor de carga del contador

//**********FUNCION Inizializa el SysTick *********************
void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;                   // Desahabilita el SysTick durante la configuración
  NVIC_ST_RELOAD_R = NVIC_ST_RELOAD_M;  // Se establece el valor de cuenta deseado en RELOAD_R
  NVIC_ST_CURRENT_R = 0;                // Se escribe al registro current para limpiarlo

  NVIC_ST_CTRL_R = 0x00000001;         // Se Habilita el SysTick y se selecciona la fuente de reloj
}

//*************FUNCION Tiempo de retardo utilizando wait.***************
// El parametro de retardo esta en unidades del reloj interno/4 = 4 MHz (250 ns)
void SysTick_Wait(uint32_t retardo){
    NVIC_ST_RELOAD_R= retardo-1;   //número de cuentas por esperar
    NVIC_ST_CURRENT_R = 0;
    while((NVIC_ST_CTRL_R&0x00010000)==0){//espera hasta que la bandera COUNT sea valida
    }
   } //

// Tiempo de retardo utilizando wait
// 4000000 igual a 1 s
void SysTick_Wait_2s(int retardo){
    int i=0;
    for(i=0; i<retardo; i++){
    SysTick_Wait(4000000);  // Espera 1 s (asume reloj de  4 MHz)
  }
}


//***************************************************************************************************************************************

int i;

//DECLARACION DE FUNCIONES
void InitPorts(void);
void ConfiPorts(void);
void Leds(void);

int main(){
    InitPorts(); // hasta aquí vamos bien. same as SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPION;
// aqui presumiblemente tendría que ir ela función
    //SysTick init


    SysTick_Init();           // inicializo SysTick

    ConfiPorts();

    Leds();

    while (1)
    {
        switch (GPIO_PORTJ_DATA_R)
        {
        case 0x01:
        Leds();
        GPIO_PORTK_DATA_R = 0x01;
        //for ( i = 0; i < 500000 ; i++){} // Esta es la parte que se tiene que cambiar
        SysTick_Wait_2s(2);    // aproximadamente 2 s
        GPIO_PORTK_DATA_R = 0x0;
            break;

        case 0x02:
        Leds();
        GPIO_PORTK_DATA_R = 0x02;
        for ( i = 0; i < 500000 ; i++){}
        GPIO_PORTK_DATA_R = 0x0;
            break;
        }
    }
}

void InitPorts(void){
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPION|SYSCTL_RCGC2_GPIOK|SYSCTL_RCGC2_GPIOJ; // activa reloj a los puertos K,N,J
    while ((SYSCTL_PRGPIO_R & SYSCTL_RCGC2_GPIOK|SYSCTL_RCGC2_GPION|SYSCTL_RCGC2_GPIOJ) == 0){};  // reloj listo?
}

void ConfiPorts(void){

    //CONFIGURACION PUERTO N
    GPIO_PORTN_DIR_R |= 0x03;    // puerto N de salida
    GPIO_PORTN_DEN_R |= 0x03;    // habilita el puerto N

    //CONFIGURACION PUERTO K
    GPIO_PORTK_DIR_R |= 0x03;    // puerto K de salida
    GPIO_PORTK_DEN_R |= 0x03;    // habilita el puerto K

    //CONFIGURACION PUERTO J
    GPIO_PORTJ_DIR_R &= ~0x03;       // (c) PJ0 dirección entrada - boton SW1
    GPIO_PORTJ_DEN_R |= 0x03;        //     PJ0 se habilita
    GPIO_PORTJ_PUR_R |= 0x03;        //     habilita weak pull-up on PJ1

}

void Leds(void){
    int L;
    GPIO_PORTN_DATA_R = 0x02;
    for ( L = 0; L < 80000; L++){}
    GPIO_PORTN_DATA_R = 0x00;
    for ( L = 0; L < 80000; L++){}
    GPIO_PORTN_DATA_R = 0x02;
    for ( L = 0; L < 80000; L++){}
    GPIO_PORTN_DATA_R = 0x00;
    for ( L = 0; L < 80000; L++){}
}

// Document this 
