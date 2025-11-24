#include "MCP23S17.h"

// SPI 写寄存器（内部使用，不对外暴露）
static void spi_write(const uint8_t reg, const uint8_t data) {
  digitalWrite(MCP_CS, LOW);
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));  // 锁定SPI配置，避免干扰
  SPI.transfer(MCP_CTRL_W);  // 发送写控制字
  SPI.transfer(reg);         // 发送寄存器地址
  SPI.transfer(data);        // 发送数据
  SPI.endTransaction();      // 释放SPI总线
  digitalWrite(MCP_CS, HIGH);
}

// SPI 读寄存器（内部使用，不对外暴露）
static uint8_t spi_read(const uint8_t reg) {
  digitalWrite(MCP_CS, LOW);
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));  // 锁定SPI配置，避免干扰
  SPI.transfer(MCP_CTRL_R);  // 发送读控制字
  SPI.transfer(reg);         // 发送寄存器地址
  const uint8_t data = SPI.transfer(0x00);  // 发送空数据，接收返回值
  SPI.endTransaction();      // 释放SPI总线
  digitalWrite(MCP_CS, HIGH);
  return data;
}

// 初始化MCP23S17：SPI初始化 + 端口设为输出
void mcp_init() {
  pinMode(MCP_CS, OUTPUT);
  digitalWrite(MCP_CS, HIGH);  // 初始不选中芯片，避免误触发
  SPI.begin(MCP_SCLK, -1, MCP_MOSI);  // 初始化SPI（MISO=-1，仅需写操作）
  
  // 配置PORTA、PORTB为全输出（0x00=输出，0xFF=输入）
  spi_write(MCP_IODIRA, 0x00);
  spi_write(MCP_IODIRB, 0x00);
  
  // 初始输出低电平（可选，根据需求调整为0x00~0xFF）
  spi_write(MCP_OLATA, 0x00);
  spi_write(MCP_OLATB, 0x00);
}

// 控制PORTA：双参数分别控制两个MUX（参数仅低4位有效，超出部分自动截断）
// 入参：mux0 → 控制低4位MUX0（通道0~3）；mux2 → 控制高4位MUX2（通道0~3）
void mux_set_porta(const uint8_t mux0, const uint8_t mux2) {
  const uint8_t mux0_val = mux0 & 0x0F;  // 仅保留低4位（通道0~3），避免高位干扰
  const uint8_t mux2_val = mux2 & 0x0F;  // 仅保留低4位（通道0~3），避免高位干扰
  const uint8_t olat = mux0_val | (mux2_val << 4);  // 拼接：低4位=MUX0，高4位=MUX2
  spi_write(MCP_OLATA, olat);
}

// 控制PORTB：双参数分别控制两个MUX（参数仅低4位有效，超出部分自动截断）
// 入参：mux1 → 控制低4位MUX1（通道0~3）；mux3 → 控制高4位MUX3（通道0~3）
// 示例：mux_set_portb(0, 1) → MUX1=通道0，MUX3=通道1
void mux_set_portb(const uint8_t mux1, const uint8_t mux3) {
  const uint8_t mux1_val = mux1 & 0x0F;  // 仅保留低4位（通道0~3），避免高位干扰
  const uint8_t mux3_val = mux3 & 0x0F;  // 仅保留低4位（通道0~3），避免高位干扰
  const uint8_t olat = mux1_val | (mux3_val << 4);  // 拼接：低4位=MUX1，高4位=MUX3
  spi_write(MCP_OLATB, olat);
}

// 读输出锁存器：获取当前端口输出状态
// 入参：port=0 → 读PORTA；port=1 → 读PORTB
uint8_t mcp_read_olat(const uint8_t port) {
  return (port == 0) ? spi_read(MCP_OLATA) : spi_read(MCP_OLATB);
}