#ifndef SI7007_H_
#define SI7007_H_

/** \addtogroup Drivers_Programable Drivers Programable
 ** @{ */
/** \addtogroup Drivers_Devices Drivers devices
 ** @{ */
/** \addtogroup  Sensor_TyH Sensor Temperatura y Humedad
 ** @{ */

/** \brief SI7007 driver for the ESP-EDU Board.
 *
 *** This is a driver for Si7007 

 * @author Venacio Natasha
 *
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
typedef struct {				/*!< Si7007 Inputs config struct*/
	    gpio_t select;			/*!< Determines the output on each pin*/
	    uint8_t PWM_1;		    /*!< Channel connected to the PWM1 output pin on the device*/
	    uint8_t PWM_2;	        /*!< Channel connected to the PWM2 output pin on the device*/
} Si7007_config;

/*==================[external functions declaration]=========================*/

/** @fn bool Si7007Init(Si7007_config *pins);
 * @brief Funcion de inicializacion del  Si7007.
 * @param[in] Si7007_config *pins 
 * @return TRUE si no hay error 
 */

bool Si7007Init(Si7007_config *pins);

/** @fn float Si7007MeasureTemperature(void)
 * @brief Mide la temperatura actual 
 * @param[in] Sin parametros 
 * @return valor de retorno  de temperatura en C°
 */

float Si7007MeasureTemperature(void);

/** @fn float Si7007MeasureHumidity(void)
 * @brief Mide la humedad relativa
 * @param[in] Sin  Parametros 
 * @return valor de retorno de humedad relativa en  %
 */

float Si7007MeasureHumidity(void);

/** @fn bool Si7007dEInit(Si7007_config *pins);
 * @brief funcion de desinicializacion del SI7007.
 * @param[in] Si7007_config *pins
 * @return TRUE si no hay error.
 */

bool Si7007Deinit(Si7007_config *pins);

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */

/*==================[end of file]============================================*/
#endif /* #ifndef SI7007_H */