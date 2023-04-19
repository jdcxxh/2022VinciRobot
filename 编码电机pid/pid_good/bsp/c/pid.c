

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

PID_TypeDef  g_speed_pid;           /* 速度环PID参数结构体 */

/**
 * @brief       pid初始化
 * @param       无
 * @retval      无
 */
void pid_init(void)
{
    g_speed_pid.SetPoint = 0;       /* 设定目标值 */
    g_speed_pid.ActualValue = 0.0;  /* 期望输出值 */
    g_speed_pid.SumError = 0.0;     /* 积分值 */
    g_speed_pid.Error = 0.0;        /* Error[1] */
    g_speed_pid.LastError = 0.0;    /* Error[-1] */
    g_speed_pid.PrevError = 0.0;    /* Error[-2] */
    g_speed_pid.Proportion = KP;    /* 比例常数 Proportional Const */
    g_speed_pid.Integral = KI;      /* 积分常数 Integral Const */
    g_speed_pid.Derivative = KD;    /* 微分常数 Derivative Const */ 
}

/**
 * @brief       pid闭环控制
 * @param       *PID：PID结构体变量地址
 * @param       Feedback_value：当前实际值
 * @retval      期望输出值
 */
int32_t increment_pid_ctrl(PID_TypeDef *PID,float Feedback_value)
{
    PID->Error = (float)(PID->SetPoint - Feedback_value);                   /* 计算偏差 */
    
#if  INCR_LOCT_SELECT                                                       /* 增量式PID */
    
    PID->ActualValue += (PID->Proportion * (PID->Error - PID->LastError))                          /* 比例环节 */
                        + (PID->Integral * PID->Error)                                             /* 积分环节 */
                        + (PID->Derivative * (PID->Error - 2 * PID->LastError + PID->PrevError));  /* 微分环节 */
    
    PID->PrevError = PID->LastError;                                        /* 存储偏差，用于下次计算 */
    PID->LastError = PID->Error;
    
#else                                                                       /* 位置式PID */
    
    PID->SumError += PID->Error;

    PID->ActualValue = (PID->Proportion * PID->Error)                       /* 比例环节 */
                       + (PID->Integral * PID->SumError)                    /* 积分环节 */
                       + (PID->Derivative * (PID->Error - PID->LastError)); /* 微分环节 */
			PID->ActualValue=  LimitMax( PID->ActualValue, 1000) ;
    PID->LastError = PID->Error;
    
#endif
    return ((int32_t)(PID->ActualValue));                                   /* 返回计算后输出的数值 */
}

