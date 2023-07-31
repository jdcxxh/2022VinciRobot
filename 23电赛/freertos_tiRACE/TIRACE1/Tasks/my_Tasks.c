#include "my_Tasks.h"
 



/******************用于电机控制计算*****************************/
extern pid_type_def pid_v_1[2]; 									//速度环pid
extern pid_type_def pid_p_1[2];										//位置环pid

/*-------------------------------------------------------------*/



/******************用于pid计算*****************************/
extern pid_type_def pid_v_1[2]; 									//速度环pid
extern pid_type_def pid_p_1[2];										//位置环pid

/*-------------------------------------------------------------*/



/******************用于接收k210数据*****************************/
int len;                                          //接收到的字符长度
extern uint8_t USART_RX_BUF[USART_REC_LEN];  			//接收到k210的字符串数据
extern int32_t  Output;                           //接收到k210的数字数据
extern uint16_t USART_RX_STA;                     //接收状态标记	
extern uint8_t aRxBuffer[RXBUFFERSIZE];           //HAL库使用的串口接收缓冲

/*-------------------------------------------------------------*/


/******************用于循迹*****************************/
int speed_find;       														//循迹的差速
uint8_t aaad[5];																	//循迹模块的值

/*-------------------------------------------------------------*/

/******************用于oled的ui*****************************/
extern u8g2_t u8g2;																//oled调用的u8g2




/*-------------------------------------------------------------*/



/**
	*@tip 闪灯任务
	*/
void led_blink_Task(void *argument)
{
  
  for(;;)
  {
		
		 HAL_GPIO_TogglePin(LED_PORT,  LED);
		
		osDelay(300);

  }
 
}


/**
	*@tip 电机运动任务
	*/
void chassis_Move_Task(void *argument)
{



for(;;)
  {
		

			 
	motor_two_dirver(PID_velocity_realize_1(MOTOR[0].torget_speed,1),PID_velocity_realize_1(MOTOR[1].torget_speed,2));    //速度环演示
//	printf("%d,%d,%f\n",MOTOR[0].speed,MOTOR[1].speed,pid_v_1[1].out);
      osDelay(1);
  }



}



/**
	*@tip oled显示任务  使用u8g2库   显示两个轮子测速
	*/
void oled_task(void *argument)
{



for(;;)
  {

      u8g2_ClearBuffer(&u8g2); 
			u8g2_DrawStr(&u8g2,4, 9, "speed1         speed2");
			char buff1[20];
			sprintf(buff1,"%d     %d",(int)(MOTOR[0].speed),(int)(MOTOR[1].speed));
			u8g2_SetFont(&u8g2,u8g2_font_helvB08_tr);
      u8g2_DrawStr(&u8g2,15,20,buff1);
     	u8g2_SendBuffer(&u8g2);	
      osDelay(10);
  }



}



/**
	*@tip usart5  接收k210发送的字符串数据并转换为int32 类型数据   进行数据处理
	*/
void find_road_task(void *argument)
{


#if find_road_moda    //摄像头循迹
for(;;)
  {

		 if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
/***************对接收到的数据处理***************************************/			
//		HAL_UART_Transmit(&huart5,(uint8_t*)USART_RX_BUF,len,1000);	//发送接收到的数据
		  str2int(USART_RX_BUF, 0, 1, &Output);
			Output=Output/100;
//				printf("%d\n\r",Output);			
			
/************************************************************************/			
			while(__HAL_UART_GET_FLAG(&huart5,UART_FLAG_TC)!=SET);		//等待发送结束
			USART_RX_STA=0;
		}

      osDelay(1);
  }
	
#else 
	
	for(;;)
  {

		
speed_find=0;
aaad[0]= HAL_GPIO_ReadPin(  GPIOF, GPIO_PIN_0);
aaad[1]= HAL_GPIO_ReadPin(  GPIOF, GPIO_PIN_1);
		aaad[2]= HAL_GPIO_ReadPin(  GPIOF, GPIO_PIN_2);
		aaad[3]= HAL_GPIO_ReadPin(  GPIOF, GPIO_PIN_3);
		aaad[4]= HAL_GPIO_ReadPin(  GPIOF, GPIO_PIN_4);
//		printf("%d,%d,%d,%d,%d\n",aaad[0],aaad[1],aaad[2],aaad[3],aaad[4]);
		if(HAL_GPIO_ReadPin(  GPIOF, GPIO_PIN_0))
		{
		speed_find+=7;
		
		}
		if(HAL_GPIO_ReadPin(  GPIOF, GPIO_PIN_4))
		{
		speed_find-=7;
		
		}
				if(HAL_GPIO_ReadPin(  GPIOF, GPIO_PIN_1))
		{
		speed_find+=5;
		
		}
						if(HAL_GPIO_ReadPin(  GPIOF, GPIO_PIN_3))
		{
		speed_find-=5;
		
		}
 						if(HAL_GPIO_ReadPin(  GPIOF, GPIO_PIN_2))
		{
		speed_find+=0;
		
		}
      osDelay(1);
  }
	

#endif

}
