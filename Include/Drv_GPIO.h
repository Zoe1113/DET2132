#ifndef _Drv_GPIO_H
#define _Drv_GPIO_H


#define Port_Set   	    FP00    //SET键
#define Port_On     	FP01    //开机键&测量键
#define Port_Mem   	    FP04    //记忆键

#define Port_Rod        FP17    //检测杆
#define Port_Hall       FP05    //霍尔

#define Port_Change_CF 	FP51	//高电平：可切换，低电平：不可切换
#define Port_CF 	FP52	    //高电平：C，低电平：F
#define Port_Cal 	FP53	    //校准模式检测入口
#define Port_Debug 	FP54	    //绑定检测模式入口
#define Port_BZ     FP11        //蜂鸣口

#define Port_Motor      FP50    //震动电机

extern uint8 g_MotorSystick;		//振动马达的剩余时间
// extern bit ET_mode_buf;		//模式备份标志(具备耳额物产品需要)

void Cal_Inspect_Detect(void);
void CF_Check(void);
void Voice_Change(void);
void GPIO_Init(void);
void GPIO_PowerDown( void );
void Forehead_Ear_Check(bit cmd);

#endif
