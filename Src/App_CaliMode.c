/**************************************************************************
文件名称：	App_CaliMode.c
说    明：	校准模式（应用层）
作    者:	Liaoy
版    本：	V1.0
芯	  片:	SN8P29XX
备    注:
修订记录：
**************************************************************************/
#include "Include.h"

//校准模式任务
enum eCaliModeTask
{
	Cali_Ntc = 0,
	Cali_Air = 1,
	Cali_37C = 2,
	Cali_41C = 3,
	Cali_Store = 4,
	Cali_Err = 5,
	Cali_End = 6
}eCaliTask;

bit F_CaliMode_KeyPress;	//按键按下标志位

//此流程属于顺序执行的死循环程序，所有主程序所有时间轮询函数失效，请留意！
void App_CaliMode(void)
{
	int32 L_AirTPCount;

	switch( eCaliTask )
	{
		//显示Ab后，显示当前环境温度
		case Cali_Ntc:
			#if Func_Ble
			F_Ble_En = Disable;	//关闭蓝牙
			#endif
			LED_CloseAll();
			Disp_Ab();
			Tone_Init();
			Delay10ms(100);
			Adc_Channel_Init(TPTONTC);//ADC初始化和通道切换
			do
			{
				//喂狗
				WDTR = 0x5A;
				if( Get_Ntc_Count() )
				{
					Ntc_Caculate();
					NtcTableWider_Check();
					if( uErrFlag.bits.Er2 )
					{
						Disp_ErN(2);
						eCaliTask = Cali_Err;
					}
					else
					{
						NtcTable_Find();
						Disp_Temp(1,1,0,g_NtcStep);
					}
				}
			}while( Port_On );
			eCaliTask = Cali_Air;
			//有错直接跳到错误处理状态
			if( uErrFlag.bits.Er2 )
				eCaliTask = Cali_Err;
			break;

		//对空采集基准值
		case Cali_Air:
			//计算相对25℃环温的Tp值
			g_Cali25TP = TpCount_Relative_25C();
			Drv_PGA_Init(Adc_PGA_Gain);
			Drv_Adc_Channel_Set(AI3_AI4);	//切换到Tp采集
			F_CaliMode_KeyPress = 1;	//强行进入S_ADStable判稳
			while( eCaliTask )
			{
				//喂狗
				WDTR = 0x5A;
				eCaliTask = S_Adc_Stable();	//用了一个返回值技巧进行case跳转，这种方法慎用
			}
			Tone_Init();
			eCaliTask = Cali_37C;
			break;

		//显示37.0并进行37.0校准
		case Cali_37C:
			L_AirTPCount = g_AdcSum;
			Clr_Disp();
			Disp_Temp(1,0,0,3700);
			Delay10ms(100);
			Clr_Disp();
			do
			{
				//喂狗
				WDTR = 0x5A;
				S_Adc_Stable();
				Disp_Temp(0,0,0,g_AdcSum);		//显示实时Adc值
			}while( Port_On );
			while( S_Adc_Stable() )				//用了一个返回值技巧进行case跳转，这种方法慎用
			{
				//喂狗
				WDTR = 0x5A;
				Disp_Temp(0,0,0,g_AdcSum);
			}
			Tone_Init();
			if( uStaFlag.bits.Er8 )
			{
				Clr_Disp();
				Disp_ErN(8);
				eCaliTask = Cali_Err;
			}
			else
			{
				g_Cali37Data = g_AdcSum - g_OpDoff;
				Disp_PAS();
				Delay10ms(100);
				eCaliTask = Cali_41C;
			}
			break;

		//显示41.0并进行41.0校准
		case Cali_41C:
			Clr_Disp();
			Disp_Temp(1,0,0,4100);
			Delay10ms(100);
			Clr_Disp();
			do
			{
				//喂狗
				WDTR = 0x5A;
				S_Adc_Stable();
				Disp_Temp(0,0,0,(g_AdcSum-L_AirTPCount));		//实时Adc值
			}while( Port_On );
			while( S_Adc_Stable() )
			{
				//喂狗
				WDTR = 0x5A;
				Disp_Temp(0,0,0,(g_AdcSum-L_AirTPCount));		//实时Adc值
			}
			Tone_Init();
			if( uStaFlag.bits.Er8 )
			{
				Clr_Disp();
				Disp_ErN(8);
				eCaliTask = Cali_Err;
			}
			else
			{
				g_Cali41Data = g_AdcSum - g_OpDoff;
				Disp_PAS();
				Delay10ms(100);
				eCaliTask = Cali_Store;
			}
			break;

		//存储校准值和校验和
		case Cali_Store:
			/* 耳套参数初始化，防止重绑的板子耳套参数不对*/
			Probecover_Param_Init();
 			Param_Calistore();

			eCaliTask = Cali_End;	//默认正常退出
			//判定校验和是否错误，错误则显示Er5
			if( uErrFlag.bits.Er5 )
			{
				Disp_ErN(5);
				I2C_masterInit();			
				I2C_Byte_W(I2C_Add_IdentifyCode, 0xFF);		//识别码清除保证可以复位擦除
				Delay1ms(5);
				I2C_Disable();
				eCaliTask = Cali_Err;
			}

			//判断校准值相对误差是否＜0.2℃
			if ( S_Adjust() )
			{
				Disp_Err();
				eCaliTask = Cali_Err;
			}

			//判定是否均为负值
			if( ( g_Cali41Data > 0x7FFF ) && ( g_Cali37Data > 0x7FFF ) )
			{
				Clr_Disp();
				Disp_ErN(7);
				eCaliTask = Cali_Err;
			}
			break;

		//校准出错等待关机
		case Cali_Err:
			Delay10ms(100);
			while( Port_On )
			{
				//喂狗
				WDTR = 0x5A;
			}
			eCaliTask = Cali_Ntc;			//恢复当前任务以便下回从第一个任务开始
			eMain_Task = Task_Sleepmode;
			break;

		//还原当前模式任务状态及各变量，并回到生产模式
		case Cali_End:
			F_CaliMode_KeyPress = 0;
			eCaliTask = Cali_Ntc;			//恢复当前任务以便下回从第一个任务开始
			eMain_Task = Task_ReadyMode;	//回到ready主任务模式
			eReadyTask_Sta = Ready_Init;
			Adc_Channel_Init(TPTONTC);//ADC初始化和通道切换
			uSetFlag.bits.Unit = Unit_C;	//进入检验态默认C
			eTestmode_num = Insptectmode;	//跳转到生产模式
			Clr_All_Memory();  
			break;

		default:
			break;
	}
}

/**************************************************************************
函数名称：	S_Adc_Stable()
函数功能：	调试态AD判稳子程序
输入参数：	无
输出参数：
返回值  ：	0：代表判稳结束，1：代表判稳未结束
占用空间：	TBD
备    注：
①、8组ADC递推平均；
②、按键按下启动判断，否则只是求递推平均，不做判断；
③、72次采样过程中，如果满足连续36次差值＜4，则认为稳定，只要中断1次超出4，都会重新计算36次
**************************************************************************/
uint8 S_Adc_Stable(void)
{
	uint8 i;
	uint16 a;

	static int16 R_Lastbuf;			//上次8组平均值
	static int16 R_Array[8];		//8组Adc数组
	static int16 *p;				//指针
	static uint8 R_StableNum = 0;	//稳定次数计数器
	static uint8 R_TotalNum = 0;	//总稳定次数计数器
	static uint8 F_Enter = 0;

	//首次需采集满8组
	if( !F_Enter )
	{
		uStaFlag.bits.Er8 = 0;
		p = R_Array;
		i = 8;
		while( i )
		{
			if( Get_Adc_SingleRead() )		//模数转换是否完成 FDRDY 1：转换完成 0：正在转换
			{
				*p++ = g_AdcData;
				i --;
			}
		}
		p = R_Array;
		g_AdcSum = 0;			//清0以便累加求均值
		F_Enter = 1;
	}

	//递推存储Adc
	if( Get_Adc_SingleRead() )
	{
		g_AdcSum = 0;			//清0以便累加求均值
		*p++ = g_AdcData;

		//此处由于R_Array为long型，必须是0x08，而不是0x10
		//如果地址溢出则初始化
		if( p == (R_Array + 0x08) )
		{
			p = R_Array;
		}

		//求8组平均
		for( i = 0; i < 8; i++)
		{
			g_AdcSum += R_Array[i];
		}
		g_AdcSum = g_AdcSum >> 3;

		//要留意主循环问题
		if( Port_On == 0 )
		{
			F_CaliMode_KeyPress = 1;
		}

		//如果按下才开始判定是否稳定
		if( F_CaliMode_KeyPress )
		{
			a = g_AdcSum - R_Lastbuf;
			a = labs(a);
			R_Lastbuf = g_AdcSum;

			R_TotalNum ++;					//采样次数累加

			if( a < 4 )
			{
				R_StableNum ++;
			}
			else
			{
				R_StableNum = 0;
			}

			//如果前后2次平均值差值判定次数超过72都没有稳定，则返回8代表稳定超时
			if( R_TotalNum > 71 )
			{
			 	//局部静态变量初始化为0，避免下次清0
				F_Enter = 0;
				R_TotalNum = 0;
				R_StableNum = 0;
				F_CaliMode_KeyPress = 0;
				uStaFlag.bits.Er8 = 1;
				return 0;
			}
			else
			{
				//如果前后2次平均值差值<4的次数达到36次，则返回3代表已稳定
				if( R_StableNum > 35 )
				{
			 		//局部静态变量初始化为0，避免下次清0
					F_Enter = 0;
					R_TotalNum = 0;
					R_StableNum = 0;
					F_CaliMode_KeyPress = 0;
					return 0;
				}
				else
				{
					return 1;			//如果没采集结束则返回1给while循环采集
				}
			}
		}
	}
	return 1;
}

/**************************************************************************
函数名称：	S_Adjust()
函数功能：	调试态下判断37和41校准值是否合理子程序
输入参数：	g_Cali25TP、g_Cali37Data、g_Cali41Data
输出参数：	无
返回值  ：	0：代表校正数据正常，1：代表校正数据异常
占用空间：	TBD
备    注：	无
**************************************************************************/
uint8 S_Adjust(void)
{
	uint16 b,c;
	int16 a;

	//37理论值-(校正时NTC对应TP相对25的0000电压差)
	a =  g_Table37C - g_Cali25TP;
	//校正系数=37校正值*1000/[37理论值-(校正时NTC对应TP相对25的0000电压差)]
	b = (int32)g_Cali37Data * 0x03E8 / a;
	//41理论值-(校正时NTC对应TP相对25的0000电压差)
	a = g_Table41C - g_Cali25TP;
	//41理应校正值=(41理论值-(校正时NTC对应TP相对25的0000电压差))*校正系数/1000
	b = (int32)a * b / 0x03E8;
	//41理应测试值-41测试实际值
	a = b - g_Cali41Data;
	a = labs(a);
	c = g_Cali41Data -g_Cali37Data;
	//不同的传感器，取值范围不同
    #if Sensor == STP9CF55
    if ( a > 0x30 || c < 100 )
    #endif
    #if Sensor == B5F55
    if ( a > 0x43 || c < 100 )
    #endif
	{
		return 1;
	}
	else
	{
		return 0;
	}
}