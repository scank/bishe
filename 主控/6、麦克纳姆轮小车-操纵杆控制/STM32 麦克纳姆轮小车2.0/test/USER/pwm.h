#ifndef __PWM_H
#define __PWM_H 			   
#include "sys.h"  

/*
目前配置：
TIM4
1-4通道： B6-B9
控制引脚：
A4 5 11 12  ,B12 13 14 15
*/
#define MOTOR_TIM									TIM4
#define MOTOR_TIM_CLK							RCC_APB1Periph_TIM4
#define MOTOR_TIM_PSC							72
#define MOTOR_TIM_PERIOD					1024    //1000    //周期1ms

#define MOTOR_TIM_CH1_GPIO_CLK		RCC_APB2Periph_GPIOB
#define MOTOR_TIM_CH1_GPIO_PORT		GPIOB
#define MOTOR_TIM_CH1_GPIO_PIN		GPIO_Pin_6

#define MOTOR_TIM_CH2_GPIO_CLK		RCC_APB2Periph_GPIOB
#define MOTOR_TIM_CH2_GPIO_PORT		GPIOB
#define MOTOR_TIM_CH2_GPIO_PIN		GPIO_Pin_7

#define MOTOR_TIM_CH3_GPIO_CLK		RCC_APB2Periph_GPIOB
#define MOTOR_TIM_CH3_GPIO_PORT		GPIOB
#define MOTOR_TIM_CH3_GPIO_PIN		GPIO_Pin_8

#define MOTOR_TIM_CH4_GPIO_CLK		RCC_APB2Periph_GPIOB
#define MOTOR_TIM_CH4_GPIO_PORT		GPIOB
#define MOTOR_TIM_CH4_GPIO_PIN		GPIO_Pin_9




#define MOTOR_ctrl11_GPIO_CLK				RCC_APB2Periph_GPIOA
#define MOTOR_ctrl11_GPIO_PORT				GPIOA
#define MOTOR_ctrl11_GPIO_PIN				GPIO_Pin_4
#define MOTOR_ctrl12_GPIO_CLK				RCC_APB2Periph_GPIOA
#define MOTOR_ctrl12_GPIO_PORT				GPIOA
#define MOTOR_ctrl12_GPIO_PIN				GPIO_Pin_5

#define MOTOR_ctrl21_GPIO_CLK				RCC_APB2Periph_GPIOA
#define MOTOR_ctrl21_GPIO_PORT				GPIOA
#define MOTOR_ctrl21_GPIO_PIN				GPIO_Pin_11
#define MOTOR_ctrl22_GPIO_CLK				RCC_APB2Periph_GPIOA
#define MOTOR_ctrl22_GPIO_PORT				GPIOA
#define MOTOR_ctrl22_GPIO_PIN				GPIO_Pin_12

#define MOTOR_ctrl31_GPIO_CLK				RCC_APB2Periph_GPIOB
#define MOTOR_ctrl31_GPIO_PORT				GPIOB
#define MOTOR_ctrl31_GPIO_PIN				GPIO_Pin_12
#define MOTOR_ctrl32_GPIO_CLK				RCC_APB2Periph_GPIOB
#define MOTOR_ctrl32_GPIO_PORT				GPIOB
#define MOTOR_ctrl32_GPIO_PIN				GPIO_Pin_13

#define MOTOR_ctrl41_GPIO_CLK				RCC_APB2Periph_GPIOB
#define MOTOR_ctrl41_GPIO_PORT				GPIOB
#define MOTOR_ctrl41_GPIO_PIN				GPIO_Pin_14
#define MOTOR_ctrl42_GPIO_CLK				RCC_APB2Periph_GPIOB
#define MOTOR_ctrl42_GPIO_PORT				GPIOB
#define MOTOR_ctrl42_GPIO_PIN				GPIO_Pin_15

void TIM_pwm_Init(void) ;
void TIM_ctrl_IO_init(void)   ;  
void channel1(u16 num);      //占空比
void channel2(u16 num) ;     //占空比
void channel3(u16 num)  ;    //占空比
void channel4(u16 num)   ;   //占空比
void all_channels(u16 num);   //占空比
void channel1_shun(void);
void channel1_ni(void);
void channel2_shun(void);
void channel2_ni(void);
void channel3_shun(void);
void channel3_ni(void);
void channel4_shun(void);		
void channel4_ni(void);
void channel1_stop(void);
void channel2_stop(void);
void channel3_stop(void);
void channel4_stop(void);







#endif





























