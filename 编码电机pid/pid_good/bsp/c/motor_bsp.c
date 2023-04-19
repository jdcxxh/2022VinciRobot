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
pwm值(带有正负号) 转化为电机的pwm和gpio设置 
flag 电机号
pwm   pwm值   +-  代表正反转
upwm  pwm值      无符号
*/
void pwm_under(int16_t pwm)
{
	
				if(pwm>=0)
				{
							HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
							HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
					__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, pwm);
				}
				if(pwm<0)                  //反转
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


 
 int Read_Encoder(void)//读取编码器走过的总编码值
{
	return __HAL_TIM_GetCounter(&htim1)+65535*encode_count;
}



int32_t now_Encode,last_Encode;

//单位时间编码器转过的编码值的四倍  相当于电机的速度
int32_t speed_Encoder(void)
{
	
	int rate=0;
  now_Encode = Read_Encoder();
	rate=now_Encode-last_Encode;
	last_Encode=now_Encode;
	return rate;
	
}






Motor_TypeDef g_motor_data;  /*电机参数变量*/
 ENCODE_TypeDef g_encode;  

/**
 * @brief       电机速度计算
 * @param       encode_now：当前编码器总的计数值
 *              ms：计算速度的间隔，中断1ms进入一次，例如ms = 5即5ms计算一次速度
 * @retval      无
 */
void speed_computer(int32_t encode_now, uint8_t ms)
{ uint8_t i = 0, j = 0;
    float temp = 0.0;
    static uint8_t sp_count = 0, k = 0;
    static float speed_arr[10] = {0.0};                     /* 存储速度进行滤波运算 */

    if (sp_count == ms)                                     /* 计算一次速度 */
    {
        /* 计算电机转速 
           第一步 ：计算ms毫秒内计数变化量
           第二步 ；计算1min内计数变化量：g_encode.speed * ((1000 / ms) * 60 ，
           第三步 ：除以编码器旋转一圈的计数次数（倍频倍数 * 编码器分辨率）
           第四步 ：除以减速比即可得出电机转速
        */
        g_encode.encode_now = encode_now;                                /* 取出编码器当前计数值 */
        g_encode.speed = (g_encode.encode_now - g_encode.encode_old);    /* 计算编码器计数值的变化量 */
        
        speed_arr[k++] = (float)(g_encode.speed * 6000 );    /* 保存电机转速 */
        
        g_encode.encode_old = g_encode.encode_now;          /* 保存当前编码器的值 */

        /* 累计10次速度值，后续进行滤波*/
        if (k == 10)
        {
            for (i = 10; i >= 1; i--)                       /* 冒泡排序*/
            {
                for (j = 0; j < (i - 1); j++) 
                {
                    if (speed_arr[j] > speed_arr[j + 1])    /* 数值比较 */
                    { 
                        temp = speed_arr[j];                /* 数值换位 */
                        speed_arr[j] = speed_arr[j + 1];
                        speed_arr[j + 1] = temp;
                    }
                }
            }
            
            temp = 0.0;
            
            for (i = 2; i < 8; i++)                         /* 去除两边高低数据 */
            {
                temp += speed_arr[i];                       /* 将中间数值累加 */
            }
            
            temp = (float)(temp / 6);                       /*求速度平均值*/
            
            /* 一阶低通滤波
             * 公式为：Y(n)= qX(n) + (1-q)Y(n-1)
             * 其中X(n)为本次采样值；Y(n-1)为上次滤波输出值；Y(n)为本次滤波输出值，q为滤波系数
             * q值越小则上一次输出对本次输出影响越大，整体曲线越平稳，但是对于速度变化的响应也会越慢
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
 if(htim->Instance==htim2.Instance)		         //1ms中断
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







