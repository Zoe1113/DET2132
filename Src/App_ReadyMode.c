/**************************************************************************
文件名称：	App_ReadyMode.c
说    明：	测量前等待就绪模式（应用层）
作    者:	Liaoy
版    本：	V1.0
芯	  片:	SN8P29XX
备    注:
修订记录：
**************************************************************************/
#include "Include.h"

eReadyModeTask eReadyTask_Sta;



/*************** 关于本程序的说明 *******************
修改前请仔细阅读！！！
1、错误讯息规则：
Lo/HI/ER1/ER3/ER4：必须等待5s时间到，并更新ntc后，才会显示———，既可以关机、也可以进入记忆，也可以切换模式；
Er2：自更新，当错误消失后，无需等待5s，立即更新ntc后，才显示———，同时无法进入记忆模式，只能关机，也无法切换模式，同时也无法响应测量出Er1；
ER6：自更新，耳套拔出立即会恢复，无需等待5s，立即更新ntc后，才显示———，同时无法进入记忆模式，只能关机和切换模式，同时也无法响应测量出Er1；
正常测量结束（有温度显示）：汇编是测量结束立即采集ntc，5s完成后不会返回重新采集ntc，已修改成必须返回采集ntc，对临床无实质影响；
2、错误产生时机：
LO/HI/ER3/发生在测量中，ER1/ER2/ER6发生在ready时，所以ER1/ER2/ER6时会刷新覆盖LO/HI/ER3/ER4，而ER2全显最高，其次ER6，再次ER1，最后LO/HI/ER3/ER4
3、错误讯息优先级：
Er2>Er6>Er1>Lo=Hi=Er3=Er4，这些错误可能会同时产生，但同时只会显示优先权高的错误，故可以用枚举替代
建议任何错误产生时都将上一个错误标志位清0，但本程序从长远考虑，不做枚举替代，防止出现错误同时产生，同时需要显示的情况---已修改都清0，因为ET不存在同时显示的情况
4、耳温模式：只采集一次ntc后，实时采集tp，其他模式均为实时采集ntc；
5、由于错误显示也可以进入记忆态，请特别注意第4点的通道切换；
6、尤其注意该程序这几个状态的跳转逻辑，修改程序必须测试该逻辑！
**************************************************/

void App_ReadyMode(void)
{
	APP_Enter_Blackbodymode();	//进入黑体模式的按键操作
	#if Func_Probecover
		App_PCKeyProcess();	//耳套按键操作
	#endif
	App_OnKeyProcess();	//测量键
	App_SetKeyProcess();	//模式切换按键
	Cal_Inspect_Detect();       //绑定检测模式、校准模式判断
	if(eMain_Task != Task_Memorymode)
	{
		Disp_Age_Select(g_AgeSelectNum);
		Disp_VoiceSign();
		Disp_Unit();	//显示单位
	}
	App_MemKeyProcess();	//记忆键
	
	switch( eReadyTask_Sta )
	{
		//初始化
		case Ready_Init:
			#if !Second_LVD
				lcd_bat_clr();
			#endif
			//lcd_clean_clr();	//清除清洁符号
			HalKey_KeyClr();               //清除所有按键信息
			g_50ms_Count = DispTime_Init;	//循环显示时间、日期、ntc计时器赋初值
			#if Func_Ble
			if(eTestmode_num == Earmode || eTestmode_num == Foreheadmode || eTestmode_num == Objectmode)
				F_Ble_En = Enable;		//仅用户测温模式开启蓝牙
			else
				F_Ble_En = Disable;
            #endif

			eReadyTask_Sta = Ready_Refresh;
			
			break;
		//5s超时判断
		case Ready_Timeout:
			if(eTestmode_num == Earmode || eTestmode_num == Blackbodymode || eTestmode_num ==Insptectmode )
			{
				if(Time_CountDown_5s_timeout(RUN))//倒计时开始
				{
					if(uErrFlag.g_ErrFlag)
					{
						eReadyTask_Sta = Ready_Refresh;      //等待时间到后不需要更新显示
					}
					else	
					{
						eReadyTask_Sta = Ready_NoRefresh;      //等待时间到后不需要更新显示
					}
				}
			}
			else
			{
				eReadyTask_Sta = Ready_NoRefresh;
			}
			break;
		//这两个为人为增加的过渡状态
		case Ready_Refresh:
            Clr_Disp888();
			Disp_Ready();	//首次开机必须显示_ _._

			//模式切换的时候需要背光
			// LED_CloseAll();
			// LED_Green_En();		//开启绿光
			// g_3s_Count = CountDown_3s;	//开启背光3s倒计时
            eReadyTask_Sta = Ready_WaitReady;
			break;
		//这两个为人为增加的过渡状态
		case Ready_NoRefresh:
            eReadyTask_Sta = Ready_WaitReady;
			break;
		//等待就绪状态更新显示
		case Ready_WaitReady:
			// 开机时如果为耳温模式需要做已就绪提示
			uErrFlag.g_ErrFlag = 0;
			Disp_ModeSign();	//显示模式符号
			Auto_TurnOff_Time_Sel();//自动关机

			#if Func_White
				if(eTestmode_num != Foreheadmode)
				{
					LED_CloseAll();
					LED_White_En();
				}
			#else
			if(eTestmode_num != Foreheadmode && eTestmode_num != Objectmode)//测量结束后有蜂鸣声
			{
				LED_CloseAll();
				
				#if Func_3color
					LED_Green_En();		
				#endif
			}

			#endif
			g_3s_Count = CountDown_3s;	//开启背光3s倒计时
			

		
			if(eTestmode_num == Earmode || eTestmode_num == Blackbodymode)//测量结束后有蜂鸣声
			{
				if(uSetFlag.bits.VoiceEnable == 1)
				{
					BZ_Beep50();
					BZ_Beep50();		//蜂鸣2声
				}
			}
		

			//if(eTestmode_num == Insptectmode ||  eTestmode_num == Foreheadmode)
			if(eTestmode_num != Earmode && eTestmode_num != Blackbodymode)
			{
				#if Func_White	//额温的白色背光只有切换到额温的第一次会亮
				if(uSetFlag.bits.Ready_First)
				{
					LED_CloseAll();
					LED_White_En();
					g_3s_Count = CountDown_3s;	//开启背光3s倒计时
				}
				#endif

				if( uSetFlag.bits.VoiceEnable == 1)//开机第一次时的语音/蜂鸣声
				{
					if(uSetFlag.bits.Ready_First)
					{
						// #if Func_White	//额温的白色背光只有切换到额温的第一次会亮
						// 	LED_CloseAll();
						// 	LED_White_En();
						// 	g_3s_Count = CountDown_3s;	//开启背光3s倒计时
						// #endif
						#if Have_Voice_Func
							voice_stop();
							PlayStatueParam(1 , Play_Di,0);
						#else
							BZ_Beep50();
							BZ_Beep50();		//蜂鸣2声
						#endif
					}
				}
				else
				{
					#if Have_Motor
						if(eTestmode_num != Earmode && eTestmode_num != Blackbodymode)
						{
							g_MotorSystick = Vibration_time;
						}
					#endif
				}
			}
			if(eTestmode_num != Insptectmode) //生产模式准备状态一直有声音
			{
				uSetFlag.bits.Ready_First = 0;
			}
			 //耳温切换到tp通道采集
            if (eTestmode_num == Earmode)
            {
                Adc_Channel_Init(NTCTOTP);
            }

			if(eTestmode_num != Airmode)
			{
				eReadyTask_Sta = Ready_ReadyOk;		//当准备好，才可以跳到Ready_ReadyOk
			}
			else
			{
				eReadyTask_Sta = Ready_DisAir;
			}
						
			break;
		case Ready_DisEr1:
			Er1_Display_Sound(RUN);
		break;

		case Ready_DisEr2:
			Disp_ErrMsg();		//显示错误代码
			Er2_Display_Sound(RUN);		//Er2错误处理
			
		break;

		#if Func_Obj
        case Ready_DisEr6:
            Er6_Display_Sound(RUN);      //Er6的声音显示
            break;
        #endif
		
		case Ready_DisAir:	//显示环温
			//Clr_Disp888();
            // L_Disp_Ntc = CToF(g_NtcStep);
            // L_Disp_Ntc = Temp_Resolution_Adjust(L_Disp_Ntc);
            // Disp_Temp(1,0,uSetFlag.bits.Unit,L_Disp_Ntc);
			Switch_Temp();
		break;
		//已就绪状态
		case Ready_ReadyOk:
			uStaFlag.bits.Fever = 0;	//清发烧标志位（记忆可能退出）
			break;
		default:
			break;
	}
}






/**************************************************************************
函数名称：	void Er1_Display_Sound(bit cmd)
函数功能：	Er1错误的蜂鸣和显示
输入参数：	cmd->   0：运行蜂鸣和等待恢复   1：复位Er1错误
输出参数：	无
返回值  ：	无
占用空间：	TBD
**************************************************************************/
void Er1_Display_Sound(bit cmd)
{
	 static uint8 Er1_First_Enter = 0;;//首次出现Er1错误
	if(cmd == RESET)//er1处理过程中被打断，清除其标志位
	{
		uErrFlag.bits.Er1 = 0;
		Er1_First_Enter = 0; 
		F_Disp_Dash = Disable;//关闭er1错误屏幕显示 ---滚动刷屏
		g_300ms_Count = 0;
		goto END;  	
	}	
	if(Er1_First_Enter == 0)
	{
		Clr_Disp888();
		lcd_badface_clr();
		lcd_smileface_clr();
		High_Precision_Flag = 0;
		F_Disp_Dash = Enable;//开启er1错误屏幕显示 ---滚动刷屏
		F_Disp_Temp_Status = 0;
		g_300ms_Count = 0;
		Er1_First_Enter = 1;
		uErrFlag.bits.Er1 = 1;
	}else
	{
		if(Time_CountDown_5s_timeout(RUN))  
		{
			Er1_First_Enter = 0; 
			uErrFlag.bits.Er1 = 0;
			F_Disp_Dash = Disable;//关闭er1错误屏幕显示 ---滚动刷屏
			eReadyTask_Sta = Ready_Refresh;
			eMain_Task = Task_ReadyMode;
		}
	}
END: ;             //空语句，结束程序
	
}

/**
 * er2错误处理;	
 * cmd:	为Enable时，开启错误处理
 * 		为Disable时，在错误处理被打时清除其标志位
 * 
*/
void Er2_Display_Sound(bit cmd)
{
	static uint8 Er2_First_Enter = 0;      //注意该标志需要在关机时清零，否则在报Er2时关机，开机后第一次出现Er2时没有蜂鸣。
	if(cmd == RESET)//er2处理过程中被打断，清除其标志位
	{
		Er2_First_Enter = 0;
		goto END;                         //使用跳转语句跳到函数末尾，复位程序时蜂鸣
	}
	if(Er2_First_Enter == 0)
	{
		Disp_ModeSign();	//显示模式符号
		LED_CloseAll();
		Auto_TurnOff_Time_Sel();
		Time_CountDown_5s_timeout(RESET);		//清除倒计时相关标志位
		Er1_Display_Sound(RESET);		//清除Er1相关标志位
		if(eMain_Task != Task_Memorymode)
		{
			lcd_mem_clr();
		}
		if( uSetFlag.bits.VoiceEnable == 1 )
		{
			#if Have_Voice_Func	//错误语音播报
				g_DiDo = (uint16)uErrFlag.g_ErrFlag;
				voice_stop();
				PlayStatueParam(1 , Play_Errmsg,0);
			#else
				BZ_Beep50();    //ER2报错，蜂鸣4声
				BZ_Beep50();
				BZ_Beep50();
				BZ_Beep50();
			#endif 
		}
		Er2_First_Enter = 1;
		uErrFlag.bits.Er1 = 0;
	}
	else
	{

		if(uErrFlag.bits.Er2 == 0)
		{
			Er2_First_Enter = 0;
			uSetFlag.bits.Ready_First = 1;
			eReadyTask_Sta = Ready_Refresh;
			eMain_Task = Task_ReadyMode;
		}

	}
END: ;        //跳转到此处。结束程序

}

/**************************************************************************
函数名称：	void Er6_Display_Sound(bit cmd)
函数功能：	Er6错误的蜂鸣和显示
输入参数：	cmd->   0：运行蜂鸣和等待恢复   1：复位Er6错误
输出参数：	无
返回值  ：	无
占用空间：	TBD
**************************************************************************/
#if Func_Obj	 
void Er6_Display_Sound(bit cmd)
{
    static uint8 Er6_First_Enter = 0;      //注意该标志需要在关机时清零，否则在报Er6时关机，开机后第一次出现Er6时没有蜂鸣。
    if(cmd == RESET)
    {
        Er6_First_Enter =0;              //提前退出函数，防止在清零该标志位时会蜂鸣。
        goto END;                          //使用跳转语句跳到函数末尾，复位程序时蜂鸣
    }
    
    if(Er6_First_Enter == 0)
    {
		LED_CloseAll();
        Er6_First_Enter = 1;
        Auto_TurnOff_Time_Sel();
		Disp_ErrMsg();	//显示错误信息
        lcd_pc_en();	//显示耳套符号
		lcd_obj_en();
		lcd_ear_clr();
		Er1_Display_Sound(RESET);		//清除Er1相关标志位
		if( uSetFlag.bits.VoiceEnable == 1 )
		{
			#if Have_Voice_Func	//错误语音播报
				g_DiDo = (uint16)uErrFlag.g_ErrFlag;
				voice_stop();
				PlayStatueParam(1 , Play_Errmsg,0);
			#else
				BZ_Beep50();    //ER2报错，蜂鸣4声
				BZ_Beep50();
				BZ_Beep50();
				BZ_Beep50();
			#endif 
		}
	}
    else
    {
        if(uErrFlag.bits.Er6 == 0)
        {
            Er6_First_Enter = 0;
            lcd_pc_clr();
            eReadyTask_Sta = Ready_Refresh;
			uSetFlag.bits.Ready_First = 1;	//错误消除后蜂鸣2声
        }
    }
END: ;                           //空语句结束程序
}
#endif




