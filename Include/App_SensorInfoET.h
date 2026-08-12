#ifndef _App_SensorInfoET_H
#define _App_SensorInfoET_H

/****************************************传感器参数RAM****************************************/
extern const int16 g_Table37C;				    //37℃校准理论值
extern const int16 g_Table41C;				    //41℃校准理论值
extern const int16 g_TpTableDown;				//TP表格下限
extern const int16 g_TpTableUp;					//TP表格上限
extern const uint16 g_NtcTableDown1;	    //10℃NTC表格值
extern const uint16 g_NtcTableUp1;		//40℃NTC表格值
extern const uint16 g_NtcTableDown2;		//0℃NTC表格值
extern const uint16 g_NtcTableUp2;		//50℃NTC表格值

void S_SensorInitial_J11(void);
void BlackBodyOffset(void);
void NtcTable_Find(void);
void Tp_Caculate(void);
int16 TpCount_Relative_25C(void);
void TpTable_Find(void);
void Ntc_Caculate(void);
void Disp_Sensor_Name(void);
void pcchange(void);


#endif
