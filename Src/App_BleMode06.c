/**************************************************************************
文件名称：	App_Ble.c
说    明：	蓝牙相关函数集合（应用层）
作    者:	Liaoy
版    本：	V1.0
芯	  片:	SN8P29XX
备    注：	适用公司的ET蓝牙
修订记录：
**************************************************************************/
#include "Include.h"

/*****************************
1、北大蓝牙模块初始化时间为200ms，需等待；
2、转发型指令由App应答，特殊型指令由BLE应答，具体详见模块要求和协议；
3、如果连接成功后断线，不报错，蓝牙标志由静止显示变为闪烁；
4、如果busy30s未空闲则报错，但依然可以重新连接蓝牙；
5、第3条和第4条均要复位蓝牙让其重启一次；
****************************/
eBleModeTask eBle_Sta;

#if Func_Ble



bit F_Ble_En;		//蓝牙使能
bit F_Uart_Receive;		//uart接收成功
bit F_Ble_Blink;		//蓝牙标志闪烁
bit F_Ble_FirstEnter = 0;
bit F_BleTimeReceivedSuccess; //蓝牙时间接收成功

uint8 g_ble_timeout;	//蓝牙超时变量
uint8 g_ble_ack_timeout;	//蓝牙数据传输超时变量
uint8 g_Ble_RxData[DataLenth];		//蓝牙接收数据缓冲区
uint8 g_Ble_RxData_Buf[DataLenth];	//蓝牙接收数据缓冲区备份区
uint8 g_Ble_TxData[250];	//蓝牙发送数据缓冲区（最大30组记忆*8byte + 10 byte = 250byte）
uint8 Ble_Waittostart;      //等待重连的延时

void App_BleMode(void)
{
	//UART RX接收事件处理
	Uart_Receive_Event();

	switch (eBle_Sta)
	{
		//待机模式
		case Ble_Standby:
			if( F_Ble_En )
			{
				Port_Power = 0;			//蓝牙上电
				Port_Ble_En = 1;		//使能蓝牙
				Drv_UartTX_Init();		//使能UART
				Drv_UartRX_Init();
				g_ble_timeout = 0;
				g_ble_ack_timeout = 0;
				F_BleTimeReceivedSuccess = 0;
				F_Ble_Blink = Enable;	//开启蓝牙标志闪烁
				eBle_Sta = Ble_DeviceInfo;
			}
			break;

		//广播前上传设备信息
		case Ble_DeviceInfo:
			//上电复位200ms再发送设备信息，此处不判断busy忙闲
			if( g_ble_ack_timeout > 4 )
			{
				App_UploadData(TxFunc_DeviceInfo);
				Uart_Send_Wakeup();
				UART_Send_To_Ble();
				g_ble_ack_timeout = 0;
			}
			break;

		//广播模式等待连接
		case Ble_Advertising:
			//如果连接成功，则立即更新时间，用户可以一直尝试连接
			if( !Port_Ble_Link )
			{
				//蓝牙符号闪烁停止常显
				F_Ble_Blink = Disable;
				lcd_ble_en();
				g_ble_timeout = 0;
				g_ble_ack_timeout = 0;
				if(eTestmode_num == Blackbodymode)
				{
					g_AutoTurnOff_Count = CountDown_6min;   //此处可能为OTA时候造成的断线，然他保持至少一分钟在线的时间完成OTA
				}
				else
				{
					g_AutoTurnOff_Count = CountDown_30s;
				}
				eBle_Sta = BLE_UpdateTime;
			}
			break;

		//更新时间
		case BLE_UpdateTime:
			//如果等待空闲时时断开连接则报错
			if( Port_Ble_Link )
			{
				eBle_Sta = Ble_LinkErr;
			}
			//空闲立即发送时间
			if( !Port_Ble_Busy )
			{
				//200ms反复发送直到接收到ack为止
				if( g_ble_ack_timeout > 4 )
				{
					App_UploadData(TxFunc_TimeUpdate);
					Uart_Send_Wakeup();
					UART_Send_To_Ble();		//立即发送命令
					g_ble_ack_timeout = 0;
					g_ble_timeout = 0;
				}
			}
			else
			{
				//如果30s到了还未空闲，则关闭蓝牙并报错
				if( g_ble_timeout > 60 )
				{
					eBle_Sta = Ble_TimeoutErr;
				}
			}
			break;

		//连接后空闲等待
		case Ble_Idle:
			//如果等待空闲时时断开连接则报错
			if( Port_Ble_Link )
			{
				eBle_Sta = Ble_LinkErr;
			}
			break;

		//上传温度数据
		case Ble_UploadData:
			//如果等待空闲时时断开连接则报错
			if( Port_Ble_Link )
			{
				eBle_Sta = Ble_LinkErr;
			}
			//蓝牙连接成功，但没有时间同步；不发送本次数据，跳转到时间同步
			if(!Port_Ble_Link && !F_BleTimeReceivedSuccess)
			{
				eBle_Sta = Ble_Advertising;
				break;
			}
			//如连接成功等待空闲，则上传数据，否则等待空闲
			if( !Port_Ble_Busy )
			{
				//200ms反复发送直到接收到ack为止
				if( g_ble_ack_timeout > 6 )
				{
					if(App_UploadData(TxFunc_Temperature))
                    {
                        Uart_Send_Wakeup();
                        UART_Send_To_Ble();		//立即发送命令
                        g_ble_ack_timeout = 0;
                        g_ble_timeout = 0;
                    }
                    else
                    {
                        g_ble_ack_timeout = 7;
                    }
				}
			}
			else
			{
				//如果30s到了还未空闲，则关闭蓝牙并报错
				if( g_ble_timeout > 10 )
				{
					eBle_Sta = Ble_TimeoutErr;
				}
			}
			break;

		//断线错误
		case Ble_LinkErr:
			Port_Ble_En = Disable;	//蓝牙睡眠重启
			F_Ble_En = Enable;		//依然允许开启蓝牙
			eBle_Sta = Ble_Standby;
			break;

		//busy超时错误
		case Ble_TimeoutErr:
			F_Ble_En = Enable;		//依然允许开启蓝牙
			F_Ble_Blink = Enable;	//关闪烁
			Drv_UartTX_Disable();
			Drv_UartRX_Disable();
			Port_Ble_En = Disable;	//蓝牙睡眠重启

			F_Uart_Receive = 0; 	//禁用UART接收
//			if(Port_Ble_Link)
			if( Ble_Waittostart>2 )  //等待5S后开始重连,2020-0614修改，因为EN需要持续5s以上低电平才可正常运行
			{
				eBle_Sta = Ble_Standby;
				Ble_Waittostart=0;
			}
			break;

		//ble除能模式
		case Ble_Disable:
			Port_Ble_En = 0;		//蓝牙睡眠
			F_Ble_En = Disable;
			F_Ble_Blink = Disable;	//关闪烁
			 lcd_ble_clr();			//清蓝牙符号
			Drv_UartTX_Disable();
			Drv_UartRX_Disable();
			F_Uart_Receive = 0; 	//禁用UART接收
			eBle_Sta = Ble_Standby;
			break;

		default:
			break;
	}
}

/**************************************************************************
函数名称：	void Uart_Receive_Event(void)
函数功能：	UART接收事件处理
输入参数：	g_Ble_RxData[]
输出参数：	g_Ble_RxData_Buf[]、UART TX ACK Data
返回值  ：	L_success(0:无)
占用空间：	TBD
备    注：	无
**************************************************************************/
uint8 Uart_Receive_Event(void)
{
	uint8 i;
	uint8 L_ble_func;		//蓝牙功能码
	uint8 L_Success = 0;	//rx接收数据校验和错误
	// static bit F_Checksum_fail = 0;

	//UART RX接收事件处理
	if( F_Uart_Receive && F_Ble_En )
	{
		F_Uart_Receive = 0;

		//备份命令(简单的产品不需要)
		for( i=0; i< DataLenth; i++ )
		{
			g_Ble_RxData_Buf[i] = g_Ble_RxData[i];
		}

		// F_Checksum_fail = 0;
		//识别接收协议中的功能码
		L_ble_func = BleData_Check();
		switch ( L_ble_func )
		{
			//收到设备信息应答则调到广播状态
			case RxAck_DeviceInfo:
				eBle_Sta = Ble_Advertising;
				break;

			case RxFunc_UpdateTime:
				L_Success = 1;
				g_Year = g_Ble_RxData_Buf[Index_data] + 2000;
				g_Month = g_Ble_RxData_Buf[Index_data + 1];
				g_Day = g_Ble_RxData_Buf[Index_data + 2];
				g_Hour = g_Ble_RxData_Buf[Index_data + 3];
				g_Minute = g_Ble_RxData_Buf[Index_data + 4];
				g_Second = 0;
				eBle_Sta = Ble_Idle;
				F_BleTimeReceivedSuccess = 1; //时间同步成功
				break;

			//收到温度应答则跳到空闲等待
			case RxAck_Temperature:
				eBle_Sta = Ble_Idle;
				break;
		
			//校验和错，不管是何种命令，不做任何响应
			case RxFunc_Checksumfail:
				//此处有重发机制，等于有响应
			default:
				break;
		}
	}
	return L_Success;
}

/**************************************************************************
函数名称：	void UART_Send_To_Ble(void)
函数功能：	串口发送
输入参数：	sBLEData[]
输出参数：	UTX1D
返回值  ：	无
占用空间：	TBD
备    注：	发送的数据长度可由lenth自由确定，只要不超过sBLEData的最大限制
**************************************************************************/
void UART_Send_To_Ble(void)
{
	uint8 i;
	uint8 L_lenth;
	uint8 *p = g_Ble_TxData;

	L_lenth = g_Ble_TxData[Index_lenth + 1] ;

	//传输到data结束
	for( i=0; i<L_lenth; i++ )
	{
		while(FUTX1BZ);
		if(FUTX1BZ==0)
		{
			UTX1D = *p++ ;
			NOP(5);
		}
	}
}
/*************************************************************************/

/**************************************************************************
函数名称：	uint8 BleData_Check(void)
函数功能：	UART接受到的数据校验和获取功能码
输入参数：	g_Ble_RxData[]
输出参数：	L_buf（功能码，0默认为校验和错误）
返回值  ：	L_buf（功能码，0默认为校验和错误）
占用空间：	TBD
备    注：	无
**************************************************************************/
uint8 BleData_Check(void)
{
	uint8 checksum = 0;
	uint8 L_buf;
	uint8 i;
	uint8 L_Lenth;

	L_Lenth = g_Ble_RxData_Buf[Index_lenth+1] - 3;
	//计算校验和from Data length(include) to Check sum(not include)
	for( i=Index_lenth; i<L_Lenth ; i++ )
	{
		checksum += g_Ble_RxData_Buf[i];
	}
	//判断校验和是否相同，是则返回1，否则0
	L_buf = 0;
	if( checksum == g_Ble_RxData_Buf[i] )
	{
		L_buf = g_Ble_RxData_Buf[Index_func];
	}
	return L_buf;
}

/**************************************************************************
函数名称：	void App_UploadData(uint8 L_Upload_func)
函数功能：	数据上传任务
输入参数：	L_Upload_func（数据协议中的功能码）
输出参数：	UART TX Ack Data
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
uint8 App_UploadData(uint8 L_Upload_func)
{
	switch( L_Upload_func )
	{
		case TxFunc_Temperature:
            if(App_Upload_Temperature())
            {
                return 1;
            }
            else
            {
                return 0;
            }
            L_Upload_func = TxFunc_Default;
			break;

		case TxFunc_DeviceInfo:
			App_Upload_DeviceInfo();
			L_Upload_func = TxFunc_Default;
            return 0;
			break;

		case TxFunc_TimeUpdate:
			App_Upload_Time();
            return 0;
			break;
				
		case TxFunc_Default : 
            return 0;
		default :
            return 0;
            break;
	}
}

/**************************************************************************
函数名称：	void App_Upload_Time(void)
函数功能：	发送时间同步命令
输入参数：	无
输出参数：	g_Ble_TxData[]
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void App_Upload_Time(void)
{
	uint8 L_Checksum = 0;
	uint8 L_Lenth = 0;

	g_Ble_TxData[L_Lenth++] = Ble_Header1;
	g_Ble_TxData[L_Lenth++] = Ble_Header2;
	g_Ble_TxData[L_Lenth++] = Ble_Header3;
	g_Ble_TxData[L_Lenth++] = Ble_Header4;
	g_Ble_TxData[L_Lenth++] = 0x00;
	g_Ble_TxData[L_Lenth++] = 0x0A;
	L_Checksum += 0x0A;
	g_Ble_TxData[L_Lenth++] = TxFunc_TimeUpdate;
	L_Checksum += TxFunc_TimeUpdate;
	g_Ble_TxData[L_Lenth++] = L_Checksum;
	g_Ble_TxData[L_Lenth++] = Ble_End1;
	g_Ble_TxData[L_Lenth++] = Ble_End2;
}

/**************************************************************************
函数名称：	void App_Upload_DeviceInfo(void)
函数功能：	上传仪器信息
输入参数：	型号、程序编码Soft_Code、程序版本Soft_External_Version
输出参数：	sBLEData[]
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void App_Upload_DeviceInfo(void)
{
	uint8 L_Checksum = 0;
	uint8 L_lenth = Index_data;

	g_Ble_TxData[0] = 0x55;
	g_Ble_TxData[1] = 0xAA;
	g_Ble_TxData[2] = 0xAA;
	g_Ble_TxData[3] = 0x55;
	g_Ble_TxData[Index_func] = TxFunc_DeviceInfo;
	L_Checksum += TxFunc_DeviceInfo;
	//发送产品名
	g_Ble_TxData[L_lenth++] = ASCII_D;
	L_Checksum += ASCII_D;
	g_Ble_TxData[L_lenth++] = ASCII_E;
	L_Checksum += ASCII_E;
	g_Ble_TxData[L_lenth++] = ASCII_T;
	L_Checksum += ASCII_T;
	//发送型号名
	g_Ble_TxData[L_lenth++] = Ble_Name>>8;
	L_Checksum += Ble_Name>>8;
	g_Ble_TxData[L_lenth++] = Ble_Name;
	L_Checksum += Ble_Name;
	g_Ble_TxData[L_lenth++] = ASCII_b;
	L_Checksum += ASCII_b;
	//发送程序编码
	g_Ble_TxData[L_lenth++] = Soft_Code >> 8;
	L_Checksum += Soft_Code >> 8;
	g_Ble_TxData[L_lenth++] = Soft_Code;
	L_Checksum += Soft_Code;
	//发送程序版本
	g_Ble_TxData[L_lenth++] = Soft_External_Version;
	L_Checksum += Soft_External_Version;
	//血压计预留位0x00填补
	g_Ble_TxData[L_lenth++] = 00;
	L_Checksum += 00;
	//发送长度
	L_lenth += 3;		//加checksum和tail3个字节
	g_Ble_TxData[Index_lenth] = L_lenth >> 8;
	L_Checksum += L_lenth >> 8;
	g_Ble_TxData[Index_lenth+1] = L_lenth;
	L_Checksum += L_lenth;
	L_lenth = L_lenth - 3;
	g_Ble_TxData[L_lenth++] = L_Checksum;
	g_Ble_TxData[L_lenth++] = 0x5A;
	g_Ble_TxData[L_lenth++] = 0xA5;
}

/**************************************************************************
函数名称：	void App_Upload_Temperature(void)
函数功能：	读取所有记忆并放入缓冲区(由新到旧)
输入参数：	无
输出参数：	g_Ble_TxData[]
返回值  ：	L_datalenth（sBLEData.Data[]数组长度）
占用空间：	TBD
备    注：	数据格式：测量部位1byte、测量值2byte（含单位标志）、时间（年、月、日、时、分）
**************************************************************************/
uint8 L_MemTotalNo;
uint8 L_MemNo;
uint8 App_Upload_Temperature()
{
	static uint8 index = 1;
	static uint8 L_datalenth;

	static uint8 L_Checksum = 0;
	uint16 L_Temp;
	uint8 L_mode;
    uint8 L_MemAdd;
	uint8 L_HourMem;		//用于表示记忆态下显示测量时间小时寄存器
	uint8 L_MinuteMem;		//用于表示记忆态下显示测量时间分钟寄存器
	uint8 L_DayMem;			//用于表示记忆态下显示测量时间日期寄存器
	uint8 L_MonthMem;		//用于表示记忆态下显示测量时间月份寄存器
	uint16 L_YearMem;		//用于表示记忆态下显示测量时间年份寄存器

	//读取总记录号和记忆号的EEPROM存放地址（注意耳温、额温、物温不是每款都有）
    if(F_Ble_FirstEnter == 0)
    {
        //计算数组的数据段标号
	    L_datalenth = Index_data;
		L_Checksum = 0;
        L_MemAdd = I2C_Add_EarMem;   //记忆混存，直接读取改地址作为开始地址就可       
        I2C_masterInit();
        L_MemTotalNo = I2C_Random_R(L_MemAdd);		//读取总记录号
        L_MemNo = I2C_Random_R(L_MemAdd + I2C_Add_Offset);		//读取记忆号
        I2C_Disable();
        F_Ble_FirstEnter = 1;
		index = 1;
    }

    if ( index <= L_MemTotalNo )      //读取总数
    {
        index++;
        
        L_MemAdd = Mem_EarAdd;

        L_MemAdd += (L_MemNo - 1) * 7;   

        //获取温度
        I2C_masterInit();
        L_Temp = I2C_Random_R(L_MemAdd);
        L_Temp = L_Temp << 8;
        L_MemAdd++;
        L_Temp |= I2C_Random_R(L_MemAdd);
        //获取时
        L_MemAdd++;
        L_HourMem = I2C_Random_R(L_MemAdd);
        //获取分
        L_MemAdd++;
        L_MinuteMem = I2C_Random_R(L_MemAdd);
        //获取月
        L_MemAdd++;
        L_MonthMem = I2C_Random_R(L_MemAdd);
        //获取日
        L_MemAdd++;
        L_DayMem = I2C_Random_R(L_MemAdd);
        //获取年
        L_MemAdd++;
        L_YearMem = I2C_Random_R(L_MemAdd);		//只传20xx-2000后的值
        I2C_Disable();
		L_mode = L_MonthMem >> 5;
        //测量部位
        if( L_mode == 0 )		//耳温
        {
            g_Ble_TxData[L_datalenth++] = 0x01;
            L_Checksum += 0x01;
        }
        else if( L_mode == 1 )	//额温
        {
            g_Ble_TxData[L_datalenth++] = 0x02;
            L_Checksum += 0x02;
        }
        else
        {
            g_Ble_TxData[L_datalenth++] = 0x03;	//物温
            L_Checksum += 0x03;
        }

        //华氏第7位置1
        if( uSetFlag.bits.Unit )
        {
            L_Temp |= 0x8000;
        }

        g_Ble_TxData[L_datalenth++] = L_Temp >> 8;
        L_Checksum += L_Temp >> 8;
        g_Ble_TxData[L_datalenth++] = L_Temp;
        L_Checksum += L_Temp;
        g_Ble_TxData[L_datalenth++] = L_YearMem;
        L_Checksum += L_YearMem;
        g_Ble_TxData[L_datalenth++] = L_MonthMem & 0x1F;
        L_Checksum += L_MonthMem & 0x1F;
        g_Ble_TxData[L_datalenth++] = L_DayMem & 0x3F;
        L_Checksum += L_DayMem & 0x3F;
        g_Ble_TxData[L_datalenth++] = L_HourMem;
        L_Checksum += L_HourMem;
        g_Ble_TxData[L_datalenth++] = L_MinuteMem;
        L_Checksum += L_MinuteMem;

        //计算下个查询地址
        L_MemNo --;
        if( L_MemNo == 0 )
        {
            //如果总记录数小于10时，说明总记录数和记录号相等，否则最大为10
            if( L_MemTotalNo < MemMaxArray )
            {
                L_MemNo	= L_MemTotalNo;
            }
            else
            {
                L_MemNo = MemMaxArray;
            }
        }
        return 0;
    }
    else
    {
        g_Ble_TxData[0] = Ble_Header1;	//包头
        g_Ble_TxData[1] = Ble_Header2;
        g_Ble_TxData[2] = Ble_Header3;
        g_Ble_TxData[3] = Ble_Header4;
        g_Ble_TxData[Index_func] = TxFunc_Temperature;	//功能码
        L_Checksum += TxFunc_Temperature;
        //包尾
        L_datalenth = L_datalenth + 3;			//加checksum和tail3个字节
        g_Ble_TxData[Index_lenth] = L_datalenth >> 8;
        L_Checksum += L_datalenth >> 8;
        g_Ble_TxData[Index_lenth+1] = L_datalenth;
        L_Checksum += L_datalenth;
        L_datalenth = L_datalenth - 3;
        g_Ble_TxData[L_datalenth++] = L_Checksum;
        g_Ble_TxData[L_datalenth++] = Ble_End1;
        g_Ble_TxData[L_datalenth++] = Ble_End2;

        index = 1;
        L_datalenth = Index_data;
        L_Checksum = 0;
        F_Ble_FirstEnter = 0;
        return 1;
    }
}


#endif
