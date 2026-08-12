/**************************************************************************
文件名称：	App_BondTestMode.c
说    明：	绑定测试模式（应用层）
作    者:	Liaoy
版    本：	V1.0
芯	  片:	SN8P29XX
备    注:
修订记录：
**************************************************************************/
#include "Include.h"

//绑定测试模式任务
enum eBondTestModeTask
{
	BondTest_Init = 0,
	BondTest_Disp1 ,
	BondTest_Disp2 ,
	BondTest_Ntc ,
	BondTest_Tp ,
	BondTest_Version ,
	BondTest_Sensor ,
	BondTest_End
}eBondTestTask;

//此流程属于顺序执行的死循环程序，所有主程序所有时间轮询函数失效，请留意！
//调试态ON键关机，Test键切换步骤
void App_BondTestMode(void)
{
	switch( eBondTestTask )
	{
		case BondTest_Init:
			#if Func_debug
				Drv_UartTX_Init();
			#endif
			Drv_Adc_Init();
			Drv_Adc_Channel_Set(ACM_ACM);
			
			eBondTestTask = BondTest_Disp1;
			break;

		//显示测试画面1
		case BondTest_Disp1:
			LED_CloseAll();
			Disp_Debug1();
			#if Func_3color
				LED_Green_En();
			#endif
			Delay10ms(30);
			Tone_Init();
			while( !Get_Adc_Avg() );
			g_AdcDoff = g_AdcSum;
			while( Port_On )
			{
				//喂狗
				WDTR = 0x5A;
			}
			eBondTestTask = BondTest_Disp2;
			break;

		//显示测试画面2，并进行Ntc采集
		case BondTest_Disp2:
			Disp_Debug2();
			LED_Green_Dis();
			#if Func_3color
				LED_Yellow_En();
			#endif
			Adc_Channel_Init(TPTONTC);//ADC初始化和通道切换
			while( !Get_Ntc_Count() )
			{
				//喂狗
				WDTR = 0x5A;
			}
			#if Have_Motor
				g_MotorSystick = Vibration_time;
			#endif

			while( Port_On )
			{
				//喂狗
				WDTR = 0x5A;
			}
			Ntc_Caculate();
			eBondTestTask = BondTest_Ntc;
			break;

		//进行Ntc判定显示相应画面
		case BondTest_Ntc:
			#if Func_3color
				LED_Yellow_Dis();
				LED_Red_En();
			#endif
			Clr_Disp();
			if( Port_Debug == 0 )
			{
				//绑定厂检测
				if ( g_NtcCount < 0x2793  &&  g_NtcCount > 0x268D )
				{
					Disp_Code(Soft_Code);
					Delay10ms(100);
					Disp_Version(Soft_Internal_Version);
				}
				else
				{
					Disp_ErN(2);
				}

				//将无法测试的IO口全部放这里测试（IO口配置要正确），显示内容与绑定文件统一
				if(!Port_Set )
				{
					lcd_obj_en(); 	//显示物温符号
				}
				if(!Port_Mem)
				{
					lcd_mem_en();	//显示记忆符号
				}
				if(!Port_Rod) 		//检测杆
				{
					lcd_pc_en();	//显示耳套符号
				}
				if(!Port_Hall) 		//检测杆
				{
					lcd_forehead_en();	//显示额温符号
				}
			}
			else
			{
				//产线检测
				NtcTable_Check();
				if( uErrFlag.bits.Er2 )
				{
					Disp_ErN(2);
				}
				else
				{
					Disp_Code(Soft_Code);
					Delay10ms(100);
					Disp_Version(Soft_Internal_Version);
				}
			}


			Drv_PGA_Init(Adc_PGA_Gain);
			Drv_Adc_Channel_Set(AI3_AI4);	//切换到Tp采集
			while( !Get_Adc_Avg());
			g_TpCount = g_AdcSum;
			Delay10ms(50);
            while( Port_On )
			{
				//喂狗
				WDTR = 0x5A;
				// 耳套检测项
				if( Port_Rod )
				{
					lcd_pc_clr();
				}
				else
				{
					lcd_pc_en();
				}

				// 设定绑定厂无法进入
				#if Check_Parameter
					if( !Port_Debug && Port_Set)	//防止受入检进入
					{
						Clr_Disp();
						//显示传感器参数循环显示
						Disp_Temp(0,1,0,g_AdcDoff);
						Delay10ms(100);
						Disp_Temp(0,1,0,g_OpDoff);
						Delay10ms(100);
						Disp_Temp(0,1,0,g_Cali25TP);
						Delay10ms(100);
						Disp_Temp(0,1,0,g_Cali37Data);
						Delay10ms(100);
						Disp_Temp(0,1,0,g_Cali41Data);
						Delay10ms(100);
						//产线检测
						NtcTable_Check();

						Clr_Disp();
						if( uErrFlag.bits.Er2 )
						{
							Disp_ErN(2);
						}
						else
						{
							Disp_Code(Soft_Code);
							Disp_Version(Soft_Internal_Version);
						}

						Delay10ms(50);
					}
				#endif
			}
			eBondTestTask = BondTest_Tp;
			break;

		//进行Tp测试显示相应画面
		case BondTest_Tp:
			Clr_Disp();
			LED_CloseAll();
			#if Func_White
				LED_White_En();	
			#endif
			//需要一个热源进行判定，但不可以超过150度以排除断线的Adc count
			if( g_TpCount > 0x014C && g_TpCount < 4000 )
			{
				Disp_Sensor_Name();
				// 正常情况1s后自动关机
				Delay10ms(100);
			}
			else
			{
				Disp_ErN(1);
				Clr_ModeSign();
				Delay10ms(100);
				while( Port_On )
				{
					//喂狗
				    WDTR = 0x5A;
				}
			}
			eBondTestTask = BondTest_End;
			break;

		//关机，并还原任务状态
		case BondTest_End:
			uSetFlag.bits.PowerFirst = 1;		//绑定模式不需要不显示OFF的状态
			eBondTestTask = BondTest_Init;		//恢复当前任务以便下回从第一个任务开始
			eMain_Task = Task_Sleepmode;		//进入睡眠模式
			eSleepTask_Sta = Sleep_false;
			break;

		//关机
		default:
			break;
	}
}