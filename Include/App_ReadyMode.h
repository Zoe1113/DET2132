#ifndef _App_ReadyMode_H
#define _App_ReadyMode_H


//测量前等待就绪模式任务
typedef enum
{
	Ready_Init = 0,
	Ready_Timeout ,		//5s超时判断
	Ready_WaitReady ,	//等待就绪处理子任务
	Ready_ReadyOk ,		//就绪处理子任务
    Ready_Refresh,      //更新显示
    Ready_NoRefresh,    //不需要更新显示
    Ready_DisEr1,       //显示Er1
    Ready_DisEr2,       //显示Er2
    Ready_DisEr6,
    Ready_DisAir,       //显示环温
}eReadyModeTask;

extern eReadyModeTask eReadyTask_Sta;


void App_ReadyMode(void);
void Er1_Display_Sound(bit cmd);
void Er2_Display_Sound(bit cmd);
void Er6_Display_Sound(bit cmd);


#endif
