#include "AD5621.h"

void setup() {
  Serial.begin(115200);
  AD5621_Init(); // 初始化AD5621（必须调用）
  Serial.println("AD5621 Ready");
}

void loop() {
  // 循环输出0V、1.65V、3.3V（每个电压保持2秒）

  
  AD5621_SetVoltage(1.65);
  Serial.println("Output: 1.65V");
  delay(2000);
  

}