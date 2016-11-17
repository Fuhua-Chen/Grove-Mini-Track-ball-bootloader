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
    //����ִ���û�����
    else
    {
        //�ж��û��Ƿ��Ѿ����س�����Ϊ��������´˵�ַ��ջ��ַ��
        //��û����һ��Ļ�����ʹû�����س���Ҳ�����������ܷɡ�
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


