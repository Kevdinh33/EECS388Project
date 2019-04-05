//--EECS388_DAC.c
//
//*****************************************************************************
/*
 *
 *  Author:			Gary J. Minden
 *  Organization:	KU/EECS/EECS 690
 *  Date:			2017-08-06 (B80806)
 *  Description:	Driver interface to the TI Audio BoosterPack
 *  				DAC for audio output. Adapted from TI's
 *  				sharp96x96.c program.
 *
 */

#include	"inc/hw_ints.h"
#include	"inc/hw_memmap.h"
#include	"inc/hw_types.h"

#include	<stddef.h>
#include	<stdbool.h>
#include	<stdint.h>
#include	<stdarg.h>

#include	"driverlib/gpio.h"
#include	"driverlib/pin_map.h"
#include	"driverlib/ssi.h"
#include	"driverlib/sysctl.h"

#include	"Drivers/EECS388_DAC.h"
#include	"Drivers/Processor_Initialization.h"

//
// Ports, port base addresses, pins, and SSI base address
// were modified by Brandon Givens to make this driver
// compatible with the TM4C1294XL
//
// Ports for TM4C1294XL connections to DAC
#define		SPI_MOSI_PORT				SYSCTL_PERIPH_GPIOD
#define		SPI_CLK_PORT				SYSCTL_PERIPH_GPIOD
#define		SPI_CS_PORT					SYSCTL_PERIPH_GPION
#define		AUDIO_AMP_PORT				SYSCTL_PERIPH_GPION

// Port base address for TM4C1294XL connections to DAC
#define		SPI_MOSI_PORT_BASE			GPIO_PORTD_BASE
#define		SPI_CLK_PORT_BASE			GPIO_PORTD_BASE
#define		SPI_CS_PORT_BASE			GPIO_PORTN_BASE
#define		AUDIO_AMP_PORT_BASE			GPIO_PORTN_BASE

// Pins for TM4C1294XL connections to DAC
#define		SPI_MOSI_PIN				GPIO_PIN_1
#define		SPI_CLK_PIN					GPIO_PIN_3
#define		SPI_CS_PIN					GPIO_PIN_3
#define		AUDIO_AMP_PIN				GPIO_PIN_2

// Definition of SSI base address to be used for SPI communication
#define		DAC_SSI_PORT				SYSCTL_PERIPH_SSI2
#define		DAC_SSI_BASE				SSI2_BASE

//
//	The CS (Chip_Select) is a low true signal.
//	We'll use "Clear" and "Set" with low true signals.
//
//*****************************************************************************
//
// Clears CS line
//
// This function clears the Chip Select (CS) line
//
// \return None
//
//*****************************************************************************
static void ClearCS(void){
	GPIOPinWrite( SPI_CS_PORT_BASE, SPI_CS_PIN, SPI_CS_PIN );
}

//*****************************************************************************
//
// Set CS line
//
// This function sets the Chip Select (CS) line
//
// \return None
//
//*****************************************************************************
static void SetCS(void) {
	GPIOPinWrite( SPI_CS_PORT_BASE, SPI_CS_PIN, 0x00 );
}

//*****************************************************************************
//
// Waits until the SPI communication with the LCD is finished a command to
// the LCD Driver
//
// \param None
//
// \return None
//*****************************************************************************
static void WaitUntilDACWriteDone(void) {
	while (SSIBusy(DAC_SSI_BASE)) {}
}

//*****************************************************************************
//
// Writes 16-bit data to the Audio BoosterPack DAC
//
// \param DAC_Data is 16-bit data to send to the DAC
//
// \return None
//
//*****************************************************************************
extern void EECS388_WriteDAC( uint32_t DAC_Data ) {
	WaitUntilDACWriteDone();
	SetCS();
	SSIDataPut( DAC_SSI_BASE, DAC_Data );
	WaitUntilDACWriteDone();
	ClearCS();
}

//*****************************************************************************
//
//! Initializes the DAC driver.
//!
//! This function initializes the TI Audio BoosterPack DAC. This function
//! configures the GPIO pins used to control the DAC when the basic
//! GPIO interface is in use. On exit, the DAC has been reset and is ready to
//! receive data writes.
//!
//! \return None.
//
//*****************************************************************************
extern int32_t EECS388_DAC_Initialization( void ) {

	Processor_Initialization();

	// Configure the SPI port and pin for SPI Chip Select for the DAC.
	SysCtlPeripheralEnable( SPI_MOSI_PORT );
	SysCtlPeripheralEnable( SPI_CLK_PORT );
	SysCtlPeripheralEnable (SPI_CS_PORT );
	SysCtlPeripheralEnable( DAC_SSI_PORT );
	SysCtlPeripheralEnable( AUDIO_AMP_PORT );

	// Setup SPI_CS pin.
	GPIOPinTypeGPIOOutput( SPI_CS_PORT_BASE, SPI_CS_PIN );
	ClearCS();

	//	Setup Audio Amp enable, active low.
	GPIOPinTypeGPIOOutput( AUDIO_AMP_PORT_BASE, AUDIO_AMP_PIN );
	GPIOPinWrite( AUDIO_AMP_PORT_BASE, AUDIO_AMP_PIN, 0x00 );

	// Set the GPIO pin directions
	// Split if SPI_SIMO and SPI_CLK are on different GPIO ports.
	GPIOPinTypeGPIOOutput( SPI_MOSI_PORT_BASE,
							SPI_MOSI_PIN | SPI_CLK_PIN );

	// CS is controlled by a simple GPIO output signal.
	GPIOPinConfigure( GPIO_PD3_SSI2CLK );
	GPIOPinConfigure( GPIO_PD1_SSI2XDAT0 );

	GPIOPinTypeSSI( SPI_MOSI_PORT_BASE, SPI_MOSI_PIN | SPI_CLK_PIN );

	SSIClockSourceSet( DAC_SSI_BASE, SSI_CLOCK_SYSTEM );
	SSIConfigSetExpClk( DAC_SSI_BASE, g_ulSystemClock, SSI_FRF_MOTO_MODE_1,
						SSI_MODE_MASTER, 1000000, 16 );

	SSIEnable( DAC_SSI_BASE );

	return( 1 );
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
