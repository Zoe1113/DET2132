/**************************************************************************
文件名称：	App_Sleep.c
说    明：	关机模式（应用层）
作    者:	Liaoy
版    本：	V1.0
芯	  片:	SN8P29XX
修订记录：
**************************************************************************/
#include "Include.h"

eSleep_Status eSleepTask_Sta;
uint8  OFF_Flag ;

void App_Sleep(void)
{
	static uint8 l_buf;

	switch ( eSleepTask_Sta )
	{
		//关机初始化
		case Sleep_false:
			//请养成习惯，还原主任务相关变量
			//如果进入过生产模式则需要清除记忆
			Time_CountDown_5s_timeout(RESET);		//清除倒计时相关标志位
			Er1_Display_Sound(RESET);
			Er2_Display_Sound(RESET);
			High_Precision_Flag = 0;
			#if Func_Obj
			Er6_Display_Sound(RESET);
			#endif
			//如果进入过检验模式或黑体模式，则需初始化
			if(eTestmode_num == Insptectmode )
			{
				TimeAndMode_Set();
				Clr_All_Memory();
				CF_Check();
				eTestmode_num = Earmode;
			}
			//如果进入过检验模式或黑体模式，则需初始化
			if(eTestmode_num == Blackbodymode )
			{
				eTestmode_num = Earmode;
			}

			//主要变量初始化（错误清0）

			
			//各任务状态初始化
			eReadyTask_Sta = Ready_Init;
			eInitTask_Sta = Init_Set;
			eBle_Sta = Ble_Disable;	

            if(  uSetFlag.bits.VoiceEnable ==1 )
            {
                #if Have_Voice_Func
                    PlayStatueParam(2 , 0 , 0);
                    AM5BA_Send_Cmd(LL_Stop, SS_Stop, Nodata);
					Delay10ms(1);
                #endif				
            }

			//记忆状态初始化
			F_Mem_FirstEnter = 0;
			F_FirstEnter_SetMode = 0;

            #if !Have_Voice_Func
			    Drv_BZ_Disable();
            #endif
			LED_CloseAll();	//关背光
			#if !Func_White //环形灯
				LED_White_Dis();
			#endif

			// 判断是否为上电关机，为上电关机则不显示OFF
            if(uSetFlag.bits.PowerFirst)
			{
				Disp_OFF();
				Delay10ms(50);
			}
			else
			{
				uSetFlag.bits.PowerFirst = 1;
			}
			Clr_Disp();
			eSleepTask_Sta = Sleep_waitkey;
			break;

		//等待按键释放
		case Sleep_waitkey:
			//如果关机时一直长按则进入黑体模式(此处与原汇编不一样)
			if(uKeyPress.bits.OKeyPress)
			{

			}
			else
			{
				eSleepTask_Sta = Sleep_true;
				uStaFlag.g_StatusFlag &= 0x22;
			}
			break;

		//完全关机状态
		case Sleep_true:
			Drv_UartTX_Disable();	//关UART功能
			I2C_Disable();		//关I2C
			GPIO_PowerDown();	//IO口省电设置

			FCLKMD = 1;			//切到Slow mode
			NOP(2);
			FSTPHX = 1;			//关IHRC
			NOP(2);
			FDA1EN = 0;			//关DAC1
			FDA2EN = 0;			//关DAC2
			FOPA1EN = 0;		//关OP1
			FOPA2EN = 0;		//关OP2
			FAMPEN = 0 ;		//关PGA
			FPCHPEN = 0 ;		//关PGA chopper
			FACHPEN = 0 ;		//关ADC chopper
			FADC1EN = 0 ;		//关ADC1
			FADC2EN = 0;		//关ADC2
			FAVEN = 0 ;			//关AVE电压
			FACMEN = 0 ;		//关ACM电压
			FAVDDREN = 0 ;		//关avddr电压
			FBGCHP = 0;			//关bandgap chooper
			FBGREN = 0 ;		//关bandgap电压
			FLCDBNK = 1 ;		//All of the LCD dots off
			FLCDEN = 0 ;		//关LCD
			FLCDMOD0 = 1 ;
			FLCDMOD1 = 1 ;		//LCD Mode All OFF
			FLBTEN = 0 ;		//关低电压检测
			FTC0ENB = 0;		//关TC0 timer
			FTC1ENB = 0;		//关TC1 timer
			FTC2ENB = 0;		//关TC2 timer
			INTEN0 = 0;			//所有中断除能
			INTEN1 = 0;			//所有中断除能
			FGIE = 0 ;			//关总中断
			#if Func_Ble
			FCPUM1 = 1;		    //绿色模式
			#else
			FCPUM0= 1;		    //睡眠模式
			#endif
			NOP(2);

			//无按键按下
			l_buf = 1;
			#if Func_Ble
			while(l_buf)
			{
				#if  Func_TakeCoverAutoOn
					
				while( Port_On && Port_Set && (!Port_Hall || OFF_Flag) )

				 {
					if(!Port_Hall || !Port_On)
					{
						OFF_Flag = 0;
					}
				#else
				while( Port_On && Port_Set )
				{

				#endif 
					//喂狗
		            WDTR = 0x5A;
					if( FT0IRQ )
					{
						FT0IRQ = 0;
						Update_SysTime();	//系统时间更新
						FCPUM1 = 1;		//Green mode（不能关T0模块）
						NOP(2);
					}
				 }
				 Delay50us(4);	//green mode fcpu=32768/4=8k，是normal模式的250分之一，故实际延迟去抖为50ms
				 #if Func_TakeCoverAutoOn
				 if( !Port_On || !Port_Set || (Port_Hall && OFF_Flag == 0))
				 #else
				 if( !Port_On || !Port_Set )
				 #endif
				 {
					l_buf = 0;
				 }
			}
			#endif

			FSTPHX = 0;		//开IHRC
			NOP(2);
			FCLKMD = 0;		//关Slow模式
			NOP(2);

			GPIO_Init();	//IO口设置
			Lcd_Init();		//Lcd设置
			TC1Init();		//TC1设置（10ms）
			FGIE = 1;

			HalKey_KeyClr();	//清所有按键信息
			HalKey_Set_KeyMode(Func_Long, &sSetKey);	//设置键设为长按（ET05)
			l_buf = 0;
			eSleepTask_Sta = Sleep_wakeup;
			break;

		//按键唤醒等待
		case Sleep_wakeup:
			l_buf ++;
			//去抖80ms，保证扫键程序执行
			if( l_buf > 8 )
			{
				//设置键长按3s进设置态（设置不全显的机型适用，一般独立按键，非开机键）
				if( uKeyPress.bits.SetKeyPress )
				{
					if( uKeyHold.bits.SetKeyHold && !uSetFlag.bits.Unit_Change)
					{
						//进入设置态和快进为同一按键适用（当然非同一个按键也适用这样的写法）
						if( uStaFlag.bits.LowBat )
						{
							Clr_Disp();
							lcd_bat_en();	//显示低电压符号；
							Auto_TurnOff_Time_Sel();	//关机时间选择
							eSleepTask_Sta = Sleep_false;
							eMain_Task = Task_InitMode;
							eInitTask_Sta = Init_Err;

						}
						else
						{
							HalKey_KeyClr();	//清除所有按键信息
							eMain_Task = Task_Unitmode;
							App_SetMode();
							while(!Port_Set)
							{
								//喂狗
								WDTR = 0x5A;
							}
							eSleepTask_Sta = Sleep_false;	//还原当前任务状态
							
						}
					}
				}
				else
				{
					eSleepTask_Sta = Sleep_true;	//默认关机
					#if Func_TakeCoverAutoOn
					if( uKeyPress.bits.OKeyPress || (Port_Hall && OFF_Flag == 0))
					#else
					if( uKeyPress.bits.OKeyPress)
					#endif
					{
						eSleepTask_Sta = Sleep_End;	//只有开机键运行开机
					}
				}
			}
			break;

		//结束睡眠
		case Sleep_End:
		    g_AgeSelectNum = Age_36;
			Port_Ble_En = 0; //蓝牙使能准备
			High_Precision_Flag = 0;
			eSleepTask_Sta = Sleep_false;
			eMain_Task = Task_InitMode;
			Forehead_Ear_Check(RESET);	
			eTestmode_num = Earmode;
			F_Disp_Temp_Status = 0;
            uSetFlag.bits.Ready_First = 1;	//默认开启已就绪提示
			HalKey_Set_KeyMode(Func_Long, &sOnKey);   //开机&测量键设为短长按
			HalKey_Set_KeyMode(Func_Long, &sMemKey);	//记忆键为短长按
			HalKey_Set_KeyMode(Func_Short_Long, &sSetKey);	//模式键为长按
			HalKey_Set_KeyMode(Func_Short_Long, &sHallKey);	//声音键为短长按
			#if Have_Voice_Func
				AM5BA_SPI_Init();
				AM5BA_Power_Enable();
				PlayStatueParam(2 , 0 , 0);
                PlayStatueParam(1 , Play_Stop,0);
			#endif
			break;
		default:
			break;
	}
}
