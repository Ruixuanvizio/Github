#include "AD9240.h"

// 静态变量：时间窗口相关参数
static uint32_t peak_window = DEFAULT_PEAK_WINDOW;  // 窗口时长（毫秒）
static uint32_t window_start_time = 0;              // 当前窗口开始时间
static float current_max = -INFINITY;               // 当前窗口最大值
static float current_min = INFINITY;                // 当前窗口最小值
static float last_peak_to_peak = 0.0f;              // 上一个窗口的峰峰值

// 初始化
void AD9240_Init(void) {
  // 14位数据引脚设为输入
  for (int i = 0; i < 14; i++) {
    pinMode(AD9240_DATA_PINS[i], INPUT);
  }
  // 时钟引脚设为输出
  pinMode(AD9240_CLK, OUTPUT);
  delay(10); 
  window_start_time = millis();  // 初始化窗口开始时间
}

// 时钟轮询（不变）
void AD9240_ClockLoop(void) {
  digitalWrite(AD9240_CLK, HIGH);
  delayMicroseconds(1);
  digitalWrite(AD9240_CLK, LOW);
  delayMicroseconds(1);
}

// 数据读取（不变）
uint16_t AD9240_Read(void) {
  uint16_t raw_data = 0;
  while (digitalRead(AD9240_CLK) == HIGH);
  
  for (int i = 0; i < 14; i++) {
    raw_data |= (digitalRead(AD9240_DATA_PINS[i]) << (13 - i));
  }
  return raw_data;
}

// 电压转换（不变）
float AD9240_RawToVoltage(uint16_t raw) {
  return raw * AD9240_LSB;
}

// 新增：设置峰峰值计算窗口时长（毫秒）
void AD9240_SetPeakWindow(uint32_t window_ms) {
  if (window_ms > 0) {  // 确保窗口时长有效
    peak_window = window_ms;
  }
}

// 新增：更新窗口内的峰峰值（核心逻辑）
// 修改更新峰峰值的核心逻辑
void AD9240_UpdatePeakToPeak(void) {
  uint32_t current_time = millis();
  // 先读取当前电压（确保每次更新都有有效数据）
  float current_voltage = AD9240_RawToVoltage(AD9240_Read());

  // 检查是否超出当前窗口
  if (current_time - window_start_time >= peak_window) {
    // 计算上一个窗口的峰峰值（过滤无效数据）
    if (current_max != -INFINITY && current_min != INFINITY) {
      last_peak_to_peak = current_max - current_min;
    } else {
      last_peak_to_peak = 0.0f;
    }
    // 重置当前窗口：强制用当前电压初始化max和min，避免初始值异常
    window_start_time = current_time;
    current_max = current_voltage;  // 用当前有效电压初始化
    current_min = current_voltage;  // 用当前有效电压初始化
  } else {
    // 在当前窗口内更新最大值和最小值
    if (current_voltage > current_max) current_max = current_voltage;
    if (current_voltage < current_min) current_min = current_voltage;
  }
}

// 新增：获取当前窗口的峰峰值（窗口未结束时实时更新）
float AD9240_GetPeakToPeak(void) {
  if (current_max == -INFINITY || current_min == INFINITY) {
    return 0.0f;
  }
  return current_max - current_min;
}

// 新增：获取上一个完整窗口的峰峰值（窗口结束后稳定值）
float AD9240_GetLastPeakToPeak(void) {
  return last_peak_to_peak;
}