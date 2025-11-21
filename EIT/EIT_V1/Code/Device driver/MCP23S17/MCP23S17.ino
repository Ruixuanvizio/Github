#include "MCP23S17.h"

// 硬件引脚配置（与你的接线一致）
#define MCP_SCLK_PIN 47
#define MCP_MOSI_PIN 17
#define MCP_CS_PIN   16

void setup() {
  Serial.begin(115200);
  mcp23s17_init(MCP_SCLK_PIN, MCP_MOSI_PIN, MCP_CS_PIN);
  mux_init();

  // 核心测试：同时设置同端口MUX（无覆盖）

  mux_set_portb_muxes(0, 7);  // PORTB：MUX2=8，MUX3=12

  // 修复：调用公开函数读取OLAT，而非直接调用mcp_spi_read

  uint8_t portb = mcp23s17_read_olat(1);  // 读PORTB输出锁存器
  

}

void loop() {
  // 动态更新示例（每5秒同步修改同端口MUX）
  
  
}