#ifndef _MOTOR_H_
#define _MOTOR_H_
 

#include "INCLUDES.h"

void my_pwm_init(void);
void pwm_under(int16_t pwm);
extern int b;

extern int encode_count ;


/* 编码器参数结构体 */
typedef struct 
{
  int encode_old;                  /* 上一次计数值 */
  int encode_now;                  /* 当前计数值 */
  float speed;                     /* 编码器速度 */
} ENCODE_TypeDef;

extern ENCODE_TypeDef g_encode;    /* 编码器参数变量 */
/* 电机参数结构体 */
typedef struct 
{
  uint8_t state;          /*电机状态*/
  float current;          /*电机电流*/
  float volatage;         /*电机电压*/
  float power;            /*电机功率*/
  float speed;            /*电机实际速度*/
  int32_t motor_pwm;      /*设置比较值大小 */
} Motor_TypeDef;

extern Motor_TypeDef  g_motor_data;  /*电机参数变量*/
extern int test;







#endif


