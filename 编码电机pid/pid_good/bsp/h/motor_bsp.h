#ifndef _MOTOR_H_
#define _MOTOR_H_
 

#include "INCLUDES.h"

void my_pwm_init(void);
void pwm_under(int16_t pwm);
extern int b;

extern int encode_count ;


/* �����������ṹ�� */
typedef struct 
{
  int encode_old;                  /* ��һ�μ���ֵ */
  int encode_now;                  /* ��ǰ����ֵ */
  float speed;                     /* �������ٶ� */
} ENCODE_TypeDef;

extern ENCODE_TypeDef g_encode;    /* �������������� */
/* ��������ṹ�� */
typedef struct 
{
  uint8_t state;          /*���״̬*/
  float current;          /*�������*/
  float volatage;         /*�����ѹ*/
  float power;            /*�������*/
  float speed;            /*���ʵ���ٶ�*/
  int32_t motor_pwm;      /*���ñȽ�ֵ��С */
} Motor_TypeDef;

extern Motor_TypeDef  g_motor_data;  /*�����������*/
extern int test;







#endif


