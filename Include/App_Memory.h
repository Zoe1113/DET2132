#ifndef _App_Memory_H
#define _App_Memory_H

#define MemMaxArray 0x1E	//定义最大记忆组数   //0x1E

extern volatile bit F_MemNull;				//记忆为空标志位
extern volatile bit F_Mem_FirstEnter;

extern uint8 g_HourMem;				//用于表示记忆态下显示测量时间小时寄存器
extern uint8 g_MinuteMem;			//用于表示记忆态下显示测量时间分钟寄存器
extern uint8 g_DayMem;				//用于表示记忆态下显示测量时间日期寄存器
extern uint8 g_MonthMem;			//用于表示记忆态下显示测量时间月份寄存器
extern uint16 g_YearMem;				//用于表示记忆态下显示测量时间年份寄存器

void App_Memory(void);
void Mem_Store(int16 L_Temp);
static void Mem_Init(void);
static int16 Disp_Mem(void);
static void Disp_MemNo(void);
void Read_Mem_RecordNo(void);
uint8 Read_Mem_Address(uint8 L_MemNo);
void Clr_All_Memory(void);


#endif
