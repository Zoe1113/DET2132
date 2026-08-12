/**************************************************************************
文件名称：	Variable.c
作    者:	Liaoy
版    本：	V1.0
说    明：	变量定义
芯	  片:	SN8P29XX
修订记录：
**************************************************************************/
#include "Include.h"

//定义全局变量在Bank0因无需切换Bank有助于节省程序空间
#pragma rambank 0
/**************************开机不可清除RAM区******************************/
#pragma data_address 0x78
volatile uint8 g_Hour;		//用于表示时间-小时寄存器
#pragma data_address 0x79
volatile uint8 g_Minute;	//用于表示时间-分钟寄存器
#pragma data_address 0x7A
volatile uint8 g_Day;		//用于表示时间-天数寄存器
#pragma data_address 0x7B
volatile uint8 g_Month;		//用于表示时间-月份寄存器
#pragma data_address 0x7C
volatile uint16 g_Year;		//用于表示时间-年份寄存器
#pragma data_address 0x7E
volatile uint8 g_Second;	//用于表示时间-秒数寄存器
#pragma data_address 0x7F
uSet uSetFlag;

/****************************校准数据RAM区********************************/
int16 g_Cali25TP;		//校准环温相对25.0℃的TP值
int16 g_Cali37Data;		//37.0℃黑体校准值
int16 g_Cali41Data;		//41.0℃黑体校准值
int16 g_AdcDoff;		//ADC通道DOFF的高字节
int16 g_OpDoff;			//PGA Doff
int16 g_OpOffsetDoff;	//PGA Offset Doff
uint16 g_AIN1Count;		//AIN1→ACM通道ADC COUNT
uint16 g_AIN2Count;		//AIN2→ACM通道ADC COUNT
uint16 g_NtcCount;		//NTC ADC COUNT
int16 g_TpCount;		//TP ADC COUNT
uint16 g_NtcStep;		//NTC查表步长(NTC温度0.01分辨率)
int16 g_TpStep;			//TP查表步长(目标温度0.01分辨率）
int16 g_RelateTemp;	    //用于关联高字节

eTask eMain_Task;
uStatus uStaFlag;
uErr uErrFlag;

bit F_DispNtc_Time_Date_En;	//0：不刷新时间、日期、环温，1：刷新时间、日期、环温
bit F_CaliOk;
uint8 g_TestNum;
uint8 g_AgeSelectNum;	    //年龄选择

#pragma rambank off
/*************************************************************************/
