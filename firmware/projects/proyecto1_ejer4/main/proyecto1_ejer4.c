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

/*==================[macros and definitions]=================================*/

/*==================[internal data definition]===============================*/

/*==================[internal functions declaration]=========================*/
void convertToBcdArray (uint32_t data, uint8_t digits, uint8_t * bcd_number)
{
	for( int i=digits-1; i>=0;i--) //Recorre de atras hacia adelante. 
	{
		bcd_number[i]= data%10;
		data=data/10;
	}
}


/*==================[external functions definition]==========================*/
void app_main(void){

	uint8_t arreglo[3];
	convertToBcdArray(456,3,arreglo); 
	
	
}
/*==================[end of file]============================================*/