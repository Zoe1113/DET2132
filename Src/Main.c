/**************************************************************************
文件名称：	ET215_2988_SJ_V1.0_20200704.c
说    明：	耳套可选,测量时间可选,单位焊接选择,语音可选,背光可选,物温可选
程序编码：	详见Variable.h
版    本：	V1.0
适用 PCB:	ET05 PCB(B)
芯	  片:	SN8P2988
作    者:	Liaoy
修订记录：
1、在ETTest20200730（215，修改蜂鸣等）基础上正式程序ET215程序；
2022-12-12
1）添加蓝牙模块，屏蔽黑体表格2；
2）记忆存储，30组混存；
3）添加振动功能；
4）修改按键功能；
5）修改LCD显示；
6）修改低电压检测：国内一级电压，国外二级电压；
2023-02-01
1)修改语音、振动、蜂鸣的配置
2023-02-16
1）第一次上电不显示
2)切到物温之后，长按set键3秒切不回耳额温
3)设置状态，日期设置后进行单位设置，月份图标会显示没有消失
4)自动关机时间实际是1min，PRD要求30S
5)额温物温模式下，测量间隔不正确，要语音播报完才能进行下一次测量
6)测量结果在37.8以下，无法显示哭脸和笑脸，37.8以上哭脸显示1秒后消失未常显
7)长按测量键关机过程中，设备会进行一次测量并语音播报
8)记忆序号从记忆12开始显示错误\查看记忆，记忆值未区分显示模式
2023-02-21
1)修改额温模式下，测量完多一个就绪现象
2）开机状态下直接断电，重新上电设备会显示一下off关机---不显示0FF关机
3）进入生产模式下，用户模式的记忆值还是会显示，通用产品不会/进入生产模式后退出到用户模式，记忆未清除
4）低电量状态无语音播报
5）校准模式，环温数值显示和精度均不正确
6）每次切换记忆时，电池电量符号都会闪烁刷新一下
7）蜂鸣款：无蓝牙款，全显时有蓝牙显示
2023-02-22
1)修改无记忆状态下，进入记忆查看，显示异常
2)耳温，额温，物温符号在测量结果显示
3)耳温模式测温等待过程中，带上额温套会显示异常
4)记忆查看情况下，直接按下测量键进行测量，显示测量结果时，时间日期的显示会慢1S左右
2023-02-28
1)优化设置模式下提示音；
2）优化测量结果显示时，再次按下测量键，测量就续语音播报会播报两次
**************************************************************************/
#include "Include.h"

//主程序
void main(void)
{
    WDTR = 0x5A; //喂狗
	//系统时钟设定
 	OSCM = 0x00;		//开启内置IHRC=8M，外置低速晶振32768，Fcpu=Normal mode
	Delay1ms(5);

	//部分参数设置，如年月日时分秒
	TimeAndMode_Set();

	GPIO_Init();//IO口设置
	Lcd_Init();	//Lcd设置
	CF_Check();	//单位状态确认

	//上电开机
    #if Electricity_poweroff==0
		eMain_Task = Task_Sleepmode;
		eSleepTask_Sta = Sleep_End;
	#elif Electricity_poweroff==1
	    eMain_Task = Task_Sleepmode;
		eSleepTask_Sta = Sleep_false;
	#endif
	eTestmode_num = Earmode;	//默认耳温模式

	#if Electricity_poweroff==1
		// 如果为上电直接关机则不显示OFF
        uSetFlag.bits.PowerFirst = 0;
	#else
		// 不是则不管
		uSetFlag.bits.PowerFirst = 1;
	#endif
	OFF_Flag = 1;
	//低电压检测
	#if Second_LVD == 1
		LBD_Chk();
	#else
		LVD_Init();
		LVD_Chk();
	#endif
	if( uStaFlag.bits.LowBat )
	{
		Clr_Disp();
		lcd_bat_en();
		uKeyRelease.bits.OKeyRelease = 1;  //防止上电报低电时需要按两次才能关机
		eMain_Task = Task_InitMode;
		eInitTask_Sta = Init_Err;
		eSleepTask_Sta = Sleep_false;
		
		uSetFlag.bits.PowerFirst = 1;
		while ( !Port_On )
		{
			//喂狗
			WDTR = 0x5A;
		}
	}
	Parm_AutoCheck();//程序参数校验
	TC1Init();	//TC1设置（10ms）
	#if Func_Ble
	T0Init();	//开启RTC中断
	#endif
	FGIE = 1;	//使能总中断等

    #if Have_Voice_Func
		AM5BA_Power_Disable();
	#endif
	HalKey_Set_KeyMode(Func_Long, &sSetKey);	//设置键为短长按
	//Tone_Init();//语音功能
	// 重新设置自动关机时间
	Auto_TurnOff_Time_Sel();	//按下关机时间清0
	// 绑定检测，因为修改上电进入模式的原因，移到此处；
	if(!uStaFlag.bits.LowBat)
	{
		Cal_Inspect_Detect();
	}
	//Param_Init();
	//Delay10ms(100);
	while(1)
	{
		//喂狗
		WDTR = 0x5A;
		//10ms基本定时器
		if(F_10ms)
		{
			F_10ms=0;
			Time_Creat_20ms_50ms();
			if (eMain_Task == Task_Memorymode || eMain_Task == Task_ReadyMode)
			{
    			ReadyMode_NtcMeas();            //F_ReadyOk置位则采集NTC
				if(uErrFlag.bits.Er2 && eReadyTask_Sta > 0)
				{
					eReadyTask_Sta = Ready_DisEr2;
					eMain_Task = Task_ReadyMode;
				}
			}

			if( (eMain_Task == Task_ReadyMode && eReadyTask_Sta != Ready_Init ) || eMain_Task == Task_Testingmode || eMain_Task == Task_Memorymode)
			{
				Forehead_Ear_Check(RUN);
			}

		}

		//20ms任务轮询
		if(F_20ms)
		{
			F_20ms = 0;

			//扫描按键状态
			HalKey_KeyScan();

			//三色背光
			if( F_LED_Enable )
			{
				Light_RGB();
			}

			if( (eMain_Task == Task_ReadyMode && eReadyTask_Sta != Ready_Init ) || eMain_Task == Task_Testingmode || eMain_Task == Task_Memorymode)
			{
				//3s背光倒计时
				Led_CountDown_3s();
			}

			//自动关机
			if (eMain_Task == Task_ReadyMode || eMain_Task == Task_Setmode || eMain_Task == Task_InitMode || eMain_Task == Task_Unitmode  || eMain_Task == Task_Memorymode)
				Auto_TurnOff();
		}

		//50ms更新显示时间
 		if(F_50ms)
		{
			F_50ms = 0;
			//额温滚屏
			if( F_Disp_Dash && ( !uErrFlag.g_ErrFlag || uErrFlag.bits.Er1 ) )
			{
				if( eTestmode_num != Objectmode && eTestmode_num != Foreheadmode )
				{
					Disp_Dash();
				}
				else
				{
					F_Disp_Dash = Disable;
				}
			}

            //蓝牙超时计数器
			#if Func_Ble
				g_ble_ack_timeout ++;
			#endif
		}

		//500ms更新显示
 		if(F_500ms)
		{
			F_500ms = 0;

			//系统时间更新
			Update_SysTime();
			if(High_Precision_Flag)
				Disp_High_Precision(uSetFlag.bits.Unit,High_Precision);
			#if Second_LVD
				if( eMain_Task == Task_InitMode || eMain_Task == Task_BondTestmode)
				{
				}
				else if( eMain_Task == Task_Setmode || eMain_Task == Task_Sleepmode)
				{
					lcd_bat_clr();
					lcd_bat_full_clr();
				}
				else
				{
					LVD_Display();
				}
			#endif
				
			#if Func_Ble
				//蓝牙超时时间累加
				g_ble_timeout ++;
				//蓝牙标志闪烁
                if( F_Ble_Blink && F_Ble_En && eMain_Task != Task_Calimode && eMain_Task != Task_BondTestmode && eMain_Task != Task_Sleepmode )
				{
					lcd_ble_xor();
				}

				if( Ble_TimeoutErr == eBle_Sta )    //如果在断线状态，长时间拉低
				{
					Ble_Waittostart++;
				}				
			#endif

		}

		#if Have_Voice_Func
			if( uSetFlag.bits.VoiceEnable )
			{
				App_PlayVoice3();
			}
		#endif

		//仅用户模式的耳温、额温、物温启用蓝牙传输，其他均不启动蓝牙传输
		#if Func_Ble
			if( uSetFlag.bits.BleEnable == Enable )
			{
                if( eMain_Task != Task_BondTestmode &&  eTestmode_num != Insptectmode )
				{
					App_BleMode();
				}
			}
		#endif
		//主任务(10ms轮询，因为ADC基本是16ms，不可以20ms）
		if( F_10ms_task )
		{
			F_10ms_task = 0;

			switch(eMain_Task)
			{
				//开机初始化状态（即全显前的流程）
				case Task_InitMode:
					App_InitMode();
					break;

				//全显后测量前这一阶段等待就绪状态
				case Task_ReadyMode:
					App_ReadyMode();
					break;

				//测量模式任务（里面细分耳温、额温、物温、生产、黑体）
				case Task_Testingmode:
					App_TestingMode();
					break;

				//设置模式任务
				case Task_Setmode:
					App_SetMode();
					break;
					
				//单位切换模式任务
				case Task_Unitmode:
					App_SetMode();
					break;

				//校准模式任务
				case Task_Calimode:
					App_CaliMode();
					break;

				//校准模式任务
				case Task_Discali:
					#if Distence_En
						App_Discali();
					#endif 
					break;

				//绑定模式任务
				case Task_BondTestmode:
					App_BondTestMode();
					break;

			#if ParamModif
				// 参数调整模式任务
				case Task_ParamModifymode:
						App_SetMode();
					break;
			#endif	

				//关机模式任务
				case Task_Sleepmode:			
					App_Sleep();
					break;

				//记忆模式任务
				case Task_Memorymode:
					App_Memory();
					break;

				//标配保留
				default:
					break;
			}
		}
	}
}
