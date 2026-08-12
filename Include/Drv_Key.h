#ifndef _Drv_Key_H
#define _Drv_Key_H

//各种按键功能去抖计数器（20ms基准时间）
#define CNT_Invalid 	(0xFE)	//无效设定
#define CNT_Dither 		(1)		//去抖计数器40ms(20ms去抖+20ms程序状态切换)
#define CNT_ShortPress 	(2)  	//40ms
#define CNT_ShortLong	(50)	//长按1s
#define CNT_LongPress 	(150) 	//长按3s
#define CNT_CPInterval 	(12) 	//加速按间隔240ms
#define CNT_EnterCP 	(60)    //进入连续按键1.2s

//g_keyFun按键功能码
#define Func_Short 		(0x00)	 	//仅短按
#define Func_Short_Long (0x01)	 	//短按和长按
#define Func_Long		(0x02)		//长按
#define Func_Short_Continue (0x03) 	//短按和连续按
#define Func_Super_Long	(0x04)		//超长按

//连续按使能码
#define En_Cp	0x01		//使能加速按标志位

//各按键按下标志位结构体
typedef union
{
	uint8 g_KeyPress_Flag;		//表示按键按下标志位，抬起自动清0
	struct
	{
		unsigned SetKeyPress:1;
        unsigned OKeyPress:1;
		unsigned TKeyPress:1;
		unsigned AKeyPress:1;
        unsigned MemKeyPress:1;
		unsigned HallKeyPress:1;
		unsigned CKeyPress:1;
		unsigned DKeyPress:1;
	}bits;
}uKey1;

//各按键长按保持标志位结构体
typedef union
{
	uint8 g_KeyHold_Flag;		//表示按键保持长按标志位，抬起自动清0
	struct
	{
        unsigned SetKeyHold:1;
		unsigned OKeyHold:1;
		unsigned TKeyHold:1;
		unsigned AKeyHold:1;
        unsigned MemKeyHold:1;
		unsigned HallKeyHold:1;
		unsigned CKeyHold:1;
		unsigned DKeyHold:1;
	}bits;
}uKey2;

//各按键释放标志位结构体
typedef union
{
	uint8 g_KeyRelease_Flag;	//表示按键抬起标志位，必须手动清0
	struct
	{
        unsigned SetKeyRelease:1;
		unsigned OKeyRelease:1;
		unsigned TKeyRelease:1;
		unsigned KeyRelease:1;
        unsigned MemKeyRelease:1;
		unsigned HallKeyRelease:1;
		unsigned CKeyRelease:1;
		unsigned DKeyRelease:1;
	}bits;
}uKey3;

//各按键连按标志位结构体
typedef union
{
	uint8 g_KeyContinue_Flag;	//表示按键加速按标志位，抬起自动清0（实际未使用）
	struct
	{
        unsigned SetKeyContinue:1;
		unsigned OKeyContinue:1;
		unsigned TKeyContinue:1;
		unsigned AKeyContinue:1;
        unsigned MemKeyContinue:1;
		unsigned HallKeyContinue:1;
		unsigned CKeyContinue:1;
		unsigned DKeyContinue:1;
	}bits;
}uKey4;

//按键信息结构体
typedef struct
{
	uint8 g_KeyFun;				//按键功能
	uint8 g_Key_Val;			//按键键值
	uint8 g_Key_Status;			//按键状态
	uint8 g_Key_Hold_cnt; 		//按键long press计数器
	uint8 g_Key_preset_cnt;		//按键预设long press计数器
}strKey;

extern uKey1 uKeyPress;
extern uKey2 uKeyHold;
extern uKey3 uKeyRelease;
extern uKey4 uKeyContinue;

extern strKey sMemKey, sOnKey, sSetKey ,sHallKey;
extern uint8 ModeFlag;
extern uint8 F_Disp_Temp_Status;
extern unsigned long L_Disp_Ntc; //显示环温
extern int16 g_Temp_Disp;

extern uint8 g_Switch_Temp_Flag;	//从耳额温切换到物温室温的标志位
// void App_KeyProcess(void);
void Switch_Temp(void);
uint8 HalKey_ReadKeyVal(void);
void HalKey_KeyScan(void);
void HalKey_Scan(uint8 L_Keydata,  strKey *sKey);
void HalKey_Set_KeyMode(uint8 function,  strKey *sKey);
void HalKey_KeyClr(void);

void App_OnKeyProcess(void);//该模式下开机键任务处理
void App_SetKeyProcess(void);
void App_MemKeyProcess(void);
void App_MemoryOnKeyProcess(void);

#if Func_Probecover
void App_PCKeyProcess(void);//耳套检测函数
#endif
void APP_CleanProcess(void);
void APP_Enter_Blackbodymode(void);
void  Select_Age(void);


#endif