/**************************************************************************
文件名称：	Drv_Timer.c
说    明：	定时器设置（驱动层）
作    者:	Liaoy
版    本：	V1.0
芯	  片:	SN8P29XX
修订记录：
**************************************************************************/
#include "Include.h"


/**************************************************************************
	Timer 0 initialize(RTC，需使能外置32768 crystal)
**************************************************************************/
void T0Init( void )
{
	FT0EN = 0;
	T0M = 0x00;
	FT0TB = 1;			//set RTC mode
	FT0IEN = 0;
	FT0IRQ = 0;			//停止T0计时/中断/中断请求位
	FT0EN = 1;			//开启0.5s计时
}

/**************************************************************************
	TC1 initialize(10ms)
	TC1R initial value = 65536 - (TC1 interrupt interval time * TC1 clock rate)
	TC1D initial value = TC1R + (PWM high pulse width period / TC1 clock rate)
**************************************************************************/
void TC1Init( void )
{
	TC1M = 0x40 ;		//0b01000000，Disable,Fcpu/8
	TC1CL = 0x3C ;
	TC1RL = 0x3C ;		//10ms（Fcpu = 2M)
	TC1CH = 0xF6 ;
	TC1RH = 0xF6 ;		//10ms（Fcpu = 2M)
	FTC1IRQ = 0;		//清TC1中断请求位
	FTC1IEN = 1;		//enable TC1中断
	FTC1ENB = 1 ;		//使能TC1定时器
}

/**************************************************************************
	TC2 initialize(Buzzer 4KHZ，PT12口)
	TC2R initial value = 65536 - (TC2 interrupt interval time * TC2 clock rate)
	TC2D initial value = TC2R + (PWM high pulse width period / TC2 clock rate)
**************************************************************************/
void TC2Init( void )
{
	TC2M = 0x40 ;		//0b01000000，Disable,Fcpu/8
	TC2CL = 0xC1 ;
	TC2RL = 0xC1 ;		//250us
	TC2CH = 0xFF ;
	TC2RH = 0xFF ;		//250us
	TC2DL6 = 0xE0 ;
	TC2DH6 = 0xFF ;		//125us（1/2 duty)
}
