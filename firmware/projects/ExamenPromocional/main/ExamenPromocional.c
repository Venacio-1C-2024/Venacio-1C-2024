/*! @mainpage Examen Promocional
 *
 * @section genDesc General Description
 *
 * En este proyecto se realiza el examen promocional de la catedra de electronica programable 
 *
*Se pretende diseñar un dispositivo basado en la ESP-EDU que permita controlar el riego y el pH de una plantera.
*El sistema está compuesto por una serie de recipientes con agua, una solución de pH ácida y otra básica, un sensor de húmedad y uno de pH, y tres bombas peristálticas para los líquidos.
*
*Por un lado se debe controlar el suministro de agua. El sensor de humedad se conecta a un GPIO de la placa y cambia su estado de “0” a “1” lógico cuando la humedad presente en la tierra es inferior a la necesaria. La bomba de agua deberá encenderse en este caso. 
*El pH de la plantera se debe mantener entre el rango de 6 a 6,7. Se cuenta con un sensor de pH analógico que brinda una salida de 0 a 3V para el rango de pH de 0 a 14. Por debajo de 6 se debe encender la bomba de la solución básica, por encima de 6.7 se debe encender la bomba de la solución ácida.
*Cuando la patita de riego da 1 es porq quiero regar y cuando da cero para es digital 
*la otra patita es analogico
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
 * | 11/06/2024 | Document creation		                         |
 *
 * @author Natasha Venacio (natasha.venacio.uner.edu.ar)
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
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

float valorph=0; 
bool humedad; 
TaskHandle_t main_task_handle = NULL;
#define GPIO_bombapha GPIO_21
#define GPIO_bombaphb GPIO_22
#define GPIO_regar GPIO_19
bool on = false;
bool hold = false;
/*==================[internal data definition]===============================*/
/** @fn void TaskSensar() 
 * * @brief Es una tarea que sensa la señal  analogica de ph  y sensa la señal digital de humedad 
 * @return  
 *  * /
*/
void TaskSensar();
/** @fn void Verificar ()  
 * * @brief Es una funcion que llama a otras funciones para que cada una verifique ciertos parametros para actuar  
 * @param 
 * @return  
 *  * /
*/
void Verificar ();
/** @fn void VerificarPh()  
 * * @brief Verifica los valores de ph, para ver si esta alcalino o acido dependiendo de esto prende
 * una bomba, puede ser bomba alcalina o bomba acida 
 * @param 
 * @return  
 *  * /
*/
void VerificarPh();
/** @fn void VerificarHumedad 
 * * @brief Verifica si hay humedad o no, así enciende el sistema de riego 
 * @param 
 * @return  
 *  * /
*/
void VerificarHumedad(); 

/** @fn void LeerTeclas ()  
 * * @brief Controla el sistema si enciende o se apaga 
 * @param 
 * @return  
 *  * /
*/
void LeerTeclas();
/** @fn void TaskNotificar ()  
 * * @brief Es una tarea que esta encarga de notificar a traves de la UART los estados de las bombas cual encencida o cual 
 * apagada y el valor del Ph
 * @param 
 * @return  
 *  * /
*/

void TaskNotificar(); 
/*==================[internal functions declaration]=========================*/

void TaskSensar(){

	while(true){
		vTaskDelay(3000/portTICK_PERIOD_MS); //3 segundos 
		if(on==true) {	

			uint16_t valor;
			//Sensando el valor de voltaje del ph 
			AnalogInputReadSingle(CH1, &valor);
			//Conversion a ph si 3v son 14ph si tengo valor de tension,  x ph-> regla de tres
			valorph= (valor*14)/3;
			//Sensado de la humedad es digital da 1 si tengo q regar y cero si tengo q apagar 
			humedad= GPIORead(GPIO_22);
			Verificar ();
	
		}

		else if(hold==on){
		
 			GPIOOff(GPIO_bombapha);
  			GPIOOff(GPIO_bombaphb);
			GPIOOff(GPIO_regar);
   			GPIOOff(GPIO_22);

		}

	}

	
}
void Verificar (){

	VerificarPh();
	VerificarHumedad(); 

}
void VerificarPh() {

	if(valorph>6.7){

		GPIOOn(GPIO_bombapha);
		GPIOOff(GPIO_bombaphb);
		
	}
	else if(valorph<6.7){
		GPIOOff(GPIO_bombapha);
		GPIOOff(GPIO_bombaphb);
	}
	else if(valorph<6){
		GPIOOff(GPIO_bombapha);
		GPIOOn(GPIO_bombaphb);
		
	}

	
}
void VerificarHumedad(){

	 if(humedad==true){

		 GPIOOn(GPIO_regar);

	 }
	 else
		 GPIOOff(GPIO_regar);

	}
void LeerTeclas() {

	uint16_t teclas;
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
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
	

}

void TaskNotificar(){
	while(true){
		vTaskDelay(5000/portTICK_PERIOD_MS); //5 segundos 

		if(on==true){

			if((valorph>6.7) && (humedad==true)){
			UartSendString( UART_PC,(char*)UartItoa(valorph, 10)); 
		 	UartSendString( UART_PC, "Bomba de pHA encendida, la bomba de agua encendida\r");
			}
			else if ((valorph>6.7) && (humedad==false)){
				UartSendString( UART_PC,(char*)UartItoa(valorph, 10)); 
		 	UartSendString( UART_PC, "Bomba de pHA encendida, la bomba de agua apagada\r");
			}

			else if((valorph<6)  && (humedad==true) ){

			UartSendString( UART_PC,(char*)UartItoa(valorph, 10)); 
		 	UartSendString( UART_PC, "Bomba de pHB encendida, la bomba encendida\r");
			}

			else if ((valorph<6) && (humedad==false)){
			UartSendString( UART_PC,(char*)UartItoa(valorph, 10)); 
		 	UartSendString( UART_PC, "Bomba de pHB encendida , la bomba de agua apagada\r");
			//No me da el tiempo para todas las formas puse algunas de eje 

			}
			
		}

	}
}
/*==================[external functions definition]==========================*/
void app_main(void){

	SwitchesInit();
	GPIOInit(GPIO_20,GPIO_OUTPUT);
	GPIOInit(GPIO_21,GPIO_OUTPUT);
	GPIOInit(GPIO_19,GPIO_OUTPUT); 
	GPIOInit(GPIO_22,GPIO_INPUT);
	


	//Para la señal analogica del sensor de ph
	analog_input_config_t sensorph = {
 	.input= CH1,			
	.mode = ADC_SINGLE,			
	.func_p=NULL, 		
	.param_p= NULL,			
	.sample_frec = 0,
	};
	AnalogInputInit(&sensorph);
	AnalogOutputInit();

	  // Declaro la uart 
	serial_config_t UARTTnotif= {
	.port= UART_PC,
	.baud_rate = 38400, 
	.func_p = UART_NO_INT,
	.param_p= NULL,
	};
	UartInit(&UARTTnotif);
	
	xTaskCreate(&LeerTeclas, "teclas_task", 512, NULL, 5, NULL);
	xTaskCreate(&TaskSensar, "Sensando", 512, NULL, 5, &main_task_handle);
	xTaskCreate(&TaskNotificar, "Notificar", 512, NULL, 5, &main_task_handle);
}
/*==================[end of file]============================================*/