#ifndef _my_motor_
#define _my_motor_
#include "includes.h"
	typedef struct
{
	uint16_t encode_count;//Òç³ö´ÎÊý
	int16_t speed;
	int16_t torget_speed;
	int32_t now_Encode;
 
	int32_t last_Encode;
	TIM_HandleTypeDef htim;

} motor_measure_t;

	extern motor_measure_t MOTOR[2];

	void motor_star(void);
	void motor_dirver(int16_t pwm,int x);
	void motor_two_dirver(int16_t pwmA,int16_t pwmB);

	void motor_init(motor_measure_t *per,TIM_HandleTypeDef htim);
  int Read_Encoder(motor_measure_t *per);
	void speed_Encoder(motor_measure_t *per);
#endif

