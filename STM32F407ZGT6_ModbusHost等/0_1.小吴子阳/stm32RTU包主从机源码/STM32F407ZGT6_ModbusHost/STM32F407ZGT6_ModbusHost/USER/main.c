
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "rs485_task.h"
#include "led/bsp_led.h"
#include "rs485/bsp_rs485.h"
#include "mbhost/bsp_mbhost.h"
#include "TimBase/bsp_TiMbase.h"

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);		//延时初始化 
	uart_init(115200);	    //串口1初始化
    RS485_init(115200);     //RS485初始化
    LED_Init();             //LED初始化
    TIM5_Init(5000,8400-1); //定时器初始化
    
	while(1)
	{
        rs485_task();
        delay_ms(500); 
	}
}

