#include "AD9240.h"

// 数据引脚数组（BIT1~BIT14）
const int adPins[] = {
  AD9240_BIT1, AD9240_BIT2, AD9240_BIT3, AD9240_BIT4,
  AD9240_BIT5, AD9240_BIT6, AD9240_BIT7, AD9240_BIT8,
  AD9240_BIT9, AD9240_BIT10, AD9240_BIT11, AD9240_BIT12,
  AD9240_BIT13, AD9240_BIT14
};

// 静态缓冲区（内部使用）
static float acData[BUFFER_LEN];          // 批量采集数据缓冲区
static float filterBuf[FILTER_DEPTH] = {0}; // 滑动滤波缓冲区
static uint8_t filterIdx = 0;

/**
 * 初始化AD9240引脚
 */
void AD9240_Init(void) {
  for (int i = 0; i < 14; i++) {
    pinMode(adPins[i], INPUT);
    gpio_set_direction((gpio_num_t)adPins[i], GPIO_MODE_INPUT);
    gpio_set_pull_mode((gpio_num_t)adPins[i], GPIO_FLOATING);
  }
}

/**
 * 封装的AD9240数据处理流程（仅计算RMS和Vpp）
 * @param rms: 输出有效值
 * @param vpp: 输出峰峰值
 * @return 处理是否成功
 */
bool AD9240_ProcessData(float& rms, float& vpp) {
  // 1. 批量读取交流数据
  if (!AD9240_ReadACBatch(acData)) {
    return false;
  }
  
  // 2. 仅计算RMS和峰峰值（删除频率计算）
  rms = AD9240_GetRMS(acData);
  vpp = AD9240_GetVpp(acData);
  
  return true;
}

/**
 * 读取原始数据
 */
uint16_t AD9240_ReadRaw(void) {
  uint16_t raw = 0;
  raw  = digitalRead(AD9240_BIT1)  << 13;
  raw |= digitalRead(AD9240_BIT2)  << 12;
  raw |= digitalRead(AD9240_BIT3)  << 11;
  raw |= digitalRead(AD9240_BIT4)  << 10;
  raw |= digitalRead(AD9240_BIT5)  << 9;
  raw |= digitalRead(AD9240_BIT6)  << 8;
  raw |= digitalRead(AD9240_BIT7)  << 7;
  raw |= digitalRead(AD9240_BIT8)  << 6;
  raw |= digitalRead(AD9240_BIT9)  << 5;
  raw |= digitalRead(AD9240_BIT10) << 4;
  raw |= digitalRead(AD9240_BIT11) << 3;
  raw |= digitalRead(AD9240_BIT12) << 2;
  raw |= digitalRead(AD9240_BIT13) << 1;
  raw |= digitalRead(AD9240_BIT14) << 0;
  return raw;
}

/**
 * 转换电压
 */
float AD9240_ConvertVINA(uint16_t raw) {
  return (raw / (float)AD9240_MAX_RAW) * AD9240_VINA_FS;
}

/**
 * 批量读取交流数据
 */
bool AD9240_ReadACBatch(float* buffer) {
  if (buffer == nullptr) return false;
  
  for (uint16_t i = 0; i < BUFFER_LEN; i++) {
    uint16_t raw = AD9240_ReadRaw();
    float voltage = AD9240_ConvertVINA(raw);
    
    // 滑动滤波
    filterBuf[filterIdx] = voltage;
    filterIdx = (filterIdx + 1) % FILTER_DEPTH;
    
    float filtered = 0;
    for (uint8_t j = 0; j < FILTER_DEPTH; j++) {
      filtered += filterBuf[j];
    }
    buffer[i] = filtered / FILTER_DEPTH;
  }
  return true;
}

/**
 * 计算有效值
 */
float AD9240_GetRMS(float* buffer) {
  if (buffer == nullptr) return NAN;
  
  float dc = 0;
  for (uint16_t i = 0; i < BUFFER_LEN; i++) {
    dc += buffer[i];
  }
  dc /= BUFFER_LEN;
  
  float sumSq = 0;
  for (uint16_t i = 0; i < BUFFER_LEN; i++) {
    float ac = buffer[i] - dc;
    sumSq += ac * ac;
  }
  return sqrt(sumSq / BUFFER_LEN);
}

/**
 * 计算峰峰值
 */
float AD9240_GetVpp(float* buffer) {
  if (buffer == nullptr) return NAN;
  
  float max = buffer[0], min = buffer[0];
  for (uint16_t i = 1; i < BUFFER_LEN; i++) {
    if (buffer[i] > max) max = buffer[i];
    if (buffer[i] < min) min = buffer[i];
  }
  return max - min;
}