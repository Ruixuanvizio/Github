#ifndef MCP23S17_H
#define MCP23S17_H

#include "Arduino.h"
#include "SPI.h"

// MCP23S17寄存器地址（BANK=0默认配置，确保无遗漏）
#define MCP23S17_IODIRA  0x00  // PORTA方向寄存器（0=输出）
#define MCP23S17_IODIRB  0x01  // PORTB方向寄存器
#define MCP23S17_GPIOA   0x12  // PORTA实际电平寄存器（用于状态刷新）
#define MCP23S17_GPIOB   0x13  // PORTB实际电平寄存器
#define MCP23S17_OLATA   0x14  // PORTA输出锁存器（优先用，确保状态稳定）
#define MCP23S17_OLATB   0x15  // PORTB输出锁存器

// SPI控制字节（A0/A1/A2=0，符合MCP23S17手册）
#define MCP_CTRL_WRITE   0x40  // SPI写操作
#define MCP_CTRL_READ    0x41  // SPI读操作

// 函数声明（分端口批量设置+单独设置+公开读取）
void mcp23s17_init(int sclk_pin, int mosi_pin, int cs_pin);  // 初始化MCP23S17
void mcp23s17_set_port_mode(uint8_t port, uint8_t mode);     // 设置端口方向（0=全输出）
void mux_init();                                             // 所有MUX初始化（通道0）

// 批量设置函数（同端口双MUX同时设置，核心稳定逻辑）
void mux_set_porta_muxes(uint8_t mux0_ch, uint8_t mux2_ch);  // GPA：MUX0（0~3）+MUX2（4~7）
void mux_set_portb_muxes(uint8_t mux1_ch, uint8_t mux3_ch);  // GPB：MUX1（0~3）+MUX3（4~7）

// 单独设置函数（按需修改单个MUX）
void mux_select_channel(uint8_t mux_num, uint8_t channel);   // mux_num:0(GPA)、1(GPB)、2(GPA)、3(GPB)

// 公开读取函数（避免调用内部static函数）
uint8_t mcp23s17_read_olat(uint8_t port);                    // port=0→OLATA，port=1→OLATB

#endif