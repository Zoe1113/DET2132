#ifndef _App_Sleep_H
#define _App_Sleep_H

typedef enum
{
	Sleep_false = 0,	//假关机状态
	Sleep_waitkey = 1,	//等待按键完全释放状态
	Sleep_true = 2,		//真关机状态
	Sleep_wakeup = 3,	//用于设置态、单位切换等无显示等待状态
	Sleep_End = 4
}eSleep_Status;

extern eSleep_Status eSleepTask_Sta;
extern uint8 OFF_Flag;
void App_Sleep(void);

#endif