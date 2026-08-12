/**************************************************************************
文件名称：	Drv_ADC.c
说    明：	ADC相关设置集合（驱动层）
作    者:	Liaoy
版    本：	V1.0
芯	  片:	SN8P29XX
修订记录：
**************************************************************************/
#include "Include.h"

//只在本文件内使用的宏定义
#define FilterNum 0x04		//ADC滤波个数

//变量定义
int16 g_AdcData;		//ADC输出缓冲区高字节
uint8 g_AdcCount;		//ADC计数单元
int32 g_AdcSum;			//ADC COUNT之和的高字节

/**************************************************************************
函数名称：	Drv_Adc_Init()
函数功能：	ADC初始化设置(只设置，不使能)
输入参数：	无
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Drv_Adc_Init( void )
{
	FADC1EN = 0 ;			//关闭AD
	VREG = 0xED ;			//使能稳压器，使能AVDDR=2.4V,使能ACM=1.0V,AVE=1.5V
	VREG2 = 0x03 ;
	AMPM = 0x40 ;			//7.8KHz，1x，bypass pga，不开启pga chooper
	FAMPEN = 1 ;			//使能增益放大器
	ADCM1 = 0x68 ;			//内部参考电压0.84V，AD增益1x
	ADCM2 = 0x46 ;			//ADC时钟源250khz、4096osr、61hz输出及偏移选择
	Delay50us( 6 ) ;
}

/**************************************************************************
函数名称：	Drv_Adc_Channel_Set()
函数功能：	将AD通道切换成指定通道
输入参数：	eADCh
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Drv_Adc_Channel_Set( eADChannel eADCh )
{
	FADC1EN = 0 ;
	CHS = eADCh ;
	g_AdcCount = 0 ;		//请AD计数器
	FADC1EN = 1 ;			//使能AD
}

/**************************************************************************
函数名称：	Drv_PGA_Init(Adc_PGA_Gain)
函数功能：	PGA+ADC初始化，只设置，不使能
输入参数：	无
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Drv_PGA_Init( eAdcPGAGainSel PGA_Gain )
{
	FADC1EN = 0 ;			//关闭AD
	VREG = 0xED ;			//使能稳压器，使能AVDDR=2.4V,使能ACM=1.0V,AVE=1.5V
	VREG2 = 0x03 ;
	AMPM = 0x50;			//set pga chooper frequency is 7.8KH
	AMPM |= PGA_Gain;		//设置放大倍数，开启pga chooper
	FAMPEN = 1 ;			//使能增益放大器
	ADCM1 = 0x68 ;			//内部参考电压0.84V，AD增益1x→不行修改成0.6v，偏压，放大倍数不考虑增大，最多改成0.6v进行偏压
	ADCM2 = 0x46 ;			//ADC时钟源250khz、4096osr、61hz输出及偏移选择
	Delay50us( 6 ) ;
}

/**************************************************************************
函数名称：	Drv_Adc_Offset_Set()
函数功能：	ADC输入偏移电压设置
输入参数：	eOffSel
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void Drv_Adc_Offset_Set( eAdcOffsetSel eOffSel )
{
	ADCM2 |= eOffSel;		//设置输入电压偏移量
	Delay50us( 6 ) ;
}

/**************************************************************************
函数名称：	Get_Adc_SingleRead()
函数功能：	单次读Adc
输入参数：	无
输出参数：	g_AdcData
返回值  ：	1：转换完成，0：转换未完成
占用空间：	TBD
备    注：	无
**************************************************************************/
int8 Get_Adc_SingleRead( void )
{
	if( FDRDY == 1 )	//模数转换是否完成 FDRDY 1：转换完成 0：正在转换
	{
		g_AdcData = ADCDH;
		g_AdcData = (g_AdcData << 8) | ADCDM;	//读采样数据
		FDRDY = 0 ;

		#if Func_debug
			if( CHS == AI3_AI4 )
				Uart_Transmit(0xAA, g_AdcData);
			else if( CHS == ACM_ACM )
				Uart_Transmit(0xEE, g_AdcData);
			else
				Uart_Transmit(0xBB, g_AdcData);
		#endif

		return 1 ;
	}
	else
		return 0;
}

/**************************************************************************
函数名称：	void Adc_Channel_Init(bit cmd)
函数功能：	ADC初始化和通道切换
输入参数：	ChannelSwitch   通道选择：NTCTOTP切换为TP通道 或者 TPTONTC切换为NTC通道
输出参数：	无
返回值  ：	无
占用空间：	TBD
**************************************************************************/
void Adc_Channel_Init(bit ChannelSwitch)
{
    if(ChannelSwitch == NTCTOTP)
    {
        Drv_PGA_Init(Adc_PGA_Gain);
        Drv_Adc_Channel_Set(AI3_AI4);	//TP通道
    }
    else
    {
        Drv_Adc_Init();
        Drv_Adc_Channel_Set(AI2_ACM);	//NTC通道
    }
}

/**************************************************************************
函数名称：	Get_Ntc_Count()
函数功能：	获取ntc温度
输入参数：	无
输出参数：	g_AIN1Count、g_AIN2Count
返回值  ：	1：完成一次完整采集，0：未完成一次完整采集
占用空间：	TBD
备    注：	本写法非死循环写法
**************************************************************************/
uint8 Get_Ntc_Count(void)
{

	//当完成当前通道采样则切换到下一个通道
	if( Get_Adc_Avg() )
	{
		if( CHS ==  AI2_ACM)
		{
			g_AIN2Count = g_AdcSum;
			Drv_Adc_Channel_Set(AI1_ACM); 
		}
        else    //这里不能时if判断，否则会直接返回1，AI1_ACM通道采集失败
        {
            g_AIN1Count = g_AdcSum;
			Drv_Adc_Channel_Set(AI2_ACM);
			return 1;	//完整的一轮采集
        }
	}
	return 0;
}

/**************************************************************************
函数名称：	Get_Adc_Avg()
函数功能：	丢弃前R_FilterNum笔数据，采集10笔，去除最大最小，求8笔平均值
输入参数：	g_AdcSum、g_AdcData
输出参数：	g_AdcSum
返回值  ：	0：未完成采样，1：完成采样
占用空间：	TBD
备    注：	无
**************************************************************************/
uint8 Get_Adc_Avg(void)
{
	static int16 R_MaxData;		//最大值
	static int16 R_MinData;		//最小值

	if( Get_Adc_SingleRead() )
	{
		g_AdcCount ++;

		//丢弃前FilterNum笔数据
		if( g_AdcCount > FilterNum )
		{
			//当数据为第一组时最大最小赋初值
			if( g_AdcCount == FilterNum + 1 )
			{
				g_AdcSum = 0;
				R_MinData = g_AdcData;
				R_MaxData = g_AdcData;
			}

			//求和
			g_AdcSum += g_AdcData;

			//找最大值
			if(R_MaxData < g_AdcData)
			{
				R_MaxData = g_AdcData;
			}
			//找最小值
			if(R_MinData > g_AdcData)
			{
				R_MinData = g_AdcData;
			}
		}
	}

	//取8笔平均值
	if(	g_AdcCount > (FilterNum + 9) )
	{
		g_AdcSum = g_AdcSum - R_MaxData - R_MinData;
		g_AdcSum = g_AdcSum >> 3;
		g_AdcCount = 0;
		return 1;		//表示整个采样完成
	}
	else
	{
		return 0;
	}
}

//实时采集NTC
void ReadyMode_NtcMeas(void)
{
#if ET_FT != 1
    if ((eTestmode_num == Earmode) && (eReadyTask_Sta == Ready_ReadyOk)  && !uErrFlag.bits.Er2)
    {
        if( Get_Adc_SingleRead() )
        {
            g_AdcCount ++;
            Get_Ear_Tp_Max(g_AdcData);
        }
    }
    else
#endif
    {
        if( Get_Ntc_Count() )
        {
            Ntc_Caculate();
            NtcTable_Check();
            if( eTestmode_num == Insptectmode )
            {
                NtcTableWider_Check();
            }
            NtcTable_Find();
        }

    }
    
}
