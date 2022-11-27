#include "pstwo.h"
#include "usart.h"

u16 Handkey;
u8 Comd[2]={0x01,0x42};	//��ʼ�����������
u8 Data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //���ݴ洢����
u16 MASK[]={
    PSB_SELECT,
    PSB_L3,
    PSB_R3 ,
    PSB_START,
	
    PSB_PAD_UP,
    PSB_PAD_RIGHT,
    PSB_PAD_DOWN,
    PSB_PAD_LEFT,
	
    PSB_L2,
    PSB_R2,
    PSB_L1,
    PSB_R1,
	
    PSB_GREEN,
    PSB_RED,
    PSB_BLUE,
    PSB_PINK
	};	//����ֵ�밴����

//�ֱ��ӿڳ�ʼ��    ����  DI->PB12 
//                  ���  DO->PB13    CS->PB0  CLK->PB1
void PS2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTA,PORTCʱ��

	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;//PB12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PB12���ó�����	Ĭ������  
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB12
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13	;//PB13
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //������� 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz	
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB13
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PB0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB0
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;//PB1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB1
	
}

//���ֱ���������
void PS2_Cmd(u8 CMD)
{
	//volatile���ױ��ͱ������Ƿ�ֹ�������Ż�����ʱ�������������ֵ����֤ÿ��С�ĵ����¶�ȡֵ��
	volatile u16 ref=0x01;
	Data[1] = 0;
	//ref�ı仯��һ����λ����������Ψһһ��1��λ�ñ仯�������λ�����λ�ƶ�����0000 0001��1000 0000��
	for(ref=0x01;ref<0x0100;ref<<=1)//
	{
	//��ref��1��λ�ö�ӦCMD�е�λ����ҲΪ1ʱ�����Ϊ1����ref��1��λ�ö�ӦCMD�е�λ����Ϊ0ʱ�����Ϊ0��CMD������λ��Ӱ��˽����
		if(ref&CMD)
		{
			DO_H;                   //�����Ϊ����λ
		}
		else DO_L;
		//������Ϊ1ʱ��DO_H�����1��������Ϊ0ʱ��DO_L�����0�����forѭ���˴Σ�DO�Ľ�����ǽ�CMD��ÿһλ�����˹�ȥ��
		
		CLK_H;                        //ʱ������
		delay_us(50);
		CLK_L;
		delay_us(50);
		CLK_H;    //�ֶ�����һ���½���ʹDO��DI����ͬʱ����
		/*
		�������ֶԽ��յ���DI�����жϣ���DIΪ1ʱ�����ð�λ�����������Data[1]��ʼֵΪ0000 0000��
		�Լ���λ��Ķ��壬�������ref | Data[1]�õ��µ�Data[1]�Ĺ����ǣ�ref���Ψһ��1��ֵ����
		λ�ò������ʽ��������Ķ��������У�����ĳһ��ѭ��Data[1] = 0000 0010��ref = 0000 1000,
		��DI=1����ref | Data[1]=0000 1010���������1�Ĳ�����ֻ����һbit��DI=1ʱ�Ż���У���DI=0��
		��refֻ����1����λ�������裬����ʵҲ���൱����һλref�Ǹ�����0��Data[1]��������ʵ�ж�DI��
		ִ�дӾ����һ��������forѭ����Ľ�����ǽ�8 bit��DI��λ���浽Data[1]��		
		*/
		if(DI)
			Data[1] = ref|Data[1];//���û����㰴λ����Data[1]��8λ
	}
}
//�ж��Ƿ�Ϊ���ģʽ
//����ֵ��0�����ģʽ
//		  ����������ģʽ
u8 PS2_RedLight(void)
{
	CS_L;
	PS2_Cmd(Comd[0]);  //��ʼ����
	PS2_Cmd(Comd[1]);  //��������
	CS_H;
	if( Data[1] == 0X73)   return 0 ;
	else return 1;

}
//��ȡ�ֱ�����
void PS2_ReadData(void)
{
	//volatile���ױ��ͱ������Ƿ�ֹ�������Ż�����ʱ�������������ֵ����֤ÿ��С�ĵ����¶�ȡֵ��
	volatile u8 byte=0;
	volatile u16 ref=0x01;

	CS_L;

	PS2_Cmd(Comd[0]);  //��ʼ����
	PS2_Cmd(Comd[1]);  //��������

	for(byte=2;byte<9;byte++)          //��ʼ��������
	{
		for(ref=0x01;ref<0x0100;ref<<=1)
		{
			CLK_H;
			CLK_L;
			delay_us(50);
			CLK_H;
		      if(DI)
		      Data[byte] = ref|Data[byte];
		}
        delay_us(50);
	}
	CS_H;	
}

//�Զ�������PS2�����ݽ��д���      ֻ�����˰�������         Ĭ�������Ǻ��ģʽ  ֻ��һ����������ʱ
//����Ϊ0�� δ����Ϊ1
u8 PS2_DataKey()
{
	u8 index;

	PS2_ClearData();
	PS2_ReadData();

	Handkey=(Data[4]<<8)|Data[3]; //����16������  ����Ϊ0�� δ����Ϊ1
	for(index=0;index<16;index++)//ѭ��16�Σ�����һ������������
	{	    
		if((Handkey&(1<<(MASK[index]-1)))==0)
		return index+1;
	}
	return 0;          //û���κΰ�������
}

//�õ�һ��ҡ�˵�ģ����	 ��Χ0~256
u8 PS2_AnologData(u8 button)
{
	return Data[button];
}

//������ݻ�����
void PS2_ClearData()
{
	u8 a;
	for(a=0;a<9;a++)
		Data[a]=0x00;
}




