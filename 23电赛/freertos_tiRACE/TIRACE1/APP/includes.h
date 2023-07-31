#ifndef _INCLUDES_H_
#define _INCLUDES_H_


#include "struct_typedef.h"

#include "main.h"
#include "tim.h"
#include "gpio.h"
#include "my_tim.h"
#include "my_gpio.h"
#include "my_motor.h"
#include  "stdio.h"
#include "my_usart.h"
#include "pid_user.h"
#include "my_iic.h"
//#include "pid_user.h"

//PE0 LED
#define LED      GPIO_PIN_0
#define LED_PORT GPIOE




/***********预编译任务配置************************/
#define find_road_moda   1     //循迹模式控制    1:摄像头循迹      0：灰度传感器循迹





#endif
