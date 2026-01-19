#include "stm32f10x.h"
#include "ModbusMaster.h"
#include "Comunication.h"
#include "SystemInit.h"




#define DEMOBLS		1
#define DEMONS		1


//无刷电机演示，通信波特率115200，IN2下降沿停车，IN3下降沿切换转动方向
static void DemoBLS(void)
{

	static int state=2;
	u16 tempdata[6]={1,1,1,1,1,1};
	static int value=500;//value 大于零正转，小于零反转
	u16 pwm=(0xffff)&value;
			switch(state)
				{
					case 0:
						if(MB_GetMasterEvent()==MM_EVENT_NONE)
							{
							
								if(MB_WirteSingleReg(1,0x42,pwm))//发送转动指令
									{
										state++;
									}
								

							}
		
						break;
					case 1:
						if(MB_GetMasterEvent()==MM_EVENT_NONE)
							{
								if(MB_GetMasterErrCode()==MB_ERR_NONE)
									{
										state++;
									}
								else
									{
										
										state--;
									}

							}
						break;
					case 2:
						if(MB_GetMasterEvent()==MM_EVENT_NONE)
							{
								if(MB_ReadHoldReg(1,0x7028,6))//读取IN2 ,IN3 边沿寄存器
									{
										state++;
									}
							}
						break;
					case 3:
						if(MB_GetMasterEvent()==MM_EVENT_NONE)
							{
								if(MB_GetMasterErrCode()==MB_ERR_NONE)
									{
										MB_GetRecData(tempdata,0,6);//获取返回数据
										if(tempdata[0]==0)//IN2下降沿
											{
												MB_WirteSingleReg(1,0x40,0);//发送停止指令
												state--;
											}
										else if(tempdata[5]==0)//IN3下降沿切换
											{
												value=value*(-1);//切换转动方向
												state=0;
											}
										else
											{
												state--;
											}
									}
								else
									{
										state--;
									}
							}
						break;
					default:
						break;
				}
}




//有刷电机演示，通信波特率115200，AI1高电平停车，AI2高电平切换转动方向

static void DemoNS(void)
{

	static int state=2;
	u16 tempdata[2]={1,1};
	static int value=500;//value 大于零正转，小于零反转
	u16 pwm=(0xffff)&value;
			switch(state)
				{
					case 0:
						if(MB_GetMasterEvent()==MM_EVENT_NONE)
							{
							
								if(MB_WirteSingleReg(1,0x40,pwm))//发送转动指令
									{
										state++;
									}
								

							}
		
						break;
					case 1:
						if(MB_GetMasterEvent()==MM_EVENT_NONE)
							{
								if(MB_GetMasterErrCode()==MB_ERR_NONE)
									{
										state++;
									}
								else
									{
										
										state--;
									}

							}
						break;
					case 2:
						if(MB_GetMasterEvent()==MM_EVENT_NONE)
							{
								if(MB_ReadHoldReg(1,0x14,2))//读取AI1,AI2电压寄存器
									{
										state++;
									}
							}
						break;
					case 3:
						if(MB_GetMasterEvent()==MM_EVENT_NONE)
							{
								if(MB_GetMasterErrCode()==MB_ERR_NONE)
									{
										MB_GetRecData(tempdata,0,2);//获取返回数据
										if(tempdata[0]>=300)//AI1电压大于3000mv停止
											{
												value=0;
												state=0;
											}
										else if(tempdata[1]>=300)//AI2电压大于3000mv切换
											{
												if(value==0)
													{
														value=500;
													}
												else
													{
														value=value*(-1);//切换转动方向
													}
												state=0;
											}
										else
											{
												state--;
											}
									}
								else
									{
										state--;
									}
							}
						break;
				
					default:
						break;
				}
}


int main(void)
{
	System_Init();
	while(1)
	{
		#if DEMOBLS
		DemoBLS();
		#else
		DemoNS();
		#endif
		COM_Task();

	}
		     
}





