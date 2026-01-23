#ifndef __ENCODE_H
#define __ENCODE_H

#include "stm32f10x.h"                  // Device header
#include "motor.h"
extern int16_t encoder_fl, encoder_fr, encoder_bl, encoder_br;
extern float speed_fl_rpm, speed_fr_rpm, speed_bl_rpm, speed_br_rpm;

//电机1：PA0 PA1
//电机2：PA6 PA7
//对应的是编码电机的返回值

void Encoder_Init_TIM2(void);//右
void Encoder_Init_TIM3(void);
void Encoder_Init_TIM4(void);
void Encoder_Init_TIM5(void);

int16_t Read_Encoder(u8 TIMX);
int Encoder_Value(TIM_TypeDef* TIMx);
void Encoder_Count_Clear(TIM_TypeDef* TIMx);
void encoder_car(void);

#endif
