/*==================[inclusions]=============================================*/
#include "ldr.h"
#include <stdint.h>
#include <stdio.h>
#include "gpio_mcu.h"
#include "analog_io_mcu.h"
/*==================[typedef]================================================*/
/*==================[macros and definitions]=================================*/

#define V_REF 3.3                /**< Tensión de referencia*/
#define TOTAL_BITS 1024          /**< Cantidad total de bits */


/*==================[internal data declaration]==============================*/
analog_input_config_t luz_config; 
/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

bool LdrInit(){

   luz_config.input=CH3;			
	luz_config.mode=ADC_SINGLE;
    AnalogInputInit(&luz_config);  
    return true;
}

float LdrMeasureLight(void){
    uint16_t Rc= 5; //resientenica en valor 10KΩ
    uint16_t B= 4;  // Resientencia a la luz de 3KΩ
    uint16_t A= 1000; //Resistencia de un 1MΩ en oscuridad 
    uint16_t value;
    float ilum=0; 
     AnalogInputReadSingle(luz_config.input, &value);
    ilum = ((long)value*A*10)/((long)B*5*(TOTAL_BITS-value)); 
    //Resistencia a 3300 mV, cable  rojo CH3 y LDR a tierra
return ilum;
}

bool LdrDeInit(){

    return true;
}

/*==================[end of file]============================================*/