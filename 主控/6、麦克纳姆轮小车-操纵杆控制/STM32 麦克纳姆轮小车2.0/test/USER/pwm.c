#include "pwm.h"

void TIM_pwm_Init(void) //                                          //
{
	TIM_TimeBaseInitTypeDef TIM_Timeinit;
  TIM_OCInitTypeDef TIM_OCInitstruct;
	
		GPIO_InitTypeDef    GPIO_InitStrucyure;
	
	RCC_APB2PeriphClockCmd(MOTOR_TIM_CH1_GPIO_CLK,ENABLE); // 使能GPIOB
        GPIO_InitStrucyure.GPIO_Mode=GPIO_Mode_AF_PP;        //改为复用推挽输出GPIO_Mode_AF_PP
        GPIO_InitStrucyure.GPIO_Pin=MOTOR_TIM_CH1_GPIO_PIN;
        GPIO_InitStrucyure.GPIO_Speed=GPIO_Speed_50MHz;
        GPIO_Init(MOTOR_TIM_CH1_GPIO_PORT,&GPIO_InitStrucyure);                 //初始化io口PB6
	
	RCC_APB2PeriphClockCmd(MOTOR_TIM_CH2_GPIO_CLK,ENABLE); // 使能GPIOB
	      GPIO_InitStrucyure.GPIO_Mode=GPIO_Mode_AF_PP;        //改为复用推挽输出GPIO_Mode_AF_PP
        GPIO_InitStrucyure.GPIO_Pin=MOTOR_TIM_CH2_GPIO_PIN;
        GPIO_InitStrucyure.GPIO_Speed=GPIO_Speed_50MHz;
        GPIO_Init(MOTOR_TIM_CH2_GPIO_PORT,&GPIO_InitStrucyure);                 //初始化io口PB6

	RCC_APB2PeriphClockCmd(MOTOR_TIM_CH3_GPIO_CLK,ENABLE); // 使能GPIOB
        GPIO_InitStrucyure.GPIO_Mode=GPIO_Mode_AF_PP;        //改为复用推挽输出GPIO_Mode_AF_PP
        GPIO_InitStrucyure.GPIO_Pin=MOTOR_TIM_CH3_GPIO_PIN;
        GPIO_InitStrucyure.GPIO_Speed=GPIO_Speed_50MHz;
        GPIO_Init(MOTOR_TIM_CH3_GPIO_PORT,&GPIO_InitStrucyure);                 //初始化io口PB6

	RCC_APB2PeriphClockCmd(MOTOR_TIM_CH4_GPIO_CLK,ENABLE); // 使能GPIOB
        GPIO_InitStrucyure.GPIO_Mode=GPIO_Mode_AF_PP;        //改为复用推挽输出GPIO_Mode_AF_PP
        GPIO_InitStrucyure.GPIO_Pin=MOTOR_TIM_CH4_GPIO_PIN;
        GPIO_InitStrucyure.GPIO_Speed=GPIO_Speed_50MHz;
        GPIO_Init(MOTOR_TIM_CH4_GPIO_PORT,&GPIO_InitStrucyure);                 //初始化io口PB6
		
   RCC_APB1PeriphClockCmd(MOTOR_TIM_CLK,ENABLE);  //使能定时器4时钟
	TIM_Timeinit.TIM_ClockDivision = TIM_CKD_DIV1 ;       //
	TIM_Timeinit.TIM_CounterMode =  TIM_CounterMode_Up;                 //计数模式
	TIM_Timeinit.TIM_Prescaler = MOTOR_TIM_PSC;           //72mhz/1 = 72Mhz
	TIM_Timeinit.TIM_Period = MOTOR_TIM_PERIOD;		//  72MHZ /(360) = 200KHZ 为计数总值,200KHZ为PWM波的频率
	TIM_TimeBaseInit(MOTOR_TIM, &TIM_Timeinit); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  TIM_ARRPreloadConfig(MOTOR_TIM, ENABLE); //使能 TIMx 在 ARR 上的预装载寄存器
 			
	TIM_Cmd(MOTOR_TIM,ENABLE);  //使能定时器4
	
	TIM_OCInitstruct.TIM_OCMode =   TIM_OCMode_PWM1 ;   //模式1   cnt<CCRX 有效  >  无效
	TIM_OCInitstruct.TIM_Pulse = MOTOR_TIM_PERIOD/5;         //占空比 ，比较值，写CCRx     50%  反正只是计脉冲的个数
	TIM_OCInitstruct.TIM_OCPolarity  = TIM_OCPolarity_High ;   //   有效为高
	TIM_OCInitstruct.TIM_OutputState = TIM_OutputState_Enable ;        // 比较输出使能     
	TIM_OC1Init(MOTOR_TIM, &TIM_OCInitstruct);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx O
	TIM_OC1PreloadConfig(MOTOR_TIM,TIM_OCPreload_Enable);//使能输出比较预装载OC1
	
	TIM_OCInitstruct.TIM_OCMode =   TIM_OCMode_PWM1 ;   //模式1   cnt<CCRX 有效  >  无效
	TIM_OCInitstruct.TIM_Pulse = MOTOR_TIM_PERIOD/5;         //占空比 ，比较值，写CCRx     50%  反正只是计脉冲的个数
	TIM_OCInitstruct.TIM_OCPolarity  = TIM_OCPolarity_High ;   //   有效为高
	TIM_OCInitstruct.TIM_OutputState = TIM_OutputState_Enable ;        // 比较输出使能     
	TIM_OC2Init(MOTOR_TIM, &TIM_OCInitstruct);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx O
	TIM_OC2PreloadConfig(MOTOR_TIM,TIM_OCPreload_Enable);//使能输出比较预装载OC1
	
	TIM_OCInitstruct.TIM_OCMode =   TIM_OCMode_PWM1 ;   //模式1   cnt<CCRX 有效  >  无效
	TIM_OCInitstruct.TIM_Pulse = MOTOR_TIM_PERIOD/5;         //占空比 ，比较值，写CCRx     50%  反正只是计脉冲的个数
	TIM_OCInitstruct.TIM_OCPolarity  = TIM_OCPolarity_High ;   //   有效为高
	TIM_OCInitstruct.TIM_OutputState = TIM_OutputState_Enable ;        // 比较输出使能     
	TIM_OC3Init(MOTOR_TIM, &TIM_OCInitstruct);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx O
	TIM_OC3PreloadConfig(MOTOR_TIM,TIM_OCPreload_Enable);//使能输出比较预装载OC1
	
	TIM_OCInitstruct.TIM_OCMode =   TIM_OCMode_PWM1 ;   //模式1   cnt<CCRX 有效  >  无效
	TIM_OCInitstruct.TIM_Pulse = MOTOR_TIM_PERIOD/5;         //占空比 ，比较值，写CCRx     50%  反正只是计脉冲的个数
	TIM_OCInitstruct.TIM_OCPolarity  = TIM_OCPolarity_High ;   //   有效为高
	TIM_OCInitstruct.TIM_OutputState = TIM_OutputState_Enable ;        // 比较输出使能     
	TIM_OC4Init(MOTOR_TIM, &TIM_OCInitstruct);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx O
	TIM_OC4PreloadConfig(MOTOR_TIM,TIM_OCPreload_Enable);//使能输出比较预装载OC1
}
void TIM_ctrl_IO_init()     
{
	GPIO_InitTypeDef    GPIO_InitStrucyure;

		RCC_APB2PeriphClockCmd(MOTOR_ctrl11_GPIO_CLK,ENABLE); 
	GPIO_InitStrucyure.GPIO_Mode=GPIO_Mode_Out_PP;        
    GPIO_InitStrucyure.GPIO_Pin=MOTOR_ctrl11_GPIO_PIN;
    GPIO_InitStrucyure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(MOTOR_ctrl11_GPIO_PORT,&GPIO_InitStrucyure);     
		
		RCC_APB2PeriphClockCmd(MOTOR_ctrl12_GPIO_CLK,ENABLE); 
	GPIO_InitStrucyure.GPIO_Mode=GPIO_Mode_Out_PP;        
    GPIO_InitStrucyure.GPIO_Pin=MOTOR_ctrl12_GPIO_PIN;
    GPIO_InitStrucyure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(MOTOR_ctrl12_GPIO_PORT,&GPIO_InitStrucyure);     

		RCC_APB2PeriphClockCmd(MOTOR_ctrl21_GPIO_CLK,ENABLE); 
	GPIO_InitStrucyure.GPIO_Mode=GPIO_Mode_Out_PP;        
    GPIO_InitStrucyure.GPIO_Pin=MOTOR_ctrl21_GPIO_PIN;
    GPIO_InitStrucyure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(MOTOR_ctrl21_GPIO_PORT,&GPIO_InitStrucyure);     
		
		RCC_APB2PeriphClockCmd(MOTOR_ctrl22_GPIO_CLK,ENABLE); 
	GPIO_InitStrucyure.GPIO_Mode=GPIO_Mode_Out_PP;        
    GPIO_InitStrucyure.GPIO_Pin=MOTOR_ctrl22_GPIO_PIN;
    GPIO_InitStrucyure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(MOTOR_ctrl22_GPIO_PORT,&GPIO_InitStrucyure);    
		
		

		
	RCC_APB2PeriphClockCmd(MOTOR_ctrl31_GPIO_CLK,ENABLE); 
	GPIO_InitStrucyure.GPIO_Mode=GPIO_Mode_Out_PP;        
    GPIO_InitStrucyure.GPIO_Pin=MOTOR_ctrl31_GPIO_PIN;
    GPIO_InitStrucyure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(MOTOR_ctrl31_GPIO_PORT,&GPIO_InitStrucyure);             

	RCC_APB2PeriphClockCmd(MOTOR_ctrl32_GPIO_CLK,ENABLE); 
	GPIO_InitStrucyure.GPIO_Mode=GPIO_Mode_Out_PP;        
    GPIO_InitStrucyure.GPIO_Pin=MOTOR_ctrl32_GPIO_PIN;
    GPIO_InitStrucyure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(MOTOR_ctrl32_GPIO_PORT,&GPIO_InitStrucyure);     

	RCC_APB2PeriphClockCmd(MOTOR_ctrl41_GPIO_CLK,ENABLE); 
	GPIO_InitStrucyure.GPIO_Mode=GPIO_Mode_Out_PP;        
    GPIO_InitStrucyure.GPIO_Pin=MOTOR_ctrl41_GPIO_PIN;
    GPIO_InitStrucyure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(MOTOR_ctrl41_GPIO_PORT,&GPIO_InitStrucyure);     
	
	RCC_APB2PeriphClockCmd(MOTOR_ctrl42_GPIO_CLK,ENABLE); 
	GPIO_InitStrucyure.GPIO_Mode=GPIO_Mode_Out_PP;        
    GPIO_InitStrucyure.GPIO_Pin=MOTOR_ctrl42_GPIO_PIN;
    GPIO_InitStrucyure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(MOTOR_ctrl42_GPIO_PORT,&GPIO_InitStrucyure);     
}
void channel1(u16 num)      //占空比    宏定义决定，最终0-1024     调用范围0-2048
{
			TIM_SetCompare1(MOTOR_TIM, num/3);      //占空比
}
void channel2(u16 num)      //占空比
{
			TIM_SetCompare2(MOTOR_TIM, num/3);      //占空比
}
void channel3(u16 num)      //占空比
{
			TIM_SetCompare3(MOTOR_TIM, num/3);      //占空比
}
void channel4(u16 num)      //占空比
{
			TIM_SetCompare4(MOTOR_TIM, num/3);      //占空比
}
void all_channels(u16 num)   //占空比
{
	TIM_SetCompare1(MOTOR_TIM, num/3);      //占空比
	TIM_SetCompare2(MOTOR_TIM, num/3);      //占空比
	TIM_SetCompare3(MOTOR_TIM, num/3);      //占空比
	TIM_SetCompare4(MOTOR_TIM, num/3);      //占空比	
}

void channel1_shun(void)
{
				GPIO_ResetBits(MOTOR_ctrl11_GPIO_PORT, MOTOR_ctrl11_GPIO_PIN );    
				GPIO_SetBits(MOTOR_ctrl12_GPIO_PORT, MOTOR_ctrl12_GPIO_PIN);  
}
void channel1_ni(void)
{
				GPIO_SetBits(MOTOR_ctrl11_GPIO_PORT, MOTOR_ctrl11_GPIO_PIN );    
				GPIO_ResetBits(MOTOR_ctrl12_GPIO_PORT, MOTOR_ctrl12_GPIO_PIN);  
}
void channel2_shun(void)
{
				GPIO_ResetBits(MOTOR_ctrl21_GPIO_PORT, MOTOR_ctrl21_GPIO_PIN );    
				GPIO_SetBits(MOTOR_ctrl22_GPIO_PORT, MOTOR_ctrl22_GPIO_PIN);  
}
void channel2_ni(void)
{
				GPIO_SetBits(MOTOR_ctrl21_GPIO_PORT, MOTOR_ctrl21_GPIO_PIN );    
				GPIO_ResetBits(MOTOR_ctrl22_GPIO_PORT, MOTOR_ctrl22_GPIO_PIN);  
}
void channel3_shun(void)
{
				GPIO_ResetBits(MOTOR_ctrl31_GPIO_PORT, MOTOR_ctrl31_GPIO_PIN );    
				GPIO_SetBits(MOTOR_ctrl32_GPIO_PORT, MOTOR_ctrl32_GPIO_PIN);  
}
void channel3_ni(void)
{
				GPIO_SetBits(MOTOR_ctrl31_GPIO_PORT, MOTOR_ctrl31_GPIO_PIN );    
				GPIO_ResetBits(MOTOR_ctrl32_GPIO_PORT, MOTOR_ctrl32_GPIO_PIN);  
}
void channel4_shun(void)
{
				GPIO_ResetBits(MOTOR_ctrl41_GPIO_PORT, MOTOR_ctrl41_GPIO_PIN );    
				GPIO_SetBits(MOTOR_ctrl42_GPIO_PORT, MOTOR_ctrl42_GPIO_PIN);  
}
void channel4_ni(void)
{
				GPIO_SetBits(MOTOR_ctrl41_GPIO_PORT, MOTOR_ctrl41_GPIO_PIN );    
				GPIO_ResetBits(MOTOR_ctrl42_GPIO_PORT, MOTOR_ctrl42_GPIO_PIN);  
}
void channel1_stop(void)
{
				GPIO_ResetBits(MOTOR_ctrl11_GPIO_PORT, MOTOR_ctrl11_GPIO_PIN );    
				GPIO_ResetBits(MOTOR_ctrl12_GPIO_PORT, MOTOR_ctrl12_GPIO_PIN);  
}
void channel2_stop(void)
{
				GPIO_ResetBits(MOTOR_ctrl21_GPIO_PORT, MOTOR_ctrl21_GPIO_PIN );    
				GPIO_ResetBits(MOTOR_ctrl22_GPIO_PORT, MOTOR_ctrl22_GPIO_PIN);  
}
void channel3_stop(void)
{
				GPIO_ResetBits(MOTOR_ctrl31_GPIO_PORT, MOTOR_ctrl31_GPIO_PIN );    
				GPIO_ResetBits(MOTOR_ctrl32_GPIO_PORT, MOTOR_ctrl32_GPIO_PIN);  
}
void channel4_stop(void)
{
				GPIO_ResetBits(MOTOR_ctrl41_GPIO_PORT, MOTOR_ctrl41_GPIO_PIN );    
				GPIO_ResetBits(MOTOR_ctrl42_GPIO_PORT, MOTOR_ctrl42_GPIO_PIN);  
}















