/**************************************************************************
	Filename:	Uart.h
	Description:The header file of Time.c
**************************************************************************/
#ifndef _Drv_Uart_H
#define _Drv_Uart_H

//UART BAUD RATE
#define B1200	0x30	//Fhosc/16
#define B2400	0x98	//Fhosc/16
#define B4800	0xCC	//Fhosc/16
#define B9600	0xE6	//Fhosc/16
#define B19200	0xF3	//Fhosc/16
#define B38400	0x98	//Fhosc/1
#define B51200	0xB2	//Fhosc/1
#define B57600	0xBB	//Fhosc/1
#define B102400	0xDA	//Fhosc/1
#define B115200	0xDD	//Fhosc/1

void Drv_UartTX_Init(void);
void Drv_UartRX_Init(void);
void Drv_UartTX_Disable( void );
void Drv_UartRX_Disable( void );
void Uart_Transmit(uint8 part, uint16 data);
void Uart_Send_Wakeup(void);

#endif
/*************************************************************************/