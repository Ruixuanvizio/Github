#include "MCP23S17.h"

static void spi_write(uint8_t reg, uint8_t data) {
  digitalWrite(MCP_CS, LOW);
  SPI.transfer(MCP_CTRL_W);
  SPI.transfer(reg);
  SPI.transfer(data);
  digitalWrite(MCP_CS, HIGH);
}

static uint8_t spi_read(uint8_t reg) {
  digitalWrite(MCP_CS, LOW);
  SPI.transfer(MCP_CTRL_R);
  SPI.transfer(reg);
  uint8_t data = SPI.transfer(0);
  digitalWrite(MCP_CS, HIGH);
  return data;
}

void mcp_init() {
  pinMode(MCP_CS, OUTPUT);
  digitalWrite(MCP_CS, HIGH);
  SPI.begin(MCP_SCLK, -1, MCP_MOSI); // MISO不用，填-1
  SPI.setFrequency(1000000);
  SPI.setDataMode(SPI_MODE0);
  spi_write(MCP_IODIRA, 0x00); // PORTA全输出
  spi_write(MCP_IODIRB, 0x00); // PORTB全输出
}

void mux_set_porta(uint8_t mux0, uint8_t mux2) {
  uint8_t olat = mcp_read_olat(0); // 读当前状态（简化后仍保留必要的读操作）
  olat = (olat & 0xF0) | (mux0 & 0x0F); // 更新MUX0（低4位）
  olat = (olat & 0x0F) | ((mux2 & 0x0F) << 4); // 更新MUX2（高4位）
  spi_write(MCP_OLATA, olat);
}

void mux_set_portb(uint8_t mux1, uint8_t mux3) {
  uint8_t olat = mcp_read_olat(1); // 读当前状态
  olat = (olat & 0xF0) | (mux1 & 0x0F); // 更新MUX1（低4位）
  olat = (olat & 0x0F) | ((mux3 & 0x0F) << 4); // 更新MUX3（高4位）
  spi_write(MCP_OLATB, olat);
}

uint8_t mcp_read_olat(uint8_t port) {
  return port ? spi_read(MCP_OLATB) : spi_read(MCP_OLATA);
}