#ifndef AD5165_H
#define AD5165_H

#include <Arduino.h>

// 引脚定义（固定，封装内部使用）
#define AD5165_CLK  47
#define AD5165_CS   45
#define AD5165_SDI  21

// 基础参数（封装内部计算用）
#define AD5165_R_AB 100.0  // 100kΩ（端到端电阻）
#define AD5165_W_RES 0.1   // 0.1kΩ（2×Wiper电阻）

// 函数声明（仅暴露2个核心接口）
void AD5165_Init(void);                  // 初始化（内部完成引脚配置和默认电阻设置）
uint8_t AD5165_SetRWA(float rwa_kohm);   // 设置R_WA电阻（单位：kΩ，返回1=成功，0=失败）

#endif