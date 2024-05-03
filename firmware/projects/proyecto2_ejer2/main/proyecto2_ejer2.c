/*! @mainpage proyecto2_ejer2
 *
 * @section genDesc General Description
 *
 * Este proyecto cumple con el mismo fin que el proyecto2_ejer1 pero
 * modificando la actividad del punto 1 de manera de utilizar
 * interrupciones de temporizador para el control de las teclas y el control de tiempos (Timers). 
 *
 * @section hardConn Hardware Connection
 *
 * |    Peripheral  |   ESP32   	|
 * |:--------------:|:--------------|
 * |	GPIO_3	    | 	ECHO		|
 * | 	GPIO_2	 	| 	TRIGGER		|
 * | 	 +5V	 	| 	 +5V		|
 * |  	 GND	 	| 	 GND	 	|
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 13/04/2024 | Document creation		                         |
 *
 *  @author Venacio Natasha (natasha.venacio@ingenieria.uner.edu.ar)
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
/*==================[macros and definitions]=================================*/
//Variables globales 
bool on = false;
bool hold = false;
uint16_t d;
#define CONFIG_BLINK_PERIOD1 1000000
TaskHandle_t Task1_task_handle = NULL;
/*==================[internal data definition]===============================*/

/** @fn void Visualizar ()
 * @brief Es una funcion que actualiza los leds y en caso de que la TEC2 este apretada congela 
 * mantiene el resultado
 * @param   
 * @return
 *  * /
*/
void Visualizar ();

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

/** @fn void CambiarEstado()
 * * @brief Es una funcion que cambia de estado al "on" dependiendo de la tecla TEC1 
 * @param 
 * @return  
 *  * /
*/

void CambiarEstado();


/** @fn void CambiarEstado()
 * * @brief Es una funcion que cambia de estado al "on" dependiendo de la tecla TEC1 
 * @param 
 * @return  
 *  * /
*/
void CambiarResultado ();
/*==================[internal functions declaration]=========================*/

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
			}
			
}

void CambiarEstado() {
	on = !on;
}
void CambiarResultado () {
	 
	 hold = !hold;
}


/*==================[external functions definition]==========================*/


void app_main(void){
	
	HcSr04Init(GPIO_3, GPIO_2);
	LedsInit();
	SwitchesInit();
	LcdItsE0803Init(); 

SwitchActivInt(SWITCH_1, CambiarEstado ,NULL );
SwitchActivInt(SWITCH_2, CambiarResultado, NULL ) ;

 timer_config_t timer_Task_1 = {
        .timer = TIMER_A,
        .period = CONFIG_BLINK_PERIOD1,
        .func_p = FuncionTask1,
        .param_p = NULL
    };
	 TimerInit(&timer_Task_1);

	xTaskCreate(&MedirDistancia, "distancia_task", 512, NULL, 5, &Task1_task_handle);
	TimerStart(timer_Task_1.timer);
 	
}
/*==================[end of file]============================================*/