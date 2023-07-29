#include "pid_user.h"
extern motor_measure_t MOTOR[2];



pid_type_def pid_v_1[2];
pid_type_def pid_p_1[2];


fp32 motor_postion_motor_pid[3] = {5,0,0 };//
fp32 motor_speed_motor_pid[3] = {85, 0.46, 8};//
//85   0.5   5



#define LimitMax(input, max)   \
    {                          \
        if (input > max)       \
        {                      \
            input = max;       \
        }                      \
        else if (input < -max) \
        {                      \
            input = -max;      \
        }                      \
    }


//电机PID初始化
void PID_devices_Init(void)
{
	for(int i=0;i<2;i++)
	{
    PID_init(&pid_v_1[i], PID_POSITION, motor_speed_motor_pid, 1000, 346);
		PID_init(&pid_p_1[i], PID_POSITION, motor_postion_motor_pid, 1000, 346);
	}
	
	
}



fp32 PID_velocity_realize_1(fp32 set_speed,int i)
{
		PID_calc(&pid_v_1[i-1],MOTOR[i-1].speed , set_speed);
		return pid_v_1[i-1].out;
}



fp32 PID_postions_realize_1(fp32 set_encode,int i)
{
		PID_calc(&pid_p_1[i-1],MOTOR[i-1].now_Encode , set_encode);
		return pid_p_1[i-1].out;
}










