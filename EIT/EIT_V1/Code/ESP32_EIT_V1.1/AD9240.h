#ifndef AD9240_H
#define AD9240_H
#include <Arduino.h>

// 核心引脚定义
#define AD9240_CLK     46
const int AD9240_DATA_PINS[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14};

// 关键参数
#define AD9240_VREF    2.488f
#define AD9240_LSB     (2*AD9240_VREF / 16384.0f)
#define DEFAULT_PEAK_WINDOW 1000  // 默认窗口时长：1000ms（1秒）

// 函数声明
void AD9240_Init(void);
uint16_t AD9240_Read(void);
float AD9240_RawToVoltage(uint16_t raw);
void AD9240_ClockLoop(void);

// 新增：时间窗口峰峰值相关
void AD9240_SetPeakWindow(uint32_t window_ms);  // 设置峰峰值计算窗口时长（毫秒）
void AD9240_UpdatePeakToPeak(void);             // 实时更新窗口内的峰峰值
float AD9240_GetPeakToPeak(void);               // 获取当前窗口的峰峰值
float AD9240_GetLastPeakToPeak(void);           // 获取上一个窗口的峰峰值（窗口结束后有效）

#endif