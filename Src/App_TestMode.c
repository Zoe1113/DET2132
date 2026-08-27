/**************************************************************************
文件名称：	App_TestMode.c
说    明：	测量模式（应用层）
作    者:	Liaoy
版    本：	V1.0
芯	  片:	SN8P29XX
备    注:
修订记录：
**************************************************************************/
#include "Include.h"


//测量模式状态
enum eTestmode_Status
{
	Test_Init = 0,
	Test_Get_Tp ,
	Test_Get_Ntc ,
	Test_Calculate ,
	Test_Disp ,
	Test_End
}eTestTask_Sta;

eTestmode eTestmode_num;

bit F_Disp_Dash;	//额温滚屏标志位
static int16 g_TpStep_max;	//额温最大值暂存区
static int16 g_TpStep_Disp;  //目标温度0.1分辨率

/*****************本程序部分介绍*******************
1、必须非常熟悉耳温、额温、物温、黑体、检验5大模式的区别才可以修改程序；
2、修改本程序必须递交科长审核，并提供测试样品和报告；
3、本程序大致流程为：ntc计算→不同模式tp采集→ntc采集→不同模式选择参数→计算→出结果
4、还要注意后面的采集平台架构，最好翻看下  貌似是ntc  tp  ntc tp？？
5、滤波算法要特别留意（filter和probe move）
probe move额温可能会有一定投诉，因为最大值已经出来了，Er3会导致清屏，耳温不会出现大幅移动概率极低；
可以考虑：
①、耳温有probe move，额温没有；---已实施；
②、Er3报错可以考虑使用POS替代，降低用户担心；
③、额温可以增加filter判断组数很低有Er3报错（接触扫描的过程中会有0.4度的变化吗）；
*************************************************/

void App_TestingMode(void)
{
	//新架构，按键的置位20ms扫描和按键的逻辑判断10ms。会造成抬起标志位清除不及时
	if( !uKeyPress.bits.OKeyPress )
	{
		uKeyRelease.bits.OKeyRelease = 0;
	}
	switch(eTestTask_Sta)
	{
		//按下测量键立即初始化任务
		case Test_Init:

		#if Func_debug
			Uart_Transmit(0xCC, 0xCCCC);
		#endif


			//关背光
			LED_CloseAll();
			// #if !Func_White //环形灯
			// 	LED_White_Dis();
			// #endif
			High_Precision_Flag = 0;

			#if Have_Voice_Func
				voice_stop();
			#endif

			//所有进入测量所需清除的变量都放在这里
			Clr_ModeSign();
			lcd_badface_clr();
			lcd_smileface_clr();
			lcd_mem_clr();
			F_Mem_FirstEnter = 0;
			uStaFlag.bits.Fever = 0;

			//保存按下时ntc数据
			Ntc_Caculate();
			NtcTable_Check();
			if(eTestmode_num == Insptectmode)
			{
				NtcTableWider_Check();
			}
			if( uErrFlag.bits.Er2)
			{
				Clr_Disp888();
				Disp_ErN(2);
				eTestTask_Sta = Test_Disp;
			}
			else
			{
				//备份相关参数
				Backup_Testing_Param();
				//除耳温外，均需切换到tp
				if( eTestmode_num != Earmode )
				{
					g_TpStep_max = 0x8000;	//负值最小值
					Adc_Channel_Init(NTCTOTP);//ADC初始化和通道切换
					F_First_Enter_ForeheadTp = 0;
					F_First_Enter_EarTp = 0;
				}
				else
				{
					g_AdcCount = 0;
				}
				eTestTask_Sta = Test_Get_Tp;
			}

		#if Func_debug
			Uart_Transmit(0x90, g_NtcCount);
		#endif

			break;

		//采集thermopile电压
		case Test_Get_Tp:
			if( Get_Adc_SingleRead() )
			{
				g_AdcCount ++;
				//程序空间紧张，因此将耳温模式和其他模式合并在一起
				if( eTestmode_num != Foreheadmode )
				{
					Get_Ear_Tp_Max( g_AdcData );		//找拐点
					if( g_AdcCount == Ear_Num )
					{
						if( Probe_Move(Ear_array,Ear_Num) )
						{
							uErrFlag.bits.Er3 = 0;
							g_AdcCount = Adc_Filter(Ear_array, Ear_Num);
							g_TpCount_avg = Get_Ear_Tp_Avg(g_AdcCount);		//计算按下后平均值
							
							if(eTestmode_num == Earmode)
								g_TpCount = TpCount_Determine();		//目标温度决定算法

							g_AdcCount = 0;		//清0保证下次可以继续循环

							if(eTestmode_num != Earmode)	//人体系数是20的用拐点算法，人体系数是45的用平均值算法
								g_TpCount = g_TpCount_avg;

							Adc_Channel_Init(TPTONTC);//ADC初始化和通道切换
							eTestTask_Sta = Test_Get_Ntc;
						}
						else
						{
							uErrFlag.g_ErrFlag = 0;		//清除其他错误
							uErrFlag.bits.Er3 = 1;
							eTestTask_Sta = Test_Disp;
						}
					}
				}
				else 
				{
					Get_Forehead_Tp( g_AdcData );
					if( g_AdcCount == Forehead_Num )
					{
						g_AdcCount = Remove_Tp_Max_Min( Forehead_array, g_AdcCount );//去除最大值
						g_AdcCount = Adc_Filter(Forehead_array, g_AdcCount);
						g_TpCount = Get_Forehead_Tp_Max_Avg(g_AdcCount);	//计算最大值平均值
						g_AdcCount = 0;		//清0保证下次可以继续循环
						eTestTask_Sta = Test_Calculate;		//额温不采集测量后ntc
					}
				}
			}
			break;

		//采集测量完成后的ntc温度
		case Test_Get_Ntc:
			if( Get_Ntc_Count() )
			{
				Ntc_Caculate();
				NtcTable_Check();
				if(eTestmode_num == Insptectmode)
				{
					NtcTableWider_Check();
				}
				//判定是否Er2错误
				if( uErrFlag.bits.Er2 )
				{
					eTestTask_Sta = Test_Disp;
				}
				else
				{
					//ntc决定
					g_NtcCount = NtcCount_Determine();
					eTestTask_Sta = Test_Calculate;
				}
			}
			break;
        


		//最终计算部分
		case Test_Calculate:
		#if Func_debug
			Uart_Transmit(0x91, g_NtcCount);
		#endif

		#if Func_debug
			Uart_Transmit(0x92, g_TpCount);
		#endif

			NtcTable_Find();

		#if Func_debug
			Uart_Transmit(0x93, g_NtcStep);
		#endif

			Tp_Caculate();

		#if Func_debug
			Uart_Transmit(0x94, g_TpCount);
		#endif

			Emissivity_correction();

		#if Func_debug
			Uart_Transmit(0x95, g_TpCount);
		#endif

			TpTable_Find();

		#if Func_debug
			Uart_Transmit(0x96, g_TpStep);
		#endif
			BlackBodyOffset();

		#if Func_debug
			Uart_Transmit(0x97, g_TpStep);
		#endif

			eTestTask_Sta = Test_Disp;	//默认测量结束
			switch( eTestmode_num )
			{
				case Earmode:
					#if Func_Probecover
						if( uStaFlag.bits.ProbeCover )
							Probecover_compensate();
					#endif
                    Ear_Compensate();
					Temp_Relate();
					LowTemp_Compensate();
					Body_MeasureRange_Check();
					g_TpStep_Disp = CToF(g_TpStep);
					g_TpStep_Disp = Temp_Resolution_Adjust(g_TpStep_Disp);
					Fever_alarm(g_TpStep_Disp ,g_AgeSelectNum);
					break;
				case Foreheadmode:
					Forehead_Compensate();
					Temp_Relate();

					#if Func_debug
						Uart_Transmit(0x98, g_TpStep);
					#endif

					// /*******额温长按保持释放结束写法********/
					if( g_TpStep > g_TpStep_max )
					{
						g_TpStep_max = g_TpStep;
					}

					F_First_Enter_ForeheadTp = 0;	//清0保证下次可以采集
					g_TpStep = g_TpStep_max;

					Body_MeasureRange_Check();
					g_TpStep_Disp = CToF(g_TpStep);
					g_TpStep_Disp = Temp_Resolution_Adjust(g_TpStep_Disp);

					Fever_alarm(g_TpStep_Disp ,g_AgeSelectNum);

					eTestTask_Sta = Test_Disp;


					break;
				case Objectmode:
					Temp_Relate();
					//g_TpStep=11000;
					Obj_MeasureRange_Check();
					g_TpStep_Disp = CToF(g_TpStep);
					g_TpStep_Disp = Temp_Resolution_Adjust(g_TpStep_Disp);
					#if Func_White
						LED_White_En();
					#elif Func_3color
						LED_Green_En();		
					#endif
					break;
				case Blackbodymode:
					#if Func_Probecover
						if( uStaFlag.bits.ProbeCover )
							Probecover_compensate();
					#endif
					Temp_Relate();
					Body_MeasureRange_Check();
					g_TpStep_Disp = CToF(g_TpStep);
					g_TpStep_Disp = Temp_Resolution_Adjust(g_TpStep_Disp);
					Fever_alarm(g_TpStep_Disp ,g_AgeSelectNum);
					break;
				case Insptectmode:
					Obj_MeasureRange_Check();
					g_TpStep_Disp = g_TpStep;
					break;
				default:
					break;
			}
			break;

		//显示处理
		case Test_Disp:
			Disp_Age_Select(g_AgeSelectNum);
			Clr_Disp888();
			if(eTestmode_num == Foreheadmode || eTestmode_num == Objectmode)
			{
				Disp_ModeSign();  
			}
			
			//不可以优化本程序，因为错误存在优先级（Er2>Lo/Hi>Er3/Er4）
			if( uErrFlag.g_ErrFlag )
			{
				Disp_ErrMsg();
				F_Disp_Temp_Status = 0;
			}
			else
			{
				if( eTestmode_num == Insptectmode )
                {
					High_Precision = g_TpStep_Disp;
                    FrameFlag = 1;
                    High_Precision_Flag = 1;
				}	
				else
					Disp_Temp(1,0,uSetFlag.bits.Unit,g_TpStep_Disp);	//显示温度
				Mem_Store(g_TpStep);	//存放的是0.01精度的
				F_Disp_Temp_Status = 1;
                g_Temp_Disp = g_TpStep;
				//只有有测量结果时才会上传记忆
                #if Func_Ble
					F_Ble_FirstEnter = 0;//重新读取
                    eBle_Sta = Ble_UploadData;
                #endif
            }
            if( uSetFlag.bits.VoiceEnable == 1 )
            {
                #if Have_Voice_Func
					if( eTestmode_num != Earmode && eTestmode_num != Blackbodymode)
					{
						PlayStatueParam(2 , 0 , 0);
						if( uErrFlag.bits.Hi || uErrFlag.bits.Lo )
						{
						
							PlayStatueParam(1 , Play_Di,0);
						}
						else if( uErrFlag.g_ErrFlag )
						{
							//PlayStatueParam(2 , 0 , 0);
							g_DiDo = (uint16)uErrFlag.g_ErrFlag;
							PlayStatueParam(1 , Play_Errmsg,0);
						}
						else
						{
							// PlayStatueParam(2 , 0 , 0);
							g_DiDo = g_TpStep_Disp;
							PlayStatueParam(1 , Play_Temp,0);
						}
					}
					else
					{
						if( uErrFlag.bits.Hi || uStaFlag.bits.Fever )
						{
							uStaFlag.bits.Fever = 0;
							BZ_Beep125();
							BZ_Beep50();
							BZ_Beep50();
							BZ_Beep50();
						}
						else if( uErrFlag.g_ErrFlag || uErrFlag.bits.Lo )
						{
							BZ_Beep50();
							BZ_Beep50();
							BZ_Beep50();
							BZ_Beep50();
						}
						else
						{
							BZ_Beep125();
						}
					}
                #else
					if( uErrFlag.bits.Hi || uStaFlag.bits.Fever )
					{
						uStaFlag.bits.Fever = 0;
						BZ_Beep125();
						BZ_Beep50();
						BZ_Beep50();
						BZ_Beep50();
					}
					else if( uErrFlag.g_ErrFlag || uErrFlag.bits.Lo )
					{
						BZ_Beep50();
						BZ_Beep50();
						BZ_Beep50();
						BZ_Beep50();
					}
					else
					{
						BZ_Beep125();
					}
                #endif
            }
            else
            {
                #if Have_Motor
                    if( eTestmode_num != Earmode && eTestmode_num != Blackbodymode )
                        g_MotorSystick = Vibration_time;
                #endif
            }
                 
                        
            eTestTask_Sta = Test_End;
            break;

		//测量结束回到Task_Ready准备就绪模式
		case Test_End:
			eTestTask_Sta = Test_Init;		//当前任务状态初始化
			eMain_Task = Task_ReadyMode;	//切换到下个任务
			uKeyRelease.bits.SetKeyRelease = 0;
			Adc_Channel_Init(TPTONTC);			//切换到ntc通道
			eReadyTask_Sta = Ready_Timeout;	//下个任务状态初始化
			g_5s_Count = CountDown_5s;		//开启5s等待
			g_3s_Count = CountDown_3s;		//开启背光3s倒计时
			break;

		default:
			break;
	}

}

//测量任务复位
void TestTask_Reset(void)
{
	eTestTask_Sta = Test_Init;		//当前任务状态初始化
}