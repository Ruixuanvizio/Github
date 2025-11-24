#include "AD5621.h"

// 初始化AD5621：设置引脚模式+初始电平
void AD5621_Init(void) {
  // 设置引脚为输出模式
  pinMode(AD5621_SYNC, OUTPUT);
  pinMode(AD5621_SCLK, OUTPUT);
  pinMode(AD5621_SDIN, OUTPUT);
  
  // 初始状态：SYNC高（未选中）、SCLK高（时钟空闲）
  digitalWrite(AD5621_SYNC, HIGH);
  digitalWrite(AD5621_SCLK, HIGH);
}

// 发送16位数据到AD5621（遵循SPI时序：SCLK下降沿移数据）
void AD5621_SendData(uint16_t data) {
  digitalWrite(AD5621_SYNC, LOW);  // 拉低SYNC，开始通信
  delayMicroseconds(1);            // 替换delayNanoseconds(10)，满足t4最小10ns要求
                                  
  // 逐位发送（先发高位DB15，再发低位DB0）
  for (int i = 15; i >= 0; i--) {
    // 提取当前位并输出
    digitalWrite(AD5621_SDIN, (data >> i) & 0x01);
    delayMicroseconds(1);          // 替换delayNanoseconds(5)，满足数据setup时间t5
    
    // SCLK下降沿：将数据移入AD5621
    digitalWrite(AD5621_SCLK, LOW);
    delayMicroseconds(1);          // 替换delayNanoseconds(5)，满足SCLK低电平时间t3
    digitalWrite(AD5621_SCLK, HIGH);
    delayMicroseconds(1);          // 替换delayNanoseconds(5)，满足SCLK高电平时间t2
  }
  
  digitalWrite(AD5621_SYNC, HIGH); // 拉高SYNC，结束通信
  delayMicroseconds(1);            // 确保通信完成
}

// 设置AD5621输出电压（范围：0~AD5621_VDD）
void AD5621_SetVoltage(float voltage) {
  // 电压范围限制（避免超出AD5621输出范围：0V~VDD）
  if (voltage < 0) voltage = 0;
  if (voltage > AD5621_VDD) voltage = AD5621_VDD;
  
  // 12位DAC值计算（AD5621为12位DAC，分辨率4096级：0~4095）
  uint16_t dac_val = (uint16_t)((voltage / AD5621_VDD) * 4095);
  
  // 构造16位数据帧：前2位=00（正常模式），中间12位=DAC值，后2位无效
  uint16_t data = (0b00 << 14) | (dac_val << 2);
  
  // 发送数据到AD5621
  AD5621_SendData(data);
}