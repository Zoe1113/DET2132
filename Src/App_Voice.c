/**************************************************************************
文件名称：	App_Voice.c
说    明：	ADC相关设置集合（驱动层）
作    者:	Liaoy
版    本：	V1.0
芯	  片:	SN8P29XX
备    注：  播报部分有三种写法，如果采用方法2，不如直接采用方法3，大约650word
修订记录：
**************************************************************************/
#include "Include.h"

#if Have_Voice_Func

#define MISO(X)  (X?(FP13=1):(FP13=0))
#define MOSI(X)  (X?(FP12=1):(FP12=0))
#define SCK(X)   (X?(FP16=1):(FP16=0))

uint8 g_LLCode;		//指令段
uint16 g_DiDo;		//数据段

//ePlayVoice eAMK_PlayStatus;

/******** 本程序的疑问 **********
1、无数据还是要发0000 ok
2、物温0℃bug； ok
3、am5ba貌似无需6ms唤醒；ok
4、音量貌似不是E0-E7而是00-0f ok
5、停止是00，貌似不是FE ok
6、音量、停止 后面要延迟多少MISO才会拉低；>4ms
7、stop、音量等播报声音的miso会拉低吗？拉低时间持续多久？也会，也是>4ms；
给的建议是：我这边接收完五个字节，在做命令处理的，所以建议你发送控制指令的时候，也是一样补全五个字
******************************/

/**************************************************************************
函数名称：  void PlayStatueParam(bit SetGetFlag, ePlayVoice Statue)
函数功能：  语音播放参数设置读取函数
输入参数：  SetGetFlag: 1:设置参数 0： 读取参数  表明是设置参数还是读取参数
           Statue：语音函数参数
输出参数：  无
返回值  ：  无
占用空间：  
备    注：  
**************************************************************************/
void PlayStatueParam(uint8 SetGetFlag, ePlayVoice StatueIn, ePlayVoice *pStatueOut)
{
    static ePlayVoice StatueParam[4];   //3--最大设置参数深度
    static int8 SetStatueParamNo = 0, GetStatueParamNo = 0;
    if(2 == SetGetFlag)
    {
        for(uint8 i = 0;i<4;i++)
            StatueParam[i] = 0;
        SetStatueParamNo = 0;
        GetStatueParamNo = 0;
    }
    else if(1 == SetGetFlag)
    {
        if(SetStatueParamNo < 3)
        {
            StatueParam[SetStatueParamNo] = StatueIn;
            SetStatueParamNo++;
        }
    }
    else
    {
        if(StatueParam[GetStatueParamNo])
        {
            *pStatueOut = StatueParam[GetStatueParamNo];
            StatueParam[GetStatueParamNo] = 0;
            if(StatueParam[GetStatueParamNo+1])
                GetStatueParamNo++;
            else
            {
                GetStatueParamNo = 0;
                SetStatueParamNo = 0;
            }
        }
        else
        {
            GetStatueParamNo = 0;
            SetStatueParamNo = 0;
        }   
    }
}


/**************************************************************************
函数名称：	void App_PlayVoice(uint8 L_LLCode, uint8 L_SSCode, uint16 L_DIDO)
函数功能：	SPI初始化
输入参数：	无
输出参数：	无
返回值  ：	无
占用空间：	773-394=379word
备    注：  菜单式写法，能当任务轮询
**************************************************************************/
void App_PlayVoice3(void)
{
    static ePlayVoice eAMK_PlayStatus = 0;
    PlayStatueParam(0,0,&eAMK_PlayStatus);
    switch( eAMK_PlayStatus )
    {
        case Play_Ready:
            break;

        case Play_Stop:
            AM5BA_Send_Cmd(LL_Stop, SS_Stop, Nodata);
            eAMK_PlayStatus = Play_Wait;
           break;

        case Play_Di:
            if( !Port_MISO )
            {
                AM5BA_Send_Cmd(LL_Broadcast, SS_Di, Nodata);
                eAMK_PlayStatus = Play_Wait;
            }
            break;

        case Play_Clean:
            if( !Port_MISO )
            {
                if( eTestmode_num == Earmode )
                {
                    Delay1ms(3);        //该延迟必须有，非常奇怪！！！
                    AM5BA_Send_Cmd(LL_Broadcast, SS_Clean, Nodata);   
                }
                eAMK_PlayStatus = Play_Wait;
            }
            break;

        case Play_Temp:
            if( !Port_MISO )
            {
                HexToBcd(g_DiDo/10);
                g_DiDo = ((uint16) Hex2Bcd[1] << 8) | Hex2Bcd[0];
                eAMK_PlayStatus = Play_Clean;   //测量结果播报完毕必定播报clean语音
                
                if ( eTestmode_num == Earmode || eTestmode_num == Foreheadmode )
                {
                    if( uSetFlag.bits.Unit )
                        AM5BA_Send_Cmd(LL_Broadcast, SS_BodyF, g_DiDo);
                    else
                        AM5BA_Send_Cmd(LL_Broadcast, SS_BodyC, g_DiDo);
                }
                else if ( eTestmode_num == Objectmode )
                {
                    if( uSetFlag.bits.Unit )
                        AM5BA_Send_Cmd(LL_Broadcast, SS_ObjF, g_DiDo);
                    else
                        AM5BA_Send_Cmd(LL_Broadcast, SS_ObjC, g_DiDo);
                }
                else
                {
                    AM5BA_Send_Cmd(LL_Broadcast, SS_Di, Nodata);
                    eAMK_PlayStatus = Play_Wait;    //防止clean的di太过连续
                }
            }
            break;

        case Play_MemTemp:
            if( !Port_MISO )
            {
                HexToBcd(g_DiDo/10);
                g_DiDo = ((uint16) Hex2Bcd[1] << 8) | Hex2Bcd[0];
                if( uSetFlag.bits.Unit )
                    AM5BA_Send_Cmd(LL_Broadcast, SS_TempF, g_DiDo);
                else
                    AM5BA_Send_Cmd(LL_Broadcast, SS_TempC, g_DiDo);
                eAMK_PlayStatus = Play_Wait;
            }
            break;

        case Play_MemNum:
            if( !Port_MISO )
            {
                g_DiDo = g_DiDo*10;
                HexToBcd(g_DiDo);
                g_DiDo = ((uint16) Hex2Bcd[1] << 8) | Hex2Bcd[0];
                AM5BA_Send_Cmd(LL_Broadcast, SS_MemNum, g_DiDo);
                eAMK_PlayStatus = Play_Wait;
            }
           break;

        case Play_NoMem:
            if( !Port_MISO )
            {
                AM5BA_Send_Cmd(LL_Broadcast, SS_NoMem, Nodata);
                eAMK_PlayStatus = Play_Wait;
            }
            break;

        case Play_Errmsg:
            if( !Port_MISO )
            {
                switch (g_DiDo)
                {
                    case 0x01:
                        g_DiDo = 0x01;  //错误1
                        break;
                    case 0x02:
                        g_DiDo = 0x02;  //错误2
                        break;
                    case 0x04:
                        g_DiDo = 0x03;  //错误3
                        break;
                    //case 0x08:
                    //    g_DiDo = 0x04;  //错误4
                    //    break;
                    case 0x10:
                        g_DiDo = 0x05;  //错误5
                        break; 
                    case 0x20:
                        g_DiDo = 0x06;  //错误6
                        break;                     
                    default:
                        break;
                }
                g_DiDo = g_DiDo*10;
                HexToBcd(g_DiDo);
                g_DiDo = ((uint16) Hex2Bcd[1] << 8) | Hex2Bcd[0];
                AM5BA_Send_Cmd(LL_Broadcast, SS_Errmsg, g_DiDo);
                eAMK_PlayStatus = Play_Wait;
            }
            break;

        case Play_VolumeMax:
            if( !Port_MISO )
            {
                AM5BA_Send_Cmd(LL_Volumn, SS_VolumeMax, Nodata);
                eAMK_PlayStatus = Play_Wait;
            }
            break;

        case Play_Lowbat:
            if( !Port_MISO )
            {
                AM5BA_Send_Cmd(LL_Broadcast, SS_Lowbat, Nodata);
                eAMK_PlayStatus = Play_Wait;
            }
            break;

        case Play_Wait:
            if( !Port_MISO )
                eAMK_PlayStatus = Play_Ready;
            break;

        default:
            break;
    }
    if(eAMK_PlayStatus)
        Delay1ms(5);
}

/**************************************************************************
函数名称：	void AM5BA_SPI_Init(void)
函数功能：	SPI初始化
输入参数：	无
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void AM5BA_SPI_Init(void)
{
    FP13M = 0;  //miso input mode，high level
    FP12M = 1;  //mosi output mode，low level
    FP16M = 1;  //SCK output mode，low level
    FP20M = 1;  //ce output mode, high level
    P1 |= 0x08;   //00001000
    P2 |= 0x01;   //00000001
}

/**************************************************************************
函数名称：	void AM5BA_Power_Enable(void)
函数功能：	开启电源
输入参数：	无
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void AM5BA_Power_Enable(void)
{
    Port_Power = 0; //使能供电
    Delay10ms(30);  //supplier recommend 300ms delay
}

/**************************************************************************
函数名称：	void AM5BA_Power_Disable(void)
函数功能：	开启电源
输入参数：	无
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void AM5BA_Power_Disable(void)
{
    //关闭供电
    Port_Power = 1;
}

/**************************************************************************
函数名称：	void AM5BA_SPI_Stop(void)
函数功能：	SPI停止命令
输入参数：  无
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void AM5BA_SPI_Stop(void)
{
    SCK(0);
    MOSI(0);
    Delay50us(2);
}

/**************************************************************************
函数名称：	void AM5BA_SPI_Send_Byte(uint8 L_data)
函数功能：	SPI发送字节函数
输入参数：  L_data
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	无
**************************************************************************/
void AM5BA_SPI_Send_Byte(uint8 L_data)
{
    uint8 i;

//     for( i=0; i<8; i++ )
//     {
//         SCK(0);
//         if( L_data & 0x80 )
//             MOSI(1);
//         else
//             MOSI(0);
//         L_data = L_data << 1;
//         SCK(1);
//         Delay50us(10);
//         SCK(0);
//         Delay50us(10);   //下降沿锁存数据
//     }

    for( i=0; i<8; i++ )
    {
        SCK(0);   //SCK低电平200us
        Delay50us(4);
        if( L_data & 0x80 )
            MOSI(1);
        else
            MOSI(0);
        L_data = L_data << 1;
        Delay50us(2);   //数据维持100us
        SCK(1);
        Delay50us(4);   //SCK高电平200us
    }
    SCK(0);
    Delay50us(2);   //SCK低电平100us
}

/**************************************************************************
函数名称：	uint8 AM5BA_Send_Cmd(uint8 L_LLCode, uint8 L_SSCode, uint16 L_DIDO)
函数功能：	发送语音命令
输入参数：  L_LLCode、L_SSCode、L_DIDO
输出参数：	MOSI
返回值  ：	无
占用空间：	TBD
备    注：	无数据命令时，DIDO为0x0000
**************************************************************************/
void AM5BA_Send_Cmd(uint8 L_LLCode, uint8 L_SSCode, uint16 L_DIDO)
{
    uint8 Checksum;   //LL+SS+D1+D0之和
    uint8 i;

    for( i=0; i<3; i++ )        //错误尝试3次
    {
        L_LLCode |= g_LLCode;   //生成完整的LL命令
        Checksum = L_LLCode;
        Checksum += L_SSCode;

        FGIE = 0;
        AM5BA_SPI_Send_Byte(L_LLCode);    //发送LL指令段
        AM5BA_SPI_Send_Byte(L_SSCode);    //发送SS语音段
        Checksum += L_DIDO>>8;
        Checksum += L_DIDO;
        AM5BA_SPI_Send_Byte(L_DIDO>>8);   //发送数据段DI
        AM5BA_SPI_Send_Byte(L_DIDO);      //发送数据段DO
        AM5BA_SPI_Send_Byte(Checksum);    //发送校验和
        AM5BA_SPI_Stop();
        Delay1ms(5);    //supplier recommend 5ms delay
        FGIE = 1;
        //如果通讯成功，MISO为高电平，否则为低电平；
        if (Port_MISO)
            break;
    }
}


#endif

/**************************************************************************
函数名称：	void voice_stop(void)
函数功能：	停止语音播报
输入参数：  无
输出参数：	无
返回值  ：	无
占用空间：	TBD
备    注：	无数据命令时，DIDO为0x0000
**************************************************************************/
void voice_stop(void)
{
    #if Have_Voice_Func
        PlayStatueParam(2 , 0 , 0);
        PlayStatueParam(1 , Play_Stop,0);
        //App_PlayVoice3();
    #endif
}

