#include "myprint.h"
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

 
 
 
/*****************  �����ַ��������¶��� **********************/
void Usart_SendString(uint8_t *str)
{
    unsigned int k=0;
  do 
  {
      HAL_UART_Transmit(&huart1,(uint8_t *)(str + k) ,1,1000);
      k++;
  } while(*(str + k)!='\0');
  
}
///�ض���c�⺯��printf������DEBUG_USART���ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
    /* ����һ���ֽ����ݵ�����DEBUG_USART */
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 1000);    
    
    return (ch);
}
 
///�ض���c�⺯��scanf������DEBUG_USART����д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
        
    int ch;
    HAL_UART_Receive(&huart1, (uint8_t *)&ch, 1, 1000);    
    return (ch);
}




//HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
