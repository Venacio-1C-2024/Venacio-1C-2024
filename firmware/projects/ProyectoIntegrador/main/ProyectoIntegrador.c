/*! @mainpage ProyectoIntegrador
 *
 * @section genDesc General Description
 *
 * Este proyecto Integrador simula ser un invernadero automatizado, donde sensa los 
 * niveles de humedad, temperatura y luz. Verifica estos parametros y si no estan dentro
 * de los valores normalizados para un invernadero activa los actuadores que en este caso
 * son un humidificador, un ventilzador y una lampara de luz led, estos se activan a traves
 * de los GPIO. Luego estos valores son mandados por bluetooh al celular. Ademas se calcula
 * el promedio de humedad y temperatura, maximo y minimo de temperatura, los datos tambien
 * son tambien enviados por bluetooh al celular.  
 *
 * @section hardConn Hardware Connection
 *
 * |    Peripheral  |   ESP32   	|
 * |:--------------:|:--------------|
 * | 	rele/vent	| 	GPIO_21	    |
 * | 	rele/foco	| 	GPIO_22	    |
 * |    Humidifcador| 	GPIO_23	    |   
 * |    PW1	        | 	CH1	        |   
 * |    PW2         | 	CH2         |   
 * |    LDR         | 	CH3         | 
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
#include "string.h"
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
#include "ble_mcu.h"
#include "math.h"
/*==================[macros and definitions]=================================*/
/**
 * * @brief variable de tipo uint16_t que almacenada el valor de temperatura medido
 * 
 * */
volatile uint16_t temperatura;
/**
 * * @brief variable de tipo float que almacena el valor de luz medido en lux 
 * 
 * */
volatile float luz;
/**
 * * @brief variable de tipo uint16_t que almacena el valor de humedad medido 
 * 
 * */
volatile uint16_t humedad;
/**
 * * @brief variable de tipo float que va a acumulando los valores de temperatura obtenidos 
 * 
 * */
float acumulador_temperatura = 0; 
/**
 * * @brief variable de tipo uint16_t que guarda el valor maximo de temperatura 
 * 
 * */
uint16_t maxtemp=0;
/**
 * * @brief variable de tipo uint16_t que guarda el valor minimo de temperatura 
 * 
 * */
uint16_t mintemp=0;
/**
 * * @brief variable de tipo float que guarda el promedio de temperatura 
 * 
 * */
float promediotemp=0; 
/**
 * * @brief 
 * 
 * */
float acumulador_humedad = 0;
/**
 * * @brief 
 * 
 * */
uint16_t contador =0; 
/**
 * * @brief variable de tipo float que permite almacenar el promedio de la humedad 
 * 
 * */
float promediohum=0;
/**
 * * @brief variable de tipo TaskHandle_t que se utiliza para manejar las tareas
 * 
 * */
TaskHandle_t main_task_handle = NULL;
/**
 * * @brief  GPIO_vent es un alias para GPIO_21 esto permite cambiar el pin del sensor de humedad fácilmente si es necesario
 * 
 * */
#define GPIO_vent GPIO_21
/**
 * * @brief GPIO_vent es un alias para GPIO_22 esto permite cambiar el pin del sensor de humedad fácilmente si es necesario
 * 
 * */
#define GPIO_foco GPIO_22
/**
 * * @brief GPIO_humi es un alias para GPIO_23 esto permite cambiar el pin del sensor de humedad fácilmente si es necesario
 * 
 * */ 
#define GPIO_humi GPIO_23
/*==================[internal data definition]===============================*/
/** @fn void TaskMedir() 
 * * @brief Tarea encargada de medir las variables de humedad, temperatura y luz 
 * @param 
 * @return  
 *  * /
*/
void TaskMedir ();

/** @fn void TaskVerificar() 
 * * @brief Tarea encargada de llamar a las funciones que verifican las condiciones de las variables 
 * @param 
 * @return  
 *  * /
*/
void TaskVerificar();

/** @fn void VerificarTemperatura() 
 * * @brief Funcion que verifica el nivel de temperatura para prender el ventilador 
 * @param 
 * @return  
 *  * /
*/
void VerificarTemperatura();

/** @fn void VerificarHumedad() 
 * * @brief Funcion que verifica el nivel de humedad para prender el humidificador 
 * @param 
 * @return  
 *  * /
*/
void VerificarHumedad();

/** @fn void VerificarLuz() 
 * * @brief Funcion que verifica el nivel de luz para prender el foco 
 * @param 
 * @return  
 *  * /
*/
void VerificarLuz();

/** @fn static void EnviarDatos () 
 * * @brief Tarea encargada de enviar los datos por bluetooh al  celular 
 * @param 
 * @return  
 *  * /
*/
static void EnviarDatos();
/** @fn void CalcularDatos ()
 * * @brief Tarea encargada de calcular el promedio el maximo y el minimo de humedad y temperatura y enviarlo al celular por bluetooh 
 * @param 
 * @return  
 *  * /
*/ 
void CalcularDatos (); 

/*==================[internal functions declaration]=========================*/

void TaskMedir(){

	while(true) {

		vTaskDelay(5000/portTICK_PERIOD_MS); //Cada 5 segundos mide las variables 
		temperatura= Si7007MeasureTemperature();
		acumulador_temperatura+= temperatura; 
		humedad=Si7007MeasureHumidity();
		acumulador_humedad+= humedad; 
		luz=LdrMeasureLight(); 

		printf("Valores de temperatura %u \n", temperatura);
		printf("Valores de humedad %u \n", humedad);
		printf("Valores de luz  %f \n", luz);
		contador+=1;
	}

}

void TaskVerificar(){

	while(true){

		vTaskDelay(5000/portTICK_PERIOD_MS);
		VerificarTemperatura();
		VerificarHumedad();
		VerificarLuz();
	}
}	
void VerificarTemperatura(){

	if(temperatura>30){ //valor de tension referido a los 30 grados 
	GPIOOff(GPIO_vent); //Actua por bajo el rele 
	}
	else if(temperatura<30) {
	GPIOOn(GPIO_22);	
	}
}

void VerificarHumedad(){

	
	if(humedad>40){ // valor de tension referido al 40% de la humedad 
	
	GPIOOn(GPIO_humi);

	}
	 
	else {
	
		GPIOOff(GPIO_humi);
	}
}

void VerificarLuz(){

	if(luz<0) { //Compara en lux 

		GPIOOff(GPIO_foco); //Actua por bajo el rele 

	}
	else if(luz>0){
		GPIOOn(GPIO_foco);
	}
}

static void EnviarDatos(){
	char msg1[25];
	char auxmsg1[25];
	char msg2[25];
	char auxmsg2[20];
	char msg3[25];
	char auxmsg3[20];	

	while (true)
	{
		/*----------------Envio los datos medidos por bluetooth-----------------*/
		
			strcpy(msg1, "");
			sprintf(auxmsg1,"*tTemperatura: %u\n*", temperatura);
			strcat(msg1, auxmsg1);
			BleSendString(msg1);

			// enviar humedad por bluetooth
			strcpy(msg2, "");
			sprintf(auxmsg2, "*hHumedad: %u\n*", humedad);
			strcat(msg2, auxmsg2);
			BleSendString(msg2);

			// enviar luz por bluetooth
			strcpy(msg3, "");
			sprintf(auxmsg3, "*lLuz: %.2f\n",luz);
			strcat(msg3, auxmsg3);
			BleSendString(msg3);

			
		vTaskDelay(5000/portTICK_PERIOD_MS);
	}
	
}

void CalcularDatos (){

	char msg4[40];
	char auxmsg4[30];
	char msg5[45];
	char auxmsg5[30];
	char msg6[45];
	char auxmsg6[30];
	char msg7[45];
	char auxmsg7[30]; 

	while(true) {

		if(contador==10) {  //Tomo 10 muestras 
		
			//Calculo el promedio de temperatura y humedad
		promediotemp= acumulador_temperatura/contador;
		promediohum= acumulador_humedad/contador; 
		//Calculo maximo y minimo de temperatura 
		if (maxtemp < temperatura)
			{
				maxtemp = temperatura;
			}
		if (mintemp > temperatura)
			{
				mintemp = temperatura;
			}
		
		//Envio los datos por bluetooh 

	strcpy(msg4, "");
			sprintf(auxmsg4, "*Promedio temperatura %.2f\n*", promediotemp);
			strcat(msg4, auxmsg4);
			BleSendString(msg4);

			strcpy(msg5, "");
			sprintf(auxmsg5, "*Promedio humedad %.2f\n*", promediohum);
			strcat(msg5, auxmsg5);
			BleSendString(msg5);

			strcpy(msg6, "");
			sprintf(auxmsg6, "*Maxima temperatura %u\n*", maxtemp);
			strcat(msg6, auxmsg6);
			BleSendString(msg6);

			strcpy(msg7, "");
			sprintf(auxmsg7, "*Minima temperatura %u\n*", mintemp);
			strcat(msg7, auxmsg7);
			BleSendString(msg7);

			printf("Temperatura maxima %u \n", maxtemp);
			printf("Temperatura minima %u \n", mintemp);
			//Pongo de nuevo el contador  y los acumuladores en cero para sacar un nuevo promedio
			contador=0;
			acumulador_temperatura=0;
			acumulador_humedad=0;
		}
		

			vTaskDelay(5000/portTICK_PERIOD_MS);
	}
	
}
/*==================[external functions definition]==========================*/

void app_main(void){

//Inicializaciones de los GPIO
GPIOInit(GPIO_23, GPIO_OUTPUT);
GPIOInit(GPIO_21, GPIO_OUTPUT);
GPIOInit(GPIO_22, GPIO_OUTPUT);
//Inicializo el LDR 
LdrInit();

//Inicializacion del SI7007
	 Si7007_config SensorTask= {
		.select=GPIO_9,		
	    .PWM_1= CH1,		   
	    .PWM_2= CH2,
	 };

	Si7007Init(&SensorTask);
	
// Inicializacion del bluetooh
	ble_config_t bluetooh_= {

		.device_name= "ESP_EDU_Natasha",
		.func_p= NULL,
	};

	BleInit(&bluetooh_);

	//Llamo a las tareas 
	xTaskCreate(&TaskMedir, "Sensando", 4096, NULL, 5, &main_task_handle);
	xTaskCreate(&TaskVerificar, "Verificando", 2048, NULL, 5, &main_task_handle);
	xTaskCreate(&CalcularDatos, "Calculando el promedio, max y min", 4096, NULL, 5, &main_task_handle);
	xTaskCreate(&EnviarDatos, "Notificar por Bluetooh", 2048, NULL, 5, &main_task_handle); 

}
/*==================[end of file]============================================*/