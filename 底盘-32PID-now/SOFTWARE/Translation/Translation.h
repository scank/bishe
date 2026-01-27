#ifndef __TRANSLATION_H
#define __TRANSLATION_H

#include "sys.h"
#include "modbus.h"

#define debug_printf 0

typedef struct {
    // 0x0010 - 0x0011
    float real_pwm;         // 实时PWM占空比(%)
    float real_current;     // 实时电流(A)
    
    // 0x0012 - 0x0013
    uint16_t comm_freq;     // 实时换向频率
    uint8_t stall_status;   // 堵转状态: 0-未堵转, 1-正转堵转, 2-反转堵转
    
    // 0x0014 - 0x0017
    float in1_voltage;      // IN1电压(V)
    float in2_voltage;      // IN2电压(V)
    float diff_voltage;     // IN1-IN2差分电压(V)
    uint8_t error_status;   // 错误状态
    
    // 0x0018 - 0x001b
    uint8_t sq1_level;      // SQ1电平
    uint8_t sq2_level;      // SQ2电平
    uint8_t in3_level;      // IN3电平
    float in3_voltage;      // IN3电压(V)
    
    // 0x001c - 0x001f
    float temperature;      // 温度(°C)
    float power_voltage;    // 电源电压(V)
    int32_t motor_speed;   // 电机转速(RPM)
    
    // 0x0020 - 0x0025
    uint16_t signal_type;   // 输入信号类型
    float in1_pwm_duty;     // IN1输入PWM占空比(%)
    uint32_t in1_pulse_freq;// IN1输入脉冲频率(Hz)
    uint32_t in1_pulse_cnt; // IN1输入脉冲数量
    
    // 0x0026 - 0x002b
    float bus_current;      // 母线电流(A)
    float in2_pwm_duty;     // IN2输入PWM占空比(%)
    uint32_t in2_pulse_freq;// IN2输入脉冲频率(Hz)
    uint32_t in2_pulse_cnt; // IN2输入脉冲数量
    
    // 0x002c - 0x0032
    uint32_t encoder_pulse_cnt;    // 编码器输入脉冲数量
    uint32_t in1_pulse_width;      // IN1脉宽(us)
    uint32_t in2_pulse_width;      // IN2脉宽(us)
    uint32_t remaining_time;       // 剩余完成时间(ms)
    uint8_t pos_ctrl_status;       // 位置控制完成状态
} MotorParams_t;


extern MotorParams_t motor_params;//第一个驱动
extern int32_t MotorSpeed[4];

int ParseMotorParameters(uint16_t *regs, uint16_t start_addr, MotorParams_t *params);
void PrintMotorParameters(MotorParams_t *params);
int32_t ParseSpeedOnly(u16 *registers, u8 motor_id);


#endif
