#ifndef _MBMASTER_TIMER_H
#define _MBMASTER_TIMER_H
#include "stm32f10x.h"

void MRtimerInit(void);

void MRtimer2Enable(void);
void MRtimer2Disable(void);
void TIM2_IRQHandler(void);

void MRSofttimerEnable(void);
void MRSofttimerDisable(void);



#endif
