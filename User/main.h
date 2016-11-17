#ifndef _MAIN_H_
#define _MAIN_H_


#include "init.h"
#include "delay.h"
#include "software_uart.h"
#include "common.h"
#include "ymodem.h"

#define APP_ADDR ApplicationAddress
#define Reset  *(uint32_t*)(APP_ADDR+4)
#define Msp    *((uint32_t*)APP_ADDR) 

void GotoApp(void);
#endif

