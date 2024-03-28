/*! @mainpage Ejercicio 6 del Proyecto 1 
 *
 * @section genDesc General Description
 *
 * El ejercicio 6 del proyecto 1, reutiliza las funciones creadas por los ejercicios 4 
 * y 5, con el fin de poder mostrar por display los valores que reciben.
 * La funcion del ejercicio 4 recibe un dato y este lo convierte a un codigo BCD.
 * La funcion del ejercicio 5, toma un digito digito BCD y evalua el estado del bit 
 * correspondiente de BCD.
 *
 * 
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
 * | 21/03/2024 | Document creation		                         |
 *
 * @author Venacio Natasha 
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

/** 
 * @struct gpioConf_t
 * @brief es una estructura que guarda informacion sobre el el numero del GPIO, y el estado si es de entrada o salida. 
 * 
 * 
 * /
*/
typedef struct
{
	gpio_t pin;			/*!< GPIO pin number */
	io_t dir;			/*!< GPIO direction '0' IN;  '1' OUT*/
} gpioConf_t;

/** @fn void convertToBcdArray (uint32_t data, uint8_t digits, uint8_t * bcd_number)
 * @brief es una funcion que convierte un dato a codigo BCD.
 * @param . recibe como parametro el dato, la cantidad de digitos de la salida y un puntero a un arreglo al cual se almacenan los n digitos. 
 * @return
 * 
 *  
 * /
*/
void convertToBcdArray (uint32_t data, uint8_t digits, uint8_t * bcd_number)
{
	for( int i=digits-1; i>=0;i--)
	{
		bcd_number[i]= data%10;
		data=data/10;
	}
}
/** @fn void funcionBCD(uint8_t dig, gpioConf_t *vec)
 * @brief verifica el estado cada bit del digito BCD y lo  cambia. 
 * @param . recibe como parametro el digito en BCD y un puntero a un arreglo del tipo gpioConf_t
 * @return
*/

void funcionBCD(uint8_t dig, gpioConf_t *vec) 
{
	for(uint8_t i=0; i<4; i++)
	{
		GPIOInit(vec[i].pin, vec[i].dir);
	}
 	for ( int i=0; i<4;i++)
   	{
		if((dig & 1<<i)==0)
	{
    	 GPIOOff(vec[i].pin);

	}
		else 
		GPIOOn(vec[i].pin);
	}

 }

/**  @fn void mostrardisplay ( uint32_t data, uint8_t digit, gpioConf_t *vec,  gpioConf_t *map)
 *  @brief llama a las funciones anteriores para que en conjunto muestren el valor dado por display.
 * @param . tiene como parametro el dato, la cantidad de digitos de la calida, un puntero a un vector de tipo gpioConf_t,   y otro puntero a un vector que actua de mapeo de tipo gpioConf_t
 *  @return 
*/

void mostrardisplay ( uint32_t data, uint8_t digit, gpioConf_t *vec,  gpioConf_t *map)
{
	uint8_t arreglo[3];

 	convertToBcdArray(data,digit,arreglo);

	for (int i=0; i<3; i++)
	{
		GPIOInit(map[i].pin, map[i].dir);
	}

	for( int i=0; i<digit; i++)
	{
	funcionBCD(arreglo[i], vec);
    GPIOOn(map[i].pin);
	GPIOOff(map[i].pin);
	}

}

/*==================[external functions definition]==========================*/
void app_main(void){

 gpioConf_t vector[4]={{ GPIO_20,GPIO_OUTPUT},{GPIO_21,GPIO_OUTPUT},{ GPIO_22,GPIO_OUTPUT}, { GPIO_23,GPIO_OUTPUT}}; 
gpioConf_t mapeo[3]={{GPIO_19, GPIO_OUTPUT},{GPIO_18,GPIO_OUTPUT},{GPIO_9,GPIO_OUTPUT}};
 mostrardisplay(145,3,vector,mapeo);

}
/*==================[end of file]============================================*/