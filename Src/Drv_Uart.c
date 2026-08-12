/**************************************************************************
文件名称：	Drv_Uart.c
说    明：	串口函数集合（驱动层）
作    者:	Liaoy
版    本：	V1.0
芯	  片:	SN8P29XX
修订记录：
**************************************************************************/
#include "Include.h"

/**************************************************************************
函数名称：	void Drv_UartTX_Init(void)
函数功能：	UART TX初始化
输入参数：	无
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Drv_UartTX_Init(void)
{
	FP14M = 1;			//TX输出
	URCR1 = B9600 ;
	URRX1 = 0x04 ;		//Fuart (Fhosc=8Hz)=Fhosc/16，Disable UART RX，Disable UART RX parity bit function.
	URTX1 = 0x84 ;		//Enable UART TX, Disable UART TX parity bit function.
}

/**************************************************************************
函数名称：	void Drv_UartRX_Init(void)
函数功能：	UART RX初始化
输入参数：	无
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Drv_UartRX_Init(void)
{
	FP15M = 0;			//RX输入
	URCR1 = B9600 ;
	URRX1 = 0x84 ;		//Fuart (Fhosc=8Hz)=Fhosc/16，Enable UART RX，Disable UART RX parity bit function.
	FURX1IEN = 1;		//RX必须在中断里接收
}

/**************************************************************************
函数名称：	void Drv_UartTX_Disable( void )
函数功能：	UART初始化
输入参数：	无
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Drv_UartTX_Disable( void )
{
	FURX1IEN = 0;		//disable RX Interrupt
	URTX1 = 0x00 ;		//关闭GPIO作为TX功能
	FP14M = 0;			//TX输入
}

/**************************************************************************
函数名称：	void Drv_UartRX_Disable( void )
函数功能：	UART初始化
输入参数：	无
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Drv_UartRX_Disable( void )
{
	FURX1IEN = 0;		//disable RX Interrupt
	URRX1 = 0x00 ;		//关闭GPIO作为RX功能
	FP15M = 0;			//RX输入
}

/**************************************************************************
函数名称：	Uart_Transmit()
函数功能：	串口发送
输入参数：	无
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
#if Func_debug
void Uart_Transmit(uint8 part, uint16 data)
{
	while(FUTX1BZ);
	if(FUTX1BZ==0)
		UTX1D = 0x55 ;	//起始位高
		NOP(5);
	while(FUTX1BZ);
	if(FUTX1BZ==0)
		UTX1D = part ;
		NOP(5);
	while(FUTX1BZ);
	if(FUTX1BZ==0)
		UTX1D = data >> 8 ;
		NOP(5);
	while(FUTX1BZ);
	if(FUTX1BZ==0)
		UTX1D = data ;
		NOP(5);
	while(FUTX1BZ);
}
#endif

/**************************************************************************
函数名称：	Uart_Send_Wakeup()
函数功能：	串口发送休眠
输入参数：	无
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Uart_Send_Wakeup(void)
{
	char Count = 0;
	while(FUTX1BZ);
	for(Count=0;Count<10;Count++)
	{
		if(FUTX1BZ==0)
			UTX1D = 0xFF ;
		NOP(5);
		while(FUTX1BZ);
	}
}
