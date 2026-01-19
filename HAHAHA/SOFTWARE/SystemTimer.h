#ifndef _SYSTEMTIMER_H
#define _SYSTEMTIMER_H
#include "stm32f10x.h"

void Timer1_Init(void);
void Timer1_Enable(void);
void Timer1_Disable(void);
u32 SYST_GetSystemTime(void);
void TIM1_UP_IRQHandler(void);

#endif
