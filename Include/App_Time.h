#ifndef _App_Time_H
#define _App_Time_H

//倒计时参数
#define CountDown_5s	500//250		//g_5ms_Count=5s/20ms=250;（20ms时基）
#define CountDown_3s	150		//g_5ms_Count=3s/20ms=150;（20ms时基）
#define CountDown_6min 	18000	//6min自动关机计时器（20ms时基）
#define CountDown_1min 	3000	//1min自动关机计时器（20ms时基）
#define CountDown_30s 	1500	//1min自动关机计时器（20ms时基）

typedef enum
{
	DispTime_Init = 0,				//初始值为0（50ms时基）,0s显示ntc温度，0s/50+1=1（50ms时基）
	DispTime_SysDate = 1,			//6s显示系统日期，3s/50+1=61（50ms时基）
	DispTime_SysTime = 61,			//3s显示系统时间，6s/50+1-1 =120（50ms时基）
	DispTime_SysEnd = 120,			//9s初始化，准备显示ntc温度
	DispTime_MemDate = 1,			//0s显示记忆值日期，0s/50+1=1（50ms时基）
	DispTime_MemTime = 41,			//2s显示记忆值时间，2s/50+1=41（50ms时基）
	DispTime_MemEnd = 80			//4s立即初始化计数器准备显示记忆值时间，4s/50+1-1=80（50ms时基）
}eDispTime;

extern const uint8 __ROM MonthTable[12];

extern bit F_10ms;
extern bit F_10ms_task;
extern bit F_20ms;
extern bit F_50ms;
extern bit F_500ms;
extern bit F_5s_TimeOut;
extern bit F_LED_Enable;	//三色背光开启标志位
extern bit F_Colon_Blink;	//冒号闪烁使能，0：除能，1：使能

extern uint8 g_10ms_Count;		//10ms计数器
extern uint8 g_20ms_Count;
extern uint8 g_300ms_Count;	//300ms计数器
extern uint16 g_5s_Count;
extern uint8 g_3s_Count;
extern uint16 g_AutoTurnOff_Count;

extern eDispTime g_50ms_Count;

void Time_Creat_20ms_50ms(void);
void Disp_Time_Date_Ambient(void);
// void Time_CountDown_5s(void);
void Led_CountDown_3s(void);
void Auto_TurnOff_Time_Sel(void);
void Auto_TurnOff(void);
void Light_RGB(void);
void Update_SysTime(void);
void Disp_Dash(void);
uint8 Time_CountDown_5s_timeout(bit State);

#endif
