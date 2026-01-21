#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "Timer.h"
#include "motor.h"
#include "Encode.h"
#include "OLED.h"
#include "string.h"
#include "Translation.h"

/*
轮1				轮2
	|--------|
	|        |
	|        |
	|        |
	|________|
轮3				轮4
*/
void test1(void);
void test2(void);
int32_t speed = 0;
int main(void)
{	
	delay_init();	    		//延时函数初始化	
	LED_Init();	
	USART1_Init(115200);		//ROS上位机通讯
	USART2_Init(9600); 			//RS485+ModBus
	RS485IOInit();
	USART3_Init(115200);		//IMU
	Timer1_Init(3599,20);		//1ms，分别为ros数据收发，RS485+ModBus接收转速
	Balance_Motor_Init();		//正反转gpio初始化
	PWM_Init(7200,0);			//100000hz，3600时，占空比达到50%，7200时为100%占空比   30%==3080   20%==2000
	Encoder_Init_TIM2();
	Encoder_Init_TIM3();
	Encoder_Init_TIM4();
	Encoder_Init_TIM5();		//编码器模式初始化
	while(1)
	{	
		test1();	
		test2();		
	}
}
void test1(void){				//转速测试，仅测试
		
//		Left_Front_Go();
		Left_Front_Back();
//		Left_Front_Stop();
		TIM_SetCompare1(TIM8, 1440);   //左前

//		Right_Front_Go();
		Right_Front_Back();		
//		Right_Front_Stop();
		TIM_SetCompare2(TIM8, 1440);   //右前
	
//		Left_Behind_Go();
		Left_Behind_Back();
//		Left_Behind_Stop();
		TIM_SetCompare3(TIM8, 1440);   //左后
		
//		Right_Behind_Go();
		Right_Behind_Back();
//		Right_Behind_Stop();
		TIM_SetCompare4(TIM8, 1440);   //右后
}

void test2(void){	
	
	UartDriverMe();
	if(DataReady == 1)
	{
		DataReady = 0;
		ParseMotorParameters(HolidingRegister, 0x0010, &motor_params1);
//		modbus_state = MODBUS_IDLE;
		#if debug_printf
//		PrintMotorParameters(&motor_params1);//调试用
		printf("PWM: %.1f%%\n", motor_params1.in1_pwm_duty);
		printf("speed: %d RPM\n", motor_params1.motor_speed);
        printf("\r\n");
		#endif
	}
	if(modbus_query_flag)
	{
		RS485_RW_Opr(0x01,0x03,0x10,0x24);
		//RS485_RW_Opr(0x02,0x03,0x10,0x24);
		//RS485_RW_Opr(0x03,0x03,0x10,0x24);
		//RS485_RW_Opr(0x04,0x03,0x10,0x24);
		modbus_query_flag = 0;
	}
	
}

