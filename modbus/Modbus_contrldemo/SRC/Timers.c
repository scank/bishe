#include "Timers.h"


#define TIMERS_MAX_NUM		18U



TIMERSERVIC TimersArray[TIMERS_MAX_NUM];



void TIMER_ArrayInit(void)
{
	TIMERINDEX i=0;
	PTIMERSERVIC ptimer=TimersArray;
	for(i=0;i<TIMERS_MAX_NUM;i++)
		{
			ptimer[i].uiTimerCount=0;
			ptimer[i].uiCurCount=0;
			ptimer[i].pCallBack=NULL;
			ptimer[i].TimerState=T_STATE_NONE;
			ptimer[i].TimerAttr=T_ATTR_ONCE;
		}
}



TIMERINDEX TIMER_Create(u32 uiTimerCnt,TIMERCALLBACK pHandler,TIMERATTR TimerAttrib,TIMERSTATE TimerSta)
{
	TIMERINDEX i=0;
	PTIMERSERVIC ptimer=TimersArray;
	for(i=0;i<TIMERS_MAX_NUM;i++)
		{
			if(ptimer[i].TimerState==T_STATE_NONE)
				{
					ptimer[i].uiTimerCount=uiTimerCnt;
					ptimer[i].uiCurCount=uiTimerCnt;
					ptimer[i].TimerState=TimerState(TimerSta);
					ptimer[i].TimerAttr=TimerAttr(TimerAttrib);
					ptimer[i].pCallBack=pHandler;
					break;
				}
		}

	if(i<TIMERS_MAX_NUM)
		{
			return i+1;
		}
	else
		{
			return 0;
		}
	
}


void TIMER_Delet(TIMERINDEX TimerIndex)
{
	TIMERINDEX i=TimerIndex-1;
	PTIMERSERVIC ptimer=TimersArray;
	
	if(i>=0 && i<TIMERS_MAX_NUM)
		{
			ptimer[i].uiTimerCount=0;
			ptimer[i].uiCurCount=0;
			ptimer[i].pCallBack=NULL;
			ptimer[i].TimerState=T_STATE_NONE;
			ptimer[i].TimerAttr=T_ATTR_ONCE;
		}
}


void TIMER_Enable(TIMERINDEX TimerIndex)
{
	TIMERINDEX i=TimerIndex-1;
	PTIMERSERVIC ptimer=TimersArray;
	
	if(i>=0 && i<TIMERS_MAX_NUM)
		{
			ptimer[i].uiCurCount=ptimer[i].uiTimerCount;
			ptimer[i].TimerState=T_STATE_RUN;

		}
}


void TIMER_Disable(TIMERINDEX TimerIndex)
{
	TIMERINDEX i=TimerIndex-1;
	PTIMERSERVIC ptimer=TimersArray;
	
	if(i>=0 && i<TIMERS_MAX_NUM)
		{
			
			ptimer[i].TimerState=T_STATE_STOP;

		}
}


void TIMER_ChangeFeq(TIMERINDEX TimerIndex,u32 uiTimerCnt)
{
	TIMERINDEX i=TimerIndex-1;
	PTIMERSERVIC ptimer=TimersArray;
	//timer1 interrupte disable
	TIM_ITConfig(TIM1, TIM_IT_Update, DISABLE); 
	if(i>=0 && i<TIMERS_MAX_NUM)
		{
			
			ptimer[i].uiTimerCount=uiTimerCnt;
			ptimer[i].uiCurCount=uiTimerCnt;

		}
	//timer1 interrupte enable
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE); 
}



void TIMER_Servic_Timer1_ISR(void)
{
	TIMERINDEX i=0;
	PTIMERSERVIC ptimer=TimersArray;
	for(i=0;i<TIMERS_MAX_NUM;i++)
		{
			if(ptimer[i].TimerState==T_STATE_RUN)
				{
					if(ptimer[i].uiCurCount--<=0)
						{
							if(ptimer[i].TimerAttr==T_ATTR_REPEAT)
								{
									ptimer[i].uiCurCount=ptimer[i].uiTimerCount;
									ptimer[i].pCallBack();
								}
							else
								{

									ptimer[i].uiCurCount=ptimer[i].uiTimerCount;
									ptimer[i].TimerState=T_STATE_STOP;

									ptimer[i].pCallBack();

								}
						}

				}
		}

}



