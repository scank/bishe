#include "motor.h"
/*
轮1				轮2
	|--------|
	|        |
	|        |
	|        |
	|________|
轮3				轮4
*/

/*
pwm信号的产生是tim8 对应如下
以及电机的正反转，
#define PWMA   TIM4->CCR3  PC9//pwm1
#define PWMB   TIM4->CCR4  PC8//pwm2

#define AIN1   PFout(2)//正2
#define AIN2   PFout(3)//反2
#define BIN1   PFout(1)//反1
#define BIN2   PFout(0)//正1
#define STBY1   PFout(8)//这个现在不需要

#define PWMC   TIM4->CCR3  PC7//pwm3
#define PWMD   TIM4->CCR4  PC6//pwm4

#define CIN1   PFout(5)//反3
#define CIN2   PFout(4)//正3
#define DIN1   PFout(6)//正4
#define DIN2   PFout(7)//反4
#define STBY1   PFout(9)//这个现在不需要

*/

void Balance_Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE); //使能PF端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;	//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
	GPIO_Init(GPIOF, &GPIO_InitStructure);					      
}


void PWM_Init (u16 arr,u16 psc)//初始化PWM 10KHZ 高频可以防止电机低频时的尖叫声  7200 1
{
    
    GPIO_InitTypeDef GPIO_InitStructure;			//GPIO结构体
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;	//TIMER配置结构体
    TIM_OCInitTypeDef TIM_OCInitStructure;			//TIMER使能结构体
 
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);//TIM8,使能定时器8时钟
	//PWM端口1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //使能GPIOC外设模块时钟   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    //PWM端口2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    //PWM端口3
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    //PWM端口4
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//TIME配置模式
	TIM_TimeBaseStructure.TIM_Period = arr;//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = psc;//设置用来作为TIMx时钟频率除数的预分频值 
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;//设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //比较输出使能
    TIM_OCInitStructure.TIM_Pulse = 0;	//CCR
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset ;//TIM8关闭空闲状态(使用TIM8时候开启)

	//TIME使能
    TIM_OC1Init(TIM8, &TIM_OCInitStructure);//根据T指定的参数初始化外设TIM8 OC1
    TIM_OC2Init(TIM8, &TIM_OCInitStructure);//根据T指定的参数初始化外设TIM8 OC2
    TIM_OC3Init(TIM8, &TIM_OCInitStructure);//根据T指定的参数初始化外设TIM8 OC3
    TIM_OC4Init(TIM8, &TIM_OCInitStructure);//根据T指定的参数初始化外设TIM8 OC4
    
    TIM_CtrlPWMOutputs(TIM8, ENABLE);//通用定时器不用配置，但是高级定时器是需要的
    
    TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);//使能TIM8在CCR1上的预装载寄存器
    TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);//使能TIM8在CCR2上的预装载寄存器
    TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);//使能TIM8在CCR3上的预装载寄存器
    TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);//使能TIM8在CCR4上的预装载寄存器
    
    TIM_ARRPreloadConfig(TIM8, ENABLE);//enable和disable都是无所谓的，因为在初始化的时候就固定了arr的值，不会再改变了
    
    TIM_Cmd(TIM8, ENABLE); //使能TIM8
	TIM_SetCompare1(TIM8,0);    //防止上电就乱动
	TIM_SetCompare2(TIM8,0);    //防止上电就乱动
	TIM_SetCompare3(TIM8,0);    //防止上电就乱动
	TIM_SetCompare4(TIM8,0);    //防止上电就乱动

}
/*绝对值函数*/
int Myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}

//***********电机控制*******************************
//pwm: 电机pwm
//n:   电机编号		
// 各轮子分别正转
void Left_Front_Go(void)     // 左前轮正转——AIN
{
	GPIO_SetBits(GPIOF,GPIO_Pin_0);
	GPIO_ResetBits(GPIOF,GPIO_Pin_1);
}

void Right_Front_Go(void)    // 右前轮正转——BIN
{
	GPIO_SetBits(GPIOF,GPIO_Pin_2);
	GPIO_ResetBits(GPIOF,GPIO_Pin_3);
}
void Left_Behind_Go(void)    // 左后轮正转——CIN
{
	GPIO_SetBits(GPIOF,GPIO_Pin_4);
	GPIO_ResetBits(GPIOF,GPIO_Pin_5);
}
void Right_Behind_Go(void)   // 右后轮正转——DIN
{
	GPIO_SetBits(GPIOF,GPIO_Pin_6);
	GPIO_ResetBits(GPIOF,GPIO_Pin_7);
}

//各轮子分别反转
void Left_Front_Back(void)   // 左前轮反转——AIN
{
	GPIO_ResetBits(GPIOF,GPIO_Pin_0);	
	GPIO_SetBits(GPIOF,GPIO_Pin_1);
}

void Right_Front_Back(void)  // 右前轮反转——BIN
{
	GPIO_ResetBits(GPIOF,GPIO_Pin_2);	
	GPIO_SetBits(GPIOF,GPIO_Pin_3);
}
void Left_Behind_Back(void)  // 左后轮反转——CIN
{
	GPIO_ResetBits(GPIOF,GPIO_Pin_4);	
	GPIO_SetBits(GPIOF,GPIO_Pin_5);
}

void Right_Behind_Back(void) // 右后轮反转——DIN
{
	GPIO_ResetBits(GPIOF,GPIO_Pin_6);	
	GPIO_SetBits(GPIOF,GPIO_Pin_7);
}

// 各轮子分别停止
void Left_Front_Stop(void)   // 左前轮停止——AIN
{
	GPIO_ResetBits(GPIOF,GPIO_Pin_0);
	GPIO_ResetBits(GPIOF,GPIO_Pin_1);	
}

void Right_Front_Stop(void)  // 右前轮停止——BIN
{
	GPIO_ResetBits(GPIOF,GPIO_Pin_2);
	GPIO_ResetBits(GPIOF,GPIO_Pin_3);	
}
void Left_Behind_Stop(void)  // 左后轮停止——CIN
{
	GPIO_ResetBits(GPIOF,GPIO_Pin_4);
	GPIO_ResetBits(GPIOF,GPIO_Pin_5);	
}
void Right_Behind_Stop(void) // 右后轮停止——DIN
{
	GPIO_ResetBits(GPIOF,GPIO_Pin_6);
	GPIO_ResetBits(GPIOF,GPIO_Pin_7);	
}

/**
  *  @brief 控制麦克纳姆小车的各种运动方式
  *  @param 运动方向，四个电机的PWM值
  *  @retval None
  */
void MotorControl(char direction,int left_front_MotorPWM, int right_front_MotorPWM,int left_behind_MotorPWM, int right_behind_MotorPWM)
{
  switch(direction)
  {
    case 0: // 停止
      Left_Front_Stop();
      Right_Front_Stop();
      Left_Behind_Stop();
      Right_Behind_Stop();
      left_front_MotorPWM = 0;
      right_front_MotorPWM = 0;
      left_behind_MotorPWM = 0;
      right_behind_MotorPWM = 0;break;
    case 1: // 前进
      Left_Front_Go();
      Right_Front_Go();
      Left_Behind_Go();
      Right_Behind_Go();break;
    case 2: // 后退
      Left_Front_Back();
      Right_Front_Back();
      Left_Behind_Back();
      Right_Behind_Back();break;
    case 3: // 左移
      Left_Front_Back();
      Right_Front_Go();
      Left_Behind_Go();
      Right_Behind_Back();break;
    case 4: // 右移
      Left_Front_Go();
      Right_Front_Back();
      Left_Behind_Back();
      Right_Behind_Go();break;
    case 5: // 左旋
      Left_Front_Back();
      Right_Front_Go();
      Left_Behind_Back();
      Right_Behind_Go();break;
    case 6: // 右旋
      Left_Front_Go();
      Right_Front_Back();
      Left_Behind_Go();
      Right_Behind_Back();break;
    case 7: // 左前方移动
      Left_Front_Stop();
      Right_Front_Go();
      Left_Behind_Go();
      Right_Behind_Stop();break;
    case 8: // 右前方移动
      Left_Front_Go();
      Right_Front_Stop();
      Left_Behind_Stop();
      Right_Behind_Go();break;
    case 9: // 左后方移动
      Left_Front_Back();
      Right_Front_Stop();
      Left_Behind_Stop();
      Right_Behind_Back();break;
    case 10: // 右后方移动
      Left_Front_Stop();
      Right_Front_Back();
      Left_Behind_Back();
      Right_Behind_Stop();break;
    case 11: // 绕前轴中点左旋
      Left_Front_Stop();
      Right_Front_Stop();
      Left_Behind_Back();
      Right_Behind_Go();break;
    case 12: // 绕前轴中点右旋
      Left_Front_Stop();
      Right_Front_Stop();
      Left_Behind_Go();
      Right_Behind_Back();break;
    case 13: // 绕后轴中点左旋
      Left_Front_Back();
      Right_Front_Go();
      Left_Behind_Stop();
      Right_Behind_Stop();break;
    case 14: // 绕后轴中点右旋
      Left_Front_Go();
      Right_Front_Back();
      Left_Behind_Stop();
      Right_Behind_Stop();break;
    default:break;
  }
	TIM_SetCompare1(TIM8, left_front_MotorPWM);   
	TIM_SetCompare2(TIM8, right_front_MotorPWM);  
	TIM_SetCompare3(TIM8, left_behind_MotorPWM);  
	TIM_SetCompare4(TIM8, right_behind_MotorPWM); 
}


