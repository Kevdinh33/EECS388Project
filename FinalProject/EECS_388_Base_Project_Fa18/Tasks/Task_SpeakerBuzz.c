/*--Task_Blinky.c
 *
 *  Author:			Gary J. Minden
 *	Organization:	KU/EECS/EECS 388
 *  Date:			February 22, 2016
 *
 *  Description:	Blinks LED D1 on Tiva TMC41294 Evaluation board
 *
 */

#include	"inc/hw_ints.h"
#include	"inc/hw_memmap.h"
#include	"inc/hw_types.h"
#include	"inc/hw_uart.h"

#include	<stddef.h>
#include	<stdbool.h>
#include	<stdint.h>
#include	<stdarg.h>

#include	"driverlib/sysctl.h"
#include	"driverlib/pin_map.h"
#include	"driverlib/gpio.h"

#include	"FreeRTOS.h"
#include	"task.h"
#include    "Drivers/EECS388_DAC.h"

extern void Task_SpeakerBuzz( void *pvParameters ) {


    //
    // Initialize the EECS_388 DAC interface.
    //
    EECS388_DAC_Initialization();

	while ( 1 ) {
	    //
	    // Set the current DAC value.
	    //
	    EECS388_WriteDAC(0x0000);

		vTaskDelay( 8 );

		EECS388_WriteDAC(0x3FFF);

		vTaskDelay( 8 );

	}
}



