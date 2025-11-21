#include "AD5165.h"

// 私有函数：发送8位数据（内部时序控制，用户无需关心）
static void send8bit(uint8_t data) {
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(AD5165_SDI, (data >> 7) & 1);  // 输出最高位
    digitalWrite(AD5165_CLK, HIGH);
    delayMicroseconds(1);                       // 满足时序要求
    digitalWrite(AD5165_CLK, LOW);
    delayMicroseconds(1);
    data <<= 1;                                 // 左移，准备下一位
  }
}

// 私有函数：电阻→数字码转换（适配R_WA接线，内部计算）
static uint8_t rwa_to_code(float rwa_kohm) {
  // R_WA = (256-D)/256*R_AB + W_RES → 反推D
  uint8_t code = 256 - ((rwa_kohm - AD5165_W_RES) * 256.0) / AD5165_R_AB;
  // 限制数字码在0-255范围
  return (code > 255) ? 255 : (code < 0 ? 0 : code);
}

// 公开函数：初始化（内部完成所有准备工作）
void AD5165_Init(void) {
  // 1. 引脚配置
  pinMode(AD5165_CLK, OUTPUT);
  pinMode(AD5165_CS, OUTPUT);
  pinMode(AD5165_SDI, OUTPUT);
  // 2. 初始电平（空闲状态）
  digitalWrite(AD5165_CS, LOW);
  digitalWrite(AD5165_CLK, LOW);
  // 3. 默认电阻设置（50kΩ R_WA，对应中间增益）
  AD5165_SetRWA(50.0);
  Serial.println("AD5165初始化完成");
}

// 公开函数：设置R_WA电阻（用户直接调用，输入kΩ值）
uint8_t AD5165_SetRWA(float rwa_kohm) {
  // 检查电阻有效性（R_WA范围：~0.1kΩ~100.1kΩ）
  if (rwa_kohm < 0.1 || rwa_kohm > 100.1) {
    Serial.printf("电阻值无效（范围0.1~100.1kΩ），输入=%.1fkΩ\n", rwa_kohm);
    return 0;
  }
  // 计算数字码
  uint8_t code = rwa_to_code(rwa_kohm);
  // 发送数据到AD5165
  digitalWrite(AD5165_CS, HIGH);
  delayMicroseconds(2);
  send8bit(code);
  digitalWrite(AD5165_CS, LOW);
  delayMicroseconds(2);
  // 打印结果（可选，用于调试）
  Serial.printf("已设置R_WA=%.1fkΩ（数字码=%d）\n", rwa_kohm, code);
  return 1;
}