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
#include    "Drivers/EECS388_DAC.h"

#include    "Drivers/EECS388_LED.h"

#define     LED_Bits_Nbr    10
#define     LED_InitialState    0b0001

extern void Task_LED( void *pvParameters){
    int32_t LED_Data = LED_InitialState;
    int i = 0;
    EECS388_LED_Initialization();
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
    enum button{toandfro, shifty, loading};
    enum button current = toandfro;


    while (1){
        int value = GPIOPinRead( GPIO_PORTJ_BASE, GPIO_PIN_0|GPIO_PIN_1 );

        switch(current){
        case toandfro:
            if (value == 3){
                LED_Data = 0b0001;
                for(i = 0; i <=9; i++){
                EECS388_SetLEDs(LED_Data);
                LED_Data = LED_Data<<1;
                vTaskDelay(1000);
                }
                for(i = 0; i <=9; i++){
                EECS388_SetLEDs(LED_Data);
                LED_Data = LED_Data>>1;
                vTaskDelay(1000);
                }
                current = shifty;
                break;
            }
            else{
                current = shifty;
                value = 1;
                break;
            }

        case shifty:
            LED_Data = 0b1010101010;
            if (value == 1){
                for(i = 0; i <= 9; i++ ){
                    EECS388_SetLEDs(LED_Data);
                    LED_Data = LED_Data>>1;
                    vTaskDelay(100);
                    EECS388_SetLEDs(LED_Data);
                    LED_Data = LED_Data<<1;
                }
                current = toandfro;
                break;
            }
            else{
                current = toandfro;
                value = 0;
                break;
            }
        }

        vTaskDelay((100* configTICK_RATE_HZ) / 1000);
    }
}
