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

// 修改后的mux_set_porta：适配新的GPA引脚接线
void mux_set_porta(uint8_t mux0, uint8_t mux2) {
  uint8_t olat = mcp_read_olat(0); // 读当前PORTA输出状态

  // 清除多路复用器1和2涉及的旧位（避免干扰）
  // 多路复用器1涉及：GPA0(bit0)、GPA1(bit1)、GPA2(bit2)、GPA7(bit7)
  // 多路复用器2涉及：GPA3(bit3)、GPA4(bit4)、GPA5(bit5)、GPA6(bit6)
  olat &= 0x00; // 所有位都被复用，直接清0（若有未使用位可保留，这里全用则清0）

  // 配置多路复用器1（A0-A3对应GPA0、GPA1、GPA2、GPA7）
  // mux0的bit0 → GPA0，bit1 → GPA1，bit2 → GPA2，bit3 → GPA7
  olat |= (mux0 & 0x01) << 0;    // mux0最低位 → GPA0
  olat |= (mux0 & 0x02) << 0;    // mux0次低位 → GPA1（0x02是bit1，左移0保持在bit1）
  olat |= (mux0 & 0x04) << 0;    // mux0第3位 → GPA2（0x04是bit2，左移0保持在bit2）
  olat |= (mux0 & 0x08) << 4;    // mux0第4位 → GPA7（0x08是bit3，左移4到bit7）

  // 配置多路复用器2（A0-A3对应GPA3、GPA4、GPA5、GPA6）
  // mux2的bit0 → GPA3，bit1 → GPA4，bit2 → GPA5，bit3 → GPA6
  olat |= (mux2 & 0x01) << 3;    // mux2最低位 → GPA3（左移3到bit3）
  olat |= (mux2 & 0x02) << 3;    // mux2次低位 → GPA4（0x02是bit1，左移3到bit4）
  olat |= (mux2 & 0x04) << 3;    // mux2第3位 → GPA5（0x04是bit2，左移3到bit5）
  olat |= (mux2 & 0x08) << 3;    // mux2第4位 → GPA6（0x08是bit3，左移3到bit6）

  spi_write(MCP_OLATA, olat); // 写入更新后的值
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