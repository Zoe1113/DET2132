/**************************************************************************
	Filename:	I2C.h
	Description:The header file of I2C.c
**************************************************************************/
#ifndef _Drv_I2C_HW_H
#define _Drv_I2C_HW_H

// void I2C_slaveInit( void );
void I2C_masterInit(void);
// void I2C_Start(void);
// void I2C_RespeatStart(void);
// void I2C_Stop(void);
// void I2C_ACK(void);
void I2C_Wait_ACK(void);
void I2C_No_ACK(void);
void I2C_Byte_W(uint8 R_E2Address, uint8 R_I2C_TXDATA);
void I2C_Page_W(uint8 R_E2Address, uint8 Num, uint8 *pd);
uint8 I2C_CurAddr_R(void);
uint8 I2C_Random_R(uint8 R_E2Address);
void I2C_Sequen_R(uint8 Num);
void I2C_Disable(void);
// void I2C_Enable(void);

#endif
/*************************************************************************/