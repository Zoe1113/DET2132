/**************************************************************************
	Filename:	Timer.h
	Description:The header file of Time.c
**************************************************************************/
#ifndef _Drv_Timer_H
#define _Drv_Timer_H

void T0Init( void ) ;					//Timer 0 initialize
void TC2Init( void ) ;					//TC2 initialize
void TC1Init( void ) ;					//TC1 initialize
void __interrupt[0x08] Interrupt_pro(void);	//interrupt service program

#endif
/*************************************************************************/