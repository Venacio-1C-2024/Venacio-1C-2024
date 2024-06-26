/*! @mainpage Blinking switch
 *
 * \section genDesc General Description
 *
 * This example makes LED_1 and LED_2 blink if SWITCH_1 or SWITCH_2 are pressed.
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 12/09/2023 | Document creation		                         |
 *
 * @author Albano Peñalva (albano.penalva@uner.edu.ar)
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
/*==================[macros and definitions]=================================*/
#define CONFIG_BLINK_PERIOD 1000
/*==================[internal data definition]===============================*/

/*==================[internal functions declaration]=========================*/

/*==================[external functions definition]==========================*/
void app_main(void){
	uint8_t teclas;
	LedsInit();
	SwitchesInit();
    while(1)    {
    	teclas  = SwitchesRead();
    	switch(teclas){
    		case SWITCH_1:
			//Mientras tenga la tecla apretada el Led1 debe titilar 
			//LedToggle(LED_1);
			printf("LED1 ON\n") ;
			LedOn(LED_1);
			vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
			printf("LED1 OFF\n");
			LedOff(LED_1);
			vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
			
    		break;

    		case SWITCH_2:
				printf("LED2 ON\n");
			LedOn(LED_2);
			vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
			printf("LED2 OFF\n");
			LedOff(LED_2);
			vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
    		break;
			case SWITCH_1 | SWITCH_2 : //la suma de las dos teclas 
			 LedOn(LED_3);
			vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
			printf("LED2 OFF\n");
			LedOff(LED_3);
			vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
			 break;
    	}
	 
	}
	
}
