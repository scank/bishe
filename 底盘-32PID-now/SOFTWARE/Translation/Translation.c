#include "Translation.h"

MotorParams_t motor_params1;//第一个驱动
MotorParams_t motor_params2;//第二个驱动
MotorParams_t motor_params3;//第一个驱动
MotorParams_t motor_params4;//第二个驱动


/**
 * @brief 解析保持寄存器数据
 * @param regs 保持寄存器数组
 * @param start_addr 起始地址(一般为0x0010)
 * @param params 输出参数结构体指针
 * @return 0:成功, -1:失败
 */
int ParseMotorParameters(uint16_t *regs, uint16_t start_addr, MotorParams_t *params)
{
    if (regs == NULL || params == NULL) {
        return -1;
    }
    
    // 计算偏移量
    int offset = start_addr - 0x0010;
    if (offset < 0) {
        return -1; 
    }
    
    // 0x0010: 实时PWM
    if (regs[offset + 0] <= 1000) {
        params->real_pwm = regs[offset + 0] * 0.1f;  // 0~100.0%
    } else {
        params->real_pwm = 0.0f;
    }
    
    // 0x0011: 实时电流
    if (regs[offset + 1] <= 1000) {
        params->real_current = regs[offset + 1] * 0.01f;  // A
    } else {
        params->real_current = 0.0f;
    }
    
    // 0x0012: 实时换向频率
    params->comm_freq = regs[offset + 2];
    
    // 0x0013: 堵转状态
    params->stall_status = (regs[offset + 3] <= 2) ? (uint8_t)regs[offset + 3] : 0;
    
    // 0x0014: IN1电压
    if (regs[offset + 4] <= 330) {
        params->in1_voltage = regs[offset + 4] / 100.0f;  // V
    } else {
        params->in1_voltage = 0.0f;
    }
    
    // 0x0015: IN2电压
    if (regs[offset + 5] <= 330) {
        params->in2_voltage = regs[offset + 5] / 100.0f;  // V
    } else {
        params->in2_voltage = 0.0f;
    }
    
    // 0x0016: IN1-IN2差分电压(有符号，补码表示)
    int16_t diff_voltage_raw = (int16_t)regs[offset + 6];
    if (diff_voltage_raw >= -330 && diff_voltage_raw <= 330) {
        params->diff_voltage = diff_voltage_raw / 100.0f;  // V
    } else {
        params->diff_voltage = 0.0f;
    }
    
    // 0x0017: 错误状态
    params->error_status = (regs[offset + 7] <= 11) ? (uint8_t)regs[offset + 7] : 0;
    
    // 0x0018: SQ1电平
    params->sq1_level = (regs[offset + 8] == 1) ? 1 : 0;
    
    // 0x0019: SQ2电平
    params->sq2_level = (regs[offset + 9] == 1) ? 1 : 0;
    
    // 0x001a: IN3电平
    params->in3_level = (regs[offset + 10] == 1) ? 1 : 0;
    
    // 0x001b: IN3电压
    if (regs[offset + 11] <= 330) {
        params->in3_voltage = regs[offset + 11] / 100.0f;  // V
    } else {
        params->in3_voltage = 0.0f;
    }
    
    // 0x001c: 芯片温度
    int16_t temp_raw = (int16_t)regs[offset + 12];
    if (temp_raw >= -400 && temp_raw <= 1250) {
        params->temperature = temp_raw / 10.0f;  // °C
    } else {
        params->temperature = -40.0f;
    }
    
    // 0x001d: 电源电压
    if (regs[offset + 13] <= 300) {
        params->power_voltage = regs[offset + 13] / 10.0f;  // V
    } else {
        params->power_voltage = 0.0f;
    }
    
    // 0x001e-0x001f: 电机转速(32位)
	int32_t speed_raw = (int32_t)(((uint32_t)regs[offset + 14] << 16) | regs[offset + 15]);//高半字＋低半字
	params->motor_speed = speed_raw;  // 自动转换回int32_t
	
    // 0x0020: 输入信号类型
    params->signal_type = regs[offset + 16];
    
    // 0x0021: IN1输入PWM占空比
    int16_t in1_duty_raw = (int16_t)regs[offset + 17];
    if (in1_duty_raw >= -1000 && in1_duty_raw <= 1000) {
        params->in1_pwm_duty = in1_duty_raw / 10.0f;  // %
    } else {
        params->in1_pwm_duty = 0.0f;
    }
    
    // 0x0022-0x0023: IN1输入脉冲频率
    params->in1_pulse_freq = ((uint32_t)regs[offset + 18] << 16) | regs[offset + 19];
    
    // 0x0024-0x0025: IN1输入脉冲数量
    params->in1_pulse_cnt = ((uint32_t)regs[offset + 20] << 16) | regs[offset + 21];
    
    // 0x0026: 母线电流
    if (regs[offset + 22] <= 1000) {
        params->bus_current = regs[offset + 22] * 0.01f;  // A
    } else {
        params->bus_current = 0.0f;
    }
    
    // 0x0027: IN2输入PWM占空比
    int16_t in2_duty_raw = (int16_t)regs[offset + 23];
    if (in2_duty_raw >= -1000 && in2_duty_raw <= 1000) {
        params->in2_pwm_duty = in2_duty_raw / 10.0f;  // %
    } else {
        params->in2_pwm_duty = 0.0f;
    }
    
    // 0x0028-0x0029: IN2输入脉冲频率
    params->in2_pulse_freq = ((uint32_t)regs[offset + 24] << 16) | regs[offset + 25];
    
    // 0x002a-0x002b: IN2输入脉冲数量
    params->in2_pulse_cnt = ((uint32_t)regs[offset + 26] << 16) | regs[offset + 27];
    
    // 0x002c-0x002d: 编码器输入脉冲数量
    params->encoder_pulse_cnt = ((uint32_t)regs[offset + 28] << 16) | regs[offset + 29];
    
    // 0x002e: IN1脉宽
    params->in1_pulse_width = regs[offset + 30];
    
    // 0x002f: IN2脉宽
    params->in2_pulse_width = regs[offset + 31];
    
    // 0x0030-0x0031: 剩余完成时间
    params->remaining_time = ((uint32_t)regs[offset + 32] << 16) | regs[offset + 33];
    
    // 0x0032: 位置控制完成状态
    params->pos_ctrl_status = (regs[offset + 34] <= 2) ? (uint8_t)regs[offset + 34] : 0;
    
    return 0;
}

/**
 * @brief 打印电机参数(用于调试)
 * @param params 电机参数结构体指针
 */
void PrintMotorParameters(MotorParams_t *params)
{
    if (params == NULL) return;
    
    printf("=== 电机参数 ===\n");
    printf("实时PWM: %.1f%%\n", params->real_pwm);
    printf("实时电流: %.2f A\n", params->real_current);
    printf("换向频率: %d Hz\n", params->comm_freq);
    printf("堵转状态: %d\n", params->stall_status);
    printf("IN1电压: %.2f V\n", params->in1_voltage);
    printf("IN2电压: %.2f V\n", params->in2_voltage);
    printf("差分电压: %.2f V\n", params->diff_voltage);
    printf("错误状态: %d\n", params->error_status);
    printf("SQ1电平: %d\n", params->sq1_level);
    printf("SQ2电平: %d\n", params->sq2_level);
    printf("IN3电平: %d\n", params->in3_level);
    printf("IN3电压: %.2f V\n", params->in3_voltage);
    printf("温度: %.1f °C\n", params->temperature);
    printf("电源电压: %.1f V\n", params->power_voltage);
    printf("电机转速: %lu RPM\n", params->motor_speed);
    printf("输入信号类型: 0x%04X\n", params->signal_type);
    printf("IN1 PWM占空比: %.1f%%\n", params->in1_pwm_duty);
    printf("IN1脉冲频率: %lu Hz\n", params->in1_pulse_freq);
    printf("IN1脉冲数量: %lu\n", params->in1_pulse_cnt);
    printf("母线电流: %.2f A\n", params->bus_current);
    printf("IN2 PWM占空比: %.1f%%\n", params->in2_pwm_duty);
    printf("IN2脉冲频率: %lu Hz\n", params->in2_pulse_freq);
    printf("IN2脉冲数量: %lu\n", params->in2_pulse_cnt);
    printf("编码器脉冲数量: %lu\n", params->encoder_pulse_cnt);
    printf("IN1脉宽: %lu us\n", params->in1_pulse_width);
    printf("IN2脉宽: %lu us\n", params->in2_pulse_width);
    printf("剩余完成时间: %lu ms\n", params->remaining_time);
    printf("位置控制状态: %d\n", params->pos_ctrl_status);
}




