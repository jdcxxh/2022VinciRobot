#include "my_Tasks.h"

extern pid_type_def pid_v_1[2];
extern pid_type_def pid_p_1[2];

extern u8g2_t u8g2;


void led_blink_Task(void *argument)
{
  
  for(;;)
  {
		
		 HAL_GPIO_TogglePin(LED_PORT,  LED);

  }
 
}
void chassis_Move_Task(void *argument)
{



for(;;)
  {
		
//	motor_two_dirver(500,PID_postions_realize_1(  a,2));    //速度环演示
 //printf("%d,%d,%f\n",MOTOR[1].now_Encode,a,pid_p_1[1].out);	
	
			 
//	motor_two_dirver(PID_velocity_realize_1(0,1),PID_velocity_realize_1(0,2));    //速度环演示
//	printf("%d,%d,%d,%f\n",MOTOR[0].speed,MOTOR[1].speed,a,pid_v_1[1].out);
      osDelay(1);
  }



}

void oled_task(void *argument)
{



for(;;)
  {

u8g2_ClearBuffer(&u8g2); 
				u8g2_DrawStr(&u8g2,4, 9, "speed1         speed2");
	    
		char buff1[5],buff2[5];
		sprintf(buff1,"%d               %d",(int)(MOTOR[0].speed),(int)(MOTOR[1].speed));
		u8g2_SetFont(&u8g2,u8g2_font_helvB08_tr);
        u8g2_DrawStr(&u8g2,4,20,buff1);
		
			
	 
     	u8g2_SendBuffer(&u8g2);	
          
 
      osDelay(100);
  }



}
