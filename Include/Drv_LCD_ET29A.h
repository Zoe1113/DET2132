#ifndef _Drv_LCD_ET29_H
#define _Drv_LCD_ET29_H

//底层驱动
sfr	lcd0 = 0xf00 ;
sfr	lcd1 = 0xf01 ;
sfr	lcd2 = 0xf02 ;
sfr	lcd3 = 0xf03 ;
sfr	lcd4 = 0xf04 ;
sfr	lcd5 = 0xf05 ;
sfr	lcd6 = 0xf06 ;
sfr	lcd7 = 0xf07 ;
sfr	lcd8 = 0xf08 ;
sfr	lcd9 = 0xf09 ;
sfr	lcd10 = 0xf0a ;
sfr	lcd11 = 0xf0b ;
sfr	lcd12 = 0xf0c ;
sfr	lcd13 = 0xf0d ;
sfr	lcd14 = 0xf0e ;
sfr	lcd15 = 0xf0f ;
sfr lcd16 = 0xf10 ;
sfr lcd17 = 0xf11 ;

#define	SA	2048	    //0000000000000001B
#define	SB	1024		//0000000000000010B
#define	SC	512		    //0000000000000100B
#define	SD	256		    //0000000000001000B
#define	SE	2		    //0000010000000000B
#define	SF	8			//0000000100000000B
#define	SG	4			//0000001000000000B
#define	P	1			//0000100000000000B

#define S_A (SA+SB+SC+SE+SF+SG)
#define S_C (SA+SF+SE+SD)
#define S_c (SG+SE+Sd)
#define S_d (SB+SC+SD+SE+SG)
#define S_E (SA+SF+SE+SG+SD)
#define S_F (SA+SF+SE+SG)
#define S_H (SB+SF+SE+SG+SC)
#define S_r (SE+SG)
#define S_T (SE+SG+SF+SD)
#define S_P (SE+SG+SF+SA+SB)
#define S_S (SA+SF+SG+SC+SD)
#define S_b (SE+SF+SG+SC+SD)
#define S_i (SE)
#define S_L (SE+SF+SD)
#define S_o (SE+SG+SD+SC)
#define S_O (SA+SB+SC+SD+SE+SF)
#define S_U (SE+SF+SD+SC+SB)
#define S_J (SD+SC+SB)
#define S_n (SG+SC+SE)


//常用lcd符号宏定义
#define lcd_mem         0x01    //lcd mem sign
#define lcd_bat         0x02    //lcd low battary sign
#define lcd_bat_full    0x01    //满电压标志
#define lcd_ble         0x08    //lcd probe cover sign
// #define lcd_colon       0x01    //lcd : sign
#define lcd_dash        0x04    //-
// #define lcd_am          0x01    //AM
// #define lcd_pm          0x01    //PM
// #define lcd_timedash    0x01    //time - sign
#define lcd_point       0x01    //温度值的小数点
#define lcd_badface     0x01    //哭脸
#define lcd_smileface   0x02    //笑脸
#define lcd_ear         0x04    //耳温符号
#define lcd_forehead    0x08    //额温符号
//#define lcd_obj         0x02    //物温符号
#define lcd_obj         0x04    //物温符号
//#define lcd_ntc         0x04    //环温符号
#define lcd_ntc         0x01    //环温符号
#define lcd_voice       0x04    //声音符号
#define lcd_sound_off   0x02    //静音符号
#define lcd_unit_c      0x02    //单位C的_
#define lcd_unit_f      0x08    //单位F的-
#define lcd_unit_cf     0x04    //单位C和F的公共显示符号
#define lcd_pc          0x01    //耳套符号
//#define lcd_clean       0x01    //清洁符号

// #define lcd_age0 0x04          //0-36月图标
// #define lcd_age36 0x08          //36+图标

#define lcd_age0 0x08          //0-3月图标
#define lcd_age3 0x04          //3-36月图标
#define lcd_age36 0x02          //36+图标

//显示指定lcd图标
#define lcd_mem_en()        { lcd3 |= lcd_mem; }	    //M点亮
#define lcd_bat_en()        { lcd0 |= lcd_bat; }
#define lcd_bat_full_en()   { lcd0 |= lcd_bat_full; }   //满电压符号点亮 2022/11/09
#define lcd_ble_en()        { lcd1 |= lcd_ble; }
// #define lcd_colon_en()      { lcd5 |= lcd_colon; }      //时间:
// #define lcd_am_en()         { lcd7 |= lcd_am; }         //AM
// #define lcd_pm_en()         { lcd9 |= lcd_pm; }         //PM
// #define lcd_timedash_en()   { lcd3 |= lcd_timedash; }  //time - sign
#define lcd_point_en()      { lcd5 |= lcd_point; }         
#define lcd_badface_en()    { lcd10 |= lcd_badface; }
#define lcd_smileface_en()  { lcd10 |= lcd_smileface; }
#define lcd_ear_en()        { lcd10 |= lcd_ear; }
#define lcd_forehead_en()   { lcd10 |= lcd_forehead; }
//#define lcd_obj_en()        { lcd2 |= lcd_obj; }
#define lcd_obj_en()        { lcd0 |= lcd_obj; }
//#define lcd_ntc_en()        { lcd0 |= lcd_ntc; }
#define lcd_ntc_en()        { lcd1 |= lcd_ntc; }
#define lcd_voice_en()      { lcd1 |= lcd_voice; }
#define lcd_sound_off_en()  { lcd1 |= lcd_sound_off;}
#define lcd_unit_c_en()     { lcd3 |= lcd_unit_c; }
#define lcd_unit_f_en()     { lcd3 |= lcd_unit_f; }
#define lcd_unit_cf_en()    { lcd3 |= lcd_unit_cf; }
#define lcd_pc_en()         { lcd7 |= lcd_pc; } 
//#define lcd_clean_en()      { lcd1 |= lcd_clean; }  

// #define lcd_age0_en()       {lcd2 |= lcd_age0;}     //显示0-6月图标
// #define lcd_age36_en()      {lcd2 |= lcd_age36;}     //显示36+月图标

#define lcd_age0_en()       {lcd2 |= lcd_age0;}     //显示0-3月图标
#define lcd_age3_en()       {lcd2 |= lcd_age3;}     //显示3-36月图标
#define lcd_age36_en()      {lcd2 |= lcd_age36;}     //显示36+月图标


//清除指定lcd图标
#define lcd_mem_clr() 	    { lcd3 &= ~lcd_mem; }	//M点亮
#define lcd_bat_clr()       { lcd0 &= ~lcd_bat; }
#define lcd_bat_full_clr()  { lcd0 &= ~lcd_bat_full; }    		//满电压符号关闭
#define lcd_ble_clr()       { lcd1 &= ~lcd_ble; }
// #define lcd_colon_clr()     { lcd5 &= ~lcd_colon; }
// #define lcd_am_clr()        { lcd7 &= ~lcd_am; }   //AM
// #define lcd_pm_clr()        { lcd9 &= ~lcd_pm; }   //PM
// #define lcd_timedash_clr()  { lcd3 &= ~lcd_timedash; }   //time - sign
#define lcd_point_clr()     { lcd5 &= ~lcd_point; }
#define lcd_badface_clr()   { lcd10 &= ~lcd_badface; }
#define lcd_smileface_clr() { lcd10 &= ~lcd_smileface; }
#define lcd_ear_clr()       { lcd10 &= ~lcd_ear; }
#define lcd_forehead_clr()  { lcd10 &= ~lcd_forehead; }
#define lcd_obj_clr()       { lcd0 &= ~lcd_obj; }
// #define lcd_ntc_clr()       { lcd0 &= ~lcd_ntc; }
#define lcd_ntc_clr()       { lcd1 &= ~lcd_ntc; }
#define lcd_voice_clr()     { lcd1 &= ~lcd_voice; }
#define lcd_sound_off_clr() { lcd1 &= ~lcd_sound_off; }
#define lcd_unit_c_clr()    { lcd3 &= ~lcd_unit_c; }
#define lcd_unit_f_clr()    { lcd3 &= ~lcd_unit_f; }
#define lcd_unit_cf_clr()   { lcd3 &= ~lcd_unit_cf; }
#define lcd_pc_clr()        { lcd7 &= ~lcd_pc; }          
//#define lcd_clean_clr()     { lcd1 &= ~lcd_clean; }  

// #define lcd_age0_clr()      {lcd2 &= ~lcd_age0;}       //清除0-36月图标
// #define lcd_age36_clr()     {lcd2 &= ~lcd_age36;}       //清除36+月图标

#define lcd_age0_clr()      {lcd2 &= ~lcd_age0;}       //清除0-3月图标
#define lcd_age3_clr()      {lcd2 &= ~lcd_age3;}       //清除3-36月图标
#define lcd_age36_clr()     {lcd2 &= ~lcd_age36;}       //清除36+月图标

//消隐指定lcd图标
// #define lcd_colon_xor()     { lcd5 ^= lcd_colon; }
#define lcd_ble_xor()       { lcd1 ^= lcd_ble; }
#define lcd_bat_xor()       { lcd0 ^= lcd_bat; }
#define lcd_clean_xor()     { lcd1 ^= lcd_clean; }



extern uint16 __ROM	DispTable[];
extern int16 High_Precision;
extern uint8 High_Precision_Flag;
extern bit FrameFlag;

void Lcd_Init(void);
void Disp_Version(uint16 num);
void Disp_Code(uint16 num);
void Clr_Disp(void);
void Clr_Disp888(void);
void Disp_All(void);
void Disp_Unit(void);
void Disp_BadFace(void);
void Disp_SmileFace(void);
void Disp_ModeSign(void);
void Clr_ModeSign(void);
void Disp_OFF(void);
// void Disp_On(void);
void Disp_Lo(void);
void Disp_Hi(void);
void Disp_Ready(void);
void Disp_VoiceSign(void);
void Disp_Null(void);
void Disp_ErN(uint8 num);
void Disp_ErrMsg(void);
void Disp_NtcEr2(void);
void Disp_CAL(void);
void Disp_Ab(void);
void Disp_PAS(void);
void Disp_Err(void);
void Disp_Debug1(void);
void Disp_Debug2(void);
// void Disp_DebugPASn(uint8 num);
void Disp_12H(void);
void Disp_24H(void);
void Disp_Ch(void);
void Disp_En(void);
void Disp_Table1(void);
void Disp_Table2(void);
void Disp_Temp(bit Point, bit High, bit Unit, int16 Temp);
void Disp_High_Precision(uint8 Unit, int16 Temp);
// void Disp_Ntc(uint16 Temp);
void Disp_Time( bit F_time, bit F_Format, uint8 left, uint8 right);
void Disp_Year(uint16 L_buf);
void Clr_SetTime(uint8 L_Blink);
void LVD_Display(void);
void Disp_Age_Select(uint8 num);
void Clr_Age_Select(void);
// void Disp_CAP(void);

#endif
/*************************************************************************/