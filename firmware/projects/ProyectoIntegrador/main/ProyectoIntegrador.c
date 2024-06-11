/*! @mainpage ProyectoIntegrador
 *
 * @section genDesc General Description
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
 * | 16/05/2024 | Document creation		                         |
 *
 * @author Venacio Natasha (natasha.venacio@ingenieria.uner.edu.ar)
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gpio_mcu.h"
#include "hc_sr04.h"
#include "timer_mcu.h"
#include "uart_mcu.h"
#include "analog_io_mcu.h"
#include "si7007.h"
#include "ble_mcu.h"
#include "ldr.h"
/*==================[macros and definitions]=================================*/
uint16_t temperatura;
float luz;
uint16_t humedad;
TaskHandle_t main_task_handle = NULL;
#define GPIO_vent GPIO_21
#define GPIO_foco GPIO_22
#define GPIO_humi GPIO_10
/*==================[internal data definition]===============================*/
void FuncionTask1(); 
void TaskSensar ();
void TaskVerificar();
void VerificarTemperatura();
void VerificarHumedad();
void VerificarLuz();
void AppBluetooh();
/*==================[internal functions declaration]=========================*/

void TaskSensar (){

while(true) {

	vTaskDelay(1000/portTICK_PERIOD_MS);
	temperatura= Si7007MeasureTemperature();
	
	humedad=Si7007MeasureHumidity();
	luz=LdrMeasureLight(); 

	printf("Valores de temperatura %u \n", temperatura);
	printf("Valores de humedad %u \n", humedad);
	printf("Valores de luz  %f \n", luz);

	
	}

}

void TaskVerificar(){

	while(true){
			vTaskDelay(1000/portTICK_PERIOD_MS);
		VerificarTemperatura();
		VerificarHumedad();
		VerificarLuz();
	}
}	
void VerificarTemperatura(){

	if(temperatura>30){ //valor de tension referido a los 30 grados 
	GPIOOn(GPIO_vent);
	}
	else {
	GPIOOff(GPIO_22);	
	}
}

void VerificarHumedad(){

	if(humedad<60){ // valor de tension referido al 60% de la humedad 
	GPIOOn(GPIO_humi);
	}
	else {
		GPIOOff(GPIO_humi);
	}
}

void VerificarLuz(){

	if(luz<0) { //Compara en lux 

		GPIOOn(GPIO_foco); 

	}
	else {
		GPIOOff(GPIO_foco);
	}
}


void AppBluetooh(){

}
/*==================[external functions definition]==========================*/
void app_main(void){

//Inicializaciones 
GPIOInit(GPIO_10, GPIO_OUTPUT);
GPIOInit(GPIO_21, GPIO_OUTPUT);
GPIOInit(GPIO_22, GPIO_OUTPUT);
LdrInit();


	 Si7007_config SensorTask= {
		.select=GPIO_9,		
	    .PWM_1= CH1,		   
	    .PWM_2= CH2,
	 };

	Si7007Init(&SensorTask);
	
	xTaskCreate(&TaskSensar, "Sensando", 512, NULL, 5, &main_task_handle);
	xTaskCreate(&TaskVerificar, "Verificando", 512, NULL, 5, &main_task_handle); 

}
/*==================[end of file]============================================*/