#ifndef _App_Param_H
#define _App_Param_H

//EEPROM地址分配
#define I2C_Add_Cali25TP 		0x00	 	//I2C 25度校准值存储地址
#define I2C_Add_Cali37TP 		0x02	 	//I2C 37度校准值存储地址
#define I2C_Add_Cali41TP 		0x04	 	//I2C 41度校准值存储地址
#define Distance_5cm     		0x06	 	//I2C 测距的5cm校准数值
#define I2C_Add_Table 			0x10	 	//I2C黑体表格选择存储地址
#define I2C_Add_Voice 			0x11	 	//I2C语音语言选择存储地址
#define I2C_Add_Unit 			0x12	 	//I2C单位选择存储地址
#if ET_FT !=1
    #define I2C_Add_Emission 		0x14	//I2C人体发射率存储地址
    #define I2C_Add_HumanRatio1 	0x15	//I2C人体系数存储地址（不戴耳套的系数）
    #define I2C_Add_HumanRatio2 	0x19	//I2C人体系数存储地址2（戴耳套的系数）
    #define I2C_Add_PcRatio 		0x16	//I2C耳套系数存储地址
    #define I2C_Add_PcStatus 		0x17	//I2C耳套状态存储地址
#endif


#define I2C_Add_EarMem 			0x18		//I2C耳温模式记忆总记录数存储地址
// #define I2C_Add_ForeMem 		0x19		//I2C额温模式记忆总记录数存储地址
// #define I2C_Add_ObjMem 		0x1A		//I2C物温模式记忆总记录数存储地址
#define I2C_Add_CheckSum 		0x1E	 	//I2C校准值的校验和存储地址(校验和一定识别码之前写入，地址不一定要在前面)
#define I2C_Add_IdentifyCode 	0x1F 		//I2C识别码存储地址

#define I2C_Add_Offset 			0x03 		//I2C耳温、额温、物温模式当前记录号相对总记录号地址偏移量

#define Mem_EarAdd 				0x20		//I2C耳温记忆截止地址  0x20
// #define Mem_ForeAdd 			0x66		//I2C额温记忆截止地址
// #define Mem_ObjAdd 			0xAC		//I2C物温记忆截止地址

#define I2C_Add_Earcap10 		0xFF	  	//I2C耳套系数存储地址
#define I2C_Add_Earcap15 		0xFE	  	//I2C耳套系数存储地址
#define I2C_Add_Earcap20 		0xFD	  	//I2C耳套系数存储地址
#define I2C_Add_Earcap25 		0xFC	  	//I2C耳套系数存储地址
#define I2C_Add_Earcap30 		0xFB	  	//I2C耳套系数存储地址
#define I2C_Add_Earcap35 		0xFA	  	//I2C耳套系数存储地址
#define I2C_Add_Earcap40 		0xF9	  	//I2C耳套系数存储地址

#define IdentifyCode            0x37        //识别码
/****************************************系数相关RAM定义****************************************/
#if ET_FT !=1
    extern uint8 g_HumanRatio1;			//人体系数1
    extern uint8 g_HumanRatio2;			//人体系数2
    extern uint8 g_Emission;			//发射率
    #if Func_Probecover
        extern uint8 g_PcRatio;				//耳套修正
    #endif
#endif
extern uint8 g_CheckSum;			//CRC校验位

extern uint8 g_Earcap10;		//变温增加
extern uint8 g_Earcap15;		//变温增加
extern uint8 g_Earcap20;		//变温增加
extern uint8 g_Earcap25;		//变温增加
extern uint8 g_Earcap30;		//变温增加
extern uint8 g_Earcap35;		//变温增加
extern uint8 g_Earcap40;		//变温增加

void Param_Init(void);
void Probecover_Param_Init(void);
void Param_Check(void);
void Parm_AutoCheck(void);
void Param_Calistore(void);
#endif