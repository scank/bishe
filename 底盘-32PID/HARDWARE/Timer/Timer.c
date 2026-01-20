#include "Timer.h"

u8 modbus_query_flag;
u16 timer_1 = 0;
u16 timer_2 = 0;
/*
	波特率 9600
	1位数据的时间为 1000000us/9600bit/s = 104us
	
	一个字节 11位=1起始位+8数据位+1校验位+1停止位
	104us*11位=1180us
	
	所以modbus确定一个数据帧完成的时间为：1180us*3.5=4ms
*/

void Timer1_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //时钟使能

  TIM_TimeBaseStructure.TIM_Period = arr; //设置自动重装载寄存器周期值
  TIM_TimeBaseStructure.TIM_Prescaler =(psc-1);//设置预分频值
  TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;//重复计数设置
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //参数初始化
  TIM_ClearFlag(TIM1, TIM_FLAG_Update);//清中断标志位

  TIM_ITConfig(      //使能或者失能指定的TIM中断
    TIM1,            //TIM1
    TIM_IT_Update  | //TIM 更新中断源
    TIM_IT_Trigger,  //TIM 触发中断源 
    ENABLE  	     //使能
    );
	
  //设置优先级
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//先占优先级0级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  	   //从优先级0级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 

  TIM_Cmd(TIM1, ENABLE);  //使能TIMx外设
}

// 定时器1更新中断服务函数
void TIM1_UP_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
		// 在这里添加1ms中断的处理代码
		UartRxMonitor(1);
		timer_1++;
		timer_2++;
		if(timer_1 >= 300)
		{
			timer_1 = 0;
			// 在这里添加20ms中断的处理代码
			encoder_car();
			modbus_query_flag = 1;
		}
		if(timer_2 >= 500)
		{
			timer_2 = 0;
			LED1 = !LED1;
			
//			printf("%.1f ,%.1f ,%.1f ,%.1f ,%.2f ,%.2f ,%.2f ,%.2f ,%.2f ,%.2f \r\n",
//					speed_fl_rpm, speed_fr_rpm, speed_bl_rpm, speed_br_rpm,
//					fAcc[0],fAcc[1],fAcc[2],fAngle[0],fAngle[1],fAngle[2]);
//			printf("%.1f ,%.1f ,%.1f ,%.1f\r\n",
//					speed_fl_rpm, speed_fr_rpm, speed_bl_rpm, speed_br_rpm);
			
		}
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update); // 清除中断标志位
    }
}

