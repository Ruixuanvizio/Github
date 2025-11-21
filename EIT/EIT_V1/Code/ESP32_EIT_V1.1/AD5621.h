#ifndef AD5621_H
#define AD5621_H

#include <Arduino.h>

// 引脚宏定义（对应接线：SYNC-IO19、SCLK-IO47、SDIN-IO21）
#define AD5621_SYNC 19
#define AD5621_SCLK 47
#define AD5621_SDIN 21

// 供电电压（AD5621支持2.7V~5.5V，默认3.3V，可按需修改）
#define AD5621_VDD 3.3

// 函数声明
void AD5621_Init(void);                // 初始化AD5621
void AD5621_SetVoltage(float voltage); // 设置输出电压
void AD5621_SendData(uint16_t data);   // 发送16位数据（内部调用）

#endif // AD5621_H