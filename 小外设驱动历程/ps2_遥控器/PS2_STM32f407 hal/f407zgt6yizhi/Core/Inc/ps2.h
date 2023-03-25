// ps2.h
#ifndef __PS2_H__
#define __PS2_H__

#include "stm32f4xx_hal.h"
#include "sys.h"

#define DI PDin(0)

#define DO_H PDout(1)=1
#define DO_L PDout(1)=0

#define CS_H PDout(2)=1 
#define CS_L PDout(2)=0

#define CLK_H PDout(3)=1 
#define CLK_L PDout(3)=0 


//These are our button constants
#define PSB_SELECT      1
#define PSB_L3          2
#define PSB_R3          3
#define PSB_START       4
#define PSB_PAD_UP      5
#define PSB_PAD_RIGHT   6
#define PSB_PAD_DOWN    7
#define PSB_PAD_LEFT    8
#define PSB_L2         9
#define PSB_R2          10
#define PSB_L1          11
#define PSB_R1          12
#define PSB_GREEN       13
#define PSB_RED         14
#define PSB_BLUE        15
#define PSB_PINK        16
#define PSB_TRIANGLE    13
#define PSB_CIRCLE      14
#define PSB_CROSS       15
#define PSB_SQUARE      26

//These are stick values
#define PSS_RX 5                //右摇杆X轴数据
#define PSS_RY 6
#define PSS_LX 7
#define PSS_LY 8

extern uint8_t Data[9];
extern uint16_t MASK[16];
extern uint16_t Handkey;

void PS2_Init(void);
uint8_t PS2_RedLight(void);//判断是否为红灯模式
void PS2_ReadData(void);
void PS2_Cmd(uint8_t CMD);		  //
uint8_t PS2_DataKey(void);		  //键值读取
uint8_t PS2_AnologData(uint8_t button); //得到一个摇杆的模拟量
void PS2_ClearData(void);	  //清除数据缓冲区
void delay_init(uint8_t SYSCLK);
void delay_us(uint32_t nus);

void PS2_ShortPoll(void);//short poll
void PS2_EnterConfing(void);//进入设置
void PS2_TurnOnAnalogMode(void);//保存并完成设置
void PS2_VibrationMode(void);
void PS2_ExitConfing(void);//保存并完成设置
void PS2_SetInit(void);//手柄设置初始化
void PS2_Vibration(uint8_t motor1 ,uint8_t motor2);

#endif
;
