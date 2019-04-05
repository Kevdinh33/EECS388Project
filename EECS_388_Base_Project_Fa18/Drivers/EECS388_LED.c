/**********************************************************************
 * --EECS388_LED.c
 *
 *  Author:			Gary J. Minden
 *	Organization:	KU/EECS/EECS 388
 *  Date:			August 1, 2018 (B80801)
 *
 *  Description:	Driver for the LED shift register on the
 *                  EECS 388 BoosterPack. This driver uses
 *                  GPION and is designed for the Tiva
 *                  BoosterPact #1 position.
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

#define		LED_Bits_Nbr	10

extern int32_t EECS388_LED_Initialization( ) {

    //
    // Enable the GPIO Port N.
    //
    SysCtlPeripheralEnable( SYSCTL_PERIPH_GPION );

	//
    // Configure GPIO_N to drive the shift register data and
    // clock signals.
    //
    GPIOPinTypeGPIOOutput( GPIO_PORTN_BASE, GPIO_PIN_2 | GPIO_PIN_3 );
    GPIOPadConfigSet( GPIO_PORTN_BASE,
    					GPIO_PIN_2 | GPIO_PIN_3,
						GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD );

    //
    //	Set clock and data to 0.
    //
    GPIOPinWrite( GPIO_PORTN_BASE, GPIO_PIN_2 | GPIO_PIN_3, 0x00 );

    return( 1 );
}

//*********************************************************************

extern int32_t EECS388_SetLEDs( uint32_t theLEDValue ) {

	int32_t		Bit_Idx = 0;

	//
	// Transmit a LED_Bits_Nbr LED signal.
	//
	for( Bit_Idx = 0; Bit_Idx < LED_Bits_Nbr; Bit_Idx++ ) {

		//
		//	Set data output based on lsb of New_LED_Data
		//
		if ( theLEDValue & 0x0001 ) {
			GPIOPinWrite( GPIO_PORTN_BASE, GPIO_PIN_3, 0x08 );
		} else {
			GPIOPinWrite( GPIO_PORTN_BASE, GPIO_PIN_3, 0x00 );
		}

		//
		//	Toggle the LED clock.
		//
		GPIOPinWrite( GPIO_PORTN_BASE, GPIO_PIN_2, 0x04 );
		SysCtlDelay( 40 );
		GPIOPinWrite( GPIO_PORTN_BASE, GPIO_PIN_2, 0x00 );
		SysCtlDelay( 40 );

		//
		//	Shift the New_LED_Data right by one bit.
		//
		theLEDValue = theLEDValue >> 1;

	}

	return( 1 );

}



