#ifndef _Drv_LED_H
#define _Drv_LED_H


#define LED_Green_En() { FP56 = 0; }    //开启绿色背光
#define LED_Green_Dis() { FP56 = 1; }   //关闭绿色背光

#define LED_Red_En() { FP57 = 0; }      //开启红色背光
#define LED_Red_Dis() { FP57 = 1; }     //关闭红色背光

#define LED_Yellow_En() { FP55 = 0; }   //开启黄色背光
#define LED_Yellow_Dis() { FP55 = 1; }  //关闭黄色背光

#define LED_White_En() { FP10 = 0; }   //开启白色背光
#define LED_White_Dis() { FP10 = 1; }  //关闭白色背光


void LED_CloseAll(void);


#endif