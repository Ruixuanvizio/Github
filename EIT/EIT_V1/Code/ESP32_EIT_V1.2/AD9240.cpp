#include "AD9240.h"

// 静态变量（仅在当前文件生效，不污染全局）
static bool is_init = false;  // 标记是否已初始化
static float voltages[SAMPLE_COUNT];  // 存储单次采样的电压数据

// --------------------------
// 内部初始化函数（仅调用一次）
// --------------------------
static void ad9240_init() {
  if (is_init) return;  // 避免重复初始化

  // 数据引脚设为输入
  for (int i = 0; i < 14; i++) {
    pinMode(AD9240_DATA_PINS[i], INPUT);
  }

  // 时钟引脚设为输出（初始低电平）
  pinMode(AD9240_CLK, OUTPUT);
  digitalWrite(AD9240_CLK, LOW);

  is_init = true;
}

// --------------------------
// 内部函数：读取单个采样点的电压
// --------------------------
static float ad9240_read_one() {
  uint16_t raw_data = 0;  // 14位原始数据

  // 等待时钟为低电平（确保与AD9240输出同步）
  while (digitalRead(AD9240_CLK) == HIGH);

  // 读取14位数据（高位到低位，对应AD9240的D13~D0）
  for (int i = 0; i < 14; i++) {
    raw_data |= (digitalRead(AD9240_DATA_PINS[i]) << (13 - i));
  }

  // 转换为电压（LSB = 2*VREF / 2^14）
  return raw_data * AD9240_LSB;
}

// --------------------------
// 核心函数：采集+计算峰峰值+输出
// --------------------------
void ad9240_get_peak() {
  ad9240_init();  // 确保初始化完成

  // 1. 采集所有样本
  for (int i = 0; i < SAMPLE_COUNT; i++) {
    // 生成时钟脉冲（上升沿触发AD9240输出数据）
    digitalWrite(AD9240_CLK, HIGH);
    delayMicroseconds(1);  // 时钟高电平持续时间（≥AD9240最小要求）
    digitalWrite(AD9240_CLK, LOW);
    delayMicroseconds(1);  // 时钟低电平持续时间

    // 读取当前点电压并存储
    voltages[i] = ad9240_read_one();
  }

  // 2. 计算峰峰值（最大值 - 最小值）
  float max_v = voltages[0];
  float min_v = voltages[0];
  for (int i = 1; i < SAMPLE_COUNT; i++) {
    if (voltages[i] > max_v) max_v = voltages[i];
    if (voltages[i] < min_v) min_v = voltages[i];
  }
  float peak_to_peak = max_v - min_v;

  // 3. 输出结果
  //Serial.print("[采样完成] 峰峰值：");
  Serial.println(peak_to_peak, 4);  // 保留4位小数
  //Serial.println(" V");
}