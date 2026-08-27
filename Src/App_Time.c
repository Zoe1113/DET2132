/**************************************************************************
文件名称：	App_DispTime.c
说    明：	显示时间函数（应用层）
作    者:	Liaoy
版    本：	V1.0
芯	  片:	SN8P29XX
备    注:
修订记录：
**************************************************************************/
#include "Include.h"

//变量定义
bit F_10ms;		//10ms中断标志位（TC1产生）
bit F_10ms_task;//10ms中断标志位（TC1产生）
bit F_20ms;		//20ms中断标志位（10ms中断生成）
bit F_50ms;		//50ms中断标志位（10ms中断生成）
bit F_500ms;	//500ms中断标志位（T0产生）
bit F_5s_TimeOut;	//5s到标志位，1：5s到，0：5s未到
bit F_LED_Enable;	//三色背光开启标志位，0：除能，1：使能
bit F_Colon_Blink;	//冒号闪烁使能，0：除能，1：使能

eDispTime g_50ms_Count;

uint8 g_10ms_Count;		//10ms计数器
uint8 g_20ms_Count;		//20ms计数器
uint8 g_300ms_Count;	//300ms计数器
uint8 g_500ms_Count;	//500ms计数器
uint8 g_3s_Count = 0;		//3s计数器
uint16 g_5s_Count;		//5s计数器
uint16 g_AutoTurnOff_Count;	//自动关机计数器

//每月最大日期数表格
const uint8 __ROM MonthTable[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

/**************************************************************************
函数名称：	void Time_Creat_20ms_50ms(void)
函数功能：	利用10ms基础定时器裂变生成20ms、50ms等定时时间
输入参数：	无
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	10ms作为时基
**************************************************************************/
void Time_Creat_20ms_50ms(void)
{
	g_10ms_Count++;

	if(g_10ms_Count%2 == 0)
	{
		F_20ms = 1;
	}

	if(g_10ms_Count%5 == 0)
	{
		F_50ms = 1;
	}
	
	#if !Func_Ble	
	if(g_10ms_Count%50 == 0)
	{
		F_500ms = 1;
		g_10ms_Count = 0;
	}
	#endif
}

/**************************************************************************
函数名称：	void Light_RGB(void)
函数功能：	开机3色背光点亮各0.5s
输入参数：	无
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	20ms作为时基
**************************************************************************/
void Light_RGB(void)
{
	static bit F_First_Enter;

	g_20ms_Count ++;

	//首次点亮绿灯
	if( !F_First_Enter )
	{
		F_First_Enter = 1;
		g_20ms_Count = 0;
		#if  Func_3color
			LED_Green_En();
		#elif Func_White
			LED_White_En();	
		#endif
	
	}

#if Func_3color
	//0.5s后点亮黄灯
	if (g_20ms_Count == 25)
	{
		LED_Green_Dis();
		LED_Yellow_En();
	}

	//1s后点亮红灯
	if (g_20ms_Count == 50)
	{
		LED_Yellow_Dis();
		LED_Red_En();
	}

	//1.5s后关红灯
	if (g_20ms_Count == 75)
	{
		LED_Red_Dis();
		g_20ms_Count = 0;
		F_First_Enter = 0;
		F_LED_Enable = Disable;
	}
#else
	if (g_20ms_Count == 75)
	{
		#if Func_White
			LED_White_Dis();
		#endif
		g_20ms_Count = 0;
		F_First_Enter = 0;
		F_LED_Enable = Disable;
	}
#endif
}

/**************************************************************************
函数名称：	void Auto_TurnOff(void)
函数功能：	自动关机时间到，则进入Sleep模式关机
输入参数：	无
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	20ms作为时基
**************************************************************************/
void Auto_TurnOff(void)
{
	g_AutoTurnOff_Count--;

	if (g_AutoTurnOff_Count == 0)
	{
		OFF_Flag = 1;
		eMain_Task = Task_Sleepmode;
	}
}

/**************************************************************************
函数名称：	void Auto_TurnOff_Time_Sel(void)
函数功能：	自动关机时间选择
输入参数：	无
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	20ms作为时机，生产模式、黑体模式下6分钟自动关机，其他模式均为1分钟自动关机
**************************************************************************/
void Auto_TurnOff_Time_Sel(void)
{
	if (eTestmode_num == Insptectmode || eTestmode_num == Blackbodymode )
	{
		g_AutoTurnOff_Count = CountDown_6min;
	}
	else
	{
		g_AutoTurnOff_Count = CountDown_30s;
	}
}

/**************************************************************************
函数名称：	void Led_CountDown_3s(void)
函数功能：	背光3s倒计时
输入参数：	无
输出参数：	F_5s_TimeOut
返回值  ：	无
占用空间：	TBD
备    注：	20ms作为时基
**************************************************************************/
void Led_CountDown_3s(void)
{
	if( g_3s_Count )
	{
		g_3s_Count --;
		if( !g_3s_Count )
		{
			LED_CloseAll();
		}
	}
}

/**************************************************************************
函数名称：	void CountDown_5s(void)
函数功能：	准备就绪5s倒计时
输入参数：	无
输出参数：	F_5s_TimeOut
返回值  ：	无
占用空间：	TBD
备    注：	20ms作为时基
**************************************************************************/
// void Time_CountDown_5s(void)
// {
// 	g_5s_Count --;
// 	if( !g_5s_Count )
// 	{
// 		F_5s_TimeOut = 1;
// 	}
// }


/**************************************************************************
函数名称：	uint8 Time_CountDown_5s_timeout(bit State)
函数功能：	准备就绪5s倒计时
输入参数：	State -> 0:运行程序    1：复位计数值
输出参数：	F_5s_TimeOut
返回值  ：	0:5秒倒计时还没结束   1：倒计时结束
占用空间：	TBD
备    注：	10ms作为时基
**************************************************************************/
uint8 Time_CountDown_5s_timeout(bit State)
{
    static uint8 a = 1;
    if(State)
    {
        a = 0;
        g_5s_Count = 0;
        return 0;
    }
    if(a == 0)
    {
        g_5s_Count = CountDown_5s;
        a = 1;
    }
    else
    {
        if(g_5s_Count == 0)
        {
            return 1;
        }
        else
        {
            g_5s_Count --;
        }
    }
    return 0;
}


/**************************************************************************
函数名称：	void Disp_Dash(void)
函数功能：	额温- - -滚屏
输入参数：	g_300ms_Count
输出参数：	 - - -
返回值  ：	无
占用空间：	TBD
备    注：	50ms时基，实测额温测量时间为3.3s，故按300ms轮询，总计1.2*3=3.6s
**************************************************************************/
void Disp_Dash(void)
{
	g_300ms_Count++;

	//总计4轮，-/-/-/消失共4次，故1s/4=250ms，250/50=5；
	if( g_300ms_Count == 1 )	//50ms
	{
		lcd9 |= lcd_dash;
		if(eTestmode_num == Earmode)
		{
			lcd_ear_en();
		}
		else
		{
			lcd_obj_en();
		}
	}
	if( g_300ms_Count == 7 )	//350ms
	{
		lcd7 |= lcd_dash;
		lcd_ear_clr();
		lcd_obj_clr();
	}
	if( g_300ms_Count == 13 )	//650ms
	{
		lcd5 |= lcd_dash;
		lcd_ear_clr();
		lcd_obj_clr();
	}
	if( g_300ms_Count == 19 )	//950ms
	{
		lcd9 &= ~lcd_dash;
		lcd7 &= ~lcd_dash;
		lcd5 &= ~lcd_dash;
		if(eTestmode_num == Earmode)
		{
			lcd_ear_en();
		}
		else
		{
			lcd_obj_en();
		}
	}
	if( g_300ms_Count == 24 )	//1200ms
	{
		g_300ms_Count = 0;
	}
}

/**************************************************************************
函数名称：	void Update_SysTime(void)
函数功能：	时间更新
输入参数：	g_Second、g_Minute、g_Hour、g_Day、g_Month、g_Year
输出参数：	g_Second、g_Minute、g_Hour、g_Day、g_Month、g_Year
返回值  ：	无
占用空间：	TBD
备    注：	500ms时基
**************************************************************************/
void Update_SysTime(void)
{
	uint8 L_MaxDay;

	g_500ms_Count ++;
	if( (g_500ms_Count & 0x01) == 0x00 )
	{
		g_Second ++;
		if( g_Second == 60 )
		{
			g_Second = 0;
			g_Minute ++;
			if( g_Minute == 60 )
			{
				g_Minute = 0;
				g_Hour ++;
				if( g_Hour == 24 )
				{
					g_Hour =0;
					g_Day ++;
					L_MaxDay = MonthTable[g_Month-1];
					if( g_Month == 2)
					{
						if( LeapYear_Judge(g_Year) )
							L_MaxDay ++;	//闰年2月份加1天
					}
					if( g_Day > L_MaxDay )
					{
						g_Day = 1;
						g_Month ++;
						if( g_Month == 13)
						{
							g_Month = 1;
							g_Year ++;
							if( g_Year > 2099 )
								g_Year = Default_Year;
						}
					}
				}
			}
		}
	}
}
