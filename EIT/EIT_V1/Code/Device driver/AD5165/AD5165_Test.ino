#include <Arduino.h>
#include "AD5165.h"

void setup() {
  Serial.begin(115200);
  delay(1000);
  AD5165_Init();  // 初始化（内部已设置默认电阻）
}

void loop() {
  // 示例：循环设置3种常用R_WA电阻（可根据需求修改数值）
  AD5165_SetRWA(1.1);   // 小电阻（对应AD8421高增益）
  delay(3000);
  AD5165_SetRWA(10.0);  // 中电阻（对应中等增益）
  delay(3000);
  AD5165_SetRWA(99.0);  // 大电阻（对应低增益）
  delay(3000);
}