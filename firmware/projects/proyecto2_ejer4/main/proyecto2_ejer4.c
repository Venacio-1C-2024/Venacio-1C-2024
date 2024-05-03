/*! @mainpage Proyecto2_ejer4
 *
 * @section genDesc General Description
 *
 * Proyecto basado en el driver analog_io_mcu.h y el driver de transmisión serie 
 * uart_mcu.h, que digitaliza una señal analógica y la transmita a un graficador
 * de puerto serie de la PC. Se  toma la entrada CH1 del conversor AD y 
 * la transmisión se  realiza por la UART conectada al puerto serie de la PC,
 * en un formato compatible con un graficador por puerto serie. 
 *
 * @section hardConn Hardware Connection
 *
 * |    Peripheral  |   ESP32   	|
 * |:--------------:|:--------------|
 * | 	GPIO_3      | ECHO          |
 * |    GPIO_2      | TRIGGER       |
 * |     +5V        |  +5V          |
 * |      GND       |    GND        |
 * 
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 25/04/2024 | Document creation		                         |
 *
 * @author Venacio Natasha (natasha.venacio@ingenieria.uner.edu.ar)
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
#include "analog_io_mcu.h"

/*==================[macros and definitions]=================================*/
uint16_t dato;
uint16_t i=0;

#define CONFIG_BLINK_PERIOD1 2000
#define BUFFER_SIZE 231
TaskHandle_t main_task_handle = NULL;
const char ecg[BUFFER_SIZE] = {
    76, 77, 78, 77, 79, 86, 81, 76, 84, 93, 85, 80,
    89, 95, 89, 85, 93, 98, 94, 88, 98, 105, 96, 91,
    99, 105, 101, 96, 102, 106, 101, 96, 100, 107, 101,
    94, 100, 104, 100, 91, 99, 103, 98, 91, 96, 105, 95,
    88, 95, 100, 94, 85, 93, 99, 92, 84, 91, 96, 87, 80,
    83, 92, 86, 78, 84, 89, 79, 73, 81, 83, 78, 70, 80, 82,
    79, 69, 80, 82, 81, 70, 75, 81, 77, 74, 79, 83, 82, 72,
    80, 87, 79, 76, 85, 95, 87, 81, 88, 93, 88, 84, 87, 94,
    86, 82, 85, 94, 85, 82, 85, 95, 86, 83, 92, 99, 91, 88,
    94, 98, 95, 90, 97, 105, 104, 94, 98, 114, 117, 124, 144,
    180, 210, 236, 253, 227, 171, 99, 49, 34, 29, 43, 69, 89,
    89, 90, 98, 107, 104, 98, 104, 110, 102, 98, 103, 111, 101,
    94, 103, 108, 102, 95, 97, 106, 100, 92, 101, 103, 100, 94, 98,
    103, 96, 90, 98, 103, 97, 90, 99, 104, 95, 90, 99, 104, 100, 93,
    100, 106, 101, 93, 101, 105, 103, 96, 105, 112, 105, 99, 103, 108,
    99, 96, 102, 106, 99, 90, 92, 100, 87, 80, 82, 88, 77, 69, 75, 79,
    74, 67, 71, 78, 72, 67, 73, 81, 77, 71, 75, 84, 79, 77, 77, 76, 76,
};
/*==================[internal data definition]===============================*/
/** @fn void TaskConversion() 
 * * @brief Es una tarea que lee la señal analogica y luego la transmite a un graficador de puerto serie 
 * @param 
 * @return  
 *  * /
*/
void TaskConversion(); 

/** @fn void FuncionTask1 (void* param)
 * * @brief  Envía una notificación a la tarea asociada MedirDistancia 
 * @param param es un puntero genérico que puede apuntar a cualquier tipo de dato.
 * @return  
 *  * /
*/
void FuncionTask1 (void* param);

/** @fn void FuncionTimerTask()
 * @brief Funcion que convierte una señal digital a analogico 
 * @param 
 * @return
 * /
*/
void FuncionTimerTask();

/*==================[internal functions declaration]=========================*/

void FuncionTask1 (void* param) {

	 vTaskNotifyGiveFromISR(main_task_handle, pdFALSE);  
}

void TaskConversion(){
 
 while(true){

	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	AnalogInputReadSingle(CH1,&dato); 
	UartSendString( UART_PC,(char*)UartItoa(dato, 10)); 
	 UartSendString( UART_PC, "\r");
 }
}


void FuncionTimerTask(){

	if(i<BUFFER_SIZE){
		
		AnalogOutputWrite(ecg[i]);
		i++;
	}
	else {
		i=0;
	}
	
}
/*==================[external functions definition]==========================*/
void app_main(void){
 
 //Declaro los timers  
 //Timer de la frecuencia de muestreo de 500 Hz 
 	timer_config_t timer_Task_1 = {
        .timer = TIMER_A,
        .period = CONFIG_BLINK_PERIOD1,
        .func_p = FuncionTask1,
        .param_p = NULL,
	 };

	 TimerInit(&timer_Task_1);

	analog_input_config_t AnalogicoTask = {
 	.input= CH1,			
	.mode = ADC_SINGLE,			
	.func_p=NULL, 		
	.param_p= NULL,			
	.sample_frec = 0,
	};

	timer_config_t timer_Task_2 = {
        .timer = TIMER_B,
        .period = CONFIG_BLINK_PERIOD1,
        .func_p = FuncionTimerTask,
        .param_p = NULL,
	 };

	 TimerInit(&timer_Task_2);

 	AnalogInputInit(&AnalogicoTask);
	AnalogOutputInit();
    // Declaro la uart 
	serial_config_t UARTTask= {
	.port= UART_PC,
	.baud_rate = 38400, 
	.func_p = UART_NO_INT,
	.param_p= NULL,
	};

	UartInit(&UARTTask);
	
	//Llamo a la tarea 

	xTaskCreate(&TaskConversion, "Conversion_task", 512, NULL, 5, &main_task_handle);
	
	TimerStart(timer_Task_1.timer);
	
	TimerStart(timer_Task_2.timer);
}
/*==================[end of file]============================================*/