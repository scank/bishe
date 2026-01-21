#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f10x_tim.h"
#include "Encode.h"
#include "usart.h"	
#include "modbus.h"
#include "led.h"
extern u8 modbus_query_flag;
extern ModbusState_t modbus_state;
void Timer1_Init(u16 arr,u16 psc);
#endif
