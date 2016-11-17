#include "software_uart.h"
#include "delay.h"
#include "init.h"

Serialport_TypeDef Serialport = {9600,1,0,0,0};

#define RX_Start();  {EXTI->IMR &= ~EXTI_Line11;EXTI->EMR &= ~EXTI_Line11;NVIC_DisableIRQ(EXTI15_10_IRQn);TIM_Cmd(RX_TIMER,ENABLE);}
#define RX_End();    {EXTI->IMR |= EXTI_Line11;EXTI->EMR |= EXTI_Line11;NVIC_EnableIRQ(EXTI15_10_IRQn);TIM_Cmd(RX_TIMER,DISABLE);RX_TIMER->CNT=0;}


#define TX_Start();  {GPIO_ResetBits(SERIALPORT,TX_PIN);TIM_Cmd(TX_TIMER,ENABLE);}
//end1 for end single
#define TX_End1();    {GPIO_SetBits(SERIALPORT,TX_PIN);}
//end2 for end transmit
#define TX_End2();    {TIM_Cmd(TX_TIMER,DISABLE);TX_TIMER->CNT=0;}


/*---------------------------------------------------------------------------------------//
extern interrupt for Rx start single

*/
void RxExtiInit(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	
	EXTI_DeInit();
	
	GPIOInit(SERIALPORT,RX_PIN,GPIO_Mode_IPU);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	EXTI_ClearITPendingBit(EXTI_Line11);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);
	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	EXTI_ClearITPendingBit(EXTI_Line11);
}


/*---------------------------------------------------------------------------------------//
Serialport init

*/
void Serialport_Init(unsigned int Baudrate)
{
	Serialport.Buadrate = Baudrate;
	
	TimerInit(TX_TIMER,(1000000/Baudrate));
	NVIC_EnableIRQ(TIM2_IRQn);
	
	TimerInit(RX_TIMER,(1000000/Baudrate));
	NVIC_EnableIRQ(TIM3_IRQn);
	
	RxExtiInit();
	
	GPIOInit(SERIALPORT,TX_PIN,GPIO_Mode_Out_PP);
}



/*---------------------------------------------------------------------------------------//
extern interrupt handler for Rx start single

*/
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line11)!= RESET)      
   {     
		 RX_Start();
	   EXTI_ClearITPendingBit(EXTI_Line11);
	 }	

}


/*---------------------------------------------------------------------------------------//
Timer interrupt for Rx

*/
void RX_Handler(void)
{
	static unsigned char i = 0;
	if(TIM_GetITStatus(RX_TIMER,TIM_IT_Update))
	{
		TIM_ClearITPendingBit(RX_TIMER,TIM_FLAG_Update);
		//Wait the end single;
		if(i==8)
		{
			if((GPIO_ReadInputDataBit(SERIALPORT,RX_PIN) & (0x01))==1)
			{
				i = 0;
				Serialport.IsRxOk = 1;
				RX_End();
			}
		}
		else
		{
			//Receive 8 Bit data;
			Serialport.RxData |= ((GPIO_ReadInputDataBit(SERIALPORT,RX_PIN) & (0x01))<<i);
			i++;
		}
	}
}


/*---------------------------------------------------------------------------------------//
receive byte 

*/
unsigned char Serialport_ReceiveByte(void)
{
	unsigned char tmp;
	tmp = Serialport.RxData;
	Serialport.RxData = 0 ;
	Serialport.IsRxOk = 0;
	return tmp;
}



/*---------------------------------------------------------------------------------------//
Timer interrupt for Tx
*/
void TX_Handler(void)
{
	static unsigned char i = 0;
	if(TIM_GetITStatus(TX_TIMER,TIM_IT_Update))
	{
		TIM_ClearITPendingBit(TX_TIMER,TIM_FLAG_Update);
		//Send end single
		if(i==8)
		{
			i++;
			TX_End1();
		}
		//End line
		else if(i == 9)
		{
			i = 0;
			Serialport.TxData = 0;
			Serialport.IsTxOk = 1;
			TX_End2();
		}
		//Send 8Bit Data
		else
		{
			if((Serialport.TxData & 0X01) == 1)
			{
				GPIO_SetBits(SERIALPORT,TX_PIN);
			}
			else
			{
				GPIO_ResetBits(SERIALPORT,TX_PIN);
			}
			Serialport.TxData >>= 1;
			i++;
	  }
	}
}


/*---------------------------------------------------------------------------------------//
SendByte

*/
void Serialport_SendByte(unsigned char value)
{
	Serialport.TxData = value;
	Serialport.IsTxOk = 0;
	TX_Start();
}
