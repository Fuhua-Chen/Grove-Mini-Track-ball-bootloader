#ifndef _DELAY_H_
#define _DELAY_H_


#include "stm32f10x_conf.h"


extern void DelayInit(void);
extern void DelayMs(unsigned int ms);
extern void DelayUs(unsigned int us);

#endif


