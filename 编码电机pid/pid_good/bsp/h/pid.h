
#ifndef __PID_H
#define __PID_H

#include "INCLUDES.h"



/******************************************************************************************/
/* PID��ز��� */

#define  INCR_LOCT_SELECT  1         /* 0��λ��ʽ ��1������ʽ */

#if INCR_LOCT_SELECT

/* ����ʽPID������غ� */
#define  KP      15.90f               /* P����*/
#define  KI      0.00f               /* I����*/
#define  KD      -2.20f               /* D����*/
#define  SMAPLSE_PID_SPEED  50       /* �������� ��λms*/

#else

/* λ��ʽPID������غ� */
#define  KP    2.0f               /* P����*/
#define  KI      0.0f               /* I����*/
#define  KD      0.0f                /* D����*/
#define  SMAPLSE_PID_SPEED  50       /* �������� ��λms*/

#endif

/* PID�����ṹ�� */
typedef struct
{
    __IO float  SetPoint;            /* �趨Ŀ�� */
    __IO float  ActualValue;         /* �������ֵ */
    __IO float  SumError;            /* ����ۼ� */
    __IO float  Proportion;          /* �������� P */
    __IO float  Integral;            /* ���ֳ��� I */
    __IO float  Derivative;          /* ΢�ֳ��� D */
    __IO float  Error;               /* Error[1] */
    __IO float  LastError;           /* Error[-1] */
    __IO float  PrevError;           /* Error[-2] */
} PID_TypeDef;

extern PID_TypeDef  g_speed_pid;     /*�ٶȻ�PID�����ṹ��*/

/******************************************************************************************/

void pid_init(void);                 /* pid��ʼ�� */
int32_t increment_pid_ctrl(PID_TypeDef *PID,float Feedback_value);      /* pid�ջ����� */

#endif
