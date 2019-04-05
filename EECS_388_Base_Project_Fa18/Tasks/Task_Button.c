/*--Task_Button.c
 *
 *  Author:			Kevin Dinh
 *	Organization:	KU/EECS/EECS 388
 *  Date:			February 22, 2016
 *
 *  Description:	Makes noise when button is pressed on Tiva TMC41294 Evaluation board
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


#include "Drivers/UARTStdio_Initialization.h"
#include "Drivers/uartstdio.h"

extern bool makeNoise;
extern void Task_Button( void *pvParameters ) {


    //
    //Enable Port J
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);

    //
    //Set Port J<0> and Port<1>
    //
    GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, GPIO_PIN_0 );
    GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, GPIO_PIN_1 );

    //
    //Set Port J<0> and Port<1> to weak pull
    //
    GPIOPadConfigSet(GPIO_PORTJ_BASE, GPIO_PIN_0|GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);


       enum button{idle, debounce, pressed, released};
       enum button currentstate = idle;
	while ( 1 ) {
	    //
	    // Set the current DAC value.
	    //
	    int value = GPIOPinRead( GPIO_PORTJ_BASE, GPIO_PIN_0|GPIO_PIN_1 );
	    int check;
	    switch(currentstate){

	    case idle :
	        if(value == 3){
	            vTaskDelay(10);
	            break;
	        }
	        else{
	            currentstate = debounce;
	            break;
	        }

	    case debounce:
	        vTaskDelay( 100 );
	        check = GPIOPinRead( GPIO_PORTJ_BASE, GPIO_PIN_0|GPIO_PIN_1 );
	        if(value != 3){
	            makeNoise = true;
	            vTaskDelay( 2000);
	            makeNoise = false;
	            currentstate = pressed;
	            break;
	        }
	        else{
	            currentstate = idle;
	            break;
	        }


	    case pressed:
	        vTaskDelay(100);
	        if(value == 0 || value == 1 || value == 2 ){
                currentstate = pressed;
                break;
	        }
	        else{
	            currentstate = released;
	            break;
	        }

	    case released:
	        if(value == 3){
	            makeNoise = true;
                vTaskDelay( 5000);
                makeNoise = false;
                if(check == 1){
                    UARTprintf( "Button 1 released."    );
                }
                else if(check == 2){
                    UARTprintf( "Button 2 released."    );
                }
                currentstate = idle;
                break;
	        }
	        else{
	            currentstate = idle;
	            break;
	        }
	    }
	}
}
