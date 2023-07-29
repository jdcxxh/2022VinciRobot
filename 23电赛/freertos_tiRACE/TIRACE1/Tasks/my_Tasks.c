#include "my_Tasks.h"

extern pid_type_def pid_v_1[2];
extern pid_type_def pid_p_1[2];
int a=0;



void led_blink_Task(void *argument)
{
  
  for(;;)
  {
		a=0;
		 HAL_GPIO_TogglePin(LED_PORT,  LED);
      osDelay(3000);
		a=7000;
		osDelay(3000);
  }
 
}
void chassis_Move_Task(void *argument)
{



for(;;)
  {
		
	motor_two_dirver(500,PID_postions_realize_1(  a,2));    //速度环演示
  printf("%d,%d,%f\n",MOTOR[1].now_Encode,a,pid_p_1[1].out);	
		
//	motor_two_dirver(500,PID_velocity_realize_1(  a,2));    //速度环演示
//	printf("%d,%d,%f\n",MOTOR[1].speed,a,pid_v_1[1].out);
      osDelay(1);
  }



}

