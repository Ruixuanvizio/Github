#include "MCP23S17.h"

// 静态变量：仅内部存储CS引脚，不对外暴露
static int _mcp_cs_pin;

// 内部SPI写函数（GPB稳定逻辑，完全复用）
static void mcp_spi_write(uint8_t reg_addr, uint8_t data) {
  digitalWrite(_mcp_cs_pin, LOW);
  delayMicroseconds(1);  // 满足CS Setup Time（≥50ns，手册要求）
  
  SPI.transfer(MCP_CTRL_WRITE);
  SPI.transfer(reg_addr);
  SPI.transfer(data);
  
  delayMicroseconds(2);  // 确保数据写入生效（＞手册500ns最大延迟）
  digitalWrite(_mcp_cs_pin, HIGH);
  delayMicroseconds(1);  // 满足CS Hold Time（≥50ns）
}

// 内部SPI读函数（GPB稳定逻辑，完全复用）
static uint8_t mcp_spi_read(uint8_t reg_addr) {
  uint8_t data;
  digitalWrite(_mcp_cs_pin, LOW);
  delayMicroseconds(1);
  
  SPI.transfer(MCP_CTRL_READ);
  SPI.transfer(reg_addr);
  data = SPI.transfer(0x00);  // 发空字节读数据
  
  delayMicroseconds(1);
  digitalWrite(_mcp_cs_pin, HIGH);
  return data;
}

// 公开函数：读取OLAT寄存器（对外提供状态查询）
uint8_t mcp23s17_read_olat(uint8_t port) {
  if (port == 0) return mcp_spi_read(MCP23S17_OLATA);  // 读GPA输出锁存器
  else return mcp_spi_read(MCP23S17_OLATB);            // 读GPB输出锁存器
}

// MCP23S17初始化（SPI参数对齐GPB稳定配置）
void mcp23s17_init(int sclk_pin, int mosi_pin, int cs_pin) {
  _mcp_cs_pin = cs_pin;
  pinMode(_mcp_cs_pin, OUTPUT);
  digitalWrite(_mcp_cs_pin, HIGH);  // CS默认高电平（未选中芯片）
  
  // SPI配置（GPB稳定参数：Mode0、1MHz、高位优先）
  SPI.begin(sclk_pin, -1, mosi_pin, cs_pin);  // MISO=-1（仅控制MUX，无需读外部信号）
  SPI.setFrequency(1000000);                  // 频率≤10MHz（手册上限）
  SPI.setDataMode(SPI_MODE0);                 // 时钟极性/相位：Mode0（CPOL=0，CPHA=0）
  SPI.setBitOrder(MSBFIRST);                  // 数据高位优先（手册要求）
}

// 设置端口方向（0=全输出，对齐GPB配置）
void mcp23s17_set_port_mode(uint8_t port, uint8_t mode) {
  if (port == 0) mcp_spi_write(MCP23S17_IODIRA, mode);  // GPA设为全输出（mode=0x00）
  else mcp_spi_write(MCP23S17_IODIRB, mode);            // GPB设为全输出
}

// 核心函数1：GPA批量设置（完全对齐GPB逻辑）
// mux0_ch：MUX0（GPA0~3）通道；mux2_ch：MUX2（GPA4~7）通道
void mux_set_porta_muxes(uint8_t mux0_ch, uint8_t mux2_ch) {
  if (mux0_ch > 15 || mux2_ch > 15) return;  // 通道范围0~15（4-16复用器）
  
  // 1. 刷新GPA状态（同GPB读GPIOB逻辑，确保OLATA是最新值）
  mcp_spi_read(MCP23S17_GPIOA);
  uint8_t porta_olat = mcp_spi_read(MCP23S17_OLATA);
  
  // 2. 控制MUX0（GPA0~3，低4位）：用0x0F掩码（仅清低4位，不影响高4位）
  porta_olat &= ~0x0F;  // 清除GPA0~3旧值
  porta_olat |= (mux0_ch & 0x0F) << 0;  // 通道值直接写入低4位（无需移位）
  
  // 3. 控制MUX2（GPA4~7，高4位）：用0xF0掩码（仅清高4位，不影响低4位）
  porta_olat &= ~0xF0;  // 清除GPA4~7旧值
  porta_olat |= (mux2_ch & 0x0F) << 4;  // 通道值左移4位，写入高4位
  
  // 4. 单次写入GPA：MUX0和MUX2状态同步生效（无覆盖）
  mcp_spi_write(MCP23S17_OLATA, porta_olat);
}

// 核心函数2：GPB批量设置（原稳定逻辑，无修改）
// mux1_ch：MUX1（GPB0~3）通道；mux3_ch：MUX3（GPB4~7）通道
void mux_set_portb_muxes(uint8_t mux1_ch, uint8_t mux3_ch) {
  if (mux1_ch > 15 || mux3_ch > 15) return;
  
  // 1. 刷新GPB状态
  mcp_spi_read(MCP23S17_GPIOB);
  uint8_t portb_olat = mcp_spi_read(MCP23S17_OLATB);
  
  // 2. 控制MUX1（GPB0~3，低4位）
  portb_olat &= ~0x0F;
  portb_olat |= (mux1_ch & 0x0F) << 0;
  
  // 3. 控制MUX3（GPB4~7，高4位）
  portb_olat &= ~0xF0;
  portb_olat |= (mux3_ch & 0x0F) << 4;
  
  // 4. 单次写入GPB
  mcp_spi_write(MCP23S17_OLATB, portb_olat);
}

// 单独设置函数（按需修改单个MUX，逻辑对齐批量函数）
void mux_select_channel(uint8_t mux_num, uint8_t channel) {
  if (channel > 15) return;
  
  switch (mux_num) {
    // MUX0（GPA0~3）：对齐批量函数的MUX0逻辑
    case 0: {
      mcp_spi_read(MCP23S17_GPIOA);
      uint8_t porta_olat = mcp_spi_read(MCP23S17_OLATA);
      porta_olat &= ~0x0F;  // 仅清低4位
      porta_olat |= (channel & 0x0F) << 0;
      mcp_spi_write(MCP23S17_OLATA, porta_olat);
      break;
    }
    
    // MUX1（GPB0~3）：对齐批量函数的MUX1逻辑
    case 1: {
      mcp_spi_read(MCP23S17_GPIOB);
      uint8_t portb_olat = mcp_spi_read(MCP23S17_OLATB);
      portb_olat &= ~0x0F;  // 仅清低4位
      portb_olat |= (channel & 0x0F) << 0;
      mcp_spi_write(MCP23S17_OLATB, portb_olat);
      break;
    }
    
    // MUX2（GPA4~7）：对齐批量函数的MUX2逻辑
    case 2: {
      mcp_spi_read(MCP23S17_GPIOA);
      uint8_t porta_olat = mcp_spi_read(MCP23S17_OLATA);
      porta_olat &= ~0xF0;  // 仅清高4位
      porta_olat |= (channel & 0x0F) << 4;
      mcp_spi_write(MCP23S17_OLATA, porta_olat);
      break;
    }
    
    // MUX3（GPB4~7）：原稳定逻辑
    case 3: {
      mcp_spi_read(MCP23S17_GPIOB);
      uint8_t portb_olat = mcp_spi_read(MCP23S17_OLATB);
      portb_olat &= ~0xF0;  // 仅清高4位
      portb_olat |= (channel & 0x0F) << 4;
      mcp_spi_write(MCP23S17_OLATB, portb_olat);
      break;
    }
  }
}

// 所有MUX初始化（批量设置通道0，确保上电状态一致）
void mux_init() {
  mcp23s17_set_port_mode(0, 0x00);  // GPA全输出
  mcp23s17_set_port_mode(1, 0x00);  // GPB全输出
  
  // 用批量函数初始化，避免单独设置的潜在问题
  mux_set_porta_muxes(0, 0);  // GPA：MUX0=0，MUX2=0
  mux_set_portb_muxes(0, 0);  // GPB：MUX1=0，MUX3=0
}