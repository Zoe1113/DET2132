/**************************************************************************
文件名称：	App_SetMode.c
说    明：	设置模式（应用层）
作    者:	Liaoy
版    本：	V1.0
芯	  片:	SN8P29XX
备    注:
修订记录：
**************************************************************************/
#include "Include.h"

#define left 0
#define right 1

uint8 g_Blink;	//消隐
uint8 g_Blink_count;	//消隐时间(10ms时间基准)

eSetModeTask eSetTask;

bit F_UpdateValue;		//更新设置值
bit F_UpdateMenu;		//更新设置项
bit F_FirstEnter_SetMode;	//首次进入设置态
uint8 L_MaxDay;
void App_SetMode(void)
{
	
	uint8 L_Buf;

	//第一次进入必须刷新，原RatioModify和unit模式合并到设置态
	//单位切换能否合并关键在于按键的功能是否一样
	if( !F_FirstEnter_SetMode )
	{
		F_DispNtc_Time_Date_En = 0;
		F_FirstEnter_SetMode = 1;
		F_UpdateMenu = 1;	//首次需显示
		g_Blink_count = 0;
		High_Precision_Flag = 0;

		//初始化开机键为短按
 		HalKey_Set_KeyMode(Func_Short, &sSetKey);
		//初始化设置键为加速按
 		HalKey_Set_KeyMode(Func_Short_Continue, &sOnKey);
		Auto_TurnOff_Time_Sel();	//按下关机时间清0

		/*** 请特别注意不同型号此处设定 ***/
		HalKey_KeyClr();	//清除所有按键信息
		// //保证首次设置键长按不快进
		// uKeyRelease.bits.SKeyRelease = 1;
		// //保证首次开机键长按不切换（开机长按进设置态适用）
		// uKeyPress.bits.OKeyPress = 0;
		/*******************************/
        #if Func_Ble
            F_Ble_En = 0;
            Port_Ble_En = 0; //蓝牙不使能准备
        #endif

		#if Func_Reverse_Disp
			LED_Reverse_Disp_En();
		#endif

		//上电复位由于未使能，故无法播报（设计就是无法播报）
		Tone_Init();//语音功能
		
		#if ParamModif 
			if( eMain_Task == Task_ParamModifymode )
			{
				
					#if ET_FT !=1                               //额温，发射率不可调
						eSetTask = Set_Emission;
					#else
						eSetTask = Set_TableNum;                //黑体表格
					#endif
			
			}
		#endif

			if(eMain_Task == Task_Unitmode && uSetFlag.bits.Unit_Change == Unit_Change_En)
			{
				eSetTask = Set_Unit;
                Clr_Disp();
				Disp_Unit();
				LVD_Display();
		
			}

			while (!Port_Set)
			{
				//喂狗
				WDTR = 0x5A;
			}	

		//绑定检测判断，放这里的作用(只允许绑定厂在设置态进入，逼员工必须开机全显后进入)
		// Cal_Inspect_Detect();
	}

	//开机键按下并抬起后才触发切换设置项并更新显示(ET05/ETH)
	if( uKeyPress.bits.SetKeyPress )
	{
		uKeyPress.bits.SetKeyPress = 0;
		Auto_TurnOff_Time_Sel();	//按下关机时间清0
		F_UpdateMenu = 1;
		eSetTask ++;

		if( eMain_Task == Task_Unitmode )
		{
			if (eSetTask > Set_Unit)
			{
				eSetTask = Set_End;
			}
		}
		else if( eMain_Task == Task_Setmode )
		{
			//注意不同模式结束任务号不同
			#if Have_Voice_Func
				#if Have_VoiceLang_Change
					if (eSetTask > Set_VoiceLang)
					{
						eSetTask = Set_End;
					}
				#else
					if (eSetTask > Set_Day)
					{
						eSetTask = Set_End;
					}
				#endif
			#else
				if (eSetTask > Set_Day)
				{
					eSetTask = Set_End;
				}
			#endif
		}
	#if ParamModif
		else if( eMain_Task == Task_ParamModifymode )
		{
			//注意不同模式结束任务号不同
			if (eSetTask > Set_TableNum)
			{
				eSetTask = Set_End;
			}
		}
	#endif
	}

	if( uKeyPress.bits.OKeyPress )
	{
		uKeyRelease.bits.OKeyRelease = 0;
		Auto_TurnOff_Time_Sel();	//按下关机时间清0
	}

	if( uKeyPress.bits.SetKeyPress )
	{
		uKeyRelease.bits.SetKeyRelease = 0;
		//Auto_TurnOff_Time_Sel();	//按下关机时间清0
	}

	//设置键按下则立即触发累加并更新显示（ET215）
	if( !uKeyRelease.bits.OKeyRelease )
	{
		if( uKeyPress.bits.OKeyPress )
		{
			uKeyPress.bits.OKeyPress = 0;
			F_UpdateValue = 1;
		}

		if( uKeyHold.bits.OKeyHold )
		{
			F_UpdateValue = 1;
			uKeyHold.bits.OKeyHold = 0;
		}
	}

	//当前设置项处理(短按递增/长按快进递增)
	if( F_UpdateValue )
	{
		F_UpdateValue = 0;
		F_UpdateMenu = 1;		//修改参数后必须立刻更新显示
		switch( eSetTask )
		{
            //单位
			case Set_Unit:
				uSetFlag.bits.Unit = ~uSetFlag.bits.Unit;
				break;
			
  			//语种
			#if Have_Voice_Func
				#if Have_VoiceLang_Change
				case Set_VoiceLang:
					if( g_LLCode == lang_En)
						g_LLCode = lang_Ch;
					else
						g_LLCode = lang_En;
					break;
				#endif
			#endif

  			
	#if ParamModif
  			//发射率
			case Set_Emission:
				g_Emission ++;
				if( g_Emission > 100 )
					g_Emission = 96;
				break;

			//人体系数
		  	case Set_HumanRatio1:
			  g_HumanRatio1 ++;
			  if( g_HumanRatio1 > 60 )
				  g_HumanRatio1 = 0;
			  break;
		  	//人体系数
		  	case Set_HumanRatio2:
			  g_HumanRatio2 ++;
			  if( g_HumanRatio2 > 60 )
				  g_HumanRatio2 = 0;
			  break;

  			//耳套系数
			#if Func_Probecover
			case Set_Earcap10:
				g_Earcap10 ++;
				if( g_Earcap10 > 200 )
					g_Earcap10 = 0;
				break;

			//耳套系数
			case Set_Earcap15:
				g_Earcap15 ++;
				if( g_Earcap15 > 200 )
					g_Earcap15 = 0;
				break;

			//耳套系数
			case Set_Earcap20:
				g_Earcap20 ++;
				if( g_Earcap20 > 200 )
					g_Earcap20 = 0;
				break;

			//耳套系数
			case Set_Earcap25:
				g_Earcap25 ++;
				if( g_Earcap25 > 200 )
					g_Earcap25 = 0;
				break;

			//耳套系数
			case Set_Earcap30:
				g_Earcap30 ++;
				if( g_Earcap30 > 200 )
					g_Earcap30 = 0;
				break;

			//耳套系数
			case Set_Earcap35:
				g_Earcap35 ++;
				if( g_Earcap35 > 200 )
					g_Earcap35 = 0;
				break;

			//耳套系数
			case Set_Earcap40:
				g_Earcap40 ++;
				if( g_Earcap40 > 200 )
					g_Earcap40 = 0;
				break;
			#endif
			#if OffsetTable2open
 			//表格选项
			case Set_TableNum:
				uStaFlag.bits.TableNum = ~uStaFlag.bits.TableNum;
				break;
			#endif
	#endif
  			//关机
			default:
				break;
		}
	}

	//设置项切换处理(更新显示)
	if( F_UpdateMenu )
	{
		F_UpdateMenu = 0;
		switch( eSetTask )
		{
            //单位
			case Set_Unit:
                Clr_Disp();
				Disp_Unit();
				LVD_Display();
				break;

  			//语种
			#if Have_Voice_Func
				#if Have_VoiceLang_Change
				case Set_VoiceLang:
					if( g_LLCode == lang_En)
						Disp_En();
					else
						Disp_Ch();
					lcd_voice_en();
					break;
				#endif
			#endif
	#if ParamModif
  			//发射率
			case Set_Emission:
				Clr_Disp888();
				Clr_ModeSign();
				lcd_unit_cf_clr();
				lcd_unit_f_clr();
				lcd_unit_c_clr();
				Disp_Temp(0,0,0, (uint16)g_Emission*10);
				break;

  			//人体系数
			case Set_HumanRatio1:   //无耳套
				lcd_ear_en();
				Disp_Temp(0,0,0, (uint16)g_HumanRatio1*10);
			  break;
		  	//人体系数
		  	case Set_HumanRatio2:   //有耳套
				lcd_pc_en();
				lcd_ear_en();
				Disp_Temp(0,0,0, (uint16)g_HumanRatio2*10);
			  break;

			//耳套系数
			#if Func_Probecover
			case Set_Earcap10:
                // g_Earcap10 ++;
				// if( g_Earcap10 > 200 )
				// 	g_Earcap10 = 0;
                lcd_ear_clr();
                lcd_pc_en();;
				Disp_Temp(0,0,0,(uint16)g_Earcap10*10);
				break;
            //耳套系数
			case Set_Earcap15:
                // g_Earcap15 ++;
				// if( g_Earcap15 > 200 )
				// 	g_Earcap15 = 0;
                lcd_pc_en();
				Disp_Temp(0,0,0,(uint16)g_Earcap15*10);
				break;

  			//耳套系数
			case Set_Earcap20:
                // g_Earcap20 ++;
				// if( g_Earcap20 > 200 )
				// 	g_Earcap20 = 0;
                lcd_pc_en();
				Disp_Temp(0,0,0,(uint16)g_Earcap20*10);
				break;

  			//耳套系数
			case Set_Earcap25:
                // g_Earcap25 ++;
				// if( g_Earcap25 > 200 )
				// 	g_Earcap25 = 0;
                lcd_pc_en();
				Disp_Temp(0,0,0,(uint16)g_Earcap25*10);
				break;

  			//耳套系数
			case Set_Earcap30:
                // g_Earcap30 ++;
				// if( g_Earcap30 > 200 )
				// 	g_Earcap30 = 0;
                lcd_pc_en();
				Disp_Temp(0,0,0,(uint16)g_Earcap30*10);
				break;

  			//耳套系数
			case Set_Earcap35:
                // g_Earcap35 ++;
				// if( g_Earcap35 > 200 )
				// 	g_Earcap35 = 0;
                lcd_pc_en();
				Disp_Temp(0,0,0,(uint16)g_Earcap35*10);
				break;

  			//耳套系数
			case Set_Earcap40:
                // g_Earcap40 ++;
				// if( g_Earcap40 > 200 )
				// 	g_Earcap40 = 0;
                lcd_pc_en();
				Disp_Temp(0,0,0,(uint16)g_Earcap40*10);
				break;
			#endif

			#if OffsetTable2open
 			//表格选项
			case Set_TableNum:
				if( uStaFlag.bits.TableNum )
					Disp_Table2();
				else
					Disp_Table1();
				break;
			#endif
	#endif
  			//关机保存参数，并还原当前任务变量
			default:
			#if ParamModif
				
				
				I2C_masterInit();
				if(eMain_Task == Task_ParamModifymode)
				{
					#if OffsetTable2open
						L_Buf = (uStaFlag.g_StatusFlag >> 2) & 0x01;
						I2C_Byte_W(I2C_Add_Table, L_Buf);				//保存黑体修正表格
						Delay1ms(5);
					#endif



					I2C_Byte_W(I2C_Add_Emission, g_Emission);		//保存人体发射率
					Delay1ms(5);

					I2C_Byte_W(I2C_Add_HumanRatio1, g_HumanRatio1);	//保存人体系数
					Delay1ms(5);

					I2C_Byte_W(I2C_Add_HumanRatio2, g_HumanRatio2);	//保存人体系数
					Delay1ms(5);

					I2C_Byte_W(I2C_Add_Earcap10, g_Earcap10);		//g_Earcap10=100
                    Delay1ms(5);

                    I2C_Byte_W(I2C_Add_Earcap15, g_Earcap15);		//g_Earcap15=100
                    Delay1ms(5);

                    I2C_Byte_W(I2C_Add_Earcap20, g_Earcap20);		//g_Earcap15=100
                    Delay1ms(5);

                    I2C_Byte_W(I2C_Add_Earcap25, g_Earcap25);		//g_Earcap15=100
                    Delay1ms(5);

                    I2C_Byte_W(I2C_Add_Earcap30, g_Earcap30);		//g_Earcap15=100
                    Delay1ms(5);

                    I2C_Byte_W(I2C_Add_Earcap35, g_Earcap35);		//g_Earcap15=100
                    Delay1ms(5);

                    I2C_Byte_W(I2C_Add_Earcap40, g_Earcap40);		//g_Earcap15=100
                    Delay1ms(5);
				}
				I2C_Disable();
			#endif
				//语种
				#if Have_Voice_Func & Have_VoiceLang_Change
					I2C_masterInit();
					if( g_LLCode == lang_En)
						I2C_Byte_W(I2C_Add_Voice, lang_En);		//保存当前语音语言
					else
						I2C_Byte_W(I2C_Add_Voice, lang_Ch);		//保存当前语音语言
					Delay1ms(5);
					AM5BA_Power_Disable();
					I2C_Disable();
				#endif


				
			

				//还原变量，保证下次进入首先初始化起始设置项
				F_FirstEnter_SetMode = 0;

				//进入关机
				eMain_Task = Task_Sleepmode;
				OFF_Flag = 1;
				break;
		}
	}
}



