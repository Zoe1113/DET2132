/**************************************************************************
文件名称：	APP_Isr.c
说    明：	中断服务程序（应用层）
作    者:	Liaoy
版    本：	V1.0
芯	  片:	SN8P29XX
修订记录：
**************************************************************************/
#include "Include.h"

#if Func_Ble
	static bit F_Receive_First = 0;
	static uint8 *p;
#endif

/**************************************************************************
	Interrupt service program
**************************************************************************/
void __interrupt[0x08] Interrupt_pro(void)
{
	#if Func_Ble		
		uint8 L_Rx_Data;

    if(FT0IRQ)
	{
		FT0IRQ=0;
		F_500ms =1;		//set 500ms flag
	}
	else
	#endif
	
	if(FADC1IRQ)
	{
		FADC1IRQ = 0;
	}
	else if(FTC0IRQ)
	{
		FTC0IRQ =0;
	}
	else if(FTC1IRQ)
	{
		FTC1IRQ = 0;
		F_10ms = 1 ;	//set 10ms flag
		F_10ms_task = 1;

        #if Have_Motor
		    if( g_MotorSystick>0 )
		    {
			    g_MotorSystick--;
			    Port_Motor = 1;
		    }
		    else
			{
				Port_Motor =0;
			}
	    #endif			
	}
	else if(FP00IRQ)
	{
		FP00IRQ = 0;
	}
	else if(FP01IRQ)
	{
		FP01IRQ = 0;
	}
	else if(FUTX1IRQ)
	{
		FUTX1IRQ = 0;
	}
	else if(FURX1IRQ)
	{
		FURX1IRQ = 0;

		#if Func_Ble
			L_Rx_Data = URX1D;
			if( !F_Receive_First )
			{
				//如果检测到包头首字节，则开始存缓存
				if( L_Rx_Data == Ble_Header1 || L_Rx_Data == Ble_Header21 )
				{
					F_Receive_First = 1;
					p = g_Ble_RxData;	//数组首地址赋给指针p
					*p ++ = L_Rx_Data;	//RX数据存入数组
				}
			}
			else
			{
				*p ++ = L_Rx_Data;	//RX数据存入数组
				if( L_Rx_Data == Ble_End2 || L_Rx_Data == Ble_End22 )
				{
					F_Receive_First = 0;	//缓存结束
					F_Uart_Receive = 1;		//置收到数据命令
				}
				else
				{
					//如果指针越界还未结束，则强行结束
					if( p == (g_Ble_RxData + DataLenth) )
					{
						F_Receive_First = 0;	//缓存结束，重新等待接收
					}
				}
			}
		#endif
	}
	else
	{
	}
}
