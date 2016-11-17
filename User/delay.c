
#include "delay.h"

static unsigned char  u=0;
static unsigned int m=0;

//
//��ʼ��systick
//
void DelayInit(void)
{
	 
	 SysTick_CLKSourceConfig(SystemCoreClock/1000);
	 u = SystemCoreClock/1000000;
	 m = (unsigned int)u*1000;
}


void DelayUs(unsigned int us)
{
	unsigned int temp;
	//װ�س�ֵ
   SysTick->LOAD=us*u;
  //��ռ�����	
   SysTick->VAL=0x00;   							
   SysTick->CTRL|=0x01	;						 	
	 //��ʼ����   
		do
		{
			temp=SysTick->CTRL;
		}
	//�ȴ�ʱ�䵽�� 
	while(temp&0x01&&!(temp&(1<<16)));	 
  //�رռ�����		
	SysTick->CTRL&=~0x01;      				 
	
}




void DelayMs(unsigned int ms)    
{
	unsigned int temp;
	uint16_t i;
	//��ʱ MS 
	for(i=0;i<ms;i++)      			
	{
		//ʱ�����1ms
		SysTick->LOAD=m;  
    //��ռ�����		
		SysTick->VAL=0x00;    		
		SysTick->CTRL|=0x01;
    //��ʼ����  		
		do
		{
			temp=SysTick->CTRL;
		}
		//�ȴ�ʱ�䵽��
		while(temp&0x01&&!(temp&(1<<16)));
    //�رռ�����		
		SysTick->CTRL&=~0x01;             	
	}
}
