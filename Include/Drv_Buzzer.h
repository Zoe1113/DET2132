/**************************************************************************
	Filename:	Buzzer.h
	Description:The header file of Buzzer.c
**************************************************************************/
#ifndef _Drv_Buzzer_H
#define _Drv_Buzzer_H

#define BZ_HW 1		//0:代表使用硬件BZ驱动口，1：代表使用PWM驱动

//底层驱动
#if !BZ_HW
	#define Drv_BZ_Enable()	{ BZRM = 0x07; }	//7.8KHz,使能Buzzer(不可以复用I2C)
	#define Drv_BZ_Disable() { BZRM = 0x00; }	//关Buzzer
#endif

// typedef enum
// {
// 	Stop_beep = 0,
// 	Enable_beep,
// 	Start_beep,
// 	Disable_beep
// }eBeep;

// extern eBeep eBeep_Status;
extern uint8 g_beep_time;	//蜂鸣时长（10ms时基）
extern uint8 g_beep_loop;	//蜂鸣循环次数

// void App_Beep(void);
void BZ_Beep50(void);
void BZ_Beep125(void);
void BZ_Beep400(void);
void Drv_BZ_Enable(void);
void Drv_BZ_Disable(void);

#endif
/*************************************************************************/