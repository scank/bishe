#include "stm32f10x.h"                  // Device header"
#include "Encode.h"

int16_t encoder_fl, encoder_fr, encoder_bl, encoder_br;
float speed_fl_rpm, speed_fr_rpm, speed_bl_rpm, speed_br_rpm;

// 定时器2
void Encoder_Init_TIM2(void)//4
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_ICInitTypeDef TIM_ICInitStruct;
  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 ,ENABLE);  // 开启定时器时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE); // 开启GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);			//开启GPIOB的时钟
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);							//将PA15引脚初始化为上拉输入
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);							//将PB3引脚初始化为上拉输入
  
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;        // 不分频
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;    // 向上计数
	TIM_TimeBaseInitStruct.TIM_Period = 65535;                      // 重装载值65535
	TIM_TimeBaseInitStruct.TIM_Prescaler =0;                        // 分频系数0
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
  
	TIM_ICStructInit(&TIM_ICInitStruct);
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;				
	TIM_ICInitStruct.TIM_ICFilter = 0xF;							
	TIM_ICInit(TIM2, &TIM_ICInitStruct);							
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;				
	TIM_ICInitStruct.TIM_ICFilter = 0xF;							
	TIM_ICInit(TIM2, &TIM_ICInitStruct);				
	
	// 编码器配置函数: 定时器2，模式3，上升沿
	TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);  

	TIM_ClearFlag(TIM2,TIM_FLAG_Update);      // 清除定时器标志位
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);  // 定时器2，溢出更新，使能
	TIM_SetCounter(TIM2,0);                   // 定时数据清零
	TIM_Cmd(TIM2,ENABLE);                     // 定时器2使能
}
// 定时器3
void Encoder_Init_TIM3(void)//2
{ 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;//配置输入捕获单元
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
        
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;        //ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;        //PSC
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
    
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICFilter = 0xF;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICFilter = 0xF;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
   
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);      // 清除定时器标志位
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);  // 定时器2，溢出更新，使能
	TIM_SetCounter(TIM3,0);                   // 定时数据清零
    TIM_Cmd(TIM3, ENABLE);
}

// 定时器4
void Encoder_Init_TIM4(void)//3
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_ICInitTypeDef TIM_ICInitStruct;
  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 ,ENABLE);  // 开启定时器时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE); // 开启GPIO时钟
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;    // 浮空输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;   // 编码器1:PA0/PA1
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB ,&GPIO_InitStruct);	
  
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;        // 不分频
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;    // 向上计数
	TIM_TimeBaseInitStruct.TIM_Period = 65535;                      // 重装载值65535
	TIM_TimeBaseInitStruct.TIM_Prescaler =0;                        // 分频系数0
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);

	TIM_ICStructInit(&TIM_ICInitStruct);
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM4, &TIM_ICInitStruct);
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStruct.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM4, &TIM_ICInitStruct);
	
	// 编码器配置函数: 定时器2，模式3，上升沿
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising); 
	
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);      // 清除定时器标志位
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);  // 定时器2，溢出更新，使能
	TIM_SetCounter(TIM4,0);                   // 定时数据清零
	TIM_Cmd(TIM4,ENABLE);                     // 定时器2使能
}
// 定时器5
void Encoder_Init_TIM5(void)//1
{ 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;		
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			//开启GPIOA的时钟
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);							//将PA0和PA1引脚初始化为上拉输入
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;               
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;                
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);             
	
	TIM_ICStructInit(&TIM_ICInitStructure);							
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;			
	TIM_ICInitStructure.TIM_ICFilter = 0xF;							
	TIM_ICInit(TIM5, &TIM_ICInitStructure);							
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;				
	TIM_ICInitStructure.TIM_ICFilter = 0xF;							
	TIM_ICInit(TIM5, &TIM_ICInitStructure);							


	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_ClearFlag(TIM5,TIM_FLAG_Update);      // 清除定时器标志位
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);  // 定时器5，溢出更新，使能
	TIM_SetCounter(TIM5,0);                   // 定时数据清零
    TIM_Cmd(TIM5, ENABLE);
}

/**************************************************************************
函数功能：单位时间读取编码器计数
入口参数：定时器
返回  值：速度值
**************************************************************************/
int16_t Read_Encoder(u8 TIMX)
{
   int16_t Encoder_TIM;    
   switch(TIMX)
	 {
		case 2:  Encoder_TIM = -(short)TIM2 -> CNT;  TIM2 -> CNT=0;break;
		case 3:  Encoder_TIM =  (short)TIM3 -> CNT;  TIM3 -> CNT=0;break;	
		case 4:  Encoder_TIM = -(short)TIM4 -> CNT;  TIM4 -> CNT=0;break;	
		case 5:  Encoder_TIM = (short)TIM5 -> CNT;  TIM5 -> CNT=0;break;	
		default: Encoder_TIM = 0;
	 }
		return Encoder_TIM;
}

//******************编码器数据读取********************************
int Encoder_Value(TIM_TypeDef* TIMx)
{ 
	int channal_val=0;
	
	channal_val = TIMx ->CNT;
	if(channal_val>>15)
	{			
		channal_val =  (channal_val&0x7FFF)-32767;
	}	
  return channal_val;
}

//****************编码器清零*************************************
void Encoder_Count_Clear(TIM_TypeDef* TIMx)
{
  TIMx ->CNT = 0;
}


void encoder_car(void)
{
	encoder_fl = Encoder_Value(TIM5);	// 左前轮 → TIM5  轮1
	encoder_fr = Encoder_Value(TIM3);	// 右前轮 → TIM3  轮2
	encoder_bl = Encoder_Value(TIM4);	// 左后轮 → TIM4  轮3
	encoder_br = Encoder_Value(TIM2);	// 右后轮 → TIM2  轮4
	//清零
	Encoder_Count_Clear(TIM2);
	Encoder_Count_Clear(TIM3);
	Encoder_Count_Clear(TIM4);
	Encoder_Count_Clear(TIM5);
	// 3. 代入公式计算四轮真实转速(rpm)
	// 通用公式：电机实际转速 (rpm) = (编码器脉冲数 × 60) / (编码器线数×4 × 减速比 × 采样时间(s))
	speed_fl_rpm = ( (int32_t)encoder_fl * 750 ) / ( PULSE_PER_ROUND * REDUCTION_RATIO );
	speed_fr_rpm = ( (int32_t)encoder_fr * 750 ) / ( PULSE_PER_ROUND * REDUCTION_RATIO );
	speed_bl_rpm = ( (int32_t)encoder_bl * 750 ) / ( PULSE_PER_ROUND * REDUCTION_RATIO );
	speed_br_rpm = ( (int32_t)encoder_br * 750 ) / ( PULSE_PER_ROUND * REDUCTION_RATIO );
}
