#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f10x_tim.h"
#include "Encode.h"
#include "usart.h"	
#include "modbus.h"

void Timer1_Init(u16 arr,u16 psc);
#endif
