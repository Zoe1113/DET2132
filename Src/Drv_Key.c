/**************************************************************************
文件名称：	Drv_Key.c
说    明：	按键处理相关函数集合（驱动层、应用层）
作    者:	Liaoy
版    本：	V1.0
芯	  片:	SN8P29XX
备	  注：	请留意本程序release的用法。
修订记录：
**************************************************************************/
#include "Include.h"

//按键状态eKeySta
enum eKeyStatus
{
	KeySta_Init = 0,	//按键初始化状态
	KeySta_Dither = 1,	//按键去抖状态
	KeySta_Comfirm = 2,	//按键确认状态
	KeySta_Release = 3	//按键释放状态
} eKeySta;

//按键键值eKeyVal
enum eKeyValue
{
	SetKey = 0x01,		 //Mode Key value
    OnKey   = 0x02,		 //On Key value
	MemKey  = 0x10,	 	//Mem Key value
    HallKey  = 0x20,	//Hall
	KeyMask = 0x33		 //按键掩码（p00 p01 p04 p05）
}eKeyVal;

//Mode键切换模式
enum ModeStatus
{
	Mode_Human_Age36 = 0,	//人体模式36+年龄分段 
	Mode_Human_Age0,		//人体模式0-36年龄分段
	Mode_Object,			//物温模式	
	Mode_Air,				//环境温度
};


uKey1 uKeyPress;
uKey2 uKeyHold;
uKey3 uKeyRelease;
uKey4 uKeyContinue;

strKey sSetKey, sOnKey, sHallKey, sMemKey ;
uint8 ModeFlag = 0;
uint16 L_OnPressTime = 0;
unsigned long L_Disp_Ntc; //显示环温
int16 g_Temp_Disp;
unsigned long L_TpStep;
uint8 F_Disp_Temp_Status = 0;

uint8 g_Switch_Temp_Flag = 0;	//从耳额温切换到物温室温的标志位
/**************************************************************************
函数名称：	Voice_Show_Init()
函数功能：	开关语音函数
输入参数：	无
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Voice_Show_Init(void)
{
    if(uSetFlag.bits.VoiceEnable==1)     //语音功能开启
    {
		lcd_voice_clr();			//显示喇叭
		lcd_sound_off_clr();
        #if Have_Voice_Func             
			voice_stop();
            PlayStatueParam(1 , Play_Di,0);
        #else
            BZ_Beep125();
        #endif
		// 耳温模式不打开震动
		g_MotorSystick = 0;
    }
    else                                //语音功能关闭
    {
		#if Have_Voice_Func
			PlayStatueParam(2 , 0 , 0);
			AM5BA_Send_Cmd(LL_Stop, SS_Stop, Nodata);
		#endif
		lcd_voice_en();
		lcd_sound_off_en();

			
			// 打开震动
			g_MotorSystick = Vibration_time;
		
    }  
}
void Switch_Temp(void)
{
	L_Disp_Ntc = CToF(g_NtcStep);
	L_Disp_Ntc = Temp_Resolution_Adjust(L_Disp_Ntc);
	Disp_Temp(1,0,uSetFlag.bits.Unit,L_Disp_Ntc);
}



/**************************************************************************
函数名称：	void App_KeyProcess(void)
函数功能：	按键功能组合处理子程序（应用层）
输入参数：	无
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	仅适用于Task_ReadyMode状态
与扫键程序不同的是，此函数属于应用层，用于识别不同事件产生对应的任务标志位
比如：
1、长按测量键开机3s进入单位切换；→ 在sleep中处理;ok
2、短按开机键开机；→ ok；
3、长按开机键3s进入设置，或长按设置键3s进入设置；→ 全显的在init模式处理，不全显的在sleep处理；ok
4、Ready模式长按开机键3s进入记忆；→ ok，在本程序中处理；ok
5、Ready模式长按开机键3s到关机再长按3s进入校准；→ ok，在sleep中处理；
6、Ready模式长按开机键并立即长按测量键1.2s进行耳物温切换；→ ok
7、Ready模式检测io进入绑定、系数调整等；ok
8、Ready模式检测耳套；ok
9、长按开机、两次按下测量进入生产态；ok；
**************************************************************************/

//长按测量键8s进入黑体
void APP_Enter_Blackbodymode(void)
{
	if(eReadyTask_Sta == Ready_ReadyOk || eReadyTask_Sta == Ready_DisEr2)
	{
		if(uKeyPress.bits.OKeyPress)
		{
			L_OnPressTime++;
		}
		if(L_OnPressTime == 800)
		{
			L_OnPressTime = 0;
			Clr_Disp();
			lcd_point_en();	//显示小数点
			#if Func_Reverse_Disp
				LED_Reverse_Disp_En();
			#endif
			if( uSetFlag.bits.VoiceEnable )
			{
				#if Have_Voice_Func
					AM5BA_Send_Cmd(LL_Broadcast, SS_Di, 0x00);
				#else
					BZ_Beep400();
					BZ_Beep400();
					BZ_Beep400();
				#endif
			}

			while( !Port_On )//等待释放进入
			{
				//喂狗
				WDTR = 0x5A;
			}	
			Disp_CAL();	//显示CAL表示进入黑体模式
			Delay10ms(100);
			Disp_Code(Soft_Code);	//显示程序编码
			Delay10ms(100);
			Disp_Version(Soft_External_Version);	//显示对外程序版本
			Delay10ms(100);
			HalKey_KeyClr();
			Adc_Channel_Init(TPTONTC);	//某些模式会只采集tp故必须切回ntc采集，ADC初始化和通道切换
			Er2_Display_Sound(RESET);//复位Er2错误
			#if Func_Ble
				if( !Port_Ble_Link )
				{
					lcd_ble_en();
				}
			#endif
			ModeFlag = Mode_Human_Age36;
			eTestmode_num = Blackbodymode;	//代表进入黑体模式
			eSleepTask_Sta = Sleep_false;	//设置当前任务状态为初始状态
			eMain_Task = Task_ReadyMode;
			eReadyTask_Sta = Ready_Init;	//进入ready模式
		}

	}
}

//Ready模式下的开机与测量键
void App_OnKeyProcess(void)
{
	if(uKeyRelease.bits.OKeyRelease && eReadyTask_Sta == Ready_ReadyOk) //ready模式准备完成，测量键按下并抬起开始测量
	{
		Auto_TurnOff_Time_Sel();
		eMain_Task = Task_Testingmode;	//准备进入测量模式
		TestTask_Reset();				//测量任务复位
	}
	if(eReadyTask_Sta == Ready_Timeout && eTestmode_num != Foreheadmode && eTestmode_num != Objectmode && uKeyRelease.bits.OKeyRelease)//耳温、黑体、生产模式测温完成有5s等待。如何在等待中按下测量键就报er1
	{
		eReadyTask_Sta = Ready_DisEr1;
		if(eTestmode_num == Insptectmode)
		{
			lcd8 = 0x00;
			lcd9 = 0x00;
		}
	}
	if( !uKeyPress.bits.OKeyPress )
	{
		uKeyRelease.bits.OKeyRelease = 0;
		L_OnPressTime = 0;
	}
}




//记忆模式下的按键操作
void App_MemoryOnKeyProcess(void)
{

	uKeyRelease.bits.SetKeyRelease = 0;
	//记忆模式下，测量键按下并抬起开始测量
	if(uKeyRelease.bits.OKeyRelease)
	{
		Auto_TurnOff_Time_Sel();
		lcd_mem_clr();
		if( eTestmode_num != Insptectmode )
		{
			F_Colon_Blink = Disable;
		}
		if(eTestmode_num==Airmode)
		{
			eMain_Task=Task_ReadyMode;
			eReadyTask_Sta = Ready_Init;
			if(uSetFlag.bits.VoiceEnable)
			{
				#if Have_Voice_Func
					voice_stop();
					PlayStatueParam(1 , Play_Di,0);
				#else
						BZ_Beep125();
				#endif
			}
			else
			{
				#if Have_Motor
						g_MotorSystick = Vibration_time;
				#endif	
			}
		}
		else
		{
			#if Func_Obj
			if(uErrFlag.bits.Er6)
			{
				Er6_Display_Sound(RESET);
				eMain_Task = Task_ReadyMode;
				eReadyTask_Sta = Ready_DisEr6;	//只要有错需跳转到错误处理状态
			}
			else
			#endif
			{
				Er1_Display_Sound(RESET);
				eMain_Task = Task_Testingmode;	//准备进入测量模式   
				TestTask_Reset();				//测量任务复位
			}	
		}
	}


	if( !uKeyPress.bits.OKeyPress )
	{
		uKeyRelease.bits.OKeyRelease = 0;
	}

}

//设置键，切换不同模式

void App_SetKeyProcess(void)
{
	static uint8 F_SKey_Deal=0;		//设置键长按处理
	//设置键长按切换物温室温
    if( uKeyPress.bits.SetKeyPress && eMain_Task != Task_Memorymode)									//模式键按下
    {
        uKeyRelease.bits.SetKeyRelease = 0;

        #if Nation	//长按切换物温室温仅国外模式(Nation=1)有效
        if(	uKeyHold.bits.SetKeyHold && !uErrFlag.bits.Er2)			//如果长按三秒
        {
            Auto_TurnOff_Time_Sel();	//按下关机时间清0
            F_SKey_Deal = 1;
            sSetKey.g_Key_Hold_cnt = 0;	//按键计时清0保证再次长按3s
            uKeyHold.bits.SetKeyHold = 0;
            #if Func_Obj
                if( (eTestmode_num !=Insptectmode )&& (eTestmode_num !=Blackbodymode ))
                {
                    //切换模式状态标志处理
                    Adc_Channel_Init(TPTONTC);			//重新采集ntc
					Er1_Display_Sound(RESET);		//清除Er1相关标志位
                    Er6_Display_Sound(RESET);
					Time_CountDown_5s_timeout(RESET);		//清除倒计时相关标志位
                    g_5s_Count = 0;       //切换模式，清零计数器。马上可以测量
                    uErrFlag.g_ErrFlag = 0;			//清除所有错误					
                    uErrFlag.bits.Er6 = 0;			//恢复原Er6产生的错误标志位
					if(g_Switch_Temp_Flag)//从耳额温默认切换到物温
                    {
						g_Switch_Temp_Flag=0;
                        eTestmode_num = Objectmode;
						lcd_pc_clr();
						Clr_Age_Select();
						Disp_ModeSign();
						eReadyTask_Sta = Ready_Refresh;
                    }
					else 
					{
						if( eTestmode_num == Objectmode )
						{
							eTestmode_num = Airmode;		
							lcd_badface_clr();		//清除哭脸
							lcd_smileface_clr();	//清除笑脸
							lcd_pc_clr();
							Clr_Age_Select();
							Disp_ModeSign();
							Switch_Temp();
							Er6_Display_Sound(RESET);		//清除Er6相关标志位
							eReadyTask_Sta = Ready_NoRefresh;
						}
						else
						{
							eTestmode_num = Objectmode;
							lcd_pc_clr();
							Clr_Age_Select();
							Disp_ModeSign();
							eReadyTask_Sta = Ready_Refresh;
						}	
					}
                    
					LED_CloseAll();
					#if Func_White
						LED_White_En();
					#elif Func_3color
						LED_Green_En();		
					#endif
					
					g_3s_Count = CountDown_3s;	//开启背光
					if(uSetFlag.bits.VoiceEnable)
					{
						#if Have_Voice_Func
							voice_stop();
							PlayStatueParam(1 , Play_Di,0);
						#else
								BZ_Beep125();
						#endif
					}
					else
					{
						#if Have_Motor
								g_MotorSystick = Vibration_time;
						#endif	
					}
                }
            #endif	
        }
        #endif
    }
	else if(!uKeyPress.bits.SetKeyPress && eMain_Task != Task_Memorymode)
	{
		if( uKeyRelease.bits.SetKeyRelease )							
        {
            uKeyRelease.bits.SetKeyRelease = 0;	
            if( !F_SKey_Deal && eReadyTask_Sta !=Ready_DisEr1 && !uErrFlag.bits.Er2 && eTestmode_num != Insptectmode&&eTestmode_num != Objectmode&&eTestmode_num != Airmode)
            {
                Auto_TurnOff_Time_Sel();	//按下关机时间清0
				Select_Age();
				Disp_Age_Select(g_AgeSelectNum);
                Disp_VoiceSign();
				Disp_Unit();	//显示单位
				LED_CloseAll();
				#if Func_White
					LED_White_En();
				#elif Func_3color
					LED_Green_En();		
				#endif
				if(eReadyTask_Sta == Ready_ReadyOk)
                    Disp_Ready();
				g_3s_Count = CountDown_3s;	//开启背光
				if(uSetFlag.bits.VoiceEnable)
				{
					#if Have_Voice_Func
						voice_stop();
						PlayStatueParam(1 , Play_Di,0);
					#else
							BZ_Beep125();
					#endif
				}
				else
				{
					#if Have_Motor
							g_MotorSystick = Vibration_time;
					#endif	
				}		
                
            }	
            F_SKey_Deal =0;
        }
	}
}

//记忆键功能
void App_MemKeyProcess(void)
{
    static uint8 F_MemKey_Deal=0;		//记忆键长按处理
    //记忆键长按3s开关蜂鸣
    if( uKeyPress.bits.MemKeyPress )									//开机键按下
    {
        uKeyRelease.bits.MemKeyRelease = 0;
        if(	uKeyHold.bits.MemKeyHold && !uErrFlag.bits.Er2 && !uErrFlag.bits.Er6&&!F_MemKey_Deal )			//如果长按三秒
        {
			F_MemKey_Deal = 1;
			uKeyHold.bits.SetKeyHold = 0;
			sSetKey.g_Key_Hold_cnt = 0;	//按键计时清0保证再次长按3s
			uKeyRelease.bits.SetKeyRelease = 0;
			Auto_TurnOff_Time_Sel();
			uSetFlag.bits.VoiceEnable =	~uSetFlag.bits.VoiceEnable;
			Voice_Show_Init();	//声音开关
            
            Disp_Ready();	//首次开机必须显示_ _._
            Disp_ModeSign();	//显示模式符号
            Disp_Age_Select(g_AgeSelectNum);
        }
        else if(!uKeyHold.bits.MemKeyHold && !uErrFlag.bits.Er2 && !uErrFlag.bits.Er6&&!F_MemKey_Deal&&eMain_Task == Task_ReadyMode&&eTestmode_num !=Insptectmode)
        {
            //Clr_Disp();
			lcd_pc_clr();	//消隐耳套符号
			
			lcd_smileface_clr();
			lcd_badface_clr();
			lcd_unit_c_clr();
			lcd_unit_f_clr();
			lcd_unit_cf_clr();
			Clr_Disp888();
			Clr_ModeSign();
			Clr_Age_Select();
            lcd4 = DispTable[ 1 ] >> 8;
		    lcd5 = DispTable[ 1 ];
			if(eTestmode_num==Airmode)
			{
				eReadyTask_Sta = Ready_ReadyOk;
			}
			
        }
    }
    //记忆键短按进入记忆查看
    else
    {
        if(uKeyRelease.bits.MemKeyRelease)
        {
            uKeyRelease.bits.MemKeyRelease = 0;
            if( !F_MemKey_Deal && !uErrFlag.bits.Er2 && !uErrFlag.bits.Er1 && eTestmode_num != Insptectmode )//生产模式无记忆
            {
                Auto_TurnOff_Time_Sel();	//按下关机时间清0
                F_Mem_FirstEnter = 0;	//清首次进入记忆模式标志位
                eMain_Task = Task_Memorymode;					//置进入记忆模式标志位
                uErrFlag.g_ErrFlag = 0;		//清错误标志位
                eReadyTask_Sta = Ready_ReadyOk;
            }
            F_MemKey_Deal =0;
        }
    }
}

#if Func_Probecover
//耳套检测函数
void App_PCKeyProcess(void)
{
	//耳温/黑体/生产模式耳套按下则表示带耳套，反之无耳套

	if( !Port_Rod && Port_Hall)
	{
		if(!uStaFlag.bits.ProbeCover && !uErrFlag.bits.Er2)
		{
			Auto_TurnOff_Time_Sel();//自动关机
		}
		lcd_pc_en();	//显示耳套符号
		uStaFlag.bits.ProbeCover = 1;
		if(eTestmode_num == Foreheadmode || eTestmode_num == Airmode)
		{
			lcd_pc_clr();	//消隐耳套符号
		}
		if (eTestmode_num == Objectmode  )
		{
			uErrFlag.g_ErrFlag = 0;	//清除其他错误
			uErrFlag.bits.Er6 = 1;
			lcd_pc_en();	//显示耳套符号	
			if(eMain_Task != Task_Memorymode)	
			{
				eReadyTask_Sta = Ready_DisEr6;	//只要有错需跳转到错误处理状态
				eMain_Task = Task_ReadyMode;
			}
	
		}

	}
	else
	{
		if(uStaFlag.bits.ProbeCover && !uErrFlag.bits.Er2)
		{
			Auto_TurnOff_Time_Sel();//自动关机
		}
		lcd_pc_clr();	//消隐耳套符号
		uStaFlag.bits.ProbeCover = 0;
		uErrFlag.bits.Er6 = 0;

	}

	
}
#endif

/**************************************************************************
    函数名称：	void Select_Age(void)
    函数功能：	选择不同年龄分段
    输入参数：	g_AgeSelectNum
    输出参数：	g_AgeSelectNum
    返回值  ：	无
    占用空间：	TBD
****************************************************************************/
void  Select_Age(void)
{
    g_AgeSelectNum ++;
    if(g_AgeSelectNum > Age_36)
    {
        g_AgeSelectNum = Age_0;
    }

}


/**************************************************************************
函数名称：	uint8 HalKey_ReadKeyVal(void)
函数功能：	读取按键键值
输入参数：	P0
输出参数：	无
返回值  ：	L_keydata（按键键值）
占用空间：	TBD
备    注：	无
**************************************************************************/
uint8 HalKey_ReadKeyVal(void)
{
	uint8 L_keydata;
	L_keydata = P0 & KeyMask;
	L_keydata ^= KeyMask;	//低电平按下需异或处理
	return L_keydata;
}

/**************************************************************************
函数名称：	void HalKey_KeyScan(void)
函数功能：	扫键函数
输入参数：	无
输出参数：	uOTKeyFlag、uSMKeyFlag
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void HalKey_KeyScan(void)
{
	uint8 L_Keydata;
	L_Keydata = HalKey_ReadKeyVal();
    HalKey_Scan(L_Keydata & MemKey, &sMemKey);
	HalKey_Scan(L_Keydata & OnKey, &sOnKey);
	HalKey_Scan(L_Keydata & SetKey,  &sSetKey);
	HalKey_Scan(L_Keydata & HallKey, &sHallKey);
}

/**************************************************************************
函数名称：	void HalKey_KeyScan(void)
函数功能：	扫键函数
输入参数：	无
输出参数：	uOTKeyFlag、uSMKeyFlag、uKeyRelease、g_KeyContinue_Flag、g_KeyPress_Flag、g_KeyHold_Flag
返回值  ：	无
占用空间：	TBD
备    注：	uKeyRelease必须手动清除
**************************************************************************/
void HalKey_Scan(uint8 L_Keydata, strKey *sKey)
{
	switch (sKey->g_Key_Status)
	{
		//按键等待状态
		case KeySta_Init:
			if (L_Keydata) //如有按键按下
			{
				sKey->g_Key_Hold_cnt = 0;
				sKey->g_Key_Val = L_Keydata;
				sKey->g_Key_Status = KeySta_Dither;
			}
			break;
		//按键去抖状态
		case KeySta_Dither:
			if ( sKey->g_Key_Val == L_Keydata )
			{
				sKey->g_Key_Hold_cnt ++;
				if ( sKey->g_Key_Hold_cnt > CNT_Dither)
				{
					sKey->g_Key_Status = KeySta_Comfirm;
					uKeyPress.g_KeyPress_Flag |= L_Keydata;	//置已按下标志位
				}
			}
			else
			{
				sKey->g_Key_Status = KeySta_Init;
			}
			break;
		//按键确认状态
		case KeySta_Comfirm:
			if (L_Keydata == sKey->g_Key_Val)
			{
				if (sKey->g_Key_Hold_cnt < 0xFE) //不可超出否则从0开始
				{
					sKey->g_Key_Hold_cnt ++;
				}
				if (sKey->g_Key_Hold_cnt > sKey->g_Key_preset_cnt)
				{
					uKeyHold.g_KeyHold_Flag |= L_Keydata;	//置按键长按标志位
					//如果使能加速按
					if ( sKey->g_KeyFun & En_Cp )
					{
						uKeyContinue.g_KeyContinue_Flag |= L_Keydata;	//置按键加速按标志位
						sKey->g_Key_Hold_cnt = sKey->g_Key_preset_cnt - CNT_CPInterval;
					}
				}
			}
			else
			{
				sKey->g_Key_Status = KeySta_Release;
			}
			break;
		//按键释放状态
		case KeySta_Release:
			if (!L_Keydata)
			{
				uKeyHold.g_KeyHold_Flag &= ~sKey->g_Key_Val;
				uKeyPress.g_KeyPress_Flag &= ~sKey->g_Key_Val;
				uKeyContinue.g_KeyContinue_Flag &= ~sKey->g_Key_Val;
				uKeyRelease.g_KeyRelease_Flag |= sKey->g_Key_Val;
				sKey->g_Key_Status = KeySta_Init;
			}
			break;
		default:
			break;
	}
}

/**************************************************************************
函数名称：	void HalKey_Set_KeyMode(uint8 function, struct strKey *sKey )
函数功能：	开机键按键功能设置
输入参数：	mode（短按/长按/超长按/加速按/无设定默认为长按）
输出参数：	sKey->g_KeyFun（按键功能）、sKey->g_Key_preset_cnt（按键计数器初值）
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void HalKey_Set_KeyMode(uint8 function, strKey *sKey )
{
	switch (function)
	{
		case Func_Short:
			sKey->g_KeyFun &= ~En_Cp; 	//非加速按
			sKey->g_Key_preset_cnt = CNT_Invalid;
			break;
		case Func_Short_Long:
			sKey->g_KeyFun &= ~En_Cp;	//非加速按
			sKey->g_Key_preset_cnt = CNT_ShortLong;
			break;
		case Func_Long:
			sKey->g_KeyFun &= ~En_Cp;	//非加速按
			sKey->g_Key_preset_cnt = CNT_LongPress;
			break;
		case Func_Short_Continue:
			sKey->g_KeyFun |= En_Cp; 	//加速按
			sKey->g_Key_preset_cnt = CNT_EnterCP;
			break;
		default:
			sKey->g_KeyFun &= ~En_Cp; 	//非加速按
			sKey->g_Key_preset_cnt = CNT_LongPress;
			break;
		}
}

/**************************************************************************
函数名称：	void HalKey_KeyClr(void)
函数功能：	清除所有按键信息（非所有，重点部分）
输入参数：	g_KeyHold_Flag、g_KeyPress_Flag、g_KeyRelease_Flag、g_KeyContinue_Flag、g_Key_Val（键值）、g_Key_Status（按键状态）
输出参数：	g_KeyHold_Flag、g_KeyPress_Flag、g_KeyRelease_Flag、g_KeyContinue_Flag、g_Key_Val（键值）、g_Key_Status（按键状态）
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void HalKey_KeyClr(void)
{
	uKeyHold.g_KeyHold_Flag = 0;
	uKeyPress.g_KeyPress_Flag = 0;
	uKeyRelease.g_KeyRelease_Flag = 0;
	uKeyContinue.g_KeyContinue_Flag = 0;
	sOnKey.g_Key_Val = 0;
	sMemKey.g_Key_Val = 0;
	sSetKey.g_Key_Val = 0;
	sHallKey.g_Key_Val = 0;
	sOnKey.g_Key_Status = 0;
	sMemKey.g_Key_Status = 0;
	sSetKey.g_Key_Status = 0;
	sHallKey.g_Key_Status = 0;

}
