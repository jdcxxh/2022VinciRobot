#include "motor_bsp.h"
/*

pa3  pa4





*/
void my_pwm_init(void)
{
	
HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
//HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);
	
}

/*
pwmֵ(����������) ת��Ϊ�����pwm��gpio���� 
flag �����
pwm   pwmֵ   +-  ��������ת
upwm  pwmֵ      �޷���
*/
void pwm_under(int16_t pwm)
{
	
				if(pwm>=0)
				{
							HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
							HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
					__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, pwm);
				}
				if(pwm<0)                  //��ת
				{
							HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
							HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
					__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3,-pwm);
				}

}











//int Read_Encoder(void)//
//{
//	int Encoder_TIM;
// 
//		  Encoder_TIM =  __HAL_TIM_GetCounter(&htim1); 
//	    __HAL_TIM_SET_COUNTER(&htim1,0); 
//	
//	return Encoder_TIM;
//}


 
 int Read_Encoder(void)//��ȡ�������߹����ܱ���ֵ
{
	return __HAL_TIM_GetCounter(&htim1)+65535*encode_count;
}



int32_t now_Encode,last_Encode;

//��λʱ�������ת���ı���ֵ���ı�  �൱�ڵ�����ٶ�
int32_t speed_Encoder(void)
{
	
	int rate=0;
  now_Encode = Read_Encoder();
	rate=now_Encode-last_Encode;
	last_Encode=now_Encode;
	return rate;
	
}






Motor_TypeDef g_motor_data;  /*�����������*/
 ENCODE_TypeDef g_encode;  

/**
 * @brief       ����ٶȼ���
 * @param       encode_now����ǰ�������ܵļ���ֵ
 *              ms�������ٶȵļ�����ж�1ms����һ�Σ�����ms = 5��5ms����һ���ٶ�
 * @retval      ��
 */
void speed_computer(int32_t encode_now, uint8_t ms)
{ uint8_t i = 0, j = 0;
    float temp = 0.0;
    static uint8_t sp_count = 0, k = 0;
    static float speed_arr[10] = {0.0};                     /* �洢�ٶȽ����˲����� */

    if (sp_count == ms)                                     /* ����һ���ٶ� */
    {
        /* ������ת�� 
           ��һ�� ������ms�����ڼ����仯��
           �ڶ��� ������1min�ڼ����仯����g_encode.speed * ((1000 / ms) * 60 ��
           ������ �����Ա�������תһȦ�ļ�����������Ƶ���� * �������ֱ��ʣ�
           ���Ĳ� �����Լ��ٱȼ��ɵó����ת��
        */
        g_encode.encode_now = encode_now;                                /* ȡ����������ǰ����ֵ */
        g_encode.speed = (g_encode.encode_now - g_encode.encode_old);    /* �������������ֵ�ı仯�� */
        
        speed_arr[k++] = (float)(g_encode.speed * 6000 );    /* ������ת�� */
        
        g_encode.encode_old = g_encode.encode_now;          /* ���浱ǰ��������ֵ */

        /* �ۼ�10���ٶ�ֵ�����������˲�*/
        if (k == 10)
        {
            for (i = 10; i >= 1; i--)                       /* ð������*/
            {
                for (j = 0; j < (i - 1); j++) 
                {
                    if (speed_arr[j] > speed_arr[j + 1])    /* ��ֵ�Ƚ� */
                    { 
                        temp = speed_arr[j];                /* ��ֵ��λ */
                        speed_arr[j] = speed_arr[j + 1];
                        speed_arr[j + 1] = temp;
                    }
                }
            }
            
            temp = 0.0;
            
            for (i = 2; i < 8; i++)                         /* ȥ�����߸ߵ����� */
            {
                temp += speed_arr[i];                       /* ���м���ֵ�ۼ� */
            }
            
            temp = (float)(temp / 6);                       /*���ٶ�ƽ��ֵ*/
            
            /* һ�׵�ͨ�˲�
             * ��ʽΪ��Y(n)= qX(n) + (1-q)Y(n-1)
             * ����X(n)Ϊ���β���ֵ��Y(n-1)Ϊ�ϴ��˲����ֵ��Y(n)Ϊ�����˲����ֵ��qΪ�˲�ϵ��
             * qֵԽС����һ������Ա������Ӱ��Խ����������Խƽ�ȣ����Ƕ����ٶȱ仯����ӦҲ��Խ��
             */
            g_motor_data.speed = (float)( ((float)0.48 * temp) + (g_motor_data.speed * (float)0.52) );
            k = 0;
        }
        sp_count = 0;
    }
    sp_count ++;
        
   
}




////////////////////////////////////////////////*///////////////////////////////////////////////////////
int encode_count=0;
int test; 
static int i =0;
static int a=0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
 if(htim->Instance==htim2.Instance)		         //1ms�ж�
	{
 
			if(i==5)
			{
				
				
				
		   test=speed_Encoder();
	     g_motor_data.speed = (float)( ((float)0.56 *test) + (g_motor_data.speed * (float)0.44) );
//				speed_computer(test, 20);
				
				
		   i=0;
			}
			

		i++;
		
			
			if(a==20)
			{
	
 	pwm_under(increment_pid_ctrl(&g_speed_pid,g_motor_data.speed));
	
 
		   a=0;
			}
		a++;
		
  }
	
	 if(htim->Instance==htim1.Instance)		        
	{
		if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim1))
		{
		encode_count--;
		}
	  else
		{		
		encode_count++;		
		}

  

  }
}







