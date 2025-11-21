#ifndef MCP23S17_H
#define MCP23S17_H

#include "Arduino.h"
#include "SPI.h"

// MCP23S17 SPI引脚（仅在这里定义，改硬件只改这里）
#define MCP_SCLK 47
#define MCP_MOSI 17
#define MCP_CS   16

// 寄存器地址（只保留必要的）
#define MCP_IODIRA 0x00
#define MCP_IODIRB 0x01
#define MCP_OLATA  0x14
#define MCP_OLATB  0x15
#define MCP_CTRL_W 0x40
#define MCP_CTRL_R 0x41

// 函数声明（只保留核心功能）
void mcp_init();                     // 初始化（内部已用上面的引脚）
void mux_set_porta(uint8_t mux0, uint8_t mux2);  // 控制PORTA的MUX0和MUX2
void mux_set_portb(uint8_t mux1, uint8_t mux3);  // 控制PORTB的MUX1和MUX3
uint8_t mcp_read_olat(uint8_t port); // 读输出锁存器（0=A，1=B）

#endif