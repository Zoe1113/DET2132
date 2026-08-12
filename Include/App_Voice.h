#ifndef _App_Voice_H
#define _App_Voice_H

#define Port_MISO FP13   //spi miso
#define Port_MOSI FP12   //spi mosi
#define Port_SCK  FP16   //spi sck
#define Port_Power  FP20   //spi ce（power使能脚）

/******* AM5BA084语音协议 ********
1、LL（1byte） + SS（1byte） + D1D0（2字节，BCD码，故最大4个数字） + Checksum（1byte）
2、LL高4bit：0001：英语。0010：表示中文。
3、LL低4bit：1011:播放语音。1101：音量调节。0101:停止。
4、当命令为播放语音时：SS表示播放语音的段。当命令为音量时：SS的低4位为调节的音量值(0xE0-0xE7，E0最小，E7最大)。当命令为停止时，SS为0xFE
5、DIDO：BCD码格式。播放语句中的数字。BCD码2字节，故最大4个数字，比如0370，代表37.0，如果1370代表137.0
6、Checksum=LL+SS+D0+D1。是不带进位的加法。当语音计算出的Checksum和接收到的Checksum不相等或者通信出错时，SO=Low，表示接收到数据错误。主控IC重发。
********************************/

//AM5BA084 指令符LL（1byte高4位）
#define lang_En 0x10    //英语
#define lang_Ch 0x20    //中文

//AM5BA084 指令符LL（1byte低4位）
#define LL_Broadcast 0x0B    //播放语音
#define LL_Volumn    0x0D    //音量调节
#define LL_Stop      0x05    //停止播放

//AM5BA084 语音段指令SS（1byte）
#define SS_Di       0x02    //滴
#define SS_Clean    0x04    //滴,测量前请检查镜片是否干净
#define SS_BodyC    0x06    //您的体温是__N__摄氏度
#define SS_BodyF    0x08    //您的体温是__N__华氏度
#define SS_ObjC     0x0A    //物体温度是__N__摄氏度
#define SS_ObjF     0x0C    //物体温度是__N__华氏度
#define SS_TempC    0x0E    //__N__摄氏度
#define SS_TempF    0x10    //__N__华氏度
#define SS_MemNum   0x12    //记忆__N__
#define SS_NoMem    0x14    //没有记录
#define SS_Errmsg   0x16    //错误__N__，请查看说明书
#define SS_Lowbat   0x18    //电池电量低
#define SS_Stop     0x00    //停止命令
//#define SS_VolumeMax   0x0F    //最大音量
#define SS_VolumeMax   0x05    //最适音量

#define Nodata  0x0000  //表示无数据(要特别小心物温0摄氏度怎么办？)

extern uint8 g_LLCode;		//指令段
extern uint16 g_DiDo;		//数据段

typedef enum
{
    Play_Ready = 1,
    Play_Stop,
    Play_Di,
    Play_Clean,
    Play_Temp,
    Play_MemTemp,
    Play_MemNum,
    Play_NoMem,
    Play_Errmsg,
    Play_VolumeMax,
    Play_Lowbat,
    Play_Wait
}ePlayVoice;

//extern ePlayVoice eAMK_PlayStatus;

void AM5BA_SPI_Init(void);
void AM5BA_Power_Enable(void);
void AM5BA_Power_Disable(void);
void AM5BA_SPI_Stop(void);
void AM5BA_SPI_Send_Byte(uint8 L_data);
void AM5BA_Send_Cmd(uint8 L_LLCode, uint8 L_SSCode, uint16 L_DIDO);
// void App_PlayVoice(uint8 L_LLCode, uint8 L_SSCode, uint16 L_DIDO);
// void App_StopPlay(void);
// void App_PlayVoice2(void);
void App_PlayVoice3(void);
void PlayStatueParam(uint8 SetGetFlag, ePlayVoice StatueIn, ePlayVoice *pStatueOut);
extern void voice_stop(void);


#endif