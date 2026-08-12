/**************************************************************************
文件名称：	Drv_I2C_HW.c
说    明：	I2C读写函数集合（驱动层）
作    者:	Liaoy
版    本：	V1.0
芯	  片:	SN8P29XX
修订记录：
**************************************************************************/
#include "Include.h"

//只在本文件内使用的宏定义
#define	ADDRR	0xA1
#define	ADDRW	0xA0
#define	C_WD_NUM	16
#define	C_RD_NUM	16

//变量定义
bit F_ERRI2C=0;
uint8 R_E2Address;		//用于存放E2存取单元的地址寄存器
uint8 R_I2C_RXDATA;
uint8 R_I2C_TXDATA;
// uint8 R_I2C_RXDATA_BUF[16];
// uint8 R_I2C_TXDATA_BUF[16]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};

/**************************************************************************
函数名称：	I2C_masterInit()
函数功能：	I2C主模式读写的数据和时钟信号初始化
输入参数：	无
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void I2C_masterInit(void)
{
	FWCOL=0;		//Clear collision
	FMSPWK=1;		//0 = MCU NOT wake-up by I2C.;1 = MCU wake-up by I2C
	FMSPC=1;		//0 = I2C operated on Slave mode, 7-bit address;1 = I2C operated on Master mode.

//	FACKEN=1;		//0 = Acknowledge sequence idle;1 = Initiate Acknowledge sequence on SDA and SCL pins, and transmit AKDT data bit. Automatically cleared by hardware.
//	FRCEN=1;		//0 = Receive idle;1 = Enables Receive mode for I2C
//	FPEN=1;			//0 = Stop condition idle;1 = Initiate Stop condition on SDA and SCL pins. Automatically cleared by hardware.
//	FRSEN=1;		//0 = Repeated Start condition idle.;1 = Initiate Repeated Start condition on SDA and SCL pins. Automatically cleared by hardware.
//	FSEN=1;			//0 = Start condition idle;1 = Initiate Start condition on SDA and SCL pins. Automatically cleared by hardware.

	//根据所用EEPROM设定
	// MSPADR=0X05;	//SCL clock rate = Fcpu/(MSPADDR)/2=2Mhz/5/2=200K
	MSPADR=0x0A;	//SCL clock rate = Fcpu/(MSPADDR)/2=2Mhz/10/2=100K

	FMSPENB=1;
}

//*******************************************************************************
//	I2C Enable
//*******************************************************************************
// void I2C_Enable(void)
// {
// 	FMSPENB = 1;
// }

//*******************************************************************************
//	I2C disable
//*******************************************************************************
void I2C_Disable(void)
{
	FMSPENB = 0;
}

// /**************************************************************************
// 函数名称：	I2C_Start()
// 函数功能：	读写开始信号函数
// 输入参数：	无
// 输出参数：	无
// 返回值  ：	无
// 占用空间：	TBD
// 备    注：	无
// **************************************************************************/
// // void I2C_Start(void)
// // {
// // 	FSEN=1;
// // }

// /**************************************************************************
// 函数名称：	I2C_RespeatStart()
// 函数功能：	读写开始信号函数
// 输入参数：	无
// 输出参数：	无
// 返回值  ：	无
// 占用空间：	TBD
// 备    注：	无
// **************************************************************************/
// void I2C_RespeatStart(void)
// {
// 	FRSEN=1;
// }

// /**************************************************************************
// 函数名称：	I2C_Stop()
// 函数功能：	读写停止信号函数
// 输入参数：	无
// 输出参数：	无
// 返回值  ：	无
// 占用空间：	TBD
// 备    注：	无
// **************************************************************************/
// void I2C_Stop(void)
// {
// 	FPEN=1;
// }

// /**************************************************************************
// 函数名称：	I2C_ACK()
// 函数功能：	应答信号函数（主机发出）
// 输入参数：	无
// 输出参数：	无
// 返回值  ：	无
// 占用空间：	TBD
// 备    注：	无
// **************************************************************************/
// void I2C_ACK(void)
// {
// 	FACKDT=0;
// 	FACKEN=1;
// }

/**************************************************************************
函数名称：	I2C_WaitACK()
函数功能：	等待应答信号函数（从机发出）
输入参数：	无
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void I2C_Wait_ACK(void)
{
	NOP(2);
	if(FACKSTAT)
	{
		F_ERRI2C=1;
	}
//	while(FACKSTAT);
}

/**************************************************************************
	程序名称：	I2C_NoACK()
	程序功能：	没有应答信号函数（主机发出）
	数据入口：	无
	数据出口：	无
**************************************************************************/
void I2C_No_ACK(void)
{
	FACKDT=1;
	FACKEN=1;
}

/**************************************************************************
函数名称：	I2C_Byte_W(uint8 R_E2Address, uint8 R_I2C_TXDATA)
函数功能：	单字节写函数
输入参数：	R_I2C_TXDATA（写入的数据）, R_E2Address（数据地址）
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void I2C_Byte_W(uint8 R_E2Address, uint8 R_I2C_TXDATA)
{
	FSEN=1;
	while(!FMSPIRQ);
	MSPBUF=ADDRW;
	FMSPIRQ=0;
	while(!FMSPIRQ);
	I2C_Wait_ACK();
	MSPBUF=R_E2Address;
	FMSPIRQ=0;
	while(!FMSPIRQ);
	I2C_Wait_ACK();
	MSPBUF=R_I2C_TXDATA;
	FMSPIRQ=0;
	while(!FMSPIRQ);
	I2C_Wait_ACK();
	FMSPIRQ=0;
	FPEN=1;
	while(!FMSPIRQ);
	FMSPIRQ=0;
}

/**************************************************************************
函数名称：	I2C_Random_R(uint8 R_E2Address)
函数功能：	随机读函数
输入参数：	R_E2Address（数据地址）
输出参数：	无
返回值  ：	R_I2C_RXDATA（返回的数据）
占用空间：	TBD
备    注：	无
**************************************************************************/
uint8 I2C_Random_R(uint8 R_E2Address)
{
	FSEN=1;
	while(!FMSPIRQ);
	MSPBUF=ADDRW;
	FMSPIRQ=0;
	while(!FMSPIRQ);
	I2C_Wait_ACK();
	MSPBUF=R_E2Address;
	FMSPIRQ=0;
	while(!FMSPIRQ);
	I2C_Wait_ACK();
	FMSPIRQ=0;
	FRSEN=1;
	while(!FMSPIRQ);
	MSPBUF=ADDRR;
	FMSPIRQ=0;
	while(!FMSPIRQ);
	I2C_Wait_ACK();
	FMSPIRQ=0;
	FRCEN=1;
	while(!FMSPIRQ);
	R_I2C_RXDATA=MSPBUF;
	FMSPIRQ=0;
	I2C_No_ACK();
	while(!FMSPIRQ);
	FMSPIRQ=0;
	FPEN=1;
	while(!FMSPIRQ);
	FMSPIRQ=0;

	return 	R_I2C_RXDATA;
}

/**************************************************************************
函数名称：	I2C_slaveInit()
函数功能：	I2C从模式读写的数据和时钟信号初始化
输入参数：	无
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
// void I2C_slaveInit(void)
// {
// 	FCKE=0;			//0 = Latch Data on SCL Rising Edge. (Default);1= Latch Data on SCL Falling Edge.
// 	FWCOL=0;		//Clear collision
// 	FSLRXCKP=1;		//0 = Disable CKP function.;1 = Enable CKP function.
// 	FCKP=0;			//0 = Hold SCL keeping Low. (Ensure data setup time and Slave device ready.);1 = Release SCL Clock
// 	FMSPWK=1;		//0 = MCU NOT wake-up by I2C.;1 = MCU wake-up by I2C
// 	FMSPC=0;		//0 = I2C operated on Slave mode, 7-bit address;1 = I2C operated on Master mode.
// 	FGCEN=0;		//0 = General call address disabled;1 = Enable interrupt when a general call address (0000h) is received.
// 	MSPADR=ADDRR;

// 	FMSPENB=1;
// }

/**************************************************************************
函数名称：	I2C_CurAddr_R()
函数功能：	当前地址读函数
输入参数：	无
输出参数：	无
返回值  ：	R_I2C_RXDATA（返回的数据）
占用空间：	TBD
备    注：	无
**************************************************************************/
// uint8 I2C_CurAddr_R(void)
// {
// 	FSEN=1;
// 	while(!FMSPIRQ);
// 	MSPBUF=ADDRR;
// 	FMSPIRQ=0;
// 	while(!FMSPIRQ);
// 	I2C_Wait_ACK();
// 	FMSPIRQ=0;
// 	FRCEN=1;
// 	while(!FMSPIRQ);
// 	R_I2C_RXDATA=MSPBUF;
// 	FMSPIRQ=0;
// 	I2C_No_ACK();
// 	while(!FMSPIRQ);
// 	FMSPIRQ=0;
// 	FPEN=1;
// 	while(!FMSPIRQ);
// 	FMSPIRQ=0;

// 	return 	R_I2C_RXDATA;
// }

/**************************************************************************
函数名称：	I2C_Sequen_R(uint8 Num)
函数功能：	整体读函数
输入参数：	Num（整体读的数据长度）
输出参数：	无
返回值  ：	R_I2C_RXDATA_BUF[]（读出数据存放数组）
占用空间：	TBD
备    注：	无
**************************************************************************/
// void I2C_Sequen_R(uint8 Num)
// {
// 	uint8 i;
// 	FSEN=1;
// 	while(!FMSPIRQ);
// 	MSPBUF=ADDRR;
// 	FMSPIRQ=0;
// 	while(!FMSPIRQ);
// 	I2C_Wait_ACK();
// 	FMSPIRQ=0;
// 	FRCEN=1;
// 	for (i = 0; i < Num-1; i++)
// 	{
// 		while(!FMSPIRQ);
// 		R_I2C_RXDATA_BUF[i]=MSPBUF;
// 		FMSPIRQ=0;
// 		I2C_ACK();
// 		while(!FMSPIRQ);
// 		FMSPIRQ=0;
// 		FRCEN=1;
// 	}
// 	while(!FMSPIRQ);
// 	R_I2C_RXDATA_BUF[i]=MSPBUF;
// 	FMSPIRQ=0;
// 	I2C_No_ACK();
// 	while(!FMSPIRQ);
// 	FMSPIRQ=0;
// 	FPEN=1;
// 	while(!FMSPIRQ);
// 	FMSPIRQ=0;
// }

/**************************************************************************
函数名称：	I2C_Page_W(uint8 R_E2Address, uint8 Num, uint8 *pd)
函数功能：	页写函数
输入参数：	R_E2Address（数据地址）,Num（写入的数据长度，不超过16）,*pd（数据数组指针）
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
// void I2C_Page_W(uint8 R_E2Address, uint8 Num, uint8 *pd)
// {
// 	uint8 i;
// 	FSEN=1;
// 	while(!FMSPIRQ);
// 	MSPBUF=ADDRW;
// 	FMSPIRQ=0;
// 	while(!FMSPIRQ);
// 	I2C_Wait_ACK();
// 	MSPBUF=R_E2Address;
// 	FMSPIRQ=0;
// 	for (i = 0; i < Num; i++)
// 	{
// 		while(!FMSPIRQ);
// 		I2C_Wait_ACK();
// 		MSPBUF=*pd++;
// 		FMSPIRQ=0;
// 	}
// 	while(!FMSPIRQ);
// 	I2C_Wait_ACK();
// 	FMSPIRQ=0;
// 	FPEN=1;
// 	while(!FMSPIRQ);
// 	FMSPIRQ=0;
// }

//*******************************************************************************
//	I2C use Example Routine
//*******************************************************************************
/*
void I2C_example(void)
{
	FMSPENB=0;
	I2C_masterInit();
	FMSPENB=1;
	I2C_Page_W();
	Delay1ms(5);

	FMSPENB=0;
	I2C_masterInit();
	FMSPENB=1;
	I2C_Byte_W();
	Delay1ms(5);

	FMSPENB=0;
	I2C_masterInit();
	FMSPENB=1;
	I2C_Random_R();
	Delay1ms(5);

	FMSPENB=0;
	I2C_masterInit();
	FMSPENB=1;
	I2C_CurAddr_R();
	Delay1ms(5);

	FMSPENB=0;
	I2C_masterInit();
	FMSPENB=1;
	I2C_Sequen_R();
	Delay1ms(5);
}
*/
/*************************************************************************/