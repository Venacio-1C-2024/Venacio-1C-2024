/*! @mainpage Template
 *
 * @section genDesc General Description
 *
 * This section describes how the program works.
 *
 * <a href="https://drive.google.com/...">Operation Example</a>
 *
 * @section hardConn Hardware Connection
 *
 * |    Peripheral  |   ESP32   	|
 * |:--------------:|:--------------|
 * | 	PIN_X	 	| 	GPIO_X		|
 *
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
#include "gpio_mcu.h"


/*==================[macros and definitions]=================================*/

/*==================[internal data definition]===============================*/

/*==================[internal functions declaration]=========================*/
typedef struct
{
	gpio_t pin;			/*!< GPIO pin number */
	io_t dir;			/*!< GPIO direction '0' IN;  '1' OUT*/
} gpioConf_t;

void funcionBCD(uint8_t dig, gpioConf_t *vec) 
{
	for(uint8_t i=0; i<4; i++) //Inicializo los GPIO
	{
		GPIOInit(vec[i].pin, vec[i].dir);
	}
 	for ( int i=0; i<4;i++) 
   	{
		if((dig & 1<<i)==0) // Si el bit es cero, GPIOOFF
	{
    	 GPIOOff(vec[i].pin);

	}
		else 
		GPIOOn(vec[i].pin);

 }
}

/*==================[external functions definition]==========================*/
void app_main(void){
//En cada posicion del arreglo, le asigno un GPIO y su estado. 
gpioConf_t arreglo[4]= {{ GPIO_20,GPIO_OUTPUT},{ GPIO_21,GPIO_OUTPUT}, { GPIO_22,GPIO_OUTPUT}, { GPIO_23,GPIO_OUTPUT}}; 

funcionBCD(6,arreglo);

}
/*==================[end of file]============================================*/