#include "MBmasterTimer.h"
#include "MBMasterRtu.h"
#include "Timers.h"

#define RESPONS_TIMEOUT_TIMEMS		2000U

//#define USE_SOFT_TIME35				1
#ifdef USE_SOFT_TIME35
#define TIME35_TIMEMS				4U
#endif

static void TIM2_Configuration(void);
static void SoftTimer_init(void);
void MR_TimeoutISR(void);

static TIMERCALLBACK MRTimeOutHandler=NULL;
static TIMERINDEX  MRTimer=0;
#ifdef USE_SOFT_TIME35
static TIMERCALLBACK MRTIME35Handler=NULL;
static TIMERINDEX  MRT35Timer=0;
#endif
static void TIM2_Configuration(void)
{
  
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure; 
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE );
   /* Time Base configuration */
  TIM_DeInit(TIM2);
   /*
   通讯控制方式，波特率=9600，数据位8位，偶校验1位，1停止位，1起始位
   则每秒传输字节数=9600/11=872.7
   每字节占用传输周期=1/872.7=1.14583ms
   3.5个字节周期=4.0104ms
   */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 71;                   //设置预分频器分频系数71，即APB2=72M, TIM1_CLK=72/72=1MHz ，
                                                               //它的取值必须在0x0000和0xFFFF之间
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
  //TIM1_TimeBaseStructure.TIM_Period = 1000;	               // 1ms定时，计数器向上计数到1000后产生更新事件，计数值归零
  TIM_TimeBaseStructure.TIM_Period = 4011;	               // 4.0104ms定时	 它的取值必须在0x0000和0xFFFF之间
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;			   //设置了定时器时钟分割，
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;		  // 设置了周期计数器值，它的取值必须在0x00和0xFF之间。

  TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);	         // 根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
   
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
 
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;    //更新事件 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //抢占优先级0 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          //响应优先级2 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //允许中断 
  NVIC_Init(&NVIC_InitStructure);  
  
  TIM_Cmd(TIM2, DISABLE);

 
}


static void SoftTimer_init(void)
{
	MRTimeOutHandler=(TIMERCALLBACK)MR_TimeoutISR;
	if(MRTimer)
		{
			TIMER_ChangeFeq(MRTimer,RESPONS_TIMEOUT_TIMEMS);
		}
	else
		{
			MRTimer=TIMER_Create(RESPONS_TIMEOUT_TIMEMS,MRTimeOutHandler,T_ATTR_ONCE,T_STATE_STOP);
		}
	#ifdef USE_SOFT_TIME35
	MRTIME35Handler=(TIMERCALLBACK)TIM2_IRQHandler;
	if(MRT35Timer)
		{
			TIMER_ChangeFeq(MRT35Timer,TIME35_TIMEMS);
		}
	else
		{
			MRT35Timer=TIMER_Create(TIME35_TIMEMS,MRTIME35Handler,T_ATTR_ONCE,T_STATE_STOP);
		}
	#endif
}

void MRtimerInit(void)
{
	#ifndef USE_SOFT_TIME35
	TIM2_Configuration();
	#endif
	SoftTimer_init();
}



void MRtimer2Enable(void)
{
	#ifdef USE_SOFT_TIME35
		TIMER_ChangeFeq(MRT35Timer,TIME35_TIMEMS);
		TIMER_Enable(MRT35Timer);
	#else
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);      //清中断，以免一启用中断后立即产生中断   
		TIM_SetCounter(TIM2,0);
		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //使能TIM1中断源 

		TIM_Cmd(TIM2, ENABLE); 
	#endif
}
void MRtimer2Disable(void)
{
	#ifdef USE_SOFT_TIME35
		TIMER_Disable(MRT35Timer);
	#else
		TIM_SetCounter(TIM2,0);
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);      //清中断，以免一启用中断后立即产生中断   
		TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE); 

		TIM_Cmd(TIM2, DISABLE); 
	#endif
}


void TIM2_IRQHandler(void)
{	
	#ifndef USE_SOFT_TIME35
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //清中断
    #endif
    MBMR_timer2T35ISR();

}




void MRSofttimerEnable(void)
{
	TIMER_Enable(MRTimer);
}

void MRSofttimerDisable(void)
{
	TIMER_Disable(MRTimer);
}
void MR_TimeoutISR(void)
{
	MBMR_ResponsesTimeoutISR();
}


