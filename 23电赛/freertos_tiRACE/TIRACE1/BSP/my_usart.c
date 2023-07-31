#include "my_usart.h"
uint16_t USART_RX_STA=0;       //����״̬���	

uint8_t aRxBuffer[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���

 uint8_t USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
 
/*************Printf�ض���UART**************/
/*��;��
*ʹ��printf����ӡ����ͨ�����ڷ��͡� 
*ԭ��
*��C���Կ⺯���е�fputc ��fgetc�������¶���UART����
*ʹ�÷�����ע�����
*ʹ��ǰ����Ҫ��main.c�����ͷ�ļ� "stdio.h""uart_printf.h" �����keil��������ͬʱ��Ҫ��ѡ��й�ѡ��ʹ��΢�⡱
*
*Ĭ��ʹ��USART1���ڣ� ����Ҫʹ���������ڵ�ʱ�� �޸�huart1 ΪĿ�괮�ڣ� 
*����ʹ��CubeMX���ɵĴ��룬����ֱ���޸�huart1����ı�ż���ֱ��ʹ��
******************************************/
///////////////////////////////////////////////////////////Ҫ��ħ�������΢��

/*****************  �����ַ��������¶��� **********************/
void Usart_SendString(uint8_t *str)
{
    unsigned int k=0;
  do 
  {
      HAL_UART_Transmit(&huart4,(uint8_t *)(str + k) ,1,1000);
      k++;
  } while(*(str + k)!='\0');
  
}
///�ض���c�⺯��printf������DEBUG_USART���ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
    /* ����һ���ֽ����ݵ�����DEBUG_USART */
    HAL_UART_Transmit(&huart4, (uint8_t *)&ch, 1, 1000);    
    
    return (ch);
}
 
///�ض���c�⺯��scanf������DEBUG_USART����д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
        
    int ch;
    HAL_UART_Receive(&huart4, (uint8_t *)&ch, 1, 1000);    
    return (ch);
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	#if find_road_moda
	if(huart->Instance==UART5)//����Ǵ���1
	{
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(aRxBuffer[0]!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(aRxBuffer[0]==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=aRxBuffer[0] ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}

	}
	#endif
}



#include "stdint.h"
 
int32_t  Output=0;
void str2int(uint8_t* str, char flag, int32_t no, int32_t* Output)
{
	int32_t id_end, count, output;
	for (id_end = 0, count = 0; count != no; ++id_end)
	{
		if (str[id_end] == flag || str[id_end] == '\r' || str[id_end] == '\n' || str[id_end] == '\0')
			++count;
	}
	id_end -= 2;
	for (output = 0, count = 1; str[id_end] != flag && id_end >= 0; --id_end)
	{
		if (str[id_end] == '-')
			output *= -1;
		else if (str[id_end] == '+')
			;
		else
		{
			output += (str[id_end] - '0') * count;
			count *= 10;
		}
	}
	*Output = output;
}
