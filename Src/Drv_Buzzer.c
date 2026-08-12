/**************************************************************************
文件名称：	Drv_Buzzer.c
说    明：	蜂鸣相关函数集合（驱动层）
作    者:	Liaoy
版    本：	V1.0
芯	  片:	SN8P29XX
修订记录：
**************************************************************************/
#include "Include.h"

#if !Have_Voice_Func

#if BZ_HW

void Drv_BZ_Enable(void)
{
	TC2Init();
	FTC2ENB = 1 ;		//使能TC0定时器
	FPWM8OUT = 1 ;		//enable PT11 pwm 4KHZ输出
}

void Drv_BZ_Disable(void)
{
	FTC2ENB = 0 ;		//关闭TC0定时器
	FPWM8OUT = 0 ;		//关闭 PT12 pwm 4KHZ输出
	Port_BZ = 0;
}

#endif

void BZ_Beep50(void)
{
#if BZ_HW
	Drv_BZ_Enable();
	Delay10ms(14);		//延迟140ms
	Drv_BZ_Disable();
	Delay10ms(5);		//延迟50ms
#else
	Drv_BZ_Enable();
	Delay10ms(14);		//延迟140ms
	Drv_BZ_Disable();
	Delay10ms(5);		//延迟50ms
#endif
}


void BZ_Beep125(void)
{
#if BZ_HW
	Drv_BZ_Enable();
	Delay10ms(14);		//延迟140ms
	Drv_BZ_Disable();
	Delay10ms(14);		//延迟140ms
#else
	Drv_BZ_Enable();
	Delay10ms(14);		//延迟140ms
	Drv_BZ_Disable();
	Delay10ms(14);		//延迟140ms
#endif
}

void BZ_Beep400(void)
{
#if BZ_HW
	Drv_BZ_Enable();
	Delay10ms(42);		//延迟420ms
	Drv_BZ_Disable();
	Delay10ms(14);		//延迟140ms
#else
	Drv_BZ_Enable();
	Delay10ms(42);		//延迟420ms
	Drv_BZ_Disable();
	Delay10ms(14);		//延迟140ms
#endif
}

#endif
