/**************************************************************************
文件名称：	App_Function.c
说    明：	功能类函数集合（应用层）
作    者:	Liaoy
版    本：	V1.0
芯	  片:	SN8P29XX
修订记录：
**************************************************************************/
#include "Include.h"

//只在本文件内使用的宏定义
//温度分段阈值
//0-3月年龄分段的发烧报警范围
#define SlightTempC0	0x0E9C		//37.4
#define FeverTempC0	    0x0E9C		//37.4
#define SlightTempF0	0x26CA		//99.3
#define FeverTempF0 	0x26CA		//99.3

//3-36月年龄分段的发烧报警范围
#define SlightTempC3	0x0EB0		//37.6
#define FeverTempC3	    0x0F0A		//38.5
#define SlightTempF3	0x26E8		//99.6
#define FeverTempF3	    0x2792		//101.3

//36+月年龄分段的发烧报警范围
#define SlightTempC36	0x0EBA		//37.7
#define FeverTempC36	0x0F64		//39.4
#define SlightTempF36	0x2706		//99.9
#define FeverTempF36	0x283C		//103.0

uint8 Hex2Bcd[3];	//hex转bcd后bcd码存储区

/**************************************************************************
函数名称：	void Tone_Init(void)
函数功能：	发音程序
输入参数：	无
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	提高解耦，减少程序占用，提高对主体程序的影响
**************************************************************************/
void Tone_Init(void)
{
    if(  uSetFlag.bits.VoiceEnable == 1 )
    {
        #if Have_Voice_Func
			AM5BA_SPI_Init();
			AM5BA_Power_Enable();
			AM5BA_Send_Cmd(LL_Stop, SS_Stop, Nodata);
			Delay1ms(5);
			AM5BA_Send_Cmd(LL_Volumn, SS_VolumeMax, Nodata);
			Delay1ms(5);
			AM5BA_Send_Cmd(LL_Broadcast, SS_Di, Nodata);
			Delay1ms(5);
        #else
            BZ_Beep125();
        #endif
    }
    else
    {
        #if Have_Motor
            //if( eTestmode_num != Earmode )
                g_MotorSystick = Vibration_time;
        #endif
    }
        

                        		
}

/**************************************************************************
函数名称：	void Fever_alarm(int16 Temp, uint8 Age_Selcet)
函数功能：	发烧报警判断子程序
输入参数：	Temp:温度值  Age_Selcet:年龄段选择
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	必须在摄氏华氏转化之后，且调整为0.1分辨率后使用
**************************************************************************/
void Fever_alarm(int16 Temp , uint8 Age_Selcet)
{
	int16 SlightTemp, FeverTemp;

	LED_CloseAll();
	uStaFlag.bits.Fever = 0;

	//按单位(华氏/摄氏)和年龄段选择发烧报警阈值
	if (uSetFlag.bits.Unit)	//Unit=1单位是F
	{
		if(Age_Selcet == Age_0)			{ SlightTemp = (int16)SlightTempF0;  FeverTemp = (int16)FeverTempF0; }
		else if(Age_Selcet == Age_3)	{ SlightTemp = (int16)SlightTempF3;  FeverTemp = (int16)FeverTempF3; }
		else							{ SlightTemp = (int16)SlightTempF36; FeverTemp = (int16)FeverTempF36; }
	}
	else
	{
		if(Age_Selcet == Age_0)			{ SlightTemp = (int16)SlightTempC0;  FeverTemp = (int16)FeverTempC0; }
		else if(Age_Selcet == Age_3)	{ SlightTemp = (int16)SlightTempC3;  FeverTemp = (int16)FeverTempC3; }
		else							{ SlightTemp = (int16)SlightTempC36; FeverTemp = (int16)FeverTempC36; }
	}

	//温度<=微热阈值显示绿光+笑脸
	if (Temp <= SlightTemp)
	{
		#if Func_3color
			LED_Green_En();
		#endif
		Disp_SmileFace();
	}
	//温度>发烧阈值显示红光+哭脸
	else if (Temp > FeverTemp)
	{
		#if Func_3color
			LED_Red_En();
		#endif
		Disp_BadFace();
		uStaFlag.bits.Fever = 1;
	}
	//其他显示黄光+笑脸
	else
	{
		#if Func_3color
			LED_Yellow_En();
		#endif
		Disp_SmileFace();
	}
}

/**************************************************************************
函数名称：	uint8 LeapYear_Judge( uint16 L_Buf )
函数功能：	闰年判断子程序
输入参数：	L_Buf（年份）
输出参数：	无
返回值  ：	1：闰年，0：非闰年
占用空间：	TBD
备    注：
;闰年条件:	a、能被4整除,不能被100整除的年份是闰年; b、能被400整除的年份是闰年
;技巧：能100整除，那么十进制的个位和十位一定为0，能被4整除，那么二进制的低2位一定也为0
**************************************************************************/
uint8 LeapYear_Judge( uint16 L_Buf )
{
	HexToBcd(L_Buf);
	if( Hex2Bcd[0] == 0 )
	{
		if( (L_Buf & 0x000F) == 0)
			return 1;
		else
			return 0;
	}
	else
	{
		if( (L_Buf & 0x0003) == 0)
			return 1;
		else
			return 0;
	}
}

/**************************************************************************
函数名称：	int16 CToF(uint16 L_Temp)
函数功能：	摄氏转华氏子程序
输入参数：	g_TpStep
输出参数：	g_TpStep
返回值  ：	无
占用空间：	TBD
备    注：	计算公式:	3700*9/5+3200=9860(双精度)
**************************************************************************/
int16 CToF(int16 L_Temp)
{
	if( uSetFlag.bits.Unit )
	{
		if( L_Temp < 0 )
		{   
            //计算方法：F =  32 - C*2 + C*0.2
			L_Temp = labs(L_Temp);
			L_Temp = L_Temp << 1;
			L_Temp = L_Temp - L_Temp/10;
			L_Temp = 0x0C80 - L_Temp;
		}
		else
		{
            //计算方法：F = C*2 - C*0.2 +32
			L_Temp = L_Temp << 1; //C*2
			L_Temp = L_Temp - (L_Temp + 9)/10; //(L_Temp + 9)/10是C*0.2,加9是为了保证小数点第二位数值正确
			L_Temp += 0x0C80;
		}
	}
	return L_Temp;
}

/**************************************************************************
函数名称：	int16 Temp_Resolution_Adjust(int16 L_Temp)
函数功能：	显示分辨率调整子程序
输入参数：	L_Temp（0.01精度）
输出参数：	L_Temp（0.1精度）
返回值  ：	L_Temp（0.1精度）
占用空间：	TBD
备    注：	无
**************************************************************************/
int16 Temp_Resolution_Adjust(int16 L_Temp)
{
	bit F_NegFlag = 0;

	if( L_Temp < 0)
	{
		L_Temp = ~L_Temp + 1;
		F_NegFlag = 1;
	}

	L_Temp += 5;
	L_Temp = L_Temp/10;
	L_Temp = L_Temp*10;

	if (F_NegFlag)
	{
		L_Temp = ~L_Temp + 1;
	}
	return L_Temp;
}

/**************************************************************************
函数名称：	void Temp_Relate(void)
函数功能：	显示分辨率调整子程序
输入参数：	g_TpStep、g_RelateTemp
输出参数：	g_RelateTemp
返回值  ：	无
占用空间：	TBD
备    注：	前后两次温度差值在0.5℃内,则求前后两次的平均值作为此次的显示值，超出则显示当前值
**************************************************************************/
void Temp_Relate(void)
{
	int16 a;
    a = g_TpStep - g_RelateTemp;
	a = labs(a);
	if ( a < 0x0033 )
    {
     	g_TpStep += g_RelateTemp;
       	g_TpStep = g_TpStep >> 1;
    }
	g_RelateTemp = g_TpStep;
}

/**************************************************************************
函数名称：	Emissivity_correction()
函数功能：	发射率调整
输入参数：	g_TpCount、g_Emission
输出参数：	g_TpCount
返回值  ：	无
占用空间：	TBD
备    注：	TPCount=TPCountH*系数/100
原理:		其实并不是发射率调整,因为发射率应该是进行除法操作,这里的目的是降低TP值以使温度与口腔等效而乘以某个特定的系数
**************************************************************************/
void Emissivity_correction(void)
{
	g_TpCount = (int32)g_TpCount * g_Emission / 100;
}

/**************************************************************************
函数名称：	void Body_MeasureRange_Check(void)
函数功能：	人体测量范围判断子程序，判断是否超出34-43℃
输入参数：	g_TpStep
输出参数：	uErrFlag.bits.Lo，uStaFlag.bits.Hi
返回值  ：	无
占用空间：	TBD
备    注：	本程序修改后必须在Temp_correction子程序后面使用
**************************************************************************/
void Body_MeasureRange_Check(void)
{
	uErrFlag.g_ErrFlag = 0;
	if( g_TpStep < DispRange_HumanDown )
	{
		uErrFlag.bits.Lo = 1;
	}

	if( g_TpStep > DispRange_HumanUp )
	{
		uErrFlag.bits.Hi = 1;
	}
}

/**************************************************************************
函数名称：	void Obj_MeasureRange_Check(void)
函数功能：	物体测量范围判断子程序，判断是否超出0-100℃
输入参数：	g_TpStep
输出参数：	uErrFlag.bits.Lo，uErrFlag.bits.Hi
返回值  ：	无
占用空间：	TBD
备    注：	本程序修改后必须在Temp_correction子程序后面使用
;带有物温模式的产品,本程序是可以只有物温模式进入,黑体态直接使用人体范围(Body_MeasureRange_Check)会有问题,因为范围不一致,故需警惕显示范围测试方法和后果.
;但无物温模式的产品,建议黑体模式只有人体范围,但黑体态直接使用人体范围(Body_MeasureRange_Check)会有问题,因为范围不一致,故需警惕显示范围测试方法和后果.
;需注意法规中显示范围和测量范围概念的区别,所以黑体态下使用人体范围是可以的(即使不一致,范围要广)
**************************************************************************/
void Obj_MeasureRange_Check(void)
{
	uErrFlag.g_ErrFlag = 0;

	if( g_TpStep < (int16)DispRange_ObjDown )
	{
		uErrFlag.bits.Lo = 1;
	}

	if(g_TpStep > DispRange_ObjUp)
	{
		uErrFlag.bits.Hi = 1;
	}
}

/**************************************************************************
函数名称：	void NtcTableWider_Check(void)
函数功能：	热敏电阻温度范围判断子程序性，判断是否超出0-50℃
输入参数：	g_NtcStep
输出参数：	uErrFlag.bits.Er2
返回值  ：	无
占用空间：	TBD
备    注：	本函数绝对不可以增加uErrFlag.g_ErrFlag = 0;的动作，如果超出范围，则NtcTable_Check也一定超出
**************************************************************************/
void NtcTableWider_Check(void)
{
	uErrFlag.bits.Er2 = 0;
	//IDE实测7FFF，考虑adc90%良性区域，故7FFF*0.9=0x7333;
	if( g_AIN2Count > 0x7333 || g_AIN1Count > 0x7333 || g_NtcCount < g_NtcTableUp2 || g_NtcCount > g_NtcTableDown2 )
	{
		uErrFlag.g_ErrFlag = 0;
	    uErrFlag.bits.Er2 = 1;
	}
}

/**************************************************************************
函数名称：	void NtcTable_Check(void)
函数功能：	热敏电阻温度范围判断子程序性，判断是否超出10-40℃
输入参数：	g_NtcStep
输出参数：	uErrFlag.bits.Er2
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void NtcTable_Check(void)
{
	uErrFlag.bits.Er2 = 0;
	//IDE实测7FFF，考虑adc90%良性区域，故7FFF*0.9=0x7333;
	if( g_AIN2Count > 0x7333 || g_AIN1Count > 0x7333 || g_NtcCount < g_NtcTableUp1 || g_NtcCount > g_NtcTableDown1 )
	{
		uErrFlag.g_ErrFlag = 0;
	    uErrFlag.bits.Er2 = 1;
	}
}

/**************************************************************************
函数名称：	void HexToBcd(uint16 Hex_Value)
函数功能：	2字节16进制转10进制程序
输入参数：	Hex_Value（2字节16进制）
输出参数：	LCD
返回值  ：	无
占用空间：	0.4ms（Fcpu=2M）  114Byte
备    注：	2字节转十进制使用该程序不会节省程序空间，建议使用/和%，2字节以上可以使用
**************************************************************************/
void HexToBcd(uint16 Hex_Value)
{
	uint8 k0,k1,loopnum;

	k0 = Hex_Value;
	Hex_Value = Hex_Value >> 8;
	k1 = Hex_Value;
	__asm{
			SelectBank(Hex2Bcd)
    		CLR	_Hex2Bcd+2
		  	CLR	_Hex2Bcd+1
		  	CLR	_Hex2Bcd
			MOV A,#16
			SelectBank(loopnum)
			MOV	CNameToAsmLabel(loopnum),A
		LOOPHEX:
			B0BCLR	FC

			SelectBank(k0)
			RLCM	CNameToAsmLabel(k0)
			SelectBank(k1)
			RLCM	CNameToAsmLabel(k1)

			SelectBank(Hex2Bcd)
			MOV	A,_Hex2Bcd
			ADC	A,_Hex2Bcd
			DAA
		 	MOV	_Hex2Bcd,A
			MOV	A,_Hex2Bcd+1
			ADC A,_Hex2Bcd+1
			DAA
			MOV	_Hex2Bcd+1,A
			MOV	A,_Hex2Bcd+2
			ADC	A,_Hex2Bcd+2
			DAA
			MOV	_Hex2Bcd+2,A
			SelectBank(loopnum)
			DECMS	CNameToAsmLabel(loopnum)
			JMP	LOOPHEX
		 }
}