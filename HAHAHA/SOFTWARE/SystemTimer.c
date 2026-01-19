#include "SystemTimer.h"
#include "Timers.h"


static u32 g_uiSYSTIME=0;

static void TIM1_Configuration(void);

static void TIM1_Configuration(void)
{
  
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure; 
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE );
   /* Time Base configuration */
  TIM_DeInit(TIM1);

  TIM_TimeBaseStructure.TIM_Prescaler = 71;                   //设置预分频器分频系数71，即APB2=72M, TIM1_CLK=72/72=1MHz ，
                                                               //它的取值必须在0x0000和0xFFFF之间
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
  TIM_TimeBaseStructure.TIM_Period = 1000;	               // 1ms定时，计数器向上计数到1000后产生更新事件，计数值归零
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;			   //设置了定时器时钟分割，
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;		  // 设置了周期计数器值，它的取值必须在0x00和0xFF之间。

  TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);	         // 根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
   
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
 
  NVIC_InitStructure.NVIC_IRQChannel =TIM1_UP_IRQn;    //更新事件 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //抢占优先级0 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          //响应优先级2 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //允许中断 
  NVIC_Init(&NVIC_InitStructure);  
  
  TIMER_ArrayInit();
  Timer1_Enable();

 
}
void Timer1_Enable(void)
{

	TIM_ClearFlag(TIM1, TIM_FLAG_Update);      //清中断，以免一启用中断后立即产生中断   
	TIM_SetCounter(TIM1,0);
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE); //使能TIM1中断源 

	TIM_Cmd(TIM1, ENABLE); 
}
void Timer1_Disable(void)
{
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);      //清中断，以免一启用中断后立即产生中断   
	TIM_SetCounter(TIM1,0);
	TIM_ITConfig(TIM1, TIM_IT_Update, DISABLE); //使能TIM1中断源 

	TIM_Cmd(TIM1, DISABLE); 
}

void Timer1_Init(void)
{
	TIM1_Configuration();
}


u32 SYST_GetSystemTime(void)
{
	return g_uiSYSTIME;
}

void TIM1_UP_IRQHandler(void)
{	
	TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update); //清中断
	g_uiSYSTIME++;
	TIMER_Servic_Timer1_ISR();
    

}


