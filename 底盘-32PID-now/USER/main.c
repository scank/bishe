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
#include "ROS.h"

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
int main(void)
{	
	delay_init();	    		//延时函数初始化	
	LED_Init();	
	USART1_Init(115200);		//ROS上位机通讯
	USART2_Init(115200); 		//RS485+ModBus
	RS485IOInit();				
	USART3_Init(115200);		//IMU
	USART4_Init(115200);		//调试
	Timer1_Init(3599,20);		//1ms，分别为ros数据收发，RS485+ModBus接收转速
	Balance_Motor_Init();		//正反转gpio初始化
	PWM_Init(7200,0);			//100000hz，3600时，占空比达到50%，7200时为100%占空比   30%==3080   20%==2000
	while(1)
	{	
		test1();	
		test2();		
	}
}
void test1(void){				//转速测试，仅测试
		Process_RX_Packet();
		Moto_Ros_Pwm(1, ros_rx_packet.target_fl);
		Moto_Ros_Pwm(2, ros_rx_packet.target_fr);
		Moto_Ros_Pwm(3, ros_rx_packet.target_bl);
		Moto_Ros_Pwm(4, ros_rx_packet.target_br);
}

void test2(void){	
	UartDriverMe();
	if(DataReady > 0)
	{
		// 根据DataReady的值知道是哪个从机
		ParseSpeedOnly(HolidingRegister, DataReady);
		#if debug_printf
		printf("%d,%d\n",DataReady,MotorSpeed[DataReady - 1]);
		printf("\r\n");
		#endif
		DataReady = 0;
	}
	if(modbus_query_flag){
		switch(target_slave){
			case 1:RS485_RW_Opr(0x01, 0x03, 0x001E, 0x0002);break;
			case 2:RS485_RW_Opr(0x02, 0x03, 0x001E, 0x0002);break;
			case 3:RS485_RW_Opr(0x03, 0x03, 0x001E, 0x0002);break;
			case 4:RS485_RW_Opr(0x04, 0x03, 0x001E, 0x0002);break;
		}
		modbus_query_flag = 0;
	}
	if(Ros_Data_flag){
		SendROSData();
		Ros_Data_flag = 0;
	}		
	if(Imu_Data_flag){
		parseWT61HexData(buf3);
		Imu_Data_flag = 0;
	}
}

