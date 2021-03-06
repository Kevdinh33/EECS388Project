/*--Task_Buzzer.c
 *
 *  Author:			Kevin Dinh
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

#include    "driverlib/adc.h"



extern void Task_ADC( void *pvParameters ) {

    unsigned long ulValue;
    //
    // Configure ADC<9> to read potentiometer voltage.
    // The TM4C
    //
    SysCtlPeripheralEnable( SYSCTL_PERIPH_ADC0 );
    ADCSequenceConfigure( ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 1 );
    ADCSequenceStepConfigure( ADC0_BASE, 0, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH9 );
    ADCSequenceEnable( ADC0_BASE, 0 );
    while(1){
    //
    // Trigger the sample sequence.
    //
    ADCProcessorTrigger(ADC0_BASE, 0);
    //
    // Wait until the sample sequence has completed.
    //
    while(!ADCIntStatus(ADC0_BASE, 0, false))
    {
    }
    //
    // Read the value from the ADC.
    //
    ADCSequenceDataGet(ADC0_BASE, 0, &ulValue);
    UARTprintf("\nADC : %d\n", ulValue);
    vTaskDelay(10000);
    }
}
