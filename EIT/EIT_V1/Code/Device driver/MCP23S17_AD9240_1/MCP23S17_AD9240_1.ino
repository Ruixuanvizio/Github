#include "MCP23S17.h"
#include "AD9240.h"

// 硬件引脚配置（与接线一致）
#define MCP_SCLK_PIN 47
#define MCP_MOSI_PIN 17
#define MCP_CS_PIN   16

void setup() {
  Serial.begin(2000000);
  mcp23s17_init(MCP_SCLK_PIN, MCP_MOSI_PIN, MCP_CS_PIN);
  mux_init();
  AD9240_Init();  // 初始化AD9240
  
  // 核心测试：设置MUX
  mux_set_porta_muxes(5, 10);  // PORTA：MUX0=5，MUX1=10
  mux_set_portb_muxes(0, 7);   // PORTB：MUX2=8，MUX3=12
  
  // 读取输出锁存器
  uint8_t porta = mcp23s17_read_olat(0);
  uint8_t portb = mcp23s17_read_olat(1);
}

void loop() {
  float rms, vpp;
  // 调用封装的AD9240处理函数（仅获取RMS和Vpp）
  bool processOk = AD9240_ProcessData(rms, vpp);
  
  if (!processOk) {
    Serial.println("数据处理失败");
    delay(100);
    return;
  }
  
  // 输出结果（仅峰峰值和有效值）
  Serial.print(" | 峰峰值：");
  Serial.print(vpp, 4);
  Serial.print("V");
  
  Serial.print(" | 有效值：");
  Serial.print(rms, 4);
  Serial.println("V");
  
  delay(10);
}