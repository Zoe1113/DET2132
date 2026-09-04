/**************************************************************************
文件名称：	Drv_GPIO.c
说    明：	GPIO初始化函数集合（驱动层）
作    者:	Liaoy
版    本：	V1.0
芯	  片:	SN8P29XX
修订记录：
**************************************************************************/
#include "Include.h"
uint8 g_MotorSystick = 0;		//振动马达的剩余时间
// bit ET_mode_buf = 0;		//模式备份标志(具备耳额物产品需要)
/**************************************************************************
函数名称：	Cal_Inspect_Detect()
函数功能：	校准模式、绑定模式入口检测
输入参数：	Port_Debug、Port_Cal
输出参数：	uSetFlag
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Cal_Inspect_Detect(void)
{
	//绑定检测模式、校温检测模式、系数调整模式判断
	if( !Port_Debug && Port_Cal)
	{
		if ( eTestmode_num == Insptectmode )
		{
			#if ParamModif
				LED_CloseAll();
				eMain_Task = Task_ParamModifymode;	//生产模式短路debug口则进入系数调整模式
			#endif
		}
		else
		{
			F_FirstEnter_SetMode = 0;
			eMain_Task = Task_BondTestmode;	//用户模式下短路debug进入绑定检测模式
		}
	}

	// //设置态决不允许进入校温，汇编和C的程序结构不一样，未初始化adc
	if( !Port_Cal && !Port_Debug&& eMain_Task == Task_ReadyMode )
	{
		#if Func_Ble
            F_Ble_En = Disable;         //进入校准关闭蓝牙
            Port_Ble_En = 0;            //蓝牙失能
        #endif
		eMain_Task = Task_Calimode;		//用户模式下短路debug和cal进入校温模式
	}
}

/**************************************************************************
函数名称：	CF_Check()
函数功能：	单位状态、单位可切换状态查询
输入参数：	Port_CF、Port_Change_CF
输出参数：	uSetFlag
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void CF_Check(void)
{
	//单位状态检测
	uSetFlag.bits.Unit = Unit_C;
	if( !Port_CF )
	{
		uSetFlag.bits.Unit = Unit_F;
	}

	//单位可切换状态检测
	uSetFlag.bits.Unit_Change = Unit_Change_En;
	if( !Port_Change_CF )
	{
		uSetFlag.bits.Unit_Change = Unit_Change_Dis;
	}

}

/**************************************************************************
函数名称：	Forehead_Ear_Check()
函数功能：	模式切换查询
输入参数：	无
输出参数：	uSetFlag.bits.VoiceEnable
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Forehead_Ear_Check(bit cmd)
{

	static bit Frist_Earmode = 0;
	static bit Frist_Foreheadmode = 0;
	static bit Frist_Mode = 0;

	if(cmd) // 状态复位
	{
		Frist_Earmode = 0;
		Frist_Foreheadmode = 0;
		Frist_Mode = 0;
		return;
	}

    if( eTestmode_num != Insptectmode && eTestmode_num != Blackbodymode && !uErrFlag.bits.Er2 ) 
    {
		#if Func_Hall
		if(Port_Hall)	//霍尔检测
		#else
        if(Port_Rod)	//检测杆检测
		#endif
		{
			if(!Frist_Earmode)
			{
				eTestmode_num = Earmode;	//默认耳温模式
				if(eReadyTask_Sta != Ready_Timeout)
				{
					Adc_Channel_Init(NTCTOTP);
				}
				Frist_Earmode = 1;
				Frist_Mode = 1;
				Frist_Foreheadmode = 0;
				// #if !Func_White
				// //没戴额温罩，声音关闭的情况下开启夜灯
				// if(!uSetFlag.bits.VoiceEnable)
				// LED_White_En();
				// #endif

			}


		}              
        else
        {
            if(!Frist_Foreheadmode)
			{
				eTestmode_num = Foreheadmode;	//默认额温模式
				Adc_Channel_Init(TPTONTC);
				Frist_Foreheadmode = 1;
				Frist_Mode = 1;
				Frist_Earmode = 0;	
				#if !Func_White
					LED_White_Dis();
				#endif
			}

            
        }

		// if(uSetFlag.bits.Ready_First)
		// {
		// 	Frist_Mode = 0;
		// }

		if(Frist_Mode)
		{
			Frist_Mode = 0;
			g_AgeSelectNum = Age_36;
			F_Disp_Temp_Status = 0;
			g_Switch_Temp_Flag = 1;//从耳额温切换到物温室温的标志位
			if(eMain_Task == Task_Memorymode)
			{
				return;
			}
        	eMain_Task = Task_ReadyMode;
			eReadyTask_Sta = Ready_Refresh;
			#if !Have_Voice_Func //蜂鸣款的在模式切换的时候，由于蜂鸣操作时延时。所以清屏和显示会有延时，为了解决这一问题把清屏的显示提前
				// Clr_Disp888();
				// Disp_Ready();	//首次开机必须显示_ _._
				// Disp_Unit();	//显示单位
				Disp_VoiceSign();
				LED_CloseAll();
				#if Func_White
					LED_White_En();
				#elif Func_3color
					LED_Green_En();		
				#endif
			#endif
			Disp_ModeSign();	//显示模式符号
			Disp_Age_Select(g_AgeSelectNum);
			Er1_Display_Sound(RESET);		//清除Er1相关标志位
			#if Func_Obj
			Er6_Display_Sound(RESET);		//清除Er6相关标志位
			#endif
			Auto_TurnOff_Time_Sel();
			uSetFlag.bits.Ready_First = 1;
			if( uSetFlag.bits.VoiceEnable == 1)// && ET_mode_buf
			{
				
				#if Have_Voice_Func
				if(eTestmode_num == Earmode)
				{
					voice_stop();
				}
				#endif
			}
			#if Have_Motor
				if( (uSetFlag.bits.VoiceEnable == 0) &&(eTestmode_num != Earmode))
				{
					g_MotorSystick = Vibration_time;
				}			 	
				else
				{
					g_MotorSystick = 0;
				}
			#endif
		}
		
    }
}

/**************************************************************************
函数名称：	Voice_Change()
函数功能：	语音开关状态切换
输入参数：	无
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Voice_Change(void)
{
    uSetFlag.bits.VoiceEnable = !uSetFlag.bits.VoiceEnable ;  
}

/**************************************************************************
函数名称：	GPIO_Init()
函数功能：	IO口初始化设置
输入参数：	P0、P1、P2、P3、P4、P5
输出参数：	P0、P1、P2、P3、P4、P5
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void GPIO_Init(void)
{
	//SDA/SCL输出为0(因为采用的是硬体I2C),按键输入上拉高电平,XIN/XOUT输入不上拉低
	//0: input mode, 1: output mode
	P0M = 0x0C;			//0b0000 1100
	//0: disable pullup, 1: enable pullup, pull resistor = 200k
	P0UR = 0x33;		//0b0011 0011
	//0: low level, 1: high level
	P0 = 0x3F;			//0b0011 1111

#if Have_Voice_Func
	//白灯输出不上拉高电平、MOSI输出不上拉高电平，MISO输入不上拉高电平,SCK输出不上拉高电平,TX输入不上拉高电平（防止向BLE供电）,RX输入不上拉,VOC_EN输出上拉低,BLE_LINK输入不上拉高
	//0: input mode, 1: output mode
	P1M = 0x47;			//0b0100 0111
	//0: LCD functon pin, 1: IO pin
	P1SEG = 0xFF;		//均为普通IO口
	//0: disable pullup, 1: enable pullup, pull resistor = 200k
	P1UR = 0xC0;		//0b1100 0000
	//0: low level, 1: hFFigh level
	P1 = 0xFD;			//0b1111 1101
#else
	//白灯输出不上拉高电平,MOSI输出不上拉高电平,MISO输入不上拉高电平,SCK输出不上拉高电平,TX输入不上拉高电平（防止向BLE供电）,RX输入不上拉,VOC_EN,BLE_LINK输入不上拉1
	//0: input mode, 1: output mode
	P1M = 0x4F;			//0b0100 1111
	//0: LCD functon pin, 1: IO pin
	P1SEG = 0xFF;		//均为普通IO口
	//0: disable pullup, 1: enable pullup, pull resistor = 200k
	P1UR = 0xC0;		//0b0100 0000
	//0: low level, 1: high lDevel
	P1 = 0xB9;			//0b1011 1001
#endif

	//MOTOR输出不上拉低电平,C/、CF、CAL、Debug输入上拉高
	//0: input mode, 1: output mode
	P5M = 0xE1;			//0b1110 0001
	//0: LCD functon pin, 1: IO pin
	P5SEG = 0xFF;		//0b1111 1111
	//0: disable pullup, 1: enable pullup, pull resistor = 200k
	P5UR = 0x1E;		//0b0001 1110
	//0: low level, 1: high level
	P5 = 0xFE;			//0b1111 1110

	//P25-P27为seg口，LINK脚输入 不上拉 高,EN输出 上拉 低，BUSY输入不上拉高，G320_EN输出不上拉高，VOC_EN输出不上拉低
	//0: input mode, 1: output mode
	P2M = 0x0B;			//0b0000 1011
	//0: LCD functon pin, 1: IO pin
	P2SEG = 0x1F;		//0b0001 1111,P25-P27均为LCD口
	//0: disable pullup, 1: enable pullup, pull resistor = 200k
	P2UR = 0x08;		//0b0000 1000
	//0: low level, 1: high level
	P2 = 0x16;			//0b0001 0110

	//0: input mode, 1: output mode
	P3M = 0x00;			//均为LCD口
	//0: LCD functon pin, 1: IO pin
	P3SEG = 0x00;		//均为LCD口
	//0: disable pullup, 1: enable pullup, pull resistor = 200k
	P3UR = 0x00;		//均为LCD口
	//0: low level, 1: high level
	P3 = 0x00;			//均为LCD口

	//0: input mode, 1: output mode
	P4M = 0x00;			//均为LCD口
	//0: LCD functon pin, 1: IO pin
	P4SEG = 0x00;		//均为LCD口
	//0: disable pullup, 1: enable pullup, pull resistor = 200k
	P4UR = 0x00;		//均为LCD口
	//0: low level, 1: high level
	P4 = 0x00;			//均为LCD口
}


/**************************************************************************
函数名称：	GPIO_PowerDown()
函数功能：	IO口初始化设置
输入参数：	P0、P1、P2、P3、P4、P5
输出参数：	P0、P1、P2、P3、P4、P5
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void GPIO_PowerDown( void )
{
	//关机时仅保留唤醒按键上拉，其余非唤醒脚输出低，降低静态电流
	//0: input mode, 1: output mode
	P0M = 0x1C;			//0b0001 1100
	//0: disable pullup, 1: enable pullup, pull resistor = 200k
	P0UR = 0x23;		//0b0010 0011
	//0: low level, 1: high level
	P0 = 0x2F;			//0b0010 1111

#if !Func_Ble
	//白灯输入上拉高，按键输入上拉高电平,BZ/MOSI/MISO/TX/RX/SCK均输出不上拉低，
	//0: input mode, 1: output mode
	P1M = 0xFE;			//0b 1111 1110
	//0: LCD functon pin, 1: IO pin
	P1SEG = 0xFF;		//均为普通IO口
	//0: disable pullup, 1: enable pullup, pull resistor = 200k
	P1UR = 0x01;		//0b 0100 0001
	//0: low level, 1: high level
	P1 = 0x01;			//0b 0100 0001

	//P25-P27为seg口，BLE_Link、BLE_EN、BLE_BUSY、G320_EN输出不上拉低，VOC_EN输出上拉高
	//0: input mode, 1: output mode
	P2M = 0x03;			//0b 0000 0011
	//0: LCD functon pin, 1: IO pin
	P2SEG = 0x1F;		//0b 0001 1111,P25-P27均为LCD口
	//0: disable pullup, 1: enable pullup, pull resistor = 200k
	P2UR = 0x1D;		//0b 0001 1101
	//0: low level, 1: high level
	P2 = 0x1D;			//0b 0001 1101
#else
	//按键输出不上拉低,，SCK/RX1/TX1/MISO/MOSI/BZ输出不上拉低,白灯输入上拉高，
	//0: input mode, 1: output mode
	P1M = 0xFE;			//0b 1111 1110
	//0: LCD functon pin, 1: IO pin
	P1SEG = 0xFF;		//均为普通IO口
	//0: disable pullup, 1: enable pullup, pull resistor = 200k
	P1UR = 0x01;		//0b 0000 0001
	//0: low level, 1: high level
	P1 = 0x01;			//0b 0000 0001

	//P25-P27为seg口，BLE_Link、BLE_EN、BLE_BUSY、G320_EN输出不上拉低，VOC_EN输出上拉高
	//0: input mode, 1: output mode
	P2M = 0x1F;			//0b 0001 1111
	//0: LCD functon pin, 1: IO pin
	P2SEG = 0x1F;		//0b 0001 1111,P25-P27均为LCD口
	//0: disable pullup, 1: enable pullup, pull resistor = 200k
	P2UR = 0x01;		//0b 0000 0001
	//0: low level, 1: high level
	P2 = 0x01;			//0b 0000 0001
#endif

    //RGB输入上拉高，CAL/Debug输入上拉高电平,C/F、CF输出不上拉0，MOTOR输出不上拉低
    //0: input mode, 1: output mode
    P5M = 0x07;			//0b0000 0111
    //0: LCD functon pin, 1: IO pin
    P5SEG = 0xFF;		//0b1111 1111
    //0: disable pullup, 1: enable pullup, pull resistor = 200k
    P5UR = 0xF8;		//0b1111 1000
    //0: low level, 1: high level
    P5 = 0xF8;			//0b1111 1000


	//0: input mode, 1: output mode
	P3M = 0x00;			//均为LCD口
	//0: LCD functon pin, 1: IO pin
	P3SEG = 0x00;		//均为LCD口
	//0: disable pullup, 1: enable pullup, pull resistor = 200k
	P3UR = 0x00;		//均为LCD口
	//0: low level, 1: high level
	P3 = 0x00;			//均为LCD口

	//0: input mode, 1: output mode
	P4M = 0x00;			//均为LCD口
	//0: LCD functon pin, 1: IO pin
	P4SEG = 0x00;		//均为LCD口
	//0: disable pullup, 1: enable pullup, pull resistor = 200k
	P4UR = 0x00;		//均为LCD口
	//0: low level, 1: high level
	P4 = 0x00;			//均为LCD口
}