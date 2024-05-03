/*! @mainpage proyecto2_ejer3
 *
 * @section genDesc General Description
 *
 * Es una modificacion del proyecto2_ejer2, cumple con la misma funcion pero 
 * agrega un puerto serie. Envia los datos de las mediciones para poder observarlos
 *  en un terminal en la PC. 
 *
 * @section hardConn Hardware Connection
 *
 * |    Peripheral  |   ESP32   	|
 * |:--------------:|:--------------|
 * | 	GPIO_3  	| 	ECHO		|
 * | 	GPIO_2	 	| 	TRIGGER		|
 * | 	 +5V	 	| 	 +5V		|
 * |  	 GND	 	| 	 GND	 	| 	
 *
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 13/04/2024 | Document creation		                         |
 *
 * @author Venacio Natasha (natasha.venacio@ingenieria.uner.edu.ar)
 *  
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
#include "switch.h"
#include "gpio_mcu.h"
#include "hc_sr04.h"
#include "lcditse0803.h"
#include "timer_mcu.h"
#include "uart_mcu.h"

/*==================[macros and definitions]=================================*/
//Variables globales 
bool on = false;
bool hold = false;
uint16_t d;
// Periodo de tiempo 
#define CONFIG_BLINK_PERIOD1 500000

TaskHandle_t Task1_task_handle = NULL;

/*==================[internal data definition]===============================*/

/** @fn void Visualizar ()
 * @brief Es una funcion que actualiza los leds y en caso de que la TEC2 este apretada congela 
 * mantiene el resultado
 * @param   
 * @return
 *  * /
*/
void Visualizar();


/** @fn void CambiarEstado()
 * * @brief Es una funcion que cambia de estado al "on" dependiendo de la tecla TEC1 
 * @param 
 * @return  
 *  * /
*/
void CambiarEstado() ; 


/** @fn void CambiarEstado()
 * * @brief Es una funcion que cambia de estado al "on" dependiendo de la tecla TEC1 
 * @param 
 * @return  
 *  * /
*/

void CambiarResultado ();

/** @fn void MandarDatos()
 * * @brief Es una funcion que envia  los datos a traves del puerto serie 
 * @param 
 * @return  
 *  * /
*/
void MandarDatos();

/** @fn void LeerTeclas(void* param)
 * * @brief Lee las teclas del teclado de la compu y cambia el estado de la EDU-ESP
 * @param param es un puntero genérico que puede apuntar a cualquier tipo de dato.
 * @return  
 *  * /
*/

void LeerTeclas(void* param);

/** @fn void FuncionTask1 (void* param)
 * * @brief  Envía una notificación a la tarea asociada MedirDistancia 
 * @param param es un puntero genérico que puede apuntar a cualquier tipo de dato.
 * @return  
 *  * /
*/

void FuncionTask1 (void* param);

/** @fn void ActualizarLed()
 * @brief Es una funcion que dependiendo de la distancia que es una variable global
 * va a encender los o el led correspondiente, en caso de que la distancia sea menor a 10 cm
 * ningun led encendera. 
 * @param 
 * @return  
 *  * /
*/

void ActualizarLed();

/** @fn void ActualizarDisplay()
 * * @brief Es una funcion que como su nombre lo indica actualiza el display. 
 * @param 
 * @return  
 *  * /
*/

void ActualizarDisplay();

/** @fn MedirDistancia()
 *  @brief Es una Tarea que si la tecla TEC1 esta en "on" le pasa a la variable d la distancia
 * que mide el sensor y llama  a la funcion visualizar, de otro modo si esta en "off" apaga todos los leds y el display. 
 * @param 
 * @return  
 *  * /
*/

void MedirDistancia();

/*==================[internal functions declaration]=========================*/

void MandarDatos() {

	 UartSendString( UART_PC,(char*)UartItoa(d, 10)); 
	 UartSendString( UART_PC, "cm\r\n"); 

}

void LeerTeclas(void* param) {

	uint16_t tecla;
	  UartReadByte(UART_PC,&tecla);

	 switch (tecla) {
		case 'O':
		 	CambiarEstado();
			 break;
		case 'H': 
			CambiarResultado ();
		break;
	 }
	
	 

}
void FuncionTask1 (void* param) {

	 vTaskNotifyGiveFromISR(Task1_task_handle, pdFALSE);  
}


void ActualizarLed()
{
		if (d <= 10)
		{
			LedOff(LED_1);
			LedOff(LED_2);
			LedOff(LED_3);
			
		}
		else if ((d >= 10) && (d <= 20))
		{
			LedOn(LED_1);
			LedOff(LED_2);
			LedOff(LED_3);
			
		}
		else if ((d >= 20) && (d <= 30))
		{
			LedOn(LED_1);
			LedOn(LED_2);
			LedOff(LED_3);
			
		}
		else if (d >= 30)
		{
			LedOn(LED_1);
			LedOn(LED_2);
			LedOn(LED_3);
		
		}
	
}
void ActualizarDisplay()
{
	LcdItsE0803Write(d);
	
}
void MedirDistancia()
{

	while (true)
	{
		 ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		if (on == true)
		{
			printf("on\n");
			d = HcSr04ReadDistanceInCentimeters();
			Visualizar();
			
		}
		else
		{
			printf("off\n");
			LedOff(LED_1);
			LedOff(LED_2);
			LedOff(LED_3);
			LcdItsE0803Off();
		
		}
	
	}
}

void Visualizar()
{
			ActualizarLed();
            if (hold==false){
				ActualizarDisplay();
				MandarDatos(); 
			}
			
}

void CambiarEstado() {
	on = !on;
}
void CambiarResultado () {
	 
	 hold = !hold;
}


/*==================[external functions definition]==========================*/
void app_main(void) {

    // Inicializaciones 
	HcSr04Init(GPIO_3, GPIO_2);
	LedsInit();
	SwitchesInit();
	LcdItsE0803Init(); 
	
	//Declaraciones de las interrupciones 
	SwitchActivInt(SWITCH_1, CambiarEstado ,NULL );
	SwitchActivInt(SWITCH_2, CambiarResultado, NULL ) ;

	serial_config_t datos= {
	.port= UART_PC,
	.baud_rate = 9600,
	.func_p = &LeerTeclas,
	.param_p= NULL,
	};

 	UartInit(&datos);


 timer_config_t timer_Task_1 = {
        .timer = TIMER_A,
        .period = CONFIG_BLINK_PERIOD1,
        .func_p = FuncionTask1,
        .param_p = NULL,
    };

	 TimerInit(&timer_Task_1);

	//Llamada de la tarea 
	xTaskCreate(&MedirDistancia, "distancia_task", 512, NULL, 5, &Task1_task_handle);
	
	TimerStart(timer_Task_1.timer);
}
/*==================[end of file]============================================*/