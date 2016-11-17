#include "main.h"


#define IsConfirmPressed GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)

void GotoApp(void)
{		

	//RESET ALL PERIPHERALS
	RCC->APB1RSTR |= 0XFFFFFFFF;
	RCC->APB2RSTR |= 0XFFFFFFFF;
	RCC->APB1RSTR &= ~0XFFFFFFFF;
	RCC->APB2RSTR &= ~0XFFFFFFFF;	
	
	__set_MSP(Msp);
	((void(*)())(Reset))();
}



void SysInit(void)
{
  SystemClockInit();	
	DelayInit();
	GPIOInit(GPIOB,GPIO_Pin_0,GPIO_Mode_IPU);
	Serialport_Init(115200);
}




int main(void)
{
	  SysInit();
    FLASH_Unlock();
	
    if (IsConfirmPressed  == 0x00)
    {
        SerialPutString("\r\n======================================================================");
        SerialPutString("\r\n=                                                                    =");
        SerialPutString("\r\n=          IAP FOR Grove - Mini Track ball  (Version 1.0.0)          =");
        SerialPutString("\r\n=                                                                    =");
        SerialPutString("\r\n======================================================================");
        SerialPutString("\r\n\r\n");
        Main_Menu ();
    }
    //否则执行用户程序
    else
    {
        //判断用户是否已经下载程序，因为正常情况下此地址是栈地址。
        //若没有这一句的话，即使没有下载程序也会进入而导致跑飞。
        if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
        {
            SerialPutString("Execute user Program\r\n\n");
            GotoApp();
        }
        else
        {
            SerialPutString("no user Program\r\n\n");
        }
    }

    while (1)
    {
    }
}


