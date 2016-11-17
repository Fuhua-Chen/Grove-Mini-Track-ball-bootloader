#ifndef _SOFTWARE_UART_
#define _SOFTWARE_UART_

#include "stm32f10x_conf.h"
#include "init.h"

#define SERIALPORT GPIOB
#define TX_PIN GPIO_Pin_10
#define RX_PIN GPIO_Pin_11

#define TX_TIMER TIM2
#define RX_TIMER TIM3

#define TX_Handler TIM2_IRQHandler
#define RX_Handler TIM3_IRQHandler


typedef struct
{
	
	unsigned int Buadrate;
	unsigned char IsTxOk;
	unsigned char IsRxOk;
	unsigned char TxData;
	unsigned char RxData;

}Serialport_TypeDef;



extern Serialport_TypeDef Serialport;

void Serialport_Init(unsigned int Baudrate);
unsigned char Serialport_ReceiveByte(void);
void Serialport_SendByte(unsigned char value);

#endif

