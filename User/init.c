#include "init.h"

/*------------------------------------------------------------------------------------------------------------------------//
systick init for timing to Clear

*/
void SystickInit(void)
{
	SysTick_Config(SystemCoreClock/1000);
}


/*------------------------------------------------------------------------------------------------------------------------//
clock init


*/
void SystemClockInit(void)
{
  RCC_DeInit();     
  RCC_HSEConfig(RCC_HSE_ON);
	while(!RCC_WaitForHSEStartUp());
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	FLASH_SetLatency(FLASH_Latency_2); 
	RCC_HCLKConfig(RCC_SYSCLK_Div1); 
	RCC_PCLK2Config(RCC_HCLK_Div1); 
	RCC_PCLK1Config(RCC_HCLK_Div2);              
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_4);   //HSE VALUE IS 16M
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while(RCC_GetSYSCLKSource() != 0x08);
	SystemCoreClock = 64000000;	
}



/*------------------------------------------------------------------------------------------------------------------------//
GPIO init


*/
void GPIOInit(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIOMode_TypeDef Mod)
{
  
	GPIO_InitTypeDef GPIO_MyInit;
	switch((uint32_t)GPIOx)
	{
	  case (uint32_t)GPIOA:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);break;
	  case (uint32_t)GPIOB:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);break;
		case (uint32_t)GPIOC:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);break;
		case (uint32_t)GPIOD:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);break;
		case (uint32_t)GPIOE:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);break;
	}
	GPIO_MyInit.GPIO_Pin   =  GPIO_Pin;
	GPIO_MyInit.GPIO_Mode  =  Mod;
	GPIO_MyInit.GPIO_Speed =  GPIO_Speed_50MHz;
	GPIO_Init(GPIOx,&GPIO_MyInit);

}


/*------------------------------------------------------------------------------------------------------------------------//
Timer init


*/
void TimerInit(TIM_TypeDef* Timer , uint16_t us)
{
	TIM_TimeBaseInitTypeDef TimerBaseStructure;
	switch((uint32_t) Timer)
	{
		case (uint32_t) TIM2 : RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);break;
 		case (uint32_t) TIM3 : RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);break;
		case (uint32_t) TIM4 : RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);break;
		case (uint32_t) TIM5 : RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);break;	
  }
	TIM_DeInit(Timer);
	TIM_InternalClockConfig(Timer);
	//SystemCoreClock/36=2M as 0.5us 
	TimerBaseStructure.TIM_Prescaler = (SystemCoreClock/2000000)-1;
	TimerBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TimerBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TimerBaseStructure.TIM_Period = 2*us;
	TIM_TimeBaseInit(Timer,&TimerBaseStructure);
	TIM_ClearFlag(Timer , TIM_FLAG_Update);
	TIM_ARRPreloadConfig(Timer,DISABLE);
	TIM_ITConfig(Timer,TIM_IT_Update,ENABLE);
	TIM_Cmd(Timer,DISABLE);
}
