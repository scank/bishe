#ifndef __MOTOR_H
#define __MOTOR_H
#include "stm32f10x.h"                  // Device header"
#include "sys.h"

#define PULSE_PER_ROUND  17    // 编码器线数
#define REDUCTION_RATIO  27    // 减速比
#define WHEEL_DIAMETER   0.077f// 轮子直径(m)
#define PI               3.1415926f

void Balance_Motor_Init(void);
void PWM_Init(u16 arr,u16 psc);//初始化PWM 10KHZ 高频可以防止电机低频时的尖叫声  7200 1
int Myabs(int a);
void MotorControl(char direction,int left_front_MotorPWM, int right_front_MotorPWM,int left_behind_MotorPWM, int right_behind_MotorPWM);



void Left_Front_Go(void);	 // 左前轮正转——AIN
void Right_Front_Go(void);	 // 右前轮正转——BIN 
void Left_Behind_Go(void);    // 左后轮正转——CIN
void Right_Behind_Go(void);   // 右后轮正转——DIN

void Left_Front_Back(void);   // 左前轮反转——AIN
void Right_Front_Back(void);  // 右前轮反转——BIN
void Left_Behind_Back(void);  // 左后轮反转——CIN
void Right_Behind_Back(void); // 右后轮反转——DIN
	
void Left_Front_Stop(void);   // 左前轮停止——AIN
void Right_Front_Stop(void);  // 右前轮停止——BIN
void Left_Behind_Stop(void);  // 左后轮停止——CIN
void Right_Behind_Stop(void); // 右后轮停止——DIN





/********************************************************** 
             TIM1    TIM2    TIM3    TIM4     TIM5    TIM8
CH1          PA8    PA0     PA6     PB6      PA0     PC6
CH2          PA9    PA1     PA7     PB7      PA1     PC7
CH3          PA10   PA2     PB0     PB8      PA2     PC8
CH4          PA11   PA3     PB1     PB9      PA3     PC9
注：这里TIM2和TIM5引脚配置一样
    如果要同时使用TIM2和TIM5的话可以把TIM2进行端口复用，
        在RCC_APB1PeriphClockCmd(TIMX_CLKCmd, ENABLE);//TIM2--TIM5（TIM2,TIM3,TIM4,TIM5）
        下方加入RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);即可
        然后把TIM2或者TIM5的引脚改成复用引脚
下面是TIM2的复用引脚
TIM2:
    CH1  PA5   PA15
    CH2  PB3
    CH3  PB10
    CH4  PB11
    
***********************************************************/
#endif
