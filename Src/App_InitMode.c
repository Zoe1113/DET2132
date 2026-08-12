/**************************************************************************
文件名称：	App_InitMode.c
说    明：	待机模式（应用层）
作    者:	Liaoy
版    本：	V1.0
芯	  片:	SN8P29XX
备    注:
修订记录：
**************************************************************************/
#include "Include.h"


eInitModeTask eInitTask_Sta;

void App_InitMode(void)
{

	switch( eInitTask_Sta )
	{
		//初始化设置
		case Init_Set:
			Drv_Adc_Init();	//AD设置
			Drv_Adc_Channel_Set(ACM_ACM);
			Delay1ms(5);
			#if Func_debug
				Drv_UartTX_Init();
			#endif

			//默认跳到下个状态
			eInitTask_Sta = Init_Disp;
			//EEPROM参数校验(如果校验和识别码都错则初始化数据，如果仅校验和错误则报错)
			//EEPROM参数校验(Er5无法启动语音播报，因为语种可能未读出)
			Parm_AutoCheck();//程序参数校验
			break;

		//全显蜂鸣，并读取参数（实测500ms）
		case Init_Disp:
			//低电压检测
			#if Second_LVD == 1
				LBD_Chk();
			#else
				LVD_Init();
				LVD_Chk();
			#endif
			//uStaFlag.bits.midBat = 1;
			if(uStaFlag.bits.LowBat)
			{
				Clr_Disp();
				lcd_bat_en();	//显示低电压符号；
				Auto_TurnOff_Time_Sel();	//关机时间选择
				eInitTask_Sta = Init_Err;	
				#if Have_Voice_Func
                    if(  uSetFlag.bits.VoiceEnable ==1 )
                    {
						PlayStatueParam(2 , 0 , 0);
						PlayStatueParam(1 , Play_Lowbat,0);			
                    }
				#endif
			}
			else
			{
				Disp_All();
				F_LED_Enable = Enable;	//开启背光
				#if  Func_3color
					LED_Green_En();
				#elif Func_White
					LED_White_En();	
				#endif

                if(  uSetFlag.bits.VoiceEnable == 1 )
                {
					lcd_voice_en();				//显示喇叭
                    #if Have_Voice_Func
                    PlayStatueParam(2 , 0 , 0);
                    PlayStatueParam(1 , Play_VolumeMax,0);
                    PlayStatueParam(1 , Play_Di,0);
                    #else
                        BZ_Beep125();
                    #endif	
					// g_MotorSystick =0;
                }
                else
                {  
					#if Have_Motor
						g_MotorSystick = 25;
					#endif
                }

				//默认跳到下个状态
				eInitTask_Sta = Init_ADDoff;
			}
			break;

		//采集Adc的Doff值
		case Init_ADDoff:
			//Adc Doff采集，14笔*0.016ms=224ms(采14，丢4，去最大最小，求8平均)，实测300ms
			if( Get_Adc_Avg() )
			{
				g_AdcDoff = g_AdcSum;
				Drv_PGA_Init(Adc_PGA_Gain);
				Drv_Adc_Channel_Set(ACM_ACM);
				eInitTask_Sta = Init_OpDoff;
			}
			break;

		//采集PGA的Doff值
		case Init_OpDoff:
			//Adc offset采集，14笔*0.016ms=224ms(采14，丢4，去最大最小，求8平均)，实测300ms
			if( Get_Adc_Avg() )
			{
				g_OpDoff = g_AdcSum;
				Adc_Channel_Init(TPTONTC);	//ADC初始化和通道切换
				eInitTask_Sta = Init_Ntc;
			}
			break;

		//采集thermistor温度
		case Init_Ntc:
			//计算ntc，保证环温可以显示，224*2=448ms（实测700ms）
			if( Get_Ntc_Count() )
			{
				Ntc_Caculate();
				NtcTable_Check();
				if( !uErrFlag.bits.Er2 )	//此处错误不做处理
				{
					NtcTable_Find();
				}
				eInitTask_Sta = Init_Wait;
			}
			break;

		//错误处理
		case Init_Err:
			//等待关机
			if ( uKeyPress.bits.OKeyPress && uKeyRelease.bits.OKeyRelease )
			{
				eInitTask_Sta = Init_Set;	//将当前状态设置为初始状态
				eMain_Task = Task_Sleepmode;
				eSleepTask_Sta = Sleep_false;
			}
			break;

		//1.5s全显背光等待
		case Init_Wait:
			if( !F_LED_Enable )
				eInitTask_Sta = Init_Key;
			break;

		//按键判断
		case Init_Key:
			//是否长按3s？（设置全显的机型适用）
			if( uKeyPress.bits.OKeyPress )
			{
				//必须等待on键释放才退出判断,否则进入ready会立刻关机
				if( uKeyRelease.bits.OKeyRelease )
				{
					eInitTask_Sta = Init_End;
				}
				//优先判断测量键是否抬起后又长按进入生产检验模式
				if( uKeyHold.bits.SetKeyHold &&  uKeyRelease.bits.SetKeyRelease )
				{
					Clr_Disp();
					Disp_BadFace();
                    Clr_All_Memory();  
					uSetFlag.bits.Unit = Unit_C;	//进入检验态默认C
					NtcTableWider_Check();
					eTestmode_num = Insptectmode;	//非任务切换，仅改变测量模式而已
				}
			}
			else
			{
				eInitTask_Sta = Init_End;
			}
			break;

		//初始化任务结束，还原当前任务为初始状态
		case Init_End:
			High_Precision_Flag = 0;
			HalKey_Set_KeyMode(Func_Long, &sSetKey);	//声音键为短长按
			HalKey_KeyClr();	//清除所有按键信息
			eInitTask_Sta = Init_Set;		//任务切换必须将当前任务状态设定为初始状态
			eMain_Task = Task_ReadyMode;	//切换到下一个任务
			eReadyTask_Sta = Ready_Init;


			break;

		//默认保留
		default:
			break;
	}
}

//上电的系统时间初始化
void TimeAndMode_Set(void)
{
	//参数设置，如年月日时分秒
	g_Hour = 0;
	g_Minute = 0;
	g_Day = 1;
	g_Month = 1;
	g_Year = Default_Year;
	g_Second = 0;
	uSetFlag.bits.TimeFormat = TimeFormat_24H;	//默认24小时制
	uSetFlag.bits.VoiceEnable = Enable;	//默认开启语音
	
	uSetFlag.bits.BleEnable = Enable;
    uSetFlag.bits.Ready_First = 1;	//默认开启已就绪提示
}



