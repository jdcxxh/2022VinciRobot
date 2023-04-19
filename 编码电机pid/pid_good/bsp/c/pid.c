

#include "pid.h"

extern float kp ,ki ,kd ;

int32_t LimitMax(int32_t input, int32_t max) 
    {                          
        if (input > max)       
        {                      
            input = max;       
        }                      
        else if (input < -max) 
        {                      
            input = -max;      
        } 
  return input;				
    }

PID_TypeDef  g_speed_pid;           /* �ٶȻ�PID�����ṹ�� */

/**
 * @brief       pid��ʼ��
 * @param       ��
 * @retval      ��
 */
void pid_init(void)
{
    g_speed_pid.SetPoint = 0;       /* �趨Ŀ��ֵ */
    g_speed_pid.ActualValue = 0.0;  /* �������ֵ */
    g_speed_pid.SumError = 0.0;     /* ����ֵ */
    g_speed_pid.Error = 0.0;        /* Error[1] */
    g_speed_pid.LastError = 0.0;    /* Error[-1] */
    g_speed_pid.PrevError = 0.0;    /* Error[-2] */
    g_speed_pid.Proportion = KP;    /* �������� Proportional Const */
    g_speed_pid.Integral = KI;      /* ���ֳ��� Integral Const */
    g_speed_pid.Derivative = KD;    /* ΢�ֳ��� Derivative Const */ 
}

/**
 * @brief       pid�ջ�����
 * @param       *PID��PID�ṹ�������ַ
 * @param       Feedback_value����ǰʵ��ֵ
 * @retval      �������ֵ
 */
int32_t increment_pid_ctrl(PID_TypeDef *PID,float Feedback_value)
{
    PID->Error = (float)(PID->SetPoint - Feedback_value);                   /* ����ƫ�� */
    
#if  INCR_LOCT_SELECT                                                       /* ����ʽPID */
    
    PID->ActualValue += (PID->Proportion * (PID->Error - PID->LastError))                          /* �������� */
                        + (PID->Integral * PID->Error)                                             /* ���ֻ��� */
                        + (PID->Derivative * (PID->Error - 2 * PID->LastError + PID->PrevError));  /* ΢�ֻ��� */
    
    PID->PrevError = PID->LastError;                                        /* �洢ƫ������´μ��� */
    PID->LastError = PID->Error;
    
#else                                                                       /* λ��ʽPID */
    
    PID->SumError += PID->Error;

    PID->ActualValue = (PID->Proportion * PID->Error)                       /* �������� */
                       + (PID->Integral * PID->SumError)                    /* ���ֻ��� */
                       + (PID->Derivative * (PID->Error - PID->LastError)); /* ΢�ֻ��� */
			PID->ActualValue=  LimitMax( PID->ActualValue, 1000) ;
    PID->LastError = PID->Error;
    
#endif
    return ((int32_t)(PID->ActualValue));                                   /* ���ؼ�����������ֵ */
}

