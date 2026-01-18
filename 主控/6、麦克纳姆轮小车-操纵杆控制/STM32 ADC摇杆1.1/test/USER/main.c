#include "sys.h"

#include "delay.h"
#include "led.h"
#include "usart.h"
#include "ADC.h"

#include "math.h"
/*
接线：
串口1
摇杆： 按键B12    
PA0    x轴
PA1    y轴
模块的坐标系指向数值数值小的方向：x向左，y向上
*/
/*

操纵杆  adc值：  0-4095

						y小
							|
 x小----|----x大
							|
						y大


*/
#define adjust 1.25     //横向修正比例


void KEY_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); // ??PC????  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	//???????
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  //???PC??
  GPIO_SetBits(GPIOB, GPIO_Pin_12 );	 // ????LED
}

u8 key_flag=0;    //0  正常平移    1旋转
u16 x_adc=0;    //x  y  的adc值
u16 y_adc=0;

u8 send_temp[15]={0};   //串口发送缓存
	u16 banjing=0;      //绕园旋转的半径
int	main()
{
  u16 i=0;
	u16 yuanzhou_flag=0;  
	
	SystemInit();	// 配置系统时钟为72M 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);   //完全禁用JTAG
	
	delay_init();
	LED_GPIO_Config();	
	KEY_init();
	USART1_Config(115200);
	//USART2_Config(9600);
	//USART3_Config(9600);
	Adc_Init();
	
	
//	USART_Send_string(USART1,"hello\r\n");
	i=0;      //角度
yuanzhou_flag=0;
	
	banjing=1200;
	while(1){
	
	//测试   绕圈圈				
		if(i>=360) i=0;    //正前方顺时针的力
			x_adc=2048+banjing*sin(i)*adjust;   //比例修正
			y_adc=2048-banjing*cos(i);
		i+=1;
		delay_ms(300);
		


		
		/*    圆角矩形
		if(yuanzhou_flag==0){    //第二象限  ok
			if(i>=banjing) {  yuanzhou_flag++;  i=0;  }
			else  i+=20;
			x_adc=2048+i;
			y_adc=2048-(banjing-i);
		}
		if(yuanzhou_flag==1){    //第四象限  ok
			if(i>=banjing) {  yuanzhou_flag++;  i=0;  }
			else  i+=20;
			x_adc=2048+(banjing-i);
			y_adc=2048+banjing-(banjing-i);
		}
		
		if(yuanzhou_flag==2){    //第三象限  ok
			if(i>=banjing) {  yuanzhou_flag++;  i=0;  }
			else  i+=20;
			x_adc=2048-i;
			y_adc=2048+(banjing-i);
		}
		
		if(yuanzhou_flag==3){    //第一象限  ok
			if(i>=banjing) {  yuanzhou_flag=0;  i=0;  }
			else  i+=20;
			x_adc=2048-banjing+i;
			y_adc=2048-banjing+(banjing-i);
		}
		*/
		
		
		/*    导致走的时候是  侧椭圆形尖型       因为角度变化不均匀
		if(yuanzhou_flag==0){    //第二象限  ok
			if(i>=banjing) {  yuanzhou_flag++;  i=0;  }
			else  i+=20;
			x_adc=2048+i;
			y_adc=2048-sqrt(banjing*banjing-i*i);
		}
		if(yuanzhou_flag==1){    //第四象限  ok
			if(i>=banjing) {  yuanzhou_flag++;  i=0;  }
			else  i+=20;
			x_adc=2048+(banjing-i);
			y_adc=2048+banjing-sqrt(banjing*banjing-i*i);
		}
		
		if(yuanzhou_flag==2){    //第三象限  ok
			if(i>=banjing) {  yuanzhou_flag++;  i=0;  }
			else  i+=20;
			x_adc=2048-i;
			y_adc=2048+sqrt(banjing*banjing-i*i);
		}
		
		if(yuanzhou_flag==3){    //第一象限  ok
			if(i>=banjing) {  yuanzhou_flag=0;  i=0;  }
			else  i+=20;
			x_adc=2048-banjing+i;
			y_adc=2048-banjing+sqrt(banjing*banjing-i*i);
		}
		
		*/
		
		
		
		
			send_temp[0]='#';
			send_temp[1]=x_adc/1000%10+48;
			send_temp[2]=x_adc/100%10+48;
			send_temp[3]=x_adc/10%10+48;
			send_temp[4]=x_adc%10+48;
			send_temp[5]=',';
			send_temp[6]=y_adc/1000%10+48;
			send_temp[7]=y_adc/100%10+48;
			send_temp[8]=y_adc/10%10+48;
			send_temp[9]=y_adc%10+48;
			send_temp[10]='*';
			send_temp[11]='\0';
			
			USART_Send_string(USART1,send_temp);
		
		
		
		
// delay_ms(9);
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)==0){
			delay_ms(5);
				if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)==0){
					break;
				}
		}	
	}
	
	
	
	USART_Send_string(USART1,"#s*");
	
	
	
	while(1)
	{
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)==0){
			delay_ms(10);
				if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)==0){
					key_flag=1-key_flag;
					while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)==0);
				}
		}			
		
		if(key_flag==0){  //平移
			GPIO_SetBits(GPIOC, GPIO_Pin_13 );		
		
			x_adc=Get_Adc(ADC_CH0); 
			y_adc=Get_Adc(ADC_CH1); 
			
x_adc = x_adc+(x_adc-2048)*adjust;   //调整    限制范围？
			
			
			send_temp[0]='#';
			send_temp[1]=x_adc/1000%10+48;
			send_temp[2]=x_adc/100%10+48;
			send_temp[3]=x_adc/10%10+48;
			send_temp[4]=x_adc%10+48;
			send_temp[5]=',';
			send_temp[6]=y_adc/1000%10+48;
			send_temp[7]=y_adc/100%10+48;
			send_temp[8]=y_adc/10%10+48;
			send_temp[9]=y_adc%10+48;
			send_temp[10]='*';
			send_temp[11]='\0';
			
			USART_Send_string(USART1,send_temp);
			
		}
		else{  //旋转
			GPIO_ResetBits(GPIOC, GPIO_Pin_13 );		
		
			delay_ms(3);
			x_adc=Get_Adc(ADC_CH0); 
			
			send_temp[0]='#';
			send_temp[1]=x_adc/1000%10+48;
			send_temp[2]=x_adc/100%10+48;
			send_temp[3]=x_adc/10%10+48;
			send_temp[4]=x_adc%10+48;
			send_temp[5]='*';
			send_temp[6]='\0';
			
			USART_Send_string(USART1,send_temp);    //先计算好再发送   发送快，间隔均匀
			

		}
//		delay_ms(5);    //间隔好像太短？
		
		delay_ms(10);
	

		
		
		//delay_s(1);

		//USART_Send_string(USART2,"hello2 \r\n");
		//USART_Send_string(USART3,"hello3 \r\n");
//		adc_num=Get_Adc(ADC_CH0);     //对应PA0口   改成其他的需要改Adc_Init();
//		USART_Send_string(USART1,"\r\n x y adc:");
//		USART_Send_Char(USART1,adc_num/1000%10+48);
//		USART_Send_Char(USART1,adc_num/100%10+48);
//		USART_Send_Char(USART1,adc_num/10%10+48);
//		USART_Send_Char(USART1,adc_num%10+48);
//		
//		USART_Send_string(USART1,"    ");
//		
//		adc_num=Get_Adc(ADC_CH1);     //对应PA0口   改成其他的需要改Adc_Init();
//		USART_Send_Char(USART1,adc_num/1000%10+48);
//		USART_Send_Char(USART1,adc_num/100%10+48);
//		USART_Send_Char(USART1,adc_num/10%10+48);
//		USART_Send_Char(USART1,adc_num%10+48);
	}

}

//			//小车中    原有的控制指令
//				if(uart_data[1]=='r')   {you();    }
//				if(uart_data[1]=='l')   {zuo();     }
//				if(uart_data[1]=='s')   {stop();     }
//				if(uart_data[1]=='d')  { hou();     }
//				if(uart_data[1]=='u')  {qian();        }

//				if(uart_data[1]=='1')   {zuoshang();    }//#1*
//				if(uart_data[1]=='2')   {youshang();     }
//				if(uart_data[1]=='3')   {zuoxia();     }
//				if(uart_data[1]=='4')  { youxia();     }
//				if(uart_data[1]=='5')   {     ni();  }
//				if(uart_data[1]=='6')  {  	shun();    }	 

//				if(uart_data[1]=='A')   { speed=300;  all_channels(speed);  }
//				if(uart_data[1]=='B')   { speed=600;  all_channels(speed);  }
//				if(uart_data[1]=='C')   { speed=1000;  all_channels(speed);  }
//				if(uart_data[1]=='D')  { speed=1600;  all_channels(speed);  }

