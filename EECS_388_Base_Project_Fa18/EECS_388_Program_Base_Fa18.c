/*
 * main.c
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

#include    "Drivers/EECS388_DAC.h"

#include    "driverlib/adc.h"

#include	"Drivers/Processor_Initialization.h"
#include	"Drivers/UARTStdio_Initialization.h"
#include	"Drivers/uartstdio.h"


#include	"FreeRTOS.h"
#include	"task.h"

#include	<stdio.h>

extern void Task_Blink_LED_PortN_1( void *pvParameters );
extern void Task_ReportTime( void *pvParameters );
extern void Task_ReportData( void *pvParameters );
extern void Task_Button(void *pvParameters);
extern void Task_SpeakerBuzz(void *pvParameters);
extern void Task_LED(void *pvParameters);
extern void Task_ADC(void *pvParameters);
extern void Task_Cap(void *pvParameters);
extern void Task_Motor_PWM_0_4(void *pvParameters);

int main( void ) {

    //
    //Initializing UART Studio
    //
    UARTStdio_Initialization();



    Processor_Initialization();
	//
	//  Create a task to make noise via DAC
	//
	    //xTaskCreate( Task_SpeakerBuzz, "Buzzy", 32, NULL, 1, NULL);
	   // xTaskCreate( Task_Button, "Button", 32, NULL, 1, NULL);
	    //xTaskCreate( Task_LED, "LED", 32, NULL, 1, NULL);
	    //xTaskCreate( Task_ADC, "LED", 32, NULL, 1, NULL);
	    //xTaskCreate( Task_Cap, "LED", 512, NULL, 1, NULL);
          xTaskCreate( Task_Motor_PWM_0_4, "PWM", 32, NULL, 1, NULL);
	
	//
	//	Create a task to report data.
	//
	xTaskCreate( Task_ReportData, "ReportData", 512, NULL, 1, NULL );

	//
	//	Create a task to report SysTickCount
	//
	xTaskCreate( Task_ReportTime, "ReportTime", 512, NULL, 1, NULL );
	UARTprintf( "FreeRTOS   Starting!\n"    );


	//
	//	Start FreeRTOS Task Scheduler
	//
	vTaskStartScheduler();


	while ( 1 ) {

	}

}
