#include "MCP23S17.h"
#include "AD9240.h"
#include "AD5165.h"
void setup() {
  Serial.begin(2000000);
  mcp_init(); // 初始化MCP（内部已用MCP_SCLK等引脚，无需再传参）
  AD9240_Init();
  
  // 设置初始MUX通道
  mux_set_porta(5, 10); // MUX0=5，MUX2=10
  mux_set_portb(0, 11);  // MUX1=0，MUX3=7
  
  AD5165_Init();  // 初始化（内部已设置默认电阻）
  AD5165_SetRWA(100);  // 中电阻（对应中等增益）
}

void loop() {
  float rms, vpp;
  AD9240_ProcessData(rms, vpp); // 采集AD数据
  
  // 输出结果
  Serial.print("峰峰值：");
  Serial.print(vpp, 4);
  Serial.print("V | 有效值：");
  Serial.print(rms, 4);
  Serial.println("V");
  
  delay(10);
}