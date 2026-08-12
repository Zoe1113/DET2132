#ifndef _App_TestMode_H
#define _App_TestMode_H

//测量模式
typedef enum
{
	Earmode = 0,	//耳温模式
	Foreheadmode,	//额温模式
	Objectmode,		//物温模式
	Blackbodymode,	//黑体模式
	Insptectmode,	//生产检验模式
	Airmode			//环温模式
}eTestmode;

extern eTestmode eTestmode_num;
extern bit F_Disp_Dash;

void App_TestingMode(void);
void TestTask_Reset(void);

#endif