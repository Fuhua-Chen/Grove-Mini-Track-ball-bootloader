
#include "delay.h"

static unsigned char  u=0;
static unsigned int m=0;

//
//初始化systick
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
	//装载初值
   SysTick->LOAD=us*u;
  //清空计数器	
   SysTick->VAL=0x00;   							
   SysTick->CTRL|=0x01	;						 	
	 //开始倒数   
		do
		{
			temp=SysTick->CTRL;
		}
	//等待时间到达 
	while(temp&0x01&&!(temp&(1<<16)));	 
  //关闭计数器		
	SysTick->CTRL&=~0x01;      				 
	
}




void DelayMs(unsigned int ms)    
{
	unsigned int temp;
	uint16_t i;
	//延时 MS 
	for(i=0;i<ms;i++)      			
	{
		//时间加载1ms
		SysTick->LOAD=m;  
    //清空计数器		
		SysTick->VAL=0x00;    		
		SysTick->CTRL|=0x01;
    //开始倒数  		
		do
		{
			temp=SysTick->CTRL;
		}
		//等待时间到达
		while(temp&0x01&&!(temp&(1<<16)));
    //关闭计数器		
		SysTick->CTRL&=~0x01;             	
	}
}
