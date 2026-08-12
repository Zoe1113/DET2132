#ifndef _App_InitMode_H
#define _App_InitMode_H

//待机模式任务
typedef enum
{
    Init_Set = 0,
    Init_Disp,
    Init_ADDoff,
    Init_OpDoff,
    Init_OpOffsetDoff,
    Init_Ntc,
    Init_Err,
    Init_Wait,
    Init_Key,
    Init_End
} eInitModeTask;

extern eInitModeTask eInitTask_Sta;

void App_InitMode(void);
void TimeAndMode_Set(void);
void ADCOp_Doffinit(void);

#endif
