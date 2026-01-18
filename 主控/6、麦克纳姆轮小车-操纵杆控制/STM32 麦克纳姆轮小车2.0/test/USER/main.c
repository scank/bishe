#include "sys.h"
#include "delay.h"
#include "led.h"
#include "usart.h"
#include "pwm.h"

//#include <math.h>
//2.0
//改动：打算串口2接收x、y的电阻值，这边解析后控制小车状态速度。
/*指令：
#1234,3333*    对比中心点（2048,2048）平移车，   有解析方向和速度
#2234*        对比2048     小就自身逆时针旋转  大顺时针     也有速度的
在中心点圆圈内（有半径），车子不动

//	 if(dat=='r')   {you();    }
//	 if(dat=='l')   {zuo();     }
//	 if(dat=='s')   {stop();     }
//	 if(dat=='d')  { hou();     }
//	 if(dat=='u')  {qian();        }
//	 
//	 if(dat=='1')   {zuoshang();    }
//	 if(dat=='2')   {youshang();     }
//	 if(dat=='3')   {zuoxia();     }
//	 if(dat=='4')  { youxia();     }
//	 if(dat=='5')   {     ni();  }
//	 if(dat=='6')  {  	shun();    }	 
//	 
//	 if(dat=='A')   { speed=300;  }
//	 if(dat=='B')   { speed=400;  }
//	 if(dat=='C')   { speed=600;  }
//	 if(dat=='D')  { speed=800;  }
*/
	void  qian(void)
	{
	channel1_shun();  
	channel2_shun();  
	channel3_shun();  
	channel4_shun();  
	}
		void  hou(void)
	{
	channel1_ni();  
	channel2_ni();  
	channel3_ni();  
	channel4_ni();  
	}
		void  zuo(void)
	{
	channel1_shun();  
	channel2_ni();  
	channel3_shun();  
	channel4_ni();  
	}
		void  you(void)
	{
	channel1_ni();  
	channel2_shun();  
	channel3_ni();  
	channel4_shun();  
	}
	
	
	
		void  zuoshang(void)
	{
	channel1_shun();  
	channel2_stop();  
	channel3_shun();  
	channel4_stop();  
	}
		void  zuoxia(void)
	{
	channel1_stop();  
	channel2_ni();  
	channel3_stop();  
	channel4_ni();  
	}
		void  youshang(void)
	{
	channel1_stop();  
	channel2_shun();  
	channel3_stop();  
	channel4_shun();  
	}
		void  youxia(void)
	{
	channel1_ni();  
	channel2_stop();  
	channel3_ni();  
	channel4_stop();  
	}
	
			void  shun(void)
	{
	channel1_shun();  
	channel2_ni();  
	channel3_ni();  
	channel4_shun();  
	}
void  ni(void)
{
	channel1_ni();  
	channel2_shun();  
	channel3_shun();  
	channel4_ni();  
}
void  stop(void)    //io控制  驱动器锁定
{
	channel1_stop();  
	channel2_stop();  
	channel3_stop();  
	channel4_stop();  
}
	/*车轮对应 PWM通道
	4          3
	
	1          2


       A轮  \\    ------   //  B轮				轮子的力与纹路垂直
									\\  ------ //
											------
											------				
											------
								 //  ------  \\
			D轮  // 	  ------    \\  C轮


操纵杆  adc值：  0-4095

						y小
							|
 x小----|----x大
							|
						y大

	*/




void zuoshang_speed(u16 speed){
	channel4(speed);
}
void youshang_speed(u16 speed){
	channel3(speed);
}
void zuoxia_speed(u16 speed){
	channel1(speed);
}
void youxia_speed(u16 speed){
	channel2(speed);
}


//有关串口通信  遥控
u8 RXD_num = 0;          //串口读取的字符数  用于判断长命令的
u8 uart_flag = 0;          //串口读取长数据 是否完成
u8 uart_data[20]={0};          //串口数据存放的数组


u16 xadc_static=2048;    //x轴  静态adc值    后续改为串口2接收到的前10组数值平均值。
u16 yadc_static=2048;    //y轴静态adc值
u16 error_value=100;    //静态的偏差值   正负

u16 x_dev=0;    //x值与中心点偏差  正值
u16 y_dev=0;    //y  与中心点偏差  正值

u16 xadc=0;    //接收到的x值
u16 yadc=0;

//u8 shunni_flag=0;    //标志    0水平平移    1根据x轴左右旋转

u16 speed=600;   //单独指令控制使用    0-2048
int	main()
{
	u16 i=0;
	SystemInit();	// 配置系统时钟为72M 	
	delay_init();


	USART2_Config(115200);
	TIM_pwm_Init() ;
	TIM_ctrl_IO_init()   ;  
	
	stop();
	all_channels(0);  //4通道速度 0

	while(1)
	{
		
		
    if (uart_flag == 1) {	 //如果数据已经接收完成     解析数据：
      if (uart_data[10] == '*') {		   //如果第14个是‘*’ 表示修改ds1302时间  数据格式举例：“#200514120000*”
				//#0123,3366*                   //x轴adc     y轴adc      按键           平时二维平面运动，
				
				xadc=(uart_data[1]-'0')*1000+(uart_data[2]-'0')*100+(uart_data[3]-'0')*10+(uart_data[4]-'0');
				yadc=(uart_data[6]-'0')*1000+(uart_data[7]-'0')*100+(uart_data[8]-'0')*10+(uart_data[9]-'0');
				
				if(((xadc-xadc_static)*(xadc-xadc_static)+(yadc-yadc_static)*(yadc-yadc_static)  )<error_value*error_value ){   //在误差范围内的一个圆圈里时    停止运动
//					stop();
				  all_channels(0);  
					x_dev=0;
					y_dev=0;
				}
				else{    //根据数值  控制  运动方向和速度        顺时针来写
					if(yadc<yadc_static){              //上半部分
						if(xadc<xadc_static){    //第一象限
							x_dev=xadc_static-xadc;
							y_dev=yadc_static-yadc;
							if(x_dev>y_dev){  //左下  x>y  ok
								zuo();
								channel1(x_dev+y_dev);		channel3(x_dev+y_dev);    //x+y 
								channel2(x_dev-y_dev);			channel4(x_dev-y_dev);    //x-y
							}
							else{   //左上  ok
								qian();
								channel1(y_dev+x_dev);channel3(y_dev+x_dev);    //y+x
								channel2(y_dev-x_dev);channel4(y_dev-x_dev);    //y-x
							}							
						}
						else{    //第二象限
							x_dev=xadc-xadc_static;
							y_dev=yadc_static-yadc;
							if(x_dev<y_dev){  //左上    x<y  ok
								qian();
								channel1(y_dev-x_dev);		channel3(y_dev-x_dev);    //y-x
								channel2(y_dev+x_dev);	channel4(y_dev+x_dev);    //y+x
							}
							else{    //右下  ok
							you();
								channel1(x_dev-y_dev);		channel3(x_dev-y_dev);    //x-y
								channel2(x_dev+y_dev);	channel4(x_dev+y_dev);    //x+y
							}
						}
					}
					else{    //下半部分
						if(xadc>xadc_static){    //第四象限
							x_dev=xadc-xadc_static;
							y_dev=yadc-yadc_static;
							if(x_dev>y_dev){  //右上  ok
								you();
								channel1(x_dev+y_dev);		channel3(x_dev+y_dev);    //x+y
								channel2(x_dev-y_dev);			channel4(x_dev-y_dev);    //x-y
							}
							else{   //右下   ok
								hou();
								channel1(y_dev+x_dev);channel3(y_dev+x_dev);    //y+x
								channel2(y_dev-x_dev);channel4(y_dev-x_dev);    //y-x
							}							
						}
						else{    //第三象限
							x_dev=xadc_static-xadc;
							y_dev=yadc-yadc_static;
							if(x_dev<y_dev){  //右下  ok
								hou();
								channel1(y_dev-x_dev);		channel3(y_dev-x_dev);    //y-x
								channel2(y_dev+x_dev);	channel4(y_dev+x_dev);    //y+x
							}
							else{    //左上  ok
							zuo();
								channel1(x_dev-y_dev);		channel3(x_dev-y_dev);    //x-y
								channel2(x_dev+y_dev);	channel4(x_dev+y_dev);    //x+y
							}
						}
					}
				}
      }
      if (uart_data[5] == '*') {		   
				//#3344*                   //小于中值   逆时针   大于就顺时针
				xadc=(uart_data[1]-'0')*1000+(uart_data[2]-'0')*100+(uart_data[3]-'0')*10+(uart_data[4]-'0');
				
				if((xadc-xadc_static)*(xadc-xadc_static)<error_value*error_value){    //在范围内    不做运动
					stop();
				  all_channels(0);  
				}
				else{		//其他情况
					if(xadc>xadc_static){    //顺时针
						shun();
						all_channels((xadc-xadc_static)/2);  
					}
					else{    //逆时针
						ni();
						all_channels((xadc_static-xadc)/2);  
					}
				}
      }
			
			
			//原有的控制指令    手机/8266控制
      if (uart_data[2] == '*') {		  				//#1*
				if(uart_data[1]=='r')   {you();    }
				if(uart_data[1]=='l')   {zuo();     }
				if(uart_data[1]=='s')   {stop();     }
				if(uart_data[1]=='d')  { hou();     }
				if(uart_data[1]=='u')  {qian();        }

				if(uart_data[1]=='1')   {zuoshang();    }
				if(uart_data[1]=='2')   {youshang();     }
				if(uart_data[1]=='3')   {zuoxia();     }
				if(uart_data[1]=='4')  { youxia();     }
				if(uart_data[1]=='5')   {     ni();  }
				if(uart_data[1]=='6')  {  	shun();    }	 

				if(uart_data[1]=='A')   { speed=300;  all_channels(speed);  }
				if(uart_data[1]=='B')   { speed=600;  all_channels(speed);  }
				if(uart_data[1]=='C')   { speed=1000;  all_channels(speed);  }
				if(uart_data[1]=='D')  { speed=1600;  all_channels(speed);  }
				
			}
			
      for (i = 1; i < 20; i++) uart_data[i] = 0;   //清空接收数组缓存
      uart_flag = 0;
    }
		
		
		
		
		//USART_printf(USART2, "percentage num: %d%% \r\n",t2num);
		//USART_printf(USART2, "step num: %d \r\n",t4_num);
		//delay_s(1);

		//USART_Send_string(USART2,"hello2 \r\n");
	}

}
void USART2_IRQHandler(void)            //在中断服务程序中，由于主机响应中断时并不知道是哪个中断源发出中断请求，因此必须在中断服务程序中对中断源进行判别，然后分别进行处理。当然，如果只涉及到一个中断请求，是不用做上述判别的。但是无论什么情况，做上述判别是个好习惯
{
  u8 dat;
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)    //若接收数据寄存器满
  {
    dat = USART_ReceiveData(USART2);  
         
    if (dat == '#')  RXD_num = 0;	  //数据以‘#’开头 ，以‘*’结尾，不符合的不存放在数组里
    uart_data[RXD_num] = dat;
    RXD_num++;
    if (dat == '*') {			//判断结尾
      RXD_num = 0;
      uart_flag = 1;			//完成数据接收 标志
			
//			USART_Send_string(USART2,"\r\n【get】：");   //发回数据，表示收到，以便验证
//      USART_Send_string(USART2,uart_data); 
		}
		
		
		
//	 if(dat=='r')   {you();    }
//	 if(dat=='l')   {zuo();     }
//	 if(dat=='s')   {stop();     }
//	 if(dat=='d')  { hou();     }
//	 if(dat=='u')  {qian();        }
//	 
//	 if(dat=='1')   {zuoshang();    }
//	 if(dat=='2')   {youshang();     }
//	 if(dat=='3')   {zuoxia();     }
//	 if(dat=='4')  { youxia();     }
//	 if(dat=='5')   {     ni();  }
//	 if(dat=='6')  {  	shun();    }	 
//	 
//	 if(dat=='A')   { speed=300;  }
//	 if(dat=='B')   { speed=400;  }
//	 if(dat=='C')   { speed=600;  }
//	 if(dat=='D')  { speed=800;  }
	
		/*
	 if(dat=='a')  { t2num+=1;if(t2num>100) t2num=1;if(t2num<=0) t2num=100;TIM_SetCompare1(TIM2, t2arr*t2num/100);TIM_SetCompare2(TIM2, t2arr*t2num/100); }
	 if(dat=='b')  {t2num-=1;	if(t2num>100) t2num=1;if(t2num<=0) t2num=100;TIM_SetCompare1(TIM2, t2arr*t2num/100);  TIM_SetCompare2(TIM2, t2arr*t2num/100); }
  
	 if(dat=='r')   {GPIO_SetBits(GPIOB,GPIO_Pin_7);TIM_Cmd(TIM4,ENABLE);}
	 if(dat=='l')   {GPIO_ResetBits(GPIOB,GPIO_Pin_7);TIM_Cmd(TIM4,ENABLE);}
	 if(dat=='s')   TIM_Cmd(TIM4,DISABLE);
	 if(dat=='d')  { t4_num+=20;TIM_SetAutoreload(TIM4, t4_num);TIM_SetCompare1(TIM4, t4_num/2);}
	 if(dat=='u')  {t4_num-=20;TIM_SetAutoreload(TIM4, t4_num);TIM_SetCompare1(TIM4, t4_num/2);}
	 */
  }
} 

