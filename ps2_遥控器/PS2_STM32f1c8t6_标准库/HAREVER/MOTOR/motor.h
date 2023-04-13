#ifndef __MOTOR_H
#define __MOTOR_H
#include "sys.h"

#define  AIN1  PAout(6)//
#define  AIN2  PAout(7)//左上

#define  BIN1  PAout(3)
#define  BIN2  PAout(4)//z右上

#define  CIN1  PAout(1)
#define  CIN2  PAout(0)//z右下

#define  DIN1  PAout(9)
#define  DIN2  PAout(10)//z左下

void motor_GPIO_Init(void);
void forward(void);
void back(void);
void turn_left(void);
void turn_right(void);
void stop(void);


#endif

