# SONIX DSP Makefile Version:1.0.0.2
# Defination
COMPILER = "d:\software\SN8_C_~1\data\bin\build\sn8pc.exe"
GMA = "D:\software\SN8_C_~1\data\Bin\Build\gma.exe"
LINKER = "D:\software\SN8_C_~1\data\Bin\Build\slink.exe"
CONVERT = "D:\software\SN8_C_~1\data\Bin\Build\RcvSN8.exe"

# Define Path Macro
PRJ_PATH=.
OBJ_PATH=.\Obj
INC_PATH=.\Include
LIB_PATH=.\Lib
C_INC_PATH=D:\software\SN8_C_~1\data\C\include
OUT_PATH=.\Bin
VPATH=.\:$(OBJ_PATH):$(LIB_PATH):$(INC_PATH):$(C_INC_PATH):$(OUT_PATH)

# Define Option Flags
COMPILER_FLAGS = -target=SN8P2988 -INI="D:\software\SN8_C_~1\data\Bin\Build\SN8P1900.ini" -PROJECTNAME=".\ETTest.prj" -WL=3 -Chip_Series=2 -A -g -I.\Include -ID:\software\SN8_C_~1\data\C\include -PUSH0 -NoCALLHL -NoX -NoGlobalOpt -tempdir="$(OBJ_PATH)"  -cpp_skip_asm -DICE_Mode=0
GMA_FLAGS = /INI:"SN8P1900.ini" /ID1:0 /ID2:0  /MACHINE:SN8P2988  /Chip_Series:2 /NOPeephole: /PATH:".\Include" /PROJECTNAME:".\ETTest.prj" /WL:3 /DEFINE:ICE_Mode=0 /DEFINE:SN8P2988=1  /OutputPath:"$(OBJ_PATH)" 
LINK_FLAGS = /MACHINE:SN8P2988 /Chip_Series:2 /INI:"SN8P1900.ini" /WL:3 /OutputFile:"$(OUT_PATH)/ETTest.out"  /MAP:"$(OBJ_PATH)\ETTest.map"   /LISTFILE:  /STDLIB:".\Obj"    /PATH:"$(LIB_PATH)" /STACK: /EnableCheckStackLevel /PROJECTNAME:".\ETTest.prj"
CONV_FLAGS = /MACHINE:SN8P2988 /INI:"SN8P1900.ini" /WL:3 /OutputFile:"$(OUT_PATH)/ETTest.sn8"  /PROJECTNAME:".\ETTest.prj" /IDSVersion:V1.20.219.376n /Chip_Series:2 /OSLIB:".\Obj"  
LINK_DEP_FILES = $(OBJ_PATH)\App_Param.o $(OBJ_PATH)\App_Function.o $(OBJ_PATH)\Drv_ADC.o $(OBJ_PATH)\Drv_Buzzer.o $(OBJ_PATH)\Drv_Delay.o $(OBJ_PATH)\Drv_I2C_HW.o $(OBJ_PATH)\Drv_Key.o $(OBJ_PATH)\Drv_LVD.o $(OBJ_PATH)\Drv_LED.o $(OBJ_PATH)\Drv_Timer.o $(OBJ_PATH)\Drv_Uart.o $(OBJ_PATH)\Variable.o $(OBJ_PATH)\Drv_GPIO.o $(OBJ_PATH)\Main.o $(OBJ_PATH)\App_Memory.o $(OBJ_PATH)\APP_Isr.o $(OBJ_PATH)\App_CaliMode.o $(OBJ_PATH)\App_ReadyMode.o $(OBJ_PATH)\App_TestMode.o $(OBJ_PATH)\App_InitMode.o $(OBJ_PATH)\App_SetMode.o $(OBJ_PATH)\App_Core.o $(OBJ_PATH)\App_Sleep.o $(OBJ_PATH)\App_BondTestMode.o $(OBJ_PATH)\App_Voice.o $(OBJ_PATH)\App_Time.o $(OBJ_PATH)\App_FTOffset215.o $(OBJ_PATH)\App_BleMode06.o $(OBJ_PATH)\App_SensorInfoET.o $(OBJ_PATH)\Drv_LCD_ET29A.o   \

# Define Rule
$(OBJ_PATH)\APP_Isr.asm: .\Src\APP_Isr.c $(C_INC_PATH)\SN8P2988.h $(PRJ_PATH)\ETTest.cfg .\Include\Include.h .\Include\SN8P2988.h .\Include\Variable.h .\Include\Main.h .\Include\App_Core.h .\Include\App_BleMode06.h .\Include\App_BondTestMode.h .\Include\App_ReadyMode.h .\Include\App_InitMode.h .\Include\App_TestMode.h .\Include\App_CaliMode.h .\Include\App_SetMode.h .\Include\App_Memory.h .\Include\App_Sleep.h .\Include\App_Function.h .\Include\App_FTOffset215.h .\Include\App_SensorInfoET.h .\Include\App_Time.h .\Include\App_Param.h .\Include\App_Voice.h .\Include\Drv_ADC.h .\Include\Drv_Delay.h .\Include\Drv_I2C_HW.h .\Include\Drv_Timer.h .\Include\Drv_Uart.h .\Include\Drv_Buzzer.h .\Include\Drv_Key.h .\Include\Drv_LCD_ET29A.h .\Include\Drv_LED.h .\Include\Drv_GPIO.h .\Include\Drv_LVD.h
	@$(COMPILER) $(COMPILER_FLAGS) -o $@ $^ 

$(OBJ_PATH)\App_BleMode06.asm: .\Src\App_BleMode06.c $(C_INC_PATH)\SN8P2988.h $(PRJ_PATH)\ETTest.cfg .\Include\Include.h .\Include\SN8P2988.h .\Include\Variable.h .\Include\Main.h .\Include\App_Core.h .\Include\App_BleMode06.h .\Include\App_BondTestMode.h .\Include\App_ReadyMode.h .\Include\App_InitMode.h .\Include\App_TestMode.h .\Include\App_CaliMode.h .\Include\App_SetMode.h .\Include\App_Memory.h .\Include\App_Sleep.h .\Include\App_Function.h .\Include\App_FTOffset215.h .\Include\App_SensorInfoET.h .\Include\App_Time.h .\Include\App_Param.h .\Include\App_Voice.h .\Include\Drv_ADC.h .\Include\Drv_Delay.h .\Include\Drv_I2C_HW.h .\Include\Drv_Timer.h .\Include\Drv_Uart.h .\Include\Drv_Buzzer.h .\Include\Drv_Key.h .\Include\Drv_LCD_ET29A.h .\Include\Drv_LED.h .\Include\Drv_GPIO.h .\Include\Drv_LVD.h
	@$(COMPILER) $(COMPILER_FLAGS) -o $@ $^ 

$(OBJ_PATH)\App_BondTestMode.asm: .\Src\App_BondTestMode.c $(C_INC_PATH)\SN8P2988.h $(PRJ_PATH)\ETTest.cfg .\Include\Include.h .\Include\SN8P2988.h .\Include\Variable.h .\Include\Main.h .\Include\App_Core.h .\Include\App_BleMode06.h .\Include\App_BondTestMode.h .\Include\App_ReadyMode.h .\Include\App_InitMode.h .\Include\App_TestMode.h .\Include\App_CaliMode.h .\Include\App_SetMode.h .\Include\App_Memory.h .\Include\App_Sleep.h .\Include\App_Function.h .\Include\App_FTOffset215.h .\Include\App_SensorInfoET.h .\Include\App_Time.h .\Include\App_Param.h .\Include\App_Voice.h .\Include\Drv_ADC.h .\Include\Drv_Delay.h .\Include\Drv_I2C_HW.h .\Include\Drv_Timer.h .\Include\Drv_Uart.h .\Include\Drv_Buzzer.h .\Include\Drv_Key.h .\Include\Drv_LCD_ET29A.h .\Include\Drv_LED.h .\Include\Drv_GPIO.h .\Include\Drv_LVD.h
	@$(COMPILER) $(COMPILER_FLAGS) -o $@ $^ 

$(OBJ_PATH)\App_CaliMode.asm: .\Src\App_CaliMode.c $(C_INC_PATH)\SN8P2988.h $(PRJ_PATH)\ETTest.cfg .\Include\Include.h .\Include\SN8P2988.h .\Include\Variable.h .\Include\Main.h .\Include\App_Core.h .\Include\App_BleMode06.h .\Include\App_BondTestMode.h .\Include\App_ReadyMode.h .\Include\App_InitMode.h .\Include\App_TestMode.h .\Include\App_CaliMode.h .\Include\App_SetMode.h .\Include\App_Memory.h .\Include\App_Sleep.h .\Include\App_Function.h .\Include\App_FTOffset215.h .\Include\App_SensorInfoET.h .\Include\App_Time.h .\Include\App_Param.h .\Include\App_Voice.h .\Include\Drv_ADC.h .\Include\Drv_Delay.h .\Include\Drv_I2C_HW.h .\Include\Drv_Timer.h .\Include\Drv_Uart.h .\Include\Drv_Buzzer.h .\Include\Drv_Key.h .\Include\Drv_LCD_ET29A.h .\Include\Drv_LED.h .\Include\Drv_GPIO.h .\Include\Drv_LVD.h
	@$(COMPILER) $(COMPILER_FLAGS) -o $@ $^ 

$(OBJ_PATH)\App_Core.asm: .\Src\App_Core.c $(C_INC_PATH)\SN8P2988.h $(PRJ_PATH)\ETTest.cfg .\Include\Include.h .\Include\SN8P2988.h .\Include\Variable.h .\Include\Main.h .\Include\App_Core.h .\Include\App_BleMode06.h .\Include\App_BondTestMode.h .\Include\App_ReadyMode.h .\Include\App_InitMode.h .\Include\App_TestMode.h .\Include\App_CaliMode.h .\Include\App_SetMode.h .\Include\App_Memory.h .\Include\App_Sleep.h .\Include\App_Function.h .\Include\App_FTOffset215.h .\Include\App_SensorInfoET.h .\Include\App_Time.h .\Include\App_Param.h .\Include\App_Voice.h .\Include\Drv_ADC.h .\Include\Drv_Delay.h .\Include\Drv_I2C_HW.h .\Include\Drv_Timer.h .\Include\Drv_Uart.h .\Include\Drv_Buzzer.h .\Include\Drv_Key.h .\Include\Drv_LCD_ET29A.h .\Include\Drv_LED.h .\Include\Drv_GPIO.h .\Include\Drv_LVD.h
	@$(COMPILER) $(COMPILER_FLAGS) -o $@ $^ 

$(OBJ_PATH)\App_FTOffset215.asm: .\Src\App_FTOffset215.c $(C_INC_PATH)\SN8P2988.h $(PRJ_PATH)\ETTest.cfg .\Include\Include.h .\Include\SN8P2988.h .\Include\Variable.h .\Include\Main.h .\Include\App_Core.h .\Include\App_BleMode06.h .\Include\App_BondTestMode.h .\Include\App_ReadyMode.h .\Include\App_InitMode.h .\Include\App_TestMode.h .\Include\App_CaliMode.h .\Include\App_SetMode.h .\Include\App_Memory.h .\Include\App_Sleep.h .\Include\App_Function.h .\Include\App_FTOffset215.h .\Include\App_SensorInfoET.h .\Include\App_Time.h .\Include\App_Param.h .\Include\App_Voice.h .\Include\Drv_ADC.h .\Include\Drv_Delay.h .\Include\Drv_I2C_HW.h .\Include\Drv_Timer.h .\Include\Drv_Uart.h .\Include\Drv_Buzzer.h .\Include\Drv_Key.h .\Include\Drv_LCD_ET29A.h .\Include\Drv_LED.h .\Include\Drv_GPIO.h .\Include\Drv_LVD.h
	@$(COMPILER) $(COMPILER_FLAGS) -o $@ $^ 

$(OBJ_PATH)\App_Function.asm: .\Src\App_Function.c $(C_INC_PATH)\SN8P2988.h $(PRJ_PATH)\ETTest.cfg .\Include\Include.h .\Include\SN8P2988.h .\Include\Variable.h .\Include\Main.h .\Include\App_Core.h .\Include\App_BleMode06.h .\Include\App_BondTestMode.h .\Include\App_ReadyMode.h .\Include\App_InitMode.h .\Include\App_TestMode.h .\Include\App_CaliMode.h .\Include\App_SetMode.h .\Include\App_Memory.h .\Include\App_Sleep.h .\Include\App_Function.h .\Include\App_FTOffset215.h .\Include\App_SensorInfoET.h .\Include\App_Time.h .\Include\App_Param.h .\Include\App_Voice.h .\Include\Drv_ADC.h .\Include\Drv_Delay.h .\Include\Drv_I2C_HW.h .\Include\Drv_Timer.h .\Include\Drv_Uart.h .\Include\Drv_Buzzer.h .\Include\Drv_Key.h .\Include\Drv_LCD_ET29A.h .\Include\Drv_LED.h .\Include\Drv_GPIO.h .\Include\Drv_LVD.h
	@$(COMPILER) $(COMPILER_FLAGS) -o $@ $^ 

$(OBJ_PATH)\App_InitMode.asm: .\Src\App_InitMode.c $(C_INC_PATH)\SN8P2988.h $(PRJ_PATH)\ETTest.cfg .\Include\Include.h .\Include\SN8P2988.h .\Include\Variable.h .\Include\Main.h .\Include\App_Core.h .\Include\App_BleMode06.h .\Include\App_BondTestMode.h .\Include\App_ReadyMode.h .\Include\App_InitMode.h .\Include\App_TestMode.h .\Include\App_CaliMode.h .\Include\App_SetMode.h .\Include\App_Memory.h .\Include\App_Sleep.h .\Include\App_Function.h .\Include\App_FTOffset215.h .\Include\App_SensorInfoET.h .\Include\App_Time.h .\Include\App_Param.h .\Include\App_Voice.h .\Include\Drv_ADC.h .\Include\Drv_Delay.h .\Include\Drv_I2C_HW.h .\Include\Drv_Timer.h .\Include\Drv_Uart.h .\Include\Drv_Buzzer.h .\Include\Drv_Key.h .\Include\Drv_LCD_ET29A.h .\Include\Drv_LED.h .\Include\Drv_GPIO.h .\Include\Drv_LVD.h
	@$(COMPILER) $(COMPILER_FLAGS) -o $@ $^ 

$(OBJ_PATH)\App_Memory.asm: .\Src\App_Memory.c $(C_INC_PATH)\SN8P2988.h $(PRJ_PATH)\ETTest.cfg .\Include\Include.h .\Include\SN8P2988.h .\Include\Variable.h .\Include\Main.h .\Include\App_Core.h .\Include\App_BleMode06.h .\Include\App_BondTestMode.h .\Include\App_ReadyMode.h .\Include\App_InitMode.h .\Include\App_TestMode.h .\Include\App_CaliMode.h .\Include\App_SetMode.h .\Include\App_Memory.h .\Include\App_Sleep.h .\Include\App_Function.h .\Include\App_FTOffset215.h .\Include\App_SensorInfoET.h .\Include\App_Time.h .\Include\App_Param.h .\Include\App_Voice.h .\Include\Drv_ADC.h .\Include\Drv_Delay.h .\Include\Drv_I2C_HW.h .\Include\Drv_Timer.h .\Include\Drv_Uart.h .\Include\Drv_Buzzer.h .\Include\Drv_Key.h .\Include\Drv_LCD_ET29A.h .\Include\Drv_LED.h .\Include\Drv_GPIO.h .\Include\Drv_LVD.h
	@$(COMPILER) $(COMPILER_FLAGS) -o $@ $^ 

$(OBJ_PATH)\App_Param.asm: .\Src\App_Param.c $(C_INC_PATH)\SN8P2988.h $(PRJ_PATH)\ETTest.cfg .\Include\Include.h .\Include\SN8P2988.h .\Include\Variable.h .\Include\Main.h .\Include\App_Core.h .\Include\App_BleMode06.h .\Include\App_BondTestMode.h .\Include\App_ReadyMode.h .\Include\App_InitMode.h .\Include\App_TestMode.h .\Include\App_CaliMode.h .\Include\App_SetMode.h .\Include\App_Memory.h .\Include\App_Sleep.h .\Include\App_Function.h .\Include\App_FTOffset215.h .\Include\App_SensorInfoET.h .\Include\App_Time.h .\Include\App_Param.h .\Include\App_Voice.h .\Include\Drv_ADC.h .\Include\Drv_Delay.h .\Include\Drv_I2C_HW.h .\Include\Drv_Timer.h .\Include\Drv_Uart.h .\Include\Drv_Buzzer.h .\Include\Drv_Key.h .\Include\Drv_LCD_ET29A.h .\Include\Drv_LED.h .\Include\Drv_GPIO.h .\Include\Drv_LVD.h
	@$(COMPILER) $(COMPILER_FLAGS) -o $@ $^ 

$(OBJ_PATH)\App_ReadyMode.asm: .\Src\App_ReadyMode.c $(C_INC_PATH)\SN8P2988.h $(PRJ_PATH)\ETTest.cfg .\Include\Include.h .\Include\SN8P2988.h .\Include\Variable.h .\Include\Main.h .\Include\App_Core.h .\Include\App_BleMode06.h .\Include\App_BondTestMode.h .\Include\App_ReadyMode.h .\Include\App_InitMode.h .\Include\App_TestMode.h .\Include\App_CaliMode.h .\Include\App_SetMode.h .\Include\App_Memory.h .\Include\App_Sleep.h .\Include\App_Function.h .\Include\App_FTOffset215.h .\Include\App_SensorInfoET.h .\Include\App_Time.h .\Include\App_Param.h .\Include\App_Voice.h .\Include\Drv_ADC.h .\Include\Drv_Delay.h .\Include\Drv_I2C_HW.h .\Include\Drv_Timer.h .\Include\Drv_Uart.h .\Include\Drv_Buzzer.h .\Include\Drv_Key.h .\Include\Drv_LCD_ET29A.h .\Include\Drv_LED.h .\Include\Drv_GPIO.h .\Include\Drv_LVD.h
	@$(COMPILER) $(COMPILER_FLAGS) -o $@ $^ 

$(OBJ_PATH)\App_SensorInfoET.asm: .\Src\App_SensorInfoET.c $(C_INC_PATH)\SN8P2988.h $(PRJ_PATH)\ETTest.cfg .\Include\Include.h .\Include\SN8P2988.h .\Include\Variable.h .\Include\Main.h .\Include\App_Core.h .\Include\App_BleMode06.h .\Include\App_BondTestMode.h .\Include\App_ReadyMode.h .\Include\App_InitMode.h .\Include\App_TestMode.h .\Include\App_CaliMode.h .\Include\App_SetMode.h .\Include\App_Memory.h .\Include\App_Sleep.h .\Include\App_Function.h .\Include\App_FTOffset215.h .\Include\App_SensorInfoET.h .\Include\App_Time.h .\Include\App_Param.h .\Include\App_Voice.h .\Include\Drv_ADC.h .\Include\Drv_Delay.h .\Include\Drv_I2C_HW.h .\Include\Drv_Timer.h .\Include\Drv_Uart.h .\Include\Drv_Buzzer.h .\Include\Drv_Key.h .\Include\Drv_LCD_ET29A.h .\Include\Drv_LED.h .\Include\Drv_GPIO.h .\Include\Drv_LVD.h
	@$(COMPILER) $(COMPILER_FLAGS) -o $@ $^ 

$(OBJ_PATH)\App_SetMode.asm: .\Src\App_SetMode.c $(C_INC_PATH)\SN8P2988.h $(PRJ_PATH)\ETTest.cfg .\Include\Include.h .\Include\SN8P2988.h .\Include\Variable.h .\Include\Main.h .\Include\App_Core.h .\Include\App_BleMode06.h .\Include\App_BondTestMode.h .\Include\App_ReadyMode.h .\Include\App_InitMode.h .\Include\App_TestMode.h .\Include\App_CaliMode.h .\Include\App_SetMode.h .\Include\App_Memory.h .\Include\App_Sleep.h .\Include\App_Function.h .\Include\App_FTOffset215.h .\Include\App_SensorInfoET.h .\Include\App_Time.h .\Include\App_Param.h .\Include\App_Voice.h .\Include\Drv_ADC.h .\Include\Drv_Delay.h .\Include\Drv_I2C_HW.h .\Include\Drv_Timer.h .\Include\Drv_Uart.h .\Include\Drv_Buzzer.h .\Include\Drv_Key.h .\Include\Drv_LCD_ET29A.h .\Include\Drv_LED.h .\Include\Drv_GPIO.h .\Include\Drv_LVD.h
	@$(COMPILER) $(COMPILER_FLAGS) -o $@ $^ 

$(OBJ_PATH)\App_Sleep.asm: .\Src\App_Sleep.c $(C_INC_PATH)\SN8P2988.h $(PRJ_PATH)\ETTest.cfg .\Include\Include.h .\Include\SN8P2988.h .\Include\Variable.h .\Include\Main.h .\Include\App_Core.h .\Include\App_BleMode06.h .\Include\App_BondTestMode.h .\Include\App_ReadyMode.h .\Include\App_InitMode.h .\Include\App_TestMode.h .\Include\App_CaliMode.h .\Include\App_SetMode.h .\Include\App_Memory.h .\Include\App_Sleep.h .\Include\App_Function.h .\Include\App_FTOffset215.h .\Include\App_SensorInfoET.h .\Include\App_Time.h .\Include\App_Param.h .\Include\App_Voice.h .\Include\Drv_ADC.h .\Include\Drv_Delay.h .\Include\Drv_I2C_HW.h .\Include\Drv_Timer.h .\Include\Drv_Uart.h .\Include\Drv_Buzzer.h .\Include\Drv_Key.h .\Include\Drv_LCD_ET29A.h .\Include\Drv_LED.h .\Include\Drv_GPIO.h .\Include\Drv_LVD.h
	@$(COMPILER) $(COMPILER_FLAGS) -o $@ $^ 

$(OBJ_PATH)\App_TestMode.asm: .\Src\App_TestMode.c $(C_INC_PATH)\SN8P2988.h $(PRJ_PATH)\ETTest.cfg .\Include\Include.h .\Include\SN8P2988.h .\Include\Variable.h .\Include\Main.h .\Include\App_Core.h .\Include\App_BleMode06.h .\Include\App_BondTestMode.h .\Include\App_ReadyMode.h .\Include\App_InitMode.h .\Include\App_TestMode.h .\Include\App_CaliMode.h .\Include\App_SetMode.h .\Include\App_Memory.h .\Include\App_Sleep.h .\Include\App_Function.h .\Include\App_FTOffset215.h .\Include\App_SensorInfoET.h .\Include\App_Time.h .\Include\App_Param.h .\Include\App_Voice.h .\Include\Drv_ADC.h .\Include\Drv_Delay.h .\Include\Drv_I2C_HW.h .\Include\Drv_Timer.h .\Include\Drv_Uart.h .\Include\Drv_Buzzer.h .\Include\Drv_Key.h .\Include\Drv_LCD_ET29A.h .\Include\Drv_LED.h .\Include\Drv_GPIO.h .\Include\Drv_LVD.h
	@$(COMPILER) $(COMPILER_FLAGS) -o $@ $^ 

$(OBJ_PATH)\App_Time.asm: .\Src\App_Time.c $(C_INC_PATH)\SN8P2988.h $(PRJ_PATH)\ETTest.cfg .\Include\Include.h .\Include\SN8P2988.h .\Include\Variable.h .\Include\Main.h .\Include\App_Core.h .\Include\App_BleMode06.h .\Include\App_BondTestMode.h .\Include\App_ReadyMode.h .\Include\App_InitMode.h .\Include\App_TestMode.h .\Include\App_CaliMode.h .\Include\App_SetMode.h .\Include\App_Memory.h .\Include\App_Sleep.h .\Include\App_Function.h .\Include\App_FTOffset215.h .\Include\App_SensorInfoET.h .\Include\App_Time.h .\Include\App_Param.h .\Include\App_Voice.h .\Include\Drv_ADC.h .\Include\Drv_Delay.h .\Include\Drv_I2C_HW.h .\Include\Drv_Timer.h .\Include\Drv_Uart.h .\Include\Drv_Buzzer.h .\Include\Drv_Key.h .\Include\Drv_LCD_ET29A.h .\Include\Drv_LED.h .\Include\Drv_GPIO.h .\Include\Drv_LVD.h
	@$(COMPILER) $(COMPILER_FLAGS) -o $@ $^ 

$(OBJ_PATH)\App_Voice.asm: .\Src\App_Voice.c $(C_INC_PATH)\SN8P2988.h $(PRJ_PATH)\ETTest.cfg .\Include\Include.h .\Include\SN8P2988.h .\Include\Variable.h .\Include\Main.h .\Include\App_Core.h .\Include\App_BleMode06.h .\Include\App_BondTestMode.h .\Include\App_ReadyMode.h .\Include\App_InitMode.h .\Include\App_TestMode.h .\Include\App_CaliMode.h .\Include\App_SetMode.h .\Include\App_Memory.h .\Include\App_Sleep.h .\Include\App_Function.h .\Include\App_FTOffset215.h .\Include\App_SensorInfoET.h .\Include\App_Time.h .\Include\App_Param.h .\Include\App_Voice.h .\Include\Drv_ADC.h .\Include\Drv_Delay.h .\Include\Drv_I2C_HW.h .\Include\Drv_Timer.h .\Include\Drv_Uart.h .\Include\Drv_Buzzer.h .\Include\Drv_Key.h .\Include\Drv_LCD_ET29A.h .\Include\Drv_LED.h .\Include\Drv_GPIO.h .\Include\Drv_LVD.h
	@$(COMPILER) $(COMPILER_FLAGS) -o $@ $^ 

$(OBJ_PATH)\Drv_ADC.asm: .\Src\Drv_ADC.c $(C_INC_PATH)\SN8P2988.h $(PRJ_PATH)\ETTest.cfg .\Include\Include.h .\Include\SN8P2988.h .\Include\Variable.h .\Include\Main.h .\Include\App_Core.h .\Include\App_BleMode06.h .\Include\App_BondTestMode.h .\Include\App_ReadyMode.h .\Include\App_InitMode.h .\Include\App_TestMode.h .\Include\App_CaliMode.h .\Include\App_SetMode.h .\Include\App_Memory.h .\Include\App_Sleep.h .\Include\App_Function.h .\Include\App_FTOffset215.h .\Include\App_SensorInfoET.h .\Include\App_Time.h .\Include\App_Param.h .\Include\App_Voice.h .\Include\Drv_ADC.h .\Include\Drv_Delay.h .\Include\Drv_I2C_HW.h .\Include\Drv_Timer.h .\Include\Drv_Uart.h .\Include\Drv_Buzzer.h .\Include\Drv_Key.h .\Include\Drv_LCD_ET29A.h .\Include\Drv_LED.h .\Include\Drv_GPIO.h .\Include\Drv_LVD.h
	@$(COMPILER) $(COMPILER_FLAGS) -o $@ $^ 

$(OBJ_PATH)\Drv_Buzzer.asm: .\Src\Drv_Buzzer.c $(C_INC_PATH)\SN8P2988.h $(PRJ_PATH)\ETTest.cfg .\Include\Include.h .\Include\SN8P2988.h .\Include\Variable.h .\Include\Main.h .\Include\App_Core.h .\Include\App_BleMode06.h .\Include\App_BondTestMode.h .\Include\App_ReadyMode.h .\Include\App_InitMode.h .\Include\App_TestMode.h .\Include\App_CaliMode.h .\Include\App_SetMode.h .\Include\App_Memory.h .\Include\App_Sleep.h .\Include\App_Function.h .\Include\App_FTOffset215.h .\Include\App_SensorInfoET.h .\Include\App_Time.h .\Include\App_Param.h .\Include\App_Voice.h .\Include\Drv_ADC.h .\Include\Drv_Delay.h .\Include\Drv_I2C_HW.h .\Include\Drv_Timer.h .\Include\Drv_Uart.h .\Include\Drv_Buzzer.h .\Include\Drv_Key.h .\Include\Drv_LCD_ET29A.h .\Include\Drv_LED.h .\Include\Drv_GPIO.h .\Include\Drv_LVD.h
	@$(COMPILER) $(COMPILER_FLAGS) -o $@ $^ 

$(OBJ_PATH)\Drv_Delay.asm: .\Src\Drv_Delay.c $(C_INC_PATH)\SN8P2988.h $(PRJ_PATH)\ETTest.cfg .\Include\Include.h .\Include\SN8P2988.h .\Include\Variable.h .\Include\Main.h .\Include\App_Core.h .\Include\App_BleMode06.h .\Include\App_BondTestMode.h .\Include\App_ReadyMode.h .\Include\App_InitMode.h .\Include\App_TestMode.h .\Include\App_CaliMode.h .\Include\App_SetMode.h .\Include\App_Memory.h .\Include\App_Sleep.h .\Include\App_Function.h .\Include\App_FTOffset215.h .\Include\App_SensorInfoET.h .\Include\App_Time.h .\Include\App_Param.h .\Include\App_Voice.h .\Include\Drv_ADC.h .\Include\Drv_Delay.h .\Include\Drv_I2C_HW.h .\Include\Drv_Timer.h .\Include\Drv_Uart.h .\Include\Drv_Buzzer.h .\Include\Drv_Key.h .\Include\Drv_LCD_ET29A.h .\Include\Drv_LED.h .\Include\Drv_GPIO.h .\Include\Drv_LVD.h
	@$(COMPILER) $(COMPILER_FLAGS) -o $@ $^ 

$(OBJ_PATH)\Drv_GPIO.asm: .\Src\Drv_GPIO.c $(C_INC_PATH)\SN8P2988.h $(PRJ_PATH)\ETTest.cfg .\Include\Include.h .\Include\SN8P2988.h .\Include\Variable.h .\Include\Main.h .\Include\App_Core.h .\Include\App_BleMode06.h .\Include\App_BondTestMode.h .\Include\App_ReadyMode.h .\Include\App_InitMode.h .\Include\App_TestMode.h .\Include\App_CaliMode.h .\Include\App_SetMode.h .\Include\App_Memory.h .\Include\App_Sleep.h .\Include\App_Function.h .\Include\App_FTOffset215.h .\Include\App_SensorInfoET.h .\Include\App_Time.h .\Include\App_Param.h .\Include\App_Voice.h .\Include\Drv_ADC.h .\Include\Drv_Delay.h .\Include\Drv_I2C_HW.h .\Include\Drv_Timer.h .\Include\Drv_Uart.h .\Include\Drv_Buzzer.h .\Include\Drv_Key.h .\Include\Drv_LCD_ET29A.h .\Include\Drv_LED.h .\Include\Drv_GPIO.h .\Include\Drv_LVD.h
	@$(COMPILER) $(COMPILER_FLAGS) -o $@ $^ 

$(OBJ_PATH)\Drv_I2C_HW.asm: .\Src\Drv_I2C_HW.c $(C_INC_PATH)\SN8P2988.h $(PRJ_PATH)\ETTest.cfg .\Include\Include.h .\Include\SN8P2988.h .\Include\Variable.h .\Include\Main.h .\Include\App_Core.h .\Include\App_BleMode06.h .\Include\App_BondTestMode.h .\Include\App_ReadyMode.h .\Include\App_InitMode.h .\Include\App_TestMode.h .\Include\App_CaliMode.h .\Include\App_SetMode.h .\Include\App_Memory.h .\Include\App_Sleep.h .\Include\App_Function.h .\Include\App_FTOffset215.h .\Include\App_SensorInfoET.h .\Include\App_Time.h .\Include\App_Param.h .\Include\App_Voice.h .\Include\Drv_ADC.h .\Include\Drv_Delay.h .\Include\Drv_I2C_HW.h .\Include\Drv_Timer.h .\Include\Drv_Uart.h .\Include\Drv_Buzzer.h .\Include\Drv_Key.h .\Include\Drv_LCD_ET29A.h .\Include\Drv_LED.h .\Include\Drv_GPIO.h .\Include\Drv_LVD.h
	@$(COMPILER) $(COMPILER_FLAGS) -o $@ $^ 

$(OBJ_PATH)\Drv_Key.asm: .\Src\Drv_Key.c $(C_INC_PATH)\SN8P2988.h $(PRJ_PATH)\ETTest.cfg .\Include\Include.h .\Include\SN8P2988.h .\Include\Variable.h .\Include\Main.h .\Include\App_Core.h .\Include\App_BleMode06.h .\Include\App_BondTestMode.h .\Include\App_ReadyMode.h .\Include\App_InitMode.h .\Include\App_TestMode.h .\Include\App_CaliMode.h .\Include\App_SetMode.h .\Include\App_Memory.h .\Include\App_Sleep.h .\Include\App_Function.h .\Include\App_FTOffset215.h .\Include\App_SensorInfoET.h .\Include\App_Time.h .\Include\App_Param.h .\Include\App_Voice.h .\Include\Drv_ADC.h .\Include\Drv_Delay.h .\Include\Drv_I2C_HW.h .\Include\Drv_Timer.h .\Include\Drv_Uart.h .\Include\Drv_Buzzer.h .\Include\Drv_Key.h .\Include\Drv_LCD_ET29A.h .\Include\Drv_LED.h .\Include\Drv_GPIO.h .\Include\Drv_LVD.h
	@$(COMPILER) $(COMPILER_FLAGS) -o $@ $^ 

$(OBJ_PATH)\Drv_LCD_ET29A.asm: .\Src\Drv_LCD_ET29A.c $(C_INC_PATH)\SN8P2988.h $(PRJ_PATH)\ETTest.cfg .\Include\Include.h .\Include\SN8P2988.h .\Include\Variable.h .\Include\Main.h .\Include\App_Core.h .\Include\App_BleMode06.h .\Include\App_BondTestMode.h .\Include\App_ReadyMode.h .\Include\App_InitMode.h .\Include\App_TestMode.h .\Include\App_CaliMode.h .\Include\App_SetMode.h .\Include\App_Memory.h .\Include\App_Sleep.h .\Include\App_Function.h .\Include\App_FTOffset215.h .\Include\App_SensorInfoET.h .\Include\App_Time.h .\Include\App_Param.h .\Include\App_Voice.h .\Include\Drv_ADC.h .\Include\Drv_Delay.h .\Include\Drv_I2C_HW.h .\Include\Drv_Timer.h .\Include\Drv_Uart.h .\Include\Drv_Buzzer.h .\Include\Drv_Key.h .\Include\Drv_LCD_ET29A.h .\Include\Drv_LED.h .\Include\Drv_GPIO.h .\Include\Drv_LVD.h
	@$(COMPILER) $(COMPILER_FLAGS) -o $@ $^ 

$(OBJ_PATH)\Drv_LED.asm: .\Src\Drv_LED.c $(C_INC_PATH)\SN8P2988.h $(PRJ_PATH)\ETTest.cfg .\Include\Include.h .\Include\SN8P2988.h .\Include\Variable.h .\Include\Main.h .\Include\App_Core.h .\Include\App_BleMode06.h .\Include\App_BondTestMode.h .\Include\App_ReadyMode.h .\Include\App_InitMode.h .\Include\App_TestMode.h .\Include\App_CaliMode.h .\Include\App_SetMode.h .\Include\App_Memory.h .\Include\App_Sleep.h .\Include\App_Function.h .\Include\App_FTOffset215.h .\Include\App_SensorInfoET.h .\Include\App_Time.h .\Include\App_Param.h .\Include\App_Voice.h .\Include\Drv_ADC.h .\Include\Drv_Delay.h .\Include\Drv_I2C_HW.h .\Include\Drv_Timer.h .\Include\Drv_Uart.h .\Include\Drv_Buzzer.h .\Include\Drv_Key.h .\Include\Drv_LCD_ET29A.h .\Include\Drv_LED.h .\Include\Drv_GPIO.h .\Include\Drv_LVD.h
	@$(COMPILER) $(COMPILER_FLAGS) -o $@ $^ 

$(OBJ_PATH)\Drv_LVD.asm: .\Src\Drv_LVD.c $(C_INC_PATH)\SN8P2988.h $(PRJ_PATH)\ETTest.cfg .\Include\Include.h .\Include\SN8P2988.h .\Include\Variable.h .\Include\Main.h .\Include\App_Core.h .\Include\App_BleMode06.h .\Include\App_BondTestMode.h .\Include\App_ReadyMode.h .\Include\App_InitMode.h .\Include\App_TestMode.h .\Include\App_CaliMode.h .\Include\App_SetMode.h .\Include\App_Memory.h .\Include\App_Sleep.h .\Include\App_Function.h .\Include\App_FTOffset215.h .\Include\App_SensorInfoET.h .\Include\App_Time.h .\Include\App_Param.h .\Include\App_Voice.h .\Include\Drv_ADC.h .\Include\Drv_Delay.h .\Include\Drv_I2C_HW.h .\Include\Drv_Timer.h .\Include\Drv_Uart.h .\Include\Drv_Buzzer.h .\Include\Drv_Key.h .\Include\Drv_LCD_ET29A.h .\Include\Drv_LED.h .\Include\Drv_GPIO.h .\Include\Drv_LVD.h
	@$(COMPILER) $(COMPILER_FLAGS) -o $@ $^ 

$(OBJ_PATH)\Drv_Timer.asm: .\Src\Drv_Timer.c $(C_INC_PATH)\SN8P2988.h $(PRJ_PATH)\ETTest.cfg .\Include\Include.h .\Include\SN8P2988.h .\Include\Variable.h .\Include\Main.h .\Include\App_Core.h .\Include\App_BleMode06.h .\Include\App_BondTestMode.h .\Include\App_ReadyMode.h .\Include\App_InitMode.h .\Include\App_TestMode.h .\Include\App_CaliMode.h .\Include\App_SetMode.h .\Include\App_Memory.h .\Include\App_Sleep.h .\Include\App_Function.h .\Include\App_FTOffset215.h .\Include\App_SensorInfoET.h .\Include\App_Time.h .\Include\App_Param.h .\Include\App_Voice.h .\Include\Drv_ADC.h .\Include\Drv_Delay.h .\Include\Drv_I2C_HW.h .\Include\Drv_Timer.h .\Include\Drv_Uart.h .\Include\Drv_Buzzer.h .\Include\Drv_Key.h .\Include\Drv_LCD_ET29A.h .\Include\Drv_LED.h .\Include\Drv_GPIO.h .\Include\Drv_LVD.h
	@$(COMPILER) $(COMPILER_FLAGS) -o $@ $^ 

$(OBJ_PATH)\Drv_Uart.asm: .\Src\Drv_Uart.c $(C_INC_PATH)\SN8P2988.h $(PRJ_PATH)\ETTest.cfg .\Include\Include.h .\Include\SN8P2988.h .\Include\Variable.h .\Include\Main.h .\Include\App_Core.h .\Include\App_BleMode06.h .\Include\App_BondTestMode.h .\Include\App_ReadyMode.h .\Include\App_InitMode.h .\Include\App_TestMode.h .\Include\App_CaliMode.h .\Include\App_SetMode.h .\Include\App_Memory.h .\Include\App_Sleep.h .\Include\App_Function.h .\Include\App_FTOffset215.h .\Include\App_SensorInfoET.h .\Include\App_Time.h .\Include\App_Param.h .\Include\App_Voice.h .\Include\Drv_ADC.h .\Include\Drv_Delay.h .\Include\Drv_I2C_HW.h .\Include\Drv_Timer.h .\Include\Drv_Uart.h .\Include\Drv_Buzzer.h .\Include\Drv_Key.h .\Include\Drv_LCD_ET29A.h .\Include\Drv_LED.h .\Include\Drv_GPIO.h .\Include\Drv_LVD.h
	@$(COMPILER) $(COMPILER_FLAGS) -o $@ $^ 

$(OBJ_PATH)\Main.asm: .\Src\Main.c $(C_INC_PATH)\SN8P2988.h $(PRJ_PATH)\ETTest.cfg .\Include\Include.h .\Include\SN8P2988.h .\Include\Variable.h .\Include\Main.h .\Include\App_Core.h .\Include\App_BleMode06.h .\Include\App_BondTestMode.h .\Include\App_ReadyMode.h .\Include\App_InitMode.h .\Include\App_TestMode.h .\Include\App_CaliMode.h .\Include\App_SetMode.h .\Include\App_Memory.h .\Include\App_Sleep.h .\Include\App_Function.h .\Include\App_FTOffset215.h .\Include\App_SensorInfoET.h .\Include\App_Time.h .\Include\App_Param.h .\Include\App_Voice.h .\Include\Drv_ADC.h .\Include\Drv_Delay.h .\Include\Drv_I2C_HW.h .\Include\Drv_Timer.h .\Include\Drv_Uart.h .\Include\Drv_Buzzer.h .\Include\Drv_Key.h .\Include\Drv_LCD_ET29A.h .\Include\Drv_LED.h .\Include\Drv_GPIO.h .\Include\Drv_LVD.h
	@$(COMPILER) $(COMPILER_FLAGS) -o $@ $^ 

$(OBJ_PATH)\Variable.asm: .\Src\Variable.c $(C_INC_PATH)\SN8P2988.h $(PRJ_PATH)\ETTest.cfg .\Include\Include.h .\Include\SN8P2988.h .\Include\Variable.h .\Include\Main.h .\Include\App_Core.h .\Include\App_BleMode06.h .\Include\App_BondTestMode.h .\Include\App_ReadyMode.h .\Include\App_InitMode.h .\Include\App_TestMode.h .\Include\App_CaliMode.h .\Include\App_SetMode.h .\Include\App_Memory.h .\Include\App_Sleep.h .\Include\App_Function.h .\Include\App_FTOffset215.h .\Include\App_SensorInfoET.h .\Include\App_Time.h .\Include\App_Param.h .\Include\App_Voice.h .\Include\Drv_ADC.h .\Include\Drv_Delay.h .\Include\Drv_I2C_HW.h .\Include\Drv_Timer.h .\Include\Drv_Uart.h .\Include\Drv_Buzzer.h .\Include\Drv_Key.h .\Include\Drv_LCD_ET29A.h .\Include\Drv_LED.h .\Include\Drv_GPIO.h .\Include\Drv_LVD.h
	@$(COMPILER) $(COMPILER_FLAGS) -o $@ $^ 

$(OBJ_PATH)\APP_Isr.o: $(OBJ_PATH)\APP_Isr.asm $(PRJ_PATH)\ETTest.cfg
	@$(GMA) $(GMA_FLAGS) /CSource: /CASE: $< 

$(OBJ_PATH)\App_BleMode06.o: $(OBJ_PATH)\App_BleMode06.asm $(PRJ_PATH)\ETTest.cfg
	@$(GMA) $(GMA_FLAGS) /CSource: /CASE: $< 

$(OBJ_PATH)\App_BondTestMode.o: $(OBJ_PATH)\App_BondTestMode.asm $(PRJ_PATH)\ETTest.cfg
	@$(GMA) $(GMA_FLAGS) /CSource: /CASE: $< 

$(OBJ_PATH)\App_CaliMode.o: $(OBJ_PATH)\App_CaliMode.asm $(PRJ_PATH)\ETTest.cfg
	@$(GMA) $(GMA_FLAGS) /CSource: /CASE: $< 

$(OBJ_PATH)\App_Core.o: $(OBJ_PATH)\App_Core.asm $(PRJ_PATH)\ETTest.cfg
	@$(GMA) $(GMA_FLAGS) /CSource: /CASE: $< 

$(OBJ_PATH)\App_FTOffset215.o: $(OBJ_PATH)\App_FTOffset215.asm $(PRJ_PATH)\ETTest.cfg
	@$(GMA) $(GMA_FLAGS) /CSource: /CASE: $< 

$(OBJ_PATH)\App_Function.o: $(OBJ_PATH)\App_Function.asm $(PRJ_PATH)\ETTest.cfg
	@$(GMA) $(GMA_FLAGS) /CSource: /CASE: $< 

$(OBJ_PATH)\App_InitMode.o: $(OBJ_PATH)\App_InitMode.asm $(PRJ_PATH)\ETTest.cfg
	@$(GMA) $(GMA_FLAGS) /CSource: /CASE: $< 

$(OBJ_PATH)\App_Memory.o: $(OBJ_PATH)\App_Memory.asm $(PRJ_PATH)\ETTest.cfg
	@$(GMA) $(GMA_FLAGS) /CSource: /CASE: $< 

$(OBJ_PATH)\App_Param.o: $(OBJ_PATH)\App_Param.asm $(PRJ_PATH)\ETTest.cfg
	@$(GMA) $(GMA_FLAGS) /CSource: /CASE: $< 

$(OBJ_PATH)\App_ReadyMode.o: $(OBJ_PATH)\App_ReadyMode.asm $(PRJ_PATH)\ETTest.cfg
	@$(GMA) $(GMA_FLAGS) /CSource: /CASE: $< 

$(OBJ_PATH)\App_SensorInfoET.o: $(OBJ_PATH)\App_SensorInfoET.asm $(PRJ_PATH)\ETTest.cfg
	@$(GMA) $(GMA_FLAGS) /CSource: /CASE: $< 

$(OBJ_PATH)\App_SetMode.o: $(OBJ_PATH)\App_SetMode.asm $(PRJ_PATH)\ETTest.cfg
	@$(GMA) $(GMA_FLAGS) /CSource: /CASE: $< 

$(OBJ_PATH)\App_Sleep.o: $(OBJ_PATH)\App_Sleep.asm $(PRJ_PATH)\ETTest.cfg
	@$(GMA) $(GMA_FLAGS) /CSource: /CASE: $< 

$(OBJ_PATH)\App_TestMode.o: $(OBJ_PATH)\App_TestMode.asm $(PRJ_PATH)\ETTest.cfg
	@$(GMA) $(GMA_FLAGS) /CSource: /CASE: $< 

$(OBJ_PATH)\App_Time.o: $(OBJ_PATH)\App_Time.asm $(PRJ_PATH)\ETTest.cfg
	@$(GMA) $(GMA_FLAGS) /CSource: /CASE: $< 

$(OBJ_PATH)\App_Voice.o: $(OBJ_PATH)\App_Voice.asm $(PRJ_PATH)\ETTest.cfg
	@$(GMA) $(GMA_FLAGS) /CSource: /CASE: $< 

$(OBJ_PATH)\Drv_ADC.o: $(OBJ_PATH)\Drv_ADC.asm $(PRJ_PATH)\ETTest.cfg
	@$(GMA) $(GMA_FLAGS) /CSource: /CASE: $< 

$(OBJ_PATH)\Drv_Buzzer.o: $(OBJ_PATH)\Drv_Buzzer.asm $(PRJ_PATH)\ETTest.cfg
	@$(GMA) $(GMA_FLAGS) /CSource: /CASE: $< 

$(OBJ_PATH)\Drv_Delay.o: $(OBJ_PATH)\Drv_Delay.asm $(PRJ_PATH)\ETTest.cfg
	@$(GMA) $(GMA_FLAGS) /CSource: /CASE: $< 

$(OBJ_PATH)\Drv_GPIO.o: $(OBJ_PATH)\Drv_GPIO.asm $(PRJ_PATH)\ETTest.cfg
	@$(GMA) $(GMA_FLAGS) /CSource: /CASE: $< 

$(OBJ_PATH)\Drv_I2C_HW.o: $(OBJ_PATH)\Drv_I2C_HW.asm $(PRJ_PATH)\ETTest.cfg
	@$(GMA) $(GMA_FLAGS) /CSource: /CASE: $< 

$(OBJ_PATH)\Drv_Key.o: $(OBJ_PATH)\Drv_Key.asm $(PRJ_PATH)\ETTest.cfg
	@$(GMA) $(GMA_FLAGS) /CSource: /CASE: $< 

$(OBJ_PATH)\Drv_LCD_ET29A.o: $(OBJ_PATH)\Drv_LCD_ET29A.asm $(PRJ_PATH)\ETTest.cfg
	@$(GMA) $(GMA_FLAGS) /CSource: /CASE: $< 

$(OBJ_PATH)\Drv_LED.o: $(OBJ_PATH)\Drv_LED.asm $(PRJ_PATH)\ETTest.cfg
	@$(GMA) $(GMA_FLAGS) /CSource: /CASE: $< 

$(OBJ_PATH)\Drv_LVD.o: $(OBJ_PATH)\Drv_LVD.asm $(PRJ_PATH)\ETTest.cfg
	@$(GMA) $(GMA_FLAGS) /CSource: /CASE: $< 

$(OBJ_PATH)\Drv_Timer.o: $(OBJ_PATH)\Drv_Timer.asm $(PRJ_PATH)\ETTest.cfg
	@$(GMA) $(GMA_FLAGS) /CSource: /CASE: $< 

$(OBJ_PATH)\Drv_Uart.o: $(OBJ_PATH)\Drv_Uart.asm $(PRJ_PATH)\ETTest.cfg
	@$(GMA) $(GMA_FLAGS) /CSource: /CASE: $< 

$(OBJ_PATH)\Main.o: $(OBJ_PATH)\Main.asm $(PRJ_PATH)\ETTest.cfg
	@$(GMA) $(GMA_FLAGS) /CSource: /CASE: $< 

$(OBJ_PATH)\Variable.o: $(OBJ_PATH)\Variable.asm $(PRJ_PATH)\ETTest.cfg
	@$(GMA) $(GMA_FLAGS) /CSource: /CASE: $< 

$(OUT_PATH)\ETTest.out: $(LINK_DEP_FILES) $(PRJ_PATH)\ETTest.cop
	@$(LINKER) $(LINK_FLAGS) /CSource: /CASE: $(LINK_DEP_FILES) 

$(OUT_PATH)\ETTest.sn8: $(OUT_PATH)\ETTest.out $(PRJ_PATH)\ETTest.cop
	@$(CONVERT) $(CONV_FLAGS) $^ 


# Define Clean Rule
Clean:
	@del /F /Q $(OBJ_PATH)\*.*;
	@if EXIST .\ETTest.prj.stb del .\ETTest.prj.stb;
	@if EXIST .\ETTest.out del .\ETTest.out;
	@if EXIST .\ETTest.sn8 del .\ETTest.sn8;
	@if EXIST .\ETTest.sn8.dep del .\ETTest.sn8.dep;
	@if EXIST $(OUT_PATH)\ETTest.out del $(OUT_PATH)\ETTest.out;
	@if EXIST $(OUT_PATH)\ETTest.sn8 del $(OUT_PATH)\ETTest.sn8;
	@if EXIST $(OUT_PATH)\ETTest.sn8.dep del $(OUT_PATH)\ETTest.sn8.dep;

# Don't keep invalid files, command line must return non-zero(1 or 2) value 
# to validate delete on error mechanism 
# else return 0 for success

.DELETE_ON_ERROR:

Begin: 

Build: Begin All 

Rebuild: Clean Build 

All:$(OUT_PATH)\ETTest.out $(OUT_PATH)\ETTest.sn8
