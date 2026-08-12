#ifndef _App_BleMode06_H
#define _App_BleMode06_H


//常用ASCII表
#define ASCII_Dash 45
#define ASCII_Point 46
#define ASCII_B 66
#define ASCII_D 68
#define ASCII_E 69
#define ASCII_G 71
#define ASCII_M 77
#define ASCII_T 84
#define ASCII_b 98
#define ASCII_0 48
#define ASCII_1 49
#define ASCII_2 50
#define ASCII_3 51
#define ASCII_4 52
#define ASCII_5 53
#define ASCII_6 54
#define ASCII_7 55
#define ASCII_8 56
#define ASCII_9 57

#define Port_Rx FP15	//mcu Rx pin
#define Port_Tx FP14	//mcu TX pin
// #define Port_Ble_Power FP16	//蓝牙供电脚（低电平有效，输出使能口）
#define Port_Ble_En FP23	//蓝牙使能脚（低电平有效，输出使能口）
#define Port_Ble_Link FP24	//蓝牙连接成功脚（低电平有效，输入检测口）
#define Port_Ble_Busy FP22	//蓝牙忙闲脚（低电平有效，输入检测口）
//#define Port_Power  FP16   //（power使能脚）

//功能码
//mcu to ble的命令码
#define TxFunc_Default 0x00		//空闲
#define TxFunc_Temperature 0x03	//ET产品温度功能码
#define TxFunc_Unit 0x05		//单位功能码
#define TxFunc_DeviceInfo 0x09	//仪器广播信息功能码
#define TxFunc_TimeUpdate 0x06	//时间更新功能码

//app to mcu的应答码
#define RxAck_Temperature 0x83	//ET产品温度应答码
#define RxAck_Unit 0x85
#define RxAck_DeviceInfo 0x89	//仪器广播信息应答码

//ble to mcu的应答码
#define RxFunc_Checksumfail 0x00	//所有Rx接受校验和错误均为0x00，校验和错，MCU不做任何响应
#define RxFunc_UpdateTime 0x86

//以下是蓝牙协议格式的预定义
#define DataLenth 18		//蓝牙协议数据-总字节数(接收数据不可以超出此长度)
#define Index_lenth 4		//蓝牙协议数据-数据长度位标（特别注意数据长度是2字节，很多时候直接用的是低字节）
#define Index_func 6		//蓝牙协议数据-功能码位标
#define Index_data 7		//蓝牙协议数据-数据段位标
#define MaxData 240			//蓝牙协议数据-数据段
//注：不可以增加校验和和包尾的位标（可变）

/* 唤醒帧 */
#define Ble_WakeupHeader 0x00

/* 帧头 */
#define Ble_Header1 0xFA
#define Ble_Header2 0xAA
#define Ble_Header3 0xAA
#define Ble_Header4 0xAF

#define Ble_Header21 0x55
#define Ble_Header22 0xAA
#define Ble_Header23 0xAA
#define Ble_Header24 0x55

/* 帧尾 */
#define Ble_End1 0xF5
#define Ble_End2 0x5F

#define Ble_End21 0x5A
#define Ble_End22 0xA5

// //蓝牙数据结构体写法(注意结构体不可以超过127byte)
// typedef struct
// {
// 	uint8 Head[4];	//包头
// 	uint8 Lenth[2];	//数据总字节数
// 	uint8 Func;		//功能码
// 	uint8 Data[MaxData];	//数据段(含校验和、包尾)
// 	uint8 CheckSum;	//校验和（Lenth到Data数据求和低8位）
// 	uint8 Tail[2];	//包尾
// }sUart;

// extern sUart sBLEData;

//蓝牙状态
typedef enum
{
	Ble_Standby = 0,
	Ble_DeviceInfo ,
	Ble_Disable ,
	Ble_Advertising ,
	BLE_UpdateTime ,
	Ble_Idle ,
	Ble_UploadData ,
	Ble_LinkErr,
	Ble_TimeoutErr
}eBleModeTask;

extern eBleModeTask eBle_Sta;

extern bit F_Ble_En;	//蓝牙使能
extern bit F_Uart_Receive;	//uart接收成功
extern bit F_Ble_Blink;		//蓝牙标志闪烁
extern bit F_Ble_FirstEnter;
extern uint8 g_ble_timeout;	//蓝牙超时变量
extern uint8 g_ble_ack_timeout;	//蓝牙数据传输超时变量
extern uint8 g_Ble_RxData[DataLenth];	//蓝牙接收数据缓冲区
extern uint8 g_Ble_RxData_Buf[DataLenth];	//蓝牙接收数据缓冲区
extern uint8 g_Ble_TxData[250];		//蓝牙发送数据缓冲区（ET218需要）
extern uint8 Ble_Waittostart;      //等待重连的延时



//函数声明
void App_BleMode(void);
uint8 Uart_Receive_Event(void);
void Uart_Ack_Event(uint8 L_ble_func);
void UART_Send_To_Ble(void);
uint8 BleData_Check(void);
uint8 App_Upload_Temperature();
void App_Upload_DeviceInfo(void);
void App_Upload_Time(void);
uint8 App_UploadData(uint8 L_Upload_func);

#endif
