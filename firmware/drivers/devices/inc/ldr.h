#ifndef LDR_H_
#define LDR_H_

/** \addtogroup Drivers_Programable Drivers Programable
 ** @{ */
/** \addtogroup Drivers_Devices Drivers devices
 ** @{ */
/** \addtogroup  Sensor de Luz 
 ** @{ */ 
/** \brief LDR driver for the ESP-EDU Board.
 *
 *** This is a driver for LDR 
 *
 * @author Venacio Natasha
 *
 *
 * |   Date	    | Description                                    						|
 * |:----------:|:----------------------------------------------------------------------|
 * | 30/05/2024 | Document creation		                         						|
 * 
 **/
/*==================[inclusions]=============================================*/
#include <stdint.h>
#include <stdio.h>
#include "gpio_mcu.h"
#include "analog_io_mcu.h"
/*==================[typedef]================================================*/

/*==================[external functions declaration]=========================*/


/** @fn float  LdrMeasureLight(void)
 * @brief Funcion que mide el nivel de ilumiacion  
 * @param[in] Sin  Parametros 
 * @return valor de retorno de nivel de iluminacion 
 */ 
float LdrMeasureLight(void);

/** @fn bool LdrInit()
 * @brief funcion de inicializacion del LDR.
 * @param[in] Sin parametros 
 * @return TRUE si no hay error.
 */
bool LdrInit();


/** @fn bool LdrDeInit()
 * @brief funcion de desinicializacion del LDR
 * @param[in] Sin parametros 
 * @return TRUE si no hay error.
 */
bool LdrDeInit();

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */

/*==================[end of file]============================================*/
#endif /* #ifndef LDR_H */