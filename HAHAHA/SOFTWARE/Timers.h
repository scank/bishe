#ifndef _TIMERS_H
#define _TIMERS_H
#include "stm32f10x.h"

#define TIMER_STATE_MASK		3
#define TIMER_ATTR_MASK			1

#define TimerState(State)		(State & TIMER_STATE_MASK)
#define TimerAttr(Attr)			(Attr & TIMER_ATTR_MASK)

#define NULL  			(void*)0

typedef u8 TIMERINDEX;

typedef enum __TIMERSTATE
{
	T_STATE_NONE=0,
	T_STATE_STOP=1,
	T_STATE_RUN=2

}TIMERSTATE;

typedef enum __TIMERATTR
{
	T_ATTR_ONCE=0,
	T_ATTR_REPEAT=1
	
}TIMERATTR;

typedef void (*TIMERCALLBACK)(void);

typedef struct __TIMERSERVIC
{
	u32 uiTimerCount;
	u32 uiCurCount;
	TIMERCALLBACK pCallBack;
	TIMERATTR TimerAttr;
	TIMERSTATE TimerState;
	
}TIMERSERVIC,*PTIMERSERVIC;


TIMERINDEX TIMER_Create(u32 uiTimerCnt,TIMERCALLBACK pHandler,TIMERATTR TimerAttrib,TIMERSTATE TimerSta);
void TIMER_ArrayInit(void);
void TIMER_Delet(TIMERINDEX TimerIndex);
void TIMER_Enable(TIMERINDEX TimerIndex);
void TIMER_Disable(TIMERINDEX TimerIndex);
void TIMER_ChangeFeq(TIMERINDEX TimerIndex,u32 uiTimerCnt);
void TIMER_Servic_Timer1_ISR(void);//called by timer1 interrupt



#endif
