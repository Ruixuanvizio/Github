#ifndef AD9240_H_
#define AD9240_H_

#include <Arduino.h>

// 14个数据引脚定义
#define AD9240_BIT1  1
#define AD9240_BIT2  2
#define AD9240_BIT3  3
#define AD9240_BIT4  4
#define AD9240_BIT5  5
#define AD9240_BIT6  6
#define AD9240_BIT7  7
#define AD9240_BIT8  8
#define AD9240_BIT9  9
#define AD9240_BIT10 10
#define AD9240_BIT11 11
#define AD9240_BIT12 12
#define AD9240_BIT13 13
#define AD9240_BIT14 14

// 关键参数
#define AD9240_VREF     2.470f   // 实测VREF值
#define AD9240_MAX_RAW  0x3FFF   // 14位最大值（16383）
#define AD9240_VINA_FS  (2 * AD9240_VREF)  // 满量程

// 高频交流配置
#define SAMPLE_RATE     100000UL // 采样率100kSPS
#define BUFFER_LEN      1000     // 批量采集点数
#define FILTER_DEPTH    6        // 滑动滤波窗口

// 函数声明（删除频率相关）
void AD9240_Init(void);                  // 初始化
bool AD9240_ProcessData(float& rms, float& vpp);  // 封装的数据处理函数（仅RMS和Vpp）
uint16_t AD9240_ReadRaw(void);           // 内部使用，读取原始数据
float AD9240_ConvertVINA(uint16_t raw);  // 内部使用，转换电压
bool AD9240_ReadACBatch(float* buffer);  // 内部使用，批量读取
float AD9240_GetRMS(float* buffer);      // 内部使用，计算RMS
float AD9240_GetVpp(float* buffer);      // 内部使用，计算Vpp

#endif // AD9240_H_