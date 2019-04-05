//--EECS388_DAC.h
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

#ifndef __EECS388_DAC_H__
#define __EECS388_DAC_H__

extern int32_t EECS388_DAC_Initialization( void );
extern void EECS388_WriteDAC( uint32_t DAC_Data );

#endif // __EECS388_DAC_H__ //

