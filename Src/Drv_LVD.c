/**************************************************************************
文件名称：	Drv_LVD.c
说    明：	低电压处理函数集合（驱动层）
作    者:	Liaoy
版    本：	V1.0
芯	  片:	SN8P29XX
修订记录：
**************************************************************************/
#include "Include.h"
#if !Second_LVD
/**************************************************************************
函数名称：	void LVD_Init(void)
函数功能：	low voltage初始化设置
输入参数：	无
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void LVD_Init(void)
{
		//internal lvd
		LBTM = 0x10 ;		//低电压<2.6V，内部电压检测
		Delay50us(1) ;
		FLBTEN = 1 ;		//enable low-voltage detect
}

/**************************************************************************
函数名称：	void LVD_Chk(void)
函数功能：	low voltage检测
输入参数：	无
输出参数：	uStaFlag.bits.LowBat
返回值  ：	无
占用空间：	TBD
备    注：	100ms（Fcup=2M）
**************************************************************************/
void LVD_Chk(void)
{
	uint8 i ;		//count with low-voltage

	for( i=0; i<10; i++ )	//若低电压连续有十次，则置低电压标志F_Lo为1，清低电压计数
	{
		Delay10ms(1) ;		//延迟10ms稳定
		if( FLBTO )			//FLBTO 1:低电压 0：正常电压
		{
			uStaFlag.bits.LowBat = 1 ;
		}
		else
		{
			uStaFlag.bits.LowBat = 0 ;
			break;
		}
	}
}
#else
/**************************************************************************
函数名称：	void LBD_Chk(void)
函数功能：	low voltage检测
输入参数：	无
输出参数：	uStaFlag.bits.LowBat\uStaFlag.bits.midBat
返回值  ：	无
占用空间：	TBD
备    注：	100ms（Fcup=2M）
**************************************************************************/
void LBD_Chk( void )
{
	uint8 i ;		//count with low-voltage

	LBTM = 0x14 ;		//低电压<2.7V，内部电压检测
	Delay50us(1) ;
	FLBTEN = 1 ;		//enable low-voltage detect

	for( i=0; i<10; i++ )	//若低电压连续有十次，则置低电压标志F_Lo为1，清低电压计数
	{
		Delay10ms(1) ;		//延迟10ms稳定
		if( FLBTO )			//FLBTO 1:低电压 0：正常电压
		{
			uStaFlag.bits.midBat = 1 ;
		}
		else
		{
			uStaFlag.bits.LowBat = 0 ;
			uStaFlag.bits.midBat = 0 ;
			return;
		}
	}

	LBTM = 0x10 ;		//低电压<2.6V，内部电压检测
	Delay50us(1) ;
	FLBTEN = 1 ;		//enable low-voltage detect

	for( i=0; i<10; i++ )	//若低电压连续有十次，则置低电压标志F_Lo为1，清低电压计数
	{
		Delay10ms(1) ;		//延迟10ms稳定
		if( FLBTO )			//FLBTO 1:低电压 0：正常电压
		{
			uStaFlag.bits.LowBat = 1 ;
		}
		else
		{
			uStaFlag.bits.LowBat = 0 ;
			return;
		}
	}

	if( uStaFlag.bits.LowBat == 1  )	
	{
		uStaFlag.bits.midBat = 0 ;
	}
	return;
}
#endif
/*************************************************************************/
