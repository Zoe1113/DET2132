/**************************************************************************
文件名称：	App_Core.c
说    明：	核心算法（应用层）
作    者:	Liaoy
版    本：	V1.0
芯	  片:	SN8P29XX
修订记录：
**************************************************************************/
#include "Include.h"

/* 本核心程序几大注意事项，修改时请留意 */
/*************************************
1、思考：要封装到什么程序？完全不知道如何计算？
2、数组大小不可随意修改；
3、F_First_Enter_EarTp和F_First_Enter_ForeheadTp注意清0位置；
与汇编不一样的地方有两点，其他均一致：
4、滤波问题，目前未写滤波函数；
5、S_Forehead_Temp_Determine被简化过，原本是新比老大0.1，则新，0.1以内求平均，改成只取大的；
6、额温和耳温的文件合并，封装本份文件，通过宏定义选择
*************************************/


//只在本文件内使用的宏定义
#define FilterNum 0x06		//ADC滤波个数
#if ET_FT !=1
	#define ProbeMoveNum 50		//探头移除阈值
#endif

int16 Forehead_array[Forehead_Num]; //额温Tp数据表格
bit F_First_Enter_ForeheadTp = 0;	//首次进入额温采集标志位
#if ET_FT !=1
	int16 g_TpCount_avg;		//Tp平均值
	int16 g_TpCount_Max;		//Tp拐点值
	int16 Ear_array[Ear_Num]; 	//耳温Tp数据表格
	int16 TpMax_Data[6];		//耳温Tp拐点数据表格
	uint8 g_Success;		//0：未找到tp max，1：找到tp max
	uint8 g_Success_Before_Press;	//0：按下前未找到tp max，1：按下前找到tp max
	uint16 g_NtcCount_Before_Press;	//按下前ntc数据
	uint16 Time_Count;		//用于记录拐点到按键按下的时间
	bit F_above_2s = 0;		//拐点到按键按下时间小于1s标志位
	bit F_below_1s = 0;		//拐点到按键按下时间超过2s标志位
	bit F_First_Enter_EarTp = 0;		//首次进入耳温采集标志位
#endif


/**************************************************************************
函数名称：	uint8 Probe_Move( int16 *ptr, uint8 length )
函数功能：	探头移除判定
输入参数：	ptr（数组）、length（数组有效长度）
输出参数：	无
返回值  ：	0（探头移除），1（代表探头未移除）
占用空间：	TBD
备    注：	保密，连续3笔下降>50count，也就是3*50=150，150/15=1℃报er3错误
**************************************************************************/
#if ET_FT !=1			////只有耳温或者耳额温使用
uint8 Probe_Move( int16 *ptr, uint8 length )
{
	uint8 i;
	uint8 R_Down_Count = 0;		//下降计数器
	int16 R_TpCount_Diff;		//差值

	for (i = 0; i < length-1; i++ )
	{
		R_TpCount_Diff = ptr[i] - ptr[i + 1];

		//仅目标温度大于环境温度+2℃才生效（需温度差大于2度）
		if( ptr[i] > 0x00FF )
		{
			if( R_TpCount_Diff > (int16)ProbeMoveNum )
				R_Down_Count ++;
			else
				R_Down_Count = 0;
		}
	}

	if( R_Down_Count > 2 )
		return 0;
	else
		return 1;
}
#endif
/**************************************************************************
函数名称：	uint8 Adc_Filter( int16 *ptr, uint8 length )
函数功能：	获取额温tp count
输入参数：	ptr（数组）、length（数组有效长度）
输出参数：	ptr（滤波后数组）
返回值  ：	length（滤波后数组有效长度）
占用空间：	TBD
备    注：	保密
廖彦版本：
1、核心：递推平均滤波；
2、前4笔平均值和后4笔平均值(后3笔+新1笔)的差异<N，则认为稳定，否则判为不稳定；
当不稳定的次数连续发生4次，则认为数据发生质变，取新数据后面的数据，丢弃前面所有数据；
当不稳定的次数连续发生小于4次，则认为数据为异常值，丢弃这中间的3笔数据；
注：等效于第1笔((a+b+c+d)/4)和第5笔((b+c+d+e)/4)的判定，依次类推；
3、陶成林版本是在廖彦版本上修改的，引入了需同时满足“(新值+前最大值)/2>前最大值”，才会重排队列→仅针对额温；
4、金顺鑫版本删除质变的判定，只当做普通滤波使用，可以允许丢弃2笔异常值(<3次)；
注：金顺鑫这么做的原因是因为如果扫描过程中，第1s的数据如果恰巧抖动厉害数据很少，可能会发生第一秒无数据的情况（尤其是异常发生在最后面4笔）
但意义不是很大，因为没有丢弃也算是参与运算了；
关于滤波程序的应用：
宏康程序：耳温（拐点无滤波）、额温（最大值）、黑体（平均值）、生产（平均值）是调用了滤波函数，物温没有滤波且是平均值；
松翰程序（增加滤波算法前）：无；
松翰程序：耳温（拐点无滤波）、额温（最大值）、物温（最大值）、黑体（平均值）、生产（平均值）是调用了滤波函数；
5、本程序发现过的bug，当lenth<=4时for循环继续导致除ptr外的ram全部被变掉，如果lenth<8笔，但4-7笔数据是不稳定的（未触发重排队列），应该丢弃末尾数据
**************************************************************************/

#if ET_FT ==1		//额温和耳温的滤波算法不同

uint8 Adc_Filter( int16 *ptr, uint8 length )
{
	uint8 i=0, j, l_UnstableCount;
	int16 l_AdcSum, l_AdcSumTemp;
	int16 AdcTemp;



	l_UnstableCount = 0;
	l_AdcSum = ptr[0];						//取数组首笔数据
#if ZCF_debug	
	for( i=0;i<length;i++ )
	{
		Send_debug(0x30+ptr[i]/1000%10);
		Send_debug(0x30+ptr[i]/100%10);
		Send_debug(0x30+ptr[i]/10%10);
		Send_debug(0x30+ptr[i]/1%10);
		Send_debug(',');								
	}	
	Send_debug('\r');Send_debug('\n');
#endif
	i=0;
	while( i<(length-4) )        //判断当前指针是否指向末尾，如不是，继续执行
	{
		l_AdcSumTemp = ptr[i+4];			//取此笔数据的后4笔数据
		AdcTemp = labs(l_AdcSumTemp-l_AdcSum);            //差取绝对值

		if( AdcTemp > 64 )                  //如果误差大于64
		{           
			l_UnstableCount++;    	        //不稳定系数<64
		}
		else
		{
			//  0     1     2     3    4    5    6     7      8      9
			//  10    10   10    10  10   100    100    11    11    11
			//  10    10   10    10  10   11     11    11     11    11 
			if( ( l_UnstableCount<3 )&&( l_UnstableCount>0 ) )          //不稳定个数在两个极其以内，剔除   
			{
				length -= l_UnstableCount;           					//总长度减去剔除的个数

				for(j=i+4-l_UnstableCount;j<length;j++)					//计算到异常的字节数，讲后面的数据前部往前移动
				{
					ptr[j]=ptr[j+l_UnstableCount];
				}	
				i-=l_UnstableCount;                                     //重新定位下一笔计算值
			}
			l_UnstableCount	=0;	                                        //清楚不稳定系数
		}
		i++;                       										//指针自加

	}

#if ZCF_debug	
	for( i=0;i<length;i++ )
	{
		Send_debug(0x30+ptr[i]/1000%10);
		Send_debug(0x30+ptr[i]/100%10);
		Send_debug(0x30+ptr[i]/10%10);
		Send_debug(0x30+ptr[i]/1%10);
		Send_debug(',');								
	}	
	Send_debug('\r');Send_debug('\n');
#endif

	return length;
}

#else

uint8 Adc_Filter( int16 *ptr, uint8 length )
{
	uint8 i, j, l_UnstableCount;
	int16 l_AdcSum, l_AdcSumTemp;
	int16 AdcTemp;

	l_UnstableCount = 0;
	l_AdcSum = ptr[0];	//取数组首笔数据

	for( i=0; i< length-4; i++ )
	{
		l_AdcSumTemp = ptr[i+4];	//取此笔数据的后4笔数据
		AdcTemp = (int16)(l_AdcSumTemp>>2) - (l_AdcSum>>2);
		AdcTemp = labs(AdcTemp);

		//如果长度小于4直接退出
		if( length<=4 )
		{
			return length;
		}

		//如果差值<16（16*4=64count，约4*0.1=0.4℃），则认为稳定，否则为异常
		if( AdcTemp < 16 )
		{
			//如果曾经发生过异常(异常个数<4次)，则队列向前移动，丢弃中间异常的笔数
			if( (l_UnstableCount < 4) &&  l_UnstableCount != 0 )
			{
				for( j=(i+4); j<length; j++ )
				{
					ptr[j-l_UnstableCount] = ptr[j];
				}
				i = i - l_UnstableCount;	//倒退新4笔平均值首地址
				length = length - l_UnstableCount;	//总数据长度减少
				l_UnstableCount = 0;	//不稳定计数器清0
			}
			l_AdcSum = ptr[i+1];	//更新前4笔平均值数据
		}
		else
		{
			l_UnstableCount ++;
			//当异常数据发生4次，则判为发生质变丢弃所有之前的数据（含当前数据）
			if( (i+4) != length-1 )
			{
				if ( l_UnstableCount == 4 )
				{
					for( j=(i+5); j<length; j++ )
					{
						ptr[j-(i+5)] = ptr[j];		//将之后的数据从数组0开始存放
					}
					l_UnstableCount = 0;	//不稳定计数器清0
					l_AdcSum = ptr[0];		//重头开始滤波
					length = length - (i+5);	//总数据长度减少
					i = -1;		//i会自动加1
				}
			}
			else
			{
				length = i+5-l_UnstableCount;	//不管最后几笔是异常数据还是质变，均丢弃最后的数据，保留之前的数据
				return length;
			}
		}
	}
	length -= l_UnstableCount;	//可能未触发4笔不稳定而最后1-3笔数据异常，需直接剔除
	return length ;
}
#endif

/**************************************************************************
函数名称：	uint8 Remove_Tp_Max_Min( int16 *ptr, uint8 length )
函数功能：	获取额温tp count
输入参数：	g_AdcData
输出参数：	Forehead_array[]
返回值  ：	无
占用空间：	TBD
备    注：	保密
**************************************************************************/
uint8 Remove_Tp_Max_Min( int16 *ptr, uint8 length )
{
	uint8 i=0, j;
	uint8 Maxi, Mini;
	Maxi = 0;
	Mini = 0;
	for( i = 1; i < length; i++)
	{
        if(ptr[Maxi]<ptr[i])
			Maxi=i;
        else if(ptr[Mini]>ptr[i])
			Mini=i;
	}
	j = 0;
    for( i = 0; i< length; i++ )
	{
        if(i!=Maxi && i!=Mini)
            ptr[j++]=ptr[i];
	}
	return j;
}


/**************************************************************************
函数名称：	void Get_Forehead_Tp( int16 L_buf )
函数功能：	获取额温tp count
输入参数：	g_AdcData
输出参数：	Forehead_array[]
返回值  ：	无
占用空间：	TBD
备    注：	保密
**************************************************************************/
void Get_Forehead_Tp( int16 L_buf )
{
	static uint8 i;
	uint8 j;

	//首次进来滤波剔除前FilterNum个Adc count
	if( !F_First_Enter_ForeheadTp )
	{
		if( g_AdcCount > FilterNum )
		{
			F_First_Enter_ForeheadTp = 1;
			//清除所有静态变量
			g_AdcCount = 0;
			i = 0;
		}
	}
	else
	{
		//当前adc入栈数组保存
		if( i != Forehead_Num )
		{
			Forehead_array[i] = L_buf;
			i++;
		}
		else
		{
			//当前值永远存放在数组的最后1个
			for( j=0; j< (Forehead_Num-1); j++ )
			{
				Forehead_array[j] = Forehead_array[j+1];
			}
			Forehead_array[Forehead_Num-1] = L_buf;
		}
	}
}

/**************************************************************************
函数名称：	int16 Get_Forehead_Tp_Max_Avg(uint8 length)
函数功能：	获取额温tp最大值平均值
输入参数：	Forehead_array[]、Num（平均值个数，最大不可超过数组个数ArrayNum_Max）
输出参数：	L_buf（最大值平均值）
返回值  ：	L_buf（最大值平均值）
占用空间：	TBD
备    注：	保密
**************************************************************************/
int16 Get_Forehead_Tp_Max_Avg(uint8 length)
{
	uint8 i;
	uint8 j = 0;
	int16 L_buf = 0x8000;	//赋最小初值
	int32 L_Sum = 0;

	//找数组中的最大值
	for( i=0; i<length; i++ )
	{
		if( Forehead_array[i] > L_buf )
		{
			L_buf = Forehead_array[i];
		}
	}

	//最大值-20count（0.2℃对应的count）作为基准值
	L_buf = L_buf - 20;

	//找出最大值0.2℃范围内所有Adc并求平均值
	for( i=0; i<length; i++ )
	{
		if( Forehead_array[i] > L_buf )
		{
			L_Sum += Forehead_array[i];
			j++;
		}
	}

	L_buf = L_Sum/j;
	return L_buf;
}

/**************************************************************************
函数名称：	void Get_Ear_Tp_Max( int16 L_buf )
函数功能：	获取tp拐点值
输入参数：	g_AdcData
输出参数：	g_Success、g_TpCount_Max（拐点值）
返回值  ：	无
占用空间：	TBD
备    注：	保密，确认与汇编一致
**************************************************************************/
#if ET_FT !=1			//只有耳温或者耳额温使用

void Get_Ear_Tp_Max( int16 L_buf )
{
	static uint8 i;
	static uint8 R_Up_Count;	//上升计数器
	static uint8 R_Down_Count;	//下降计数器
	static bit F_Up_Stage;		//0：未处于上升阶段，1：已判定为上升阶段
	int16 R_TpCount_Diff;		//差值
	uint8 j;
	static uint8 n = 0;

	//首次进来滤波剔除前FilterNum个Adc count
	if( !F_First_Enter_EarTp )
	{
		if( g_AdcCount > FilterNum )
		{
			//清除所有静态变量
			F_First_Enter_EarTp = 1;
			g_AdcCount = 0;
			g_TpCount_Max = 0;
			R_Up_Count = 0;
			R_Down_Count = 0;
			F_Up_Stage = 0;
			n = 0;
			i = 0;
   	    	g_Success = 0;      //置未找到拐点标志位
		}
	}
	else
	{
		//当前adc入栈数组保存
		if( i != Ear_Num )
		{
			Ear_array[i] = L_buf;
			i++;
		}
		else
		{
			//当前值永远存放在数组的最后1个
			for( j=0; j< (Ear_Num-1); j++ )
			{
				Ear_array[j] = Ear_array[j+1];
			}
			Ear_array[Ear_Num-1] = L_buf;
		}

		Time_Count ++;
		R_TpCount_Diff = L_buf - Ear_array[i-2];

		//未处于上升阶段，判定是否处于上升阶段
		if( !F_Up_Stage )
		{
			//差值大于阈值，上升计数器累加，否则清0
			if( R_TpCount_Diff > Up_Diff_Val )
			{
				R_Up_Count ++;
			}
			else
			{
				R_Up_Count = 0;
			}
			//上升次数大于阈值则表示已处于上升阶段
			if( R_Up_Count >= Up_Val )
			{
				F_Up_Stage = 1;
				R_Up_Count = 0;
			}
		}
		//处于上升阶段，判定是否开始下降
		else
		{
			//差值小于阈值，下降计数器累加，否则清0
			if( R_TpCount_Diff < Down_Diff_Val )
			{
				R_Down_Count ++;
			}
			else
			{
				R_Down_Count = 0;
			}

			//下降次数大于阈值则表示已处于下降阶段
			if( R_Down_Count >= Down_Val )
			{
				F_Up_Stage = 0;		//清上升阶段标志位
				R_Down_Count = 0;

				//保存拐点
				if( Ear_array[i-4] > g_TpCount_Max )
				{
					g_Success = 1;		//找到拐点
					Time_Count = 0;		//时间清0
					g_TpCount_Max = Ear_array[i-4];

					/******调试用*******/
					//当前adc入栈数组保存
					#if Func_debug
						if( n != 6 )
						{
							TpMax_Data[n] = g_TpCount_Max;
							n++;
						}
						else
						{
							//当前值永远存放在数组的最后1个
							for( j=0; j< 5; j++ )
							{
								TpMax_Data[j] = TpMax_Data[j+1];
							}
							TpMax_Data[5] = g_TpCount_Max;
						}
						Uart_Transmit(0xEE, g_TpCount_Max);
					#endif
					/******调试用*******/
				}
			}
		}
	}
}
#endif

/**************************************************************************
函数名称：	Backup_Testing_Param()
函数功能：	获取tp值
输入参数：	Time_Count
输出参数：	F_belwo_1s（拐点到按键按下时间小于1s标志位）、F_above_2s（拐点到按键按下时间超过2s标志位）、g_NtcCount_Before_Press（按下时Ntc Count）、g_Success(清0)
返回值  ：	无
占用空间：	TBD
备    注：	保密
**************************************************************************/
#if ET_FT !=1			////只有耳温或者耳额温使用

void Backup_Testing_Param(void)
{
	//保存按下时的ntc温度
	g_NtcCount_Before_Press = g_NtcCount;
	g_Success_Before_Press = g_Success;

	//按下前找到拐点进行判断
	if ( g_Success )
	{
		//拐点到按键按下时间小于1s
		if( Time_Count < 61 )
		{
			F_below_1s = 1;
		}

		//拐点到按键按下时间超过2s
		if( Time_Count > 122)
		{
			F_above_2s = 1;
		}
	}
	g_Success = 0;	//找到tpmax清0
}
#endif

/**************************************************************************
函数名称：	int16 Get_Ear_Tp_Avg(uint8 length)
函数功能：	求Ear_Num组平均值
输入参数：	Ear_array[]
输出参数：	g_TpCount
返回值  ：	L_buf(Ear_Num的平均值)
占用空间：	TBD
备    注：	保密
**************************************************************************/
#if ET_FT !=1			////只有耳温或者耳额温使用

int16 Get_Ear_Tp_Avg(uint8 length)
{
	uint8 i;
	int32 L_buf = 0;

	F_First_Enter_EarTp = 0;	//保证下次测量重新采集

	//求平均值
	for( i = 0; i <length; i++)
	{
		L_buf += Ear_array[i];
	}
	L_buf = L_buf/length;
	return L_buf;
}
#endif

/**************************************************************************
函数名称：	uint16 NtcCount_Determine(void)
函数功能：	根据不同条件决定最终ntc
输入参数：	g_NtcCount_Press（开机ntc）、g_NtcCount（测量完成时ntc）
输出参数：	L_NtcCount_buf
返回值  ：	L_NtcCount_buf(g_NtcCount_Press、g_NtcCount二选一)
占用空间：	TBD
备    注：	保密，确认与汇编一致
**************************************************************************/
#if ET_FT !=1			////只有耳温或者耳额温使用

uint16 NtcCount_Determine(void)
{
	int16 L_NtcCount_buf;

	//耳温模式下，判断使用开机ntc还是测量结束时ntc还是两者平均值
	if( eTestmode_num == Earmode )
	{
		//默认使用开机ntc
		L_NtcCount_buf = g_NtcCount_Before_Press;

		//按键按下后找到最大拐点，取开机ntc
		if( g_Success )
		{
			L_NtcCount_buf = g_NtcCount_Before_Press;
		}
		else
		{
			//温度小于16℃(0x3A6C)，取开机ntc
			if( g_NtcCount > 0x3A6C)
			{
				L_NtcCount_buf = g_NtcCount_Before_Press;
			}
			else
			{
				//温度大于16℃，差值大于0.3℃，取开机ntc，否则取平均值
				if( (g_NtcCount - g_NtcCount_Before_Press) > 0x96 )
				{
					L_NtcCount_buf = g_NtcCount_Before_Press;
				}
				else
				{
					L_NtcCount_buf = ( g_NtcCount + g_NtcCount_Before_Press ) >> 1;
				}
			}
		}
	}
	else if( eTestmode_num == Foreheadmode )
	{
		L_NtcCount_buf = g_NtcCount_Before_Press;
	}
	//其他模式外均为平均值
	else
	{
		L_NtcCount_buf = ( g_NtcCount + g_NtcCount_Before_Press ) >> 1;
	}
	return L_NtcCount_buf;
}
#endif
/**************************************************************************
函数名称：	int16 TpCount_Determine(void)
函数功能：	根据不同条件决定最终tp
输入参数：	g_TpCount_Max（拐点值）、g_TpCount_avg（平均值）
输出参数：	L_buf0
返回值  ：	L_buf0（g_TpCount_Max、g_TpCount_avg二选一）
占用空间：	TBD
备    注：	保密，确认与汇编一致
**************************************************************************/
#if ET_FT !=1			////只有耳温或者耳额温使用

int16 TpCount_Determine(void)
{
	int16 L_TpCount_buf;

	//耳温模式下，根据不同条件决定使用平均值还是拐点值
	if( eTestmode_num == Earmode)
	{
		//耳温模式默认均取拐点值
		L_TpCount_buf = g_TpCount_Max;

		//环境温度＞34℃求平均
		if( g_NtcCount < 0x1AA0 )
		{
			L_TpCount_buf = g_TpCount_avg;
		}
		else
		{
			//按下前后均未找到拐点
			if( !g_Success_Before_Press && !g_Success )
			{
				L_TpCount_buf = g_TpCount_avg;
			}

			//只找到按下前拐点值
			if( g_Success_Before_Press && !g_Success )
			{
				//按下前的拐点值到按下的时间>2s，取拐点(这段程序无意义)
				if( F_above_2s )
				{
					F_above_2s = 0;
				}

				//平均值大于拐点则判断是否Er3，小于则取拐点
				if( g_TpCount_avg > g_TpCount_Max )
				{
					//平均值-拐点>1.5℃，同时满足拐点和平均值时间小于1s
					if( (g_TpCount_avg - g_TpCount_Max) > 0xFF && F_below_1s )
					{
						uErrFlag.g_ErrFlag = 0;
						uErrFlag.bits.Er3 = 1;
						F_below_1s = 0;
					}
				}
			}
		}
	}
	//其他模式均取平均值
	else
	{
		L_TpCount_buf = g_TpCount_avg;
	}
	return L_TpCount_buf;
}
#endif