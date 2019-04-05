/*--EECS388_LED.h
 *
 * 		Author: 		Gary J. Minden
 *		Organization:	KU/EECS/EECS 388
 *		Date:			2018-08-01 (B80801)
 *		Version:		1.0
 *
 *		Description:	This file defines the interface to the
 *						EECS 388 LED driver.
 *
 */

#ifndef EECS_388_LED_s
#define EECS_388_LED_s


#ifdef __cplusplus
extern "C" {
#endif

#include	<stddef.h>
#include	<stdbool.h>
#include	<stdint.h>
#include	<stdarg.h>

//
//	Define initialization interfaces.
//
extern int32_t EECS388_LED_Initialization( );
extern int32_t EECS388_SetLEDs( uint32_t theLEDValue );

#endif	// #define EECS_388_LED_s


