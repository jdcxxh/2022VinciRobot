#include "my_Tasks.h"
 



/******************���ڵ�����Ƽ���*****************************/
extern pid_type_def pid_v_1[2]; 									//�ٶȻ�pid
extern pid_type_def pid_p_1[2];										//λ�û�pid

/*-------------------------------------------------------------*/



/******************����pid����*****************************/
extern pid_type_def pid_v_1[2]; 									//�ٶȻ�pid
extern pid_type_def pid_p_1[2];										//λ�û�pid

/*-------------------------------------------------------------*/



/******************���ڽ���k210����*****************************/
int len;                                          //���յ����ַ�����
extern uint8_t USART_RX_BUF[USART_REC_LEN];  			//���յ�k210���ַ�������
extern int32_t  Output;                           //���յ�k210����������
extern uint16_t USART_RX_STA;                     //����״̬���	
extern uint8_t aRxBuffer[RXBUFFERSIZE];           //HAL��ʹ�õĴ��ڽ��ջ���

/*-------------------------------------------------------------*/


/******************����ѭ��*****************************/
int speed_find;       														//ѭ���Ĳ���
uint8_t aaad[5];																	//ѭ��ģ���ֵ

/*-------------------------------------------------------------*/

/******************����oled��ui*****************************/
extern u8g2_t u8g2;																//oled���õ�u8g2




/*-------------------------------------------------------------*/



/**
	*@tip ��������
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
	*@tip ����˶�����
	*/
void chassis_Move_Task(void *argument)
{



for(;;)
  {
		

			 
	motor_two_dirver(PID_velocity_realize_1(MOTOR[0].torget_speed,1),PID_velocity_realize_1(MOTOR[1].torget_speed,2));    //�ٶȻ���ʾ
//	printf("%d,%d,%f\n",MOTOR[0].speed,MOTOR[1].speed,pid_v_1[1].out);
      osDelay(1);
  }



}



/**
	*@tip oled��ʾ����  ʹ��u8g2��   ��ʾ�������Ӳ���
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
	*@tip usart5  ����k210���͵��ַ������ݲ�ת��Ϊint32 ��������   �������ݴ���
	*/
void find_road_task(void *argument)
{


#if find_road_moda    //����ͷѭ��
for(;;)
  {

		 if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
/***************�Խ��յ������ݴ���***************************************/			
//		HAL_UART_Transmit(&huart5,(uint8_t*)USART_RX_BUF,len,1000);	//���ͽ��յ�������
		  str2int(USART_RX_BUF, 0, 1, &Output);
			Output=Output/100;
//				printf("%d\n\r",Output);			
			
/************************************************************************/			
			while(__HAL_UART_GET_FLAG(&huart5,UART_FLAG_TC)!=SET);		//�ȴ����ͽ���
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
