#ifndef LDR_H_
#define LDR_H_

/*==================[inclusions]=============================================*/
#include <stdint.h>
#include <stdio.h>
#include "gpio_mcu.h"
#include "analog_io_mcu.h"
/*==================[typedef]================================================*/

/*==================[internal data declaration]==============================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/


float LdrMeasureLight(void);
bool LdrInit();
bool LdrDeInit();


/*==================[end of file]============================================*/
#endif /* #ifndef LDR_H */