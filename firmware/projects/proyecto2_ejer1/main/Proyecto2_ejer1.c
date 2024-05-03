/*! @mainpage Proyecto2_ejer1
 *
 * @section genDesc General Description
 *
 * Este proyecto mide la distancia sensada por un sensor de ultrasonido HC-SR04, luego
 * muestra la distancia en "cm" en una pantalla LCD, y dependiendo la distancia que
 * mida, encendera los LEDs (LED_1, LED_2, LED_3) correspondientes, esto lo realiza  utilizando los drivers provistos
 * por la cátedra para implementar la aplicación adecuada.
 * 
 * 
 *
 * @section hardConn Hardware Connection
 *
 * |    Peripheral  |   ESP32   	|
 * |:--------------:|:--------------|
 * | 	GPIO_3	    | 	ECHO		|
 * | 	GPIO_2	 	| 	TRIGGER		|
 * | 	 +5V	 	| 	 +5V		|
 * |  	 GND	 	| 	 GND	 	|
 *
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 11/04/2024 | Document creation		                         |
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

/*==================[macros and definitions]=================================*/
//Variables globales 
bool on = false;
bool hold = false;
uint16_t d;
// Los utilizo como periodos 
#define CONFIG_BLINK_PERIOD 1000
#define CONFIG_BLINK_PERIOD1 200
/*==================[internal data definition]===============================*/
/** @fn void Visualizar ()
 * @brief Es una funcion que actualiza los leds y en caso de que la TEC2 este apretada congela 
 * mantiene el resultado
 * @param   
 * @return
 *  * /
*/

void Visualizar ();

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

/** @fn void Teclas()
 * @brief Es una tarea que cambia el estado de las opciones "on" y "hold" 
 * dependiendo de las teclas TEC1 y TEC2 
 * @param 
 * @return  
 *  * /
*/
void Teclas();
/*==================[internal functions declaration]=========================*/

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

		if (on == true)
		{
			
			d = HcSr04ReadDistanceInCentimeters();
			Visualizar();
			
		}
		else
		{
			
			LedOff(LED_1);
			LedOff(LED_2);
			LedOff(LED_3);
			LcdItsE0803Off();
		
		}
	vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
	}
}

void Visualizar()
{
			ActualizarLed();

			if (hold == true)
			{

				vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
			}
			else
			{
				ActualizarDisplay();
			}
	
}
void Teclas()
{
	uint8_t teclas;
	while (true)
	{
		teclas = SwitchesRead();
		switch (teclas)
		{
		case SWITCH_1:
			on = !on;
			break;
		case SWITCH_2:
			hold = !hold;
			break;
		}
		vTaskDelay(CONFIG_BLINK_PERIOD1 / portTICK_PERIOD_MS);
	}
}

/*==================[external functions definition]==========================*/
void app_main(void)
{
	//Inicializaciones 
	HcSr04Init(GPIO_3, GPIO_2);
	LedsInit();
	SwitchesInit();
	LcdItsE0803Init();

    // Llamada a las tareas 

	xTaskCreate(&Teclas, "teclas_task", 512, NULL, 5, NULL);
	xTaskCreate(&MedirDistancia, "distancia_task", 512, NULL, 5, NULL);
}
/*==================[end of file]============================================*/