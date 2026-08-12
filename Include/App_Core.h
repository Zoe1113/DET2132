#ifndef _App_Core_H
#define _App_Core_H



#if ET_FT ==1                       /***********************/ //额温

#define Forehead_Num 32 //额温采样个数
#else               /***********************/ //耳温或者耳额温

#define Ear_Num 32		//耳温采样个数
#define Forehead_Num 32 //额温采样个数
#define Up_Val 4     	//上次判定次数：4次
#define Down_Val 3 		//下降判定次数：3次
#define Up_Diff_Val 5	//上升判定阈值：前后差值大于5
#define Down_Diff_Val 1	//下降判定阈值：前后差值小于1
extern int16 g_TpCount_avg;
extern int16 g_TpCount_Max;
extern bit F_First_Enter_EarTp;
extern int16 Ear_array[Ear_Num]; 	//耳温Tp数据表格
void Get_Ear_Tp_Max(int16 L_buf);
int16 Get_Ear_Tp_Avg(uint8 length);
int16 TpCount_Determine(void);
uint16 NtcCount_Determine(void);
uint8 Probe_Move(int16 *ptr, uint8 length);
#endif 

//额温和耳温共同定义
extern bit F_First_Enter_ForeheadTp;
extern int16 Forehead_array[Forehead_Num];
void Get_Forehead_Tp( int16 L_buf );
int16 Get_Forehead_Tp_Max_Avg(uint8 length);
void Backup_Testing_Param(void);
uint8 Adc_Filter( int16 *ptr, uint8 length );
uint8 Remove_Tp_Max_Min( int16 *ptr, uint8 length );
#endif
