/**************************************************************************
文件名称：	Drv_LCD_ET05.c
说    明：	液晶显示函数集合（驱动层）
作    者:	Liaoy
版    本：	V1.0
芯	  片:	SN8P29XX
修订记录：
**************************************************************************/
#include "Include.h"
int16 High_Precision;
uint8 High_Precision_Flag = 0;
bit FrameFlag = 1;
//只在本文件内使用的宏定义



// #define	P	1			//0000000000000001B
// #define	SE	2			//0000000000000010B
// #define	SG	4			//0000000000000100B
// #define	SF	8			//0000000000001000B
// #define	SB	1024		//0000010000000000B
// #define	SD	256			//0000000100000000B
// #define	SC	512			//0000001000000000B
// #define	SA	2048		//0000100000000000B

//常量定义
//后面应该用AbCdEF取代
uint16 __ROM	DispTable[10] =	 {	SA+SB+SC+SD+SE+SF,			//0		0
									SB+SC,						//1		1
									SA+SB+SD+SE+SG,				//2		2
									SA+SB+SC+SD+SG,				//3		3
									SB+SC+SF+SG,				//4		4
									SA+SC+SD+SF+SG,				//5		5
									SA+SC+SD+SE+SF+SG,			//6		6
									SA+SB+SC,					//7		7
									SA+SB+SC+SD+SE+SF+SG,		//8		8
									SA+SB+SC+SD+SF+SG,			//9		9
								} ;

/**************************************************************************
函数名称：	Lcd_Init()
函数功能：	Lcd初始化
输入参数：	无
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Lcd_Init( void )
{
	P4SEG = 0x00 ;
	P3SEG = 0X00 ;
	P2SEG = 0x1F ;			//设置LCD显示IO

	LCDM1 = 0x03 ;			//1/3Bias,C-Type LCD Mode.
	LCDM2 = 0x04 ;			//VLCD = 3.0V,
	LCDM3 = 0X02 ;			//Disable LCD low power mode./0 = 4-COM
	Delay50us(100) ;
	FLCDEN = 1 ;			//使能LCD
}

/**************************************************************************
函数名称：	Disp_Time()
函数功能：	显示时间子程序
输入参数：	日期时间标志（0为月日，1为时分）、时间制式（0位为24H，1为12H）、月-日、时:分
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	其中月1-12，日1-31，时最大0-23，分0-59
特规品：	一般就是月日调换，形参传递互换left和right即可
**************************************************************************/
/*void Disp_Time( bit F_time, bit F_Format, uint8 left, uint8 right)
{
	uint8 R_LCD1,R_LCD2,R_LCD3,R_LCD4;

	//如果是时间且12小时制则进行转换
	if( F_Format )
	{
		if ( left > 12 )
		{
			left = left - 12;
			lcd_pm_en();		//点亮PM
			lcd_am_clr();
		}
		else if ( left == 12 )
		{
			lcd_pm_en();		//点亮PM
			lcd_am_clr();
		}
		else if ( left == 0 )
		{
			left = 12;
			lcd_am_en();		//点亮AM
			lcd_pm_clr();
		}
		else
		{
			lcd_am_en();		//点亮AM
			lcd_pm_clr();
		}
	}

	//取十位
	R_LCD1 = left / 10;
	//取个位
	R_LCD2 = left % 10;
	//取十位
	R_LCD3 = right / 10;
	//取个位
	R_LCD4 = right % 10;
    // if(eMain_Task == Task_Memorymode)  //2023-02-22
    //     lcd2 &= 0x01;  
    // else
    //     lcd2 = 0x00;
    
	lcd3 = DispTable[ R_LCD1 ] ;
    lcd2 = DispTable[ R_LCD1 ] >> 8;
	lcd5 = DispTable[ R_LCD2 ] ;
    lcd4 = DispTable[ R_LCD2 ] >> 8;
	lcd7 = DispTable[ R_LCD3 ] | (lcd7 & 0x01);
    lcd6 = DispTable[ R_LCD3 ] >> 8;
	lcd9 = DispTable[ R_LCD4 ] | (lcd9 & 0x01);
	lcd8 = DispTable[ R_LCD4 ] >> 8;

	if(F_time)
    {
        lcd_colon_en();	//点亮:
		lcd_timedash_clr();	//点亮-
    }	
	else
	{
		lcd_pm_clr();
		lcd_am_clr();	//清除AM PM
		lcd_colon_clr();	//清除:
		lcd_timedash_en();	//点亮-
	}
}*/

/**************************************************************************
函数名称：	Disp_Year()
函数功能：	显示时间子程序
输入参数：	g_Year
输出参数：	LCD
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
// void Disp_Year(uint16 L_buf)
// {
// 	uint8 R_LCD1,R_LCD2;

// 	L_buf -= 2000;

// 	//取十位
// 	R_LCD1 = L_buf /10;
// 	//取个位
// 	R_LCD2 = L_buf % 10;

// 	lcd3 = DispTable[ 2 ];
//     lcd2 = DispTable[ 2 ]>> 8;
// 	lcd5 = DispTable[ 0 ];
//     lcd4 = DispTable[ 0 ]>> 8;
// 	lcd7 = DispTable[ R_LCD1 ];
//     lcd6 = DispTable[ R_LCD1 ]>> 8;
// 	lcd9 = DispTable[ R_LCD2 ];
// 	lcd8 = DispTable[ R_LCD2 ]>> 8;
// 	//lcd7 = 0;
// }

/**************************************************************************
函数名称：	void Clr_SetTime( uint8 L_Blink )
函数功能：	设置模式当前设置项0.5s消隐
输入参数：	L_Blink（0为左边消隐，1为右边消隐）
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	无
特规品：	修改左右消隐即可
**************************************************************************/
void Clr_SetTime( uint8 L_Blink )
{
	if( L_Blink )
	{
		lcd7 &= 0x01;
		lcd6 = 0x00;
		lcd9 &= 0x01;
		lcd8 = 0x00;
	}
	else
	{
		lcd2 = 0x00;
		lcd3 = 0x00;
		lcd4 = 0x00;
		lcd5 = 0x00;
	}
}

// /**************************************************************************
// 函数名称：	Disp_Ntc()
// 函数功能：	显示环境温度
// 输入参数：	温度
// 输出参数：	LCD
// 返回值  ：	无
// 占用空间：	TBD
// 备    注：	环境温度依然是双精度,但显示单精度,且不进行四舍五入,直接舍弃最低位
// **************************************************************************/
// void Disp_Ntc(uint16 Temp)
// {
// 	uint8 R_LCD1,R_LCD2,R_LCD3,R_LCD4;

// 	HexToBcd(Temp);

// 	//取千位
// 	R_LCD1 = Hex2Bcd[2] & 0x0F;
// 	//取百位
// 	R_LCD2 = Hex2Bcd[1] >> 4;
// 	//取十位
// 	R_LCD3 = Hex2Bcd[1] & 0x0F;
// 	//取个位
// 	R_LCD4 = Hex2Bcd[0] >> 4;

// 	//R_LCD1如果非0，不管R_LCD2是否为0都要显示，如果R_LCD1为0，那么R_LCD2非0才会显示
// 	if( R_LCD2 !=0 || R_LCD1!=0)
// 	{
// 		lcd0 = DispTable[ R_LCD2 ] >> 8;
//     	lcd1 = DispTable[ R_LCD2 ];
// 	}
// 	lcd2 = DispTable[ R_LCD3 ] >> 8;
//     lcd3 = DispTable[ R_LCD3 ];
// 	lcd4 = DispTable[ R_LCD4 ] >> 8;
//     lcd5 = DispTable[ R_LCD4 ];
// 	lcd6 = 0x00;
// 	lcd7 = 0x00;

// 	//最高位置1
// 	if( R_LCD1 )
// 	{
// 		//将第4位置1
// 		lcd0 |= 0x08;
// 	}

// 	//小数点必亮
// 	lcd6 |= lcd_point;
// }

/**************************************************************************
函数名称：	Disp_Temp()
函数功能：	显示测量值
输入参数：	point：0表示不显示小数点、1表示显示小数点
			High：0表示0.1显示模式，1表示0.01高精度模式
			Unit: 华氏＞=200判断标志位，此时High需置0
			temp：温度值（16进制）
输出参数：	LCD中188.88
返回值  ：	无
占用空间：	411words
备    注：	0.01精度可能需要在其他地方显示，视不同机型决定
			F华氏单位当>=200时需移位处理
			适合低精度显示、高精度显示、无小数点显示
**************************************************************************/
void Disp_Temp(bit Point, bit High, bit Unit, int16 Temp)
{
	uint8 R_LCD1,R_LCD2,R_LCD3,R_LCD4,R_LCD5;
	bit F_NegFlag = 0;

	//如果为负数转出正数，同时-20.00禁止显示高精度
	if ( Temp < 0 )
	{
		F_NegFlag = 1;
		Temp = ~Temp + 1;
		if (Temp == 2000)
		{
			Temp = Temp / 10;
			Point = 0;		//强行置零以防用户误输入
		}
	}

	//如果是华氏且大于199.99则禁止高精度显示
	if( Unit && (Temp > 0x4E1F) )
	{
		Temp = Temp / 10;
		Point = 0;		//强行置零以防用户误输入
	}

	//强制转换成无符号数
	Temp = (uint16)Temp;

	HexToBcd(Temp);

	//取百位
	R_LCD1 = Hex2Bcd[2] & 0x0F;
	//取十位
	R_LCD2 = Hex2Bcd[1] >> 4;
	//取个位
	R_LCD3 = Hex2Bcd[1] & 0x0F;
	//取0.1位
	R_LCD4 = Hex2Bcd[0] >> 4;
	//取0.01位
	R_LCD5 = Hex2Bcd[0] & 0x0F;

	//R_LCD1如果非0，不管R_LCD2是否为0都要显示，如果R_LCD1为0，那么R_LCD2非0才会显示
	if( R_LCD2 !=0 || R_LCD1!=0)
	{
		lcd9 = DispTable[ R_LCD2 ];
    	lcd8 = DispTable[ R_LCD2 ] >> 8;
	}
	else
	{
		lcd8 = 0x00;
		lcd9 = 0x00;
	}

	//对于温度来说8.8一定会显示的（当然要考虑校准态AD显示另算）
    //lcd4 &= 0x01;
	lcd7 = DispTable[ R_LCD3 ] | (lcd7 & 0x01);
    lcd6 = DispTable[ R_LCD3 ] >> 8;
	lcd5 = DispTable[ R_LCD4 ];
    lcd4 = DispTable[ R_LCD4 ] >> 8;

	//最高位置1
	if( R_LCD1 )
	{
		lcd9 |= 0x01;
	}

	//小数点点亮
	if( Point )
	{
		lcd_point_en();
	}

	//高精度点亮小8
	if( High )
	{
		Delay10ms(50);
		lcd9 = 0x00;
		lcd8 = 0x00;
		lcd7 = 0x00;
		lcd6 = 0x00;
		lcd5 = DispTable[ R_LCD5 ];
        lcd4 = DispTable[ R_LCD5 ] >> 8;
	}
	
	if(eMain_Task == Task_BondTestmode )
	{
		//负数显示﹣
		if( F_NegFlag )
		{
			lcd9 |= lcd_dash;
		}
		else
		{
			lcd9 &= ~lcd_dash;
		}
	}

}

/**************************************************************************
函数名称:	Disp_High_Precision()
函数功能:	显示高精度数值
输入参数:	
输出参数:	
返回值  :	无
占用空间:	
备    注:	LED屏幕限制最大只能显示199.9，使用双帧切换进行高精度显示 ----王壮21/8/16
**************************************************************************/
void Disp_High_Precision(uint8 Unit, int16 Temp)
{
    if(High_Precision_Flag)
    {
        uint16 R_LCD1,R_LCD2,R_LCD3,R_LCD4,R_LCD5;
        

        //强制转换成无符号数
        Temp = (uint16)Temp;

        HexToBcd(Temp);

        //取百位
        R_LCD1 = Hex2Bcd[2] & 0x0F;
        //取十位
        R_LCD2 = Hex2Bcd[1] >> 4;
        //取个位
        R_LCD3 = Hex2Bcd[1] & 0x0F;
        //取0.1位
        R_LCD4 = Hex2Bcd[0] >> 4;
        //取0.01位
        R_LCD5 = Hex2Bcd[0] & 0x0F;

        
        if(FrameFlag)
        {
			//R_LCD1如果非0，不管R_LCD2是否为0都要显示，如果R_LCD1为0，那么R_LCD2非0才会显示
			if( R_LCD2 !=0 || R_LCD1!=0)
			{
				lcd9 = DispTable[ R_LCD2 ];
				lcd8 = DispTable[ R_LCD2 ] >> 8;
			}
			else
			{
				lcd8 = 0x00;
				lcd9 = 0x00;
			}

			//对于温度来说8.8一定会显示的（当然要考虑校准态AD显示另算）
			//lcd4 &= 0x01;
			lcd7 = DispTable[ R_LCD3 ] | (lcd7 & 0x01);
			lcd6 = DispTable[ R_LCD3 ] >> 8;
			lcd5 = DispTable[ R_LCD4 ];
			lcd4 = DispTable[ R_LCD4 ] >> 8;

			//最高位置1
			if( R_LCD1 )
			{
				lcd9 |= 0x01;
			}

			 //小数点点亮
        	lcd_point_en();
        }
        else
        {
			lcd9 = 0x00;
			lcd8 = 0x00;
			lcd7 = 0x00;
			lcd6 = 0x00;
			lcd5 = DispTable[ R_LCD5 ];
			lcd4 = DispTable[ R_LCD5 ] >> 8;
			//小数点熄灭
        	lcd_point_clr();
        }
        FrameFlag = ~FrameFlag;
        Disp_VoiceSign();
       
    }
    

    /*if(F_NegFlag)
    {
        Table[1] = 0xFE;
    }*/
}

/**************************************************************************
函数名称：	Disp_Code()
函数功能：	显示程序编码
输入参数：	程序编码（如142，则输入142）
输出参数：	LCD
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Disp_Code(uint16 num)
{
	uint8 R_LCD1,R_LCD2,R_LCD3;

	//取百位
	R_LCD1 = num / 100;
	//取十位
	num = num % 100;
	R_LCD2 = num / 10;
	//取个位
	R_LCD3 = num % 10;

	//对于程序编码888一定会显示的
	lcd8 = DispTable[ R_LCD1 ] >> 8;
    lcd9 = DispTable[ R_LCD1 ];
	lcd6 = DispTable[ R_LCD2 ] >> 8;
    lcd7 = DispTable[ R_LCD2 ];
	lcd4 = DispTable[ R_LCD3 ] >> 8;
    lcd5 = DispTable[ R_LCD3 ];
}

/**************************************************************************
函数名称：	Disp_Version()
函数功能：	显示程序版本
输入参数：	格式为U1.0，U和.为固定显示，10为输入的2位数字
输出参数：	LCD
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Disp_Version(uint16 num)
{
	uint8 R_LCD1,R_LCD2;

	//取十位
	R_LCD1 = num / 10;
	//取个位
	R_LCD2 = num % 10;

	lcd8 = S_U >> 8;
    lcd9 = S_U;
	lcd6 = DispTable[ R_LCD1 ] >> 8;
    lcd7 = DispTable[ R_LCD1 ];
	lcd4 = DispTable[ R_LCD2 ] >> 8;
    lcd5 = DispTable[ R_LCD2 ];
}

/**************************************************************************
函数名称：	Clr_Disp()
函数功能：	完全清屏
输入参数：	无
输出参数：	LCD
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Clr_Disp(void)
{
	lcd0 = 0x00;
    lcd1 = 0x00;
    lcd2 = 0x00;
    lcd3 = 0x00;
    lcd4 = 0x00;
    lcd5 = 0x00;
    lcd6 = 0x00;
    lcd7 = 0x00;
    lcd8 = 0x00;
    lcd9 = 0x00;
	lcd10 = 0x00;
}

/**************************************************************************
函数名称：	Clr_Disp888()
函数功能：	清除温度的3个888
输入参数：	无
输出参数：	LCD
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Clr_Disp888(void)
{
	lcd4 = 0x00;
    lcd5 = 0x00;
    lcd6 = 0x00;
    lcd7 = (lcd7 & 0x01) | 0x00;;
    lcd8 = 0x00;
    lcd9 = 0x00;
}

/**************************************************************************
函数名称：	Disp_All()
函数功能：	全显
输入参数：	无
输出参数：	LCD
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Disp_All(void)
{
    lcd0 = 0xff;
    lcd1 = 0xff;
    lcd2 = 0xff;
    lcd3 = 0xff;
    lcd4 = 0xff;
    lcd5 = 0xff;
    lcd6 = 0xff;
    lcd7 = 0xff;
    lcd8 = 0xff;
    lcd9 = 0xff;
    lcd10 = 0xff;
	if( uSetFlag.bits.Unit_Change )
	{
		Disp_Unit();
	}
	#if !Func_Probecover
		lcd_pc_clr();
	#endif

	//lcd_clean_clr();
	
	#if !Func_Obj
		lcd_obj_clr();
	#endif
	
	#if Second_LVD != 1
		lcd_bat_full_clr();
	#endif
    
	#if !Func_Ble
		lcd_ble_clr();
	#endif
	//lcd_sound_off_clr();
}

/**************************************************************************
函数名称：	Disp_Unit()
函数功能：	根据单位设置显示对应单位
输入参数：	无
输出参数：	LCD
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Disp_Unit(void)
{
	lcd_unit_cf_en();
    lcd_unit_f_clr();
	lcd_unit_c_en();
	if( uSetFlag.bits.Unit )
	{
        lcd_unit_c_clr();
		lcd_unit_f_en();
	}
}

/**************************************************************************
函数名称：	Disp_SmileFace()
函数功能：	仅显示笑脸
输入参数：	无
输出参数：	LCD
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Disp_SmileFace(void)
{
	lcd_badface_clr();	//清哭脸
	lcd_smileface_en();	//显笑脸
}

/**************************************************************************
函数名称：	void Disp_BadFace(void)
函数功能：	仅显示哭脸
输入参数：	无
输出参数：	LCD16
返回值  ：	无
占用空间：	TBD
备    注：	生产检验模式、黑体模式使用
**************************************************************************/
void Disp_BadFace(void)
{
	lcd_smileface_clr();	//清笑脸
	lcd_badface_en();		//显哭脸
}

/**************************************************************************
函数名称：	void Disp_ModeSign(void)
函数功能：	根据不同模式显示不同标志
输入参数：	无
输出参数：	LCD16
返回值  ：	无
占用空间：	TBD
备    注：	生产检验模式、黑体模式使用
**************************************************************************/
void Disp_ModeSign(void)
{
	lcd_ear_clr();		//清除耳温标志
	lcd_forehead_clr();	//清除额温标志
	lcd_obj_clr();		//清除物温标志
	lcd_ntc_clr();		//清除环温标志
	switch ( eTestmode_num )
	{
		case Earmode:
			lcd_ear_en();
			break;

		case Foreheadmode:
			lcd_forehead_en();
			break;

		case Objectmode:
		case Insptectmode:
		case Blackbodymode:
			lcd_obj_en();
			break;
		case Airmode:
			lcd_ntc_en();
			break;
	}
}

/**************************************************************************
函数名称：	void Clr_ModeSign(void)
函数功能：	清除额温、耳温、物温标志
输入参数：	无
输出参数：	LCD16
返回值  ：	无
占用空间：	TBD
备    注：	生产检验模式、黑体模式使用
**************************************************************************/
void Clr_ModeSign(void)
{
	lcd_ear_clr();		//清除耳温标志
	lcd_forehead_clr();	//清除额温标志
	lcd_obj_clr();		//清除物温标志
	lcd_ntc_clr();		//清除环温标志
}

/**************************************************************************
函数名称：	Disp_OFF()
函数功能：	显示Off
输入参数：	无
输出参数：	LCD
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Disp_OFF(void)
{
	Clr_Disp();
	lcd8 = S_O >> 8;
    lcd9 = S_O;
    lcd6 = S_F >> 8;
    lcd7 = S_F;
    lcd4 = S_F >> 8;
    lcd5 = S_F;
}

/**************************************************************************
函数名称：	Disp_On()
函数功能：	显示On
输入参数：	无
输出参数：	LCD
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
// void Disp_On(void)
// {
// 	Clr_Disp();
// 	lcd6 = 0x0A;
//     lcd5 = 0x0F;
//     lcd4 = 0x06;
//     lcd3 = 0x02;
// }

/**************************************************************************
函数名称：	Disp_Lo()
函数功能：	显示Lo
输入参数：	无
输出参数：	LCD
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Disp_Lo(void)
{
	lcd8 = 0x00;
    lcd9 = 0x00;
    lcd6 = S_L >> 8;
    lcd7 = S_L;
    lcd4 = S_o >> 8;
    lcd5= S_o;
    lcd_badface_clr();      //清笑脸
    lcd_smileface_clr();    //清哭脸
}

/**************************************************************************
函数名称：	Disp_Hi()
函数功能：	显示Hi
输入参数：	无
输出参数：	LCD
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Disp_Hi(void)
{
	lcd8 = 0x00;
    lcd9 = 0x00;
    lcd6 = S_H >> 8;
    lcd7 = S_H;
    lcd4 = S_i >> 8;
    lcd5 = S_i;
    lcd_badface_clr();      //清笑脸
    lcd_smileface_clr();    //清哭脸
}

/**************************************************************************
函数名称：	Disp_Ready()
函数功能：	显示_ _ . _
输入参数：	无
输出参数：	LCD
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Disp_Ready(void)
{
	lcd4 = 0x01;
    lcd5 = 0x00;
    lcd6 = 0x01;
    lcd7 = 0x00;
    lcd8 = 0x01;
    lcd9 = 0x00;
	lcd_smileface_clr();	//清哭笑脸（从测试态退出）
	lcd_badface_clr();
    lcd_mem_clr();   //清M标志（从记忆态退出）
}

/**************************************************************************
函数名称：	Disp_VoiceSign()
函数功能：	显示声音符号
输入参数：	无
输出参数：	LCD
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Disp_VoiceSign()
{
    if( uSetFlag.bits.VoiceEnable ) 
    {
        
		lcd_voice_clr();		
		lcd_sound_off_clr();
		lcd_voice_en();
    }
    else
    {
		lcd_voice_en();
		lcd_sound_off_en();
    }
}

/**************************************************************************
函数名称：	Disp_Null()
函数功能：	记忆模式显示- - -
输入参数：	无
输出参数：	LCD
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Disp_Null(void)
{
    Clr_Disp888();
	Clr_Age_Select();
    lcd4 = 0x00;
    lcd5 = 0x04;
    lcd6 = 0x00;
    lcd7 = 0x04;
    lcd8 = 0x00;
    lcd9 = 0x04;
    lcd_mem_en();  //显示M标志
}

/**************************************************************************
函数名称：	Disp_ErN()
函数功能：	显示错误代码ErN，n可以为0-9的值
输入参数：	无
输出参数：	LCD
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Disp_ErN(uint8 num)
{
	lcd8 = S_E >> 8;
    lcd9 = S_E;
    lcd6 = S_r >> 8;
    lcd7 = S_r;
	lcd4 = DispTable[ num ] >> 8;
	lcd5 = DispTable[ num ] ;
}

/**************************************************************************
函数名称：	void Disp_ErrMsg(uint8 ErrNum)
函数功能：	显示错误菜单
输入参数：	uErrFlag.g_ErrFlag
输出参数：	LCD
返回值  ：	无
占用空间：	28 words
备    注：	错误存在优先级（Er2>Lo/Hi>Er3/Er4），优先级可以在本程序定义，也可以有根据错误发生的先后决定
**************************************************************************/
void Disp_ErrMsg(void)
{
	lcd_badface_clr();
	lcd_smileface_clr();
	switch (uErrFlag.g_ErrFlag)
	{
		case 1:
			//Disp_ErN(1);
            
            //F_Disp_Dash = Enable;
			break;
		case 2:
			Disp_ErN(2);
			break;
		case 4:
			Disp_ErN(3);
			break;
		case 8:
			Disp_ErN(4);
			break;
		case 0x10:
			Disp_ErN(5);
			break;
		case 0x20:
			Disp_ErN(6);
			break;
		case 0x40:
			Disp_Lo();
			break;
		case 0x80:
			Disp_Hi();
			break;
		default:
			break;
	}
}

// /**************************************************************************
// 函数名称：	Disp_NtcEr2()
// 函数功能：	显示环温错误代码Er2
// 输入参数：	无
// 输出参数：	LCD
// 返回值  ：	无
// 占用空间：	TBD
// 备    注：	无
// **************************************************************************/
// void Disp_NtcEr2(void)
// {
//     lcd0 = 0x07;
//     lcd1 = 0x09;
//     lcd2 = 0x06;
//     lcd3 = 0x00;
//     lcd4 = 0x06;
//     lcd5 = 0x0B;
//     lcd6 = 0x00;
//     lcd7 = 0x00;
//     lcd8 &= 0x80;
//     lcd9 = 0x00;
//     lcd16 &= 0x80;
// }

/**************************************************************************
函数名称：	Disp_CAL()
函数功能：	黑体模式显示CAL
输入参数：	无
输出参数：	LCD
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Disp_CAL(void)
{
	Clr_Disp();
	lcd8 = S_C >> 8;
    lcd9 = S_C;
    lcd6 = S_A >> 8;
    lcd7 = S_A;
    lcd4 = S_L >> 8;
    lcd5 = S_L;
}

/**************************************************************************
函数名称：	Disp_Ab()
函数功能：	调试模式显示Ab
输入参数：	无
输出参数：	LCD
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Disp_Ab(void)
{
	Clr_Disp();
	lcd8 = 0x00;
    lcd9 = 0x00;
    lcd6 = S_A >> 8;
    lcd7 = S_A;
    lcd4 = S_b >> 8;
    lcd5 = S_b;
}

/**************************************************************************
函数名称：	Disp_PAS()
函数功能：	调试模式显示PAS
输入参数：	无
输出参数：	LCD
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Disp_PAS(void)
{
	Clr_Disp();
	lcd8 = S_P >> 8;
    lcd9 = S_P;
    lcd6 = S_A >> 8;
    lcd7 = S_A;
    lcd4 = S_S >> 8;
    lcd5 = S_S;
}

/**************************************************************************
函数名称：	Disp_Err()
函数功能：	调试模式显示Err
输入参数：	无
输出参数：	LCD
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Disp_Err(void)
{
	lcd8 = S_E >> 8;
    lcd9 = S_E;
    lcd6 = S_r >> 8;
    lcd7 = S_r;
    lcd4 = S_r >> 8;
    lcd5 = S_r;
}

/**************************************************************************
函数名称：	Disp_Debug1()
函数功能：	绑定检测模式显示测试画面1
输入参数：	无
输出参数：	LCD
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Disp_Debug1(void)
{
    lcd0 = 0x01;
    lcd1 = 0x0C;
    lcd2 = 0x0A;
    lcd3 = 0x07;
    lcd4 = 0x0A;
    lcd5 = 0x07;
    lcd6 = 0x0A;
    lcd7 = 0x07;
    lcd8 = 0x0A;
    lcd9 = 0x06;
    lcd10 = 0x0A;
}

/**************************************************************************
函数名称：	Disp_Debug2()
函数功能：	绑定检测模式显示测试画面2
输入参数：	无
输出参数：	LCD
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Disp_Debug2(void)
{
	lcd0 = 0x06;
    lcd1 = 0x03;
    lcd2 = 0x04;
    lcd3 = 0x08;
    lcd4 = 0x05;
    lcd5 = 0x08;
    lcd6 = 0x05;
    lcd7 = 0x08;
    lcd8 = 0x05;
    lcd9 = 0x09;
    lcd10 = 0x05;
}

/**************************************************************************
函数名称：	void Disp_DebugPASn(uint8 num)
函数功能：	绑定检测模式显示PASn(n为：0-9)
输入参数：	无
输出参数：	LCD
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
// void Disp_DebugPASn(uint8 num)
// {
// 	lcd15 = 0x0E;
//     lcd14 = 0x0C;
//     lcd13 = 0x0E;
//     lcd12 = 0x0E;
//     lcd11 = 0x0C;
//     lcd10 = 0x0B;
// 	lcd9  = DispTable[ num ] ;
//     lcd8  = DispTable[ num ] >> 8;
// }

/**************************************************************************
函数名称：	Disp_12H()
函数功能：	设置态显示12H
输入参数：	无
输出参数：	LCD
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
/*void Disp_12H(void)
{
	Clr_Disp();
    lcd2 = 0x00;
    lcd3 = 0x0A;
    lcd4 = 0x0D;
    lcd5 = 0x06;
    lcd6 = S_H >> 8;
    lcd7 = S_H;
}*/

/**************************************************************************
函数名称：	Disp_24H()
函数功能：	设置态显示24H
输入参数：	无
输出参数：	LCD
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
/*void Disp_24H(void)
{
	Clr_Disp();
    lcd2 = 0x0D;
    lcd3 = 0x06;
    lcd4 = 0x02;
    lcd5 = 0x0E;
    lcd6 = S_H >> 8;
    lcd7 = S_H;
}*/

/**************************************************************************
函数名称：	Disp_Ch()
函数功能：	设置态显示CH
输入参数：	无
输出参数：	LCD
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Disp_Ch(void)
{
	Clr_Disp();
    lcd6 = S_C >> 8;
    lcd7 = S_C;
    lcd4 = S_H >> 8;
    lcd5 = S_H;
}

/**************************************************************************
函数名称：	Disp_En()
函数功能：	设置态显示EN
输入参数：	无
输出参数：	LCD
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Disp_En(void)
{
  	Clr_Disp();
    lcd6 = S_E >> 8;
    lcd7 = S_E;
    lcd4 = S_n >> 8;
    lcd5 = S_n;
}


// /**************************************************************************
// 函数名称：	Disp_Table1()
// 函数功能：	显示Table1
// 输入参数：	无
// 输出参数：	LCD
// 返回值  ：	无
// 占用空间：	TBD
// 备    注：	无
// **************************************************************************/
#if OffsetTable2open

void Disp_Table1(void)
{
	Clr_Disp();
	lcd8 = 0x00;
    lcd9 = 0x00;
    lcd6 = S_T >> 8;
    lcd7 = S_T;
    lcd4 = 0x00;
    lcd5 = 0x0A;
}

// /**************************************************************************
// 函数名称：	Disp_Table2()
// 函数功能：	显示Table2
// 输入参数：	无
// 输出参数：	LCD
// 返回值  ：	无
// 占用空间：	TBD
// 备    注：	无
// **************************************************************************/
void Disp_Table2(void)
{
	Clr_Disp();
	lcd8 = 0x00;
    lcd9 = 0x00;
    lcd6 = S_T >> 8;
    lcd7 = S_T;
    lcd4 = 0x0D;
    lcd5 = 0x06;
}
#endif


/**************************************************************************
函数名称：	void LVD_Display(void)
函数功能：	低电时显示
输入参数：	void
输出参数：	void
返回值  ：	无
**************************************************************************/
void LVD_Display(void)
{
	if( uStaFlag.bits.midBat == 0 )
	{
		//正常电压
		lcd_bat_full_en();
		lcd_bat_en();
	}
	else if( uStaFlag.bits.LowBat != 1 && uStaFlag.bits.midBat == 1 )
	{
		lcd_bat_full_clr();
		lcd_bat_xor();
	}
}

/**************************************************************************
函数名称:	Disp_Age_Select()
函数功能:	选择不同的年龄段
输入参数:	num年龄选择
输出参数:	LCD
返回值  :	无
占用空间:	TBD
备    注:	无
**************************************************************************/
void Disp_Age_Select(uint8 num)
{
    if(eTestmode_num != Objectmode && eTestmode_num != Insptectmode && eTestmode_num != Airmode)
    {
        //lcd_age_en(); //年龄分段外框
        if(num == Age_0)    //显示0-36月的图标
        {
            lcd_age0_en();
			lcd_age3_clr();
            lcd_age36_clr();
        }
		else if(num == Age_3)   //显示36+月的图标
        {
            lcd_age0_clr();
            lcd_age3_en();
            lcd_age36_clr();
        }
        else if(num == Age_36)   //显示36+月的图标
        {
            lcd_age0_clr();
			lcd_age3_clr();
            lcd_age36_en();
        }
    }
    else
    {
        Clr_Age_Select();
    }

}

/**************************************************************************
函数名称:	Clr_Age_Select()
函数功能:	清除年龄选择
输入参数:	无
输出参数:	LCD
返回值  :	无
占用空间:	TBD
备    注:	无
**************************************************************************/
void Clr_Age_Select(void)
{
    lcd_age0_clr();     //清除3-36月年龄图标
	lcd_age3_clr();
    lcd_age36_clr();    //清除36+月年龄图标
}
