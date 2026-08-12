/**************************************************************************
	Filename:	ADC.h
	Description:The header file of ADC.c
**************************************************************************/
#ifndef _Drv_ADC_H
#define _Drv_ADC_H

//底层驱动
typedef enum
{
	AI2_ACM = 0x1F,
	AI1_ACM = 0x0F,
	ACM_ACM = 0xFF,
	AI3_AI4 = 0x32
}eADChannel;

typedef enum
{
	Offset_75 = 0x00,
	Offset_50 = 0x02,
	Offset_25 = 0x04,
	Offset_0 = 0x06
}eAdcOffsetSel;

typedef enum
{
	PGA_x1 = 0 << 1,
	PGA_x4 = 1 << 1,
	PGA_x8 = 2 << 1,
	PGA_x16 = 3 << 1,
	PGA_x32 = 4 << 1,
	PGA_x64 = 5 << 1,
	PGA_x128 = 6 << 1,
	PGA_x256 = 7 << 1
}eAdcPGAGainSel;

extern int16 g_AdcData;		//ADC输出缓冲区高字节
extern uint8 g_AdcCount;	//ADC计数单元
extern int32 g_AdcSum;		//ADC COUNT之和的高字节


void Drv_Adc_Init( void ) ;
void Drv_Adc_Channel_Set( eADChannel eADCh ) ;
void Drv_PGA_Init( eAdcPGAGainSel PGA_Gain ) ;
int8 Get_Adc_SingleRead( void ) ;		//单次Read the ADC Data
void Adc_Channel_Init(bit ChannelSwitch);
uint8 Get_Adc_Avg(void);
uint8 Get_Ntc_Count(void);
void Drv_Adc_Offset_Set( eAdcOffsetSel eOffSel );
void ReadyMode_NtcMeas(void);

#endif
/*************************************************************************/