#ifndef MCP23S17_H
#define MCP23S17_H

#include "Arduino.h"
#include "SPI.h"

// -------------------------- 硬件引脚配置（仅需修改这里）--------------------------
#define MCP_SCLK 38  // SPI 时钟引脚
#define MCP_MOSI 37  // SPI 主机输出/从机输入引脚（写MCP23S17用）
#define MCP_CS   39  // SPI 片选引脚（低电平有效）

// -------------------------- 寄存器地址（MCP23S17 标准地址）--------------------------
#define MCP_IODIRA 0x00  // 端口A 方向寄存器（0=输出，1=输入）
#define MCP_IODIRB 0x01  // 端口B 方向寄存器
#define MCP_OLATA  0x14  // 端口A 输出锁存器（写=设置输出值，读=获取当前输出值）
#define MCP_OLATB  0x15  // 端口B 输出锁存器
#define MCP_CTRL_W 0x40  // SPI 写控制字（A2A1A0=000，R/W=0）
#define MCP_CTRL_R 0x41  // SPI 读控制字（A2A1A0=000，R/W=1）

// -------------------------- MUX 与 MCP23S17 引脚映射（GPA/GPB 完全对称）--------------------------
// 核心规则：每个端口分「低4位」和「高4位」，各对应一个 MUX，双参数分别控制两个 MUX
// PORTA（双参数：mux0, mux2）：
// - 低4位（bit0~bit3）→ MUX0：GPA0=通道0、GPA1=通道1、GPA2=通道2、GPA3=通道3（第一个参数控制）
// - 高4位（bit4~bit7）→ MUX2：GPA4=通道0、GPA5=通道1、GPA6=通道2、GPA7=通道3（第二个参数控制）
// PORTB（双参数：mux1, mux3）：
// - 低4位（bit0~bit3）→ MUX1：GPB0=通道0、GPB1=通道1、GPB2=通道2、GPB3=通道3（第一个参数控制）
// - 高4位（bit4~bit7）→ MUX3：GPB4=通道0、GPB5=通道1、GPB6=通道2、GPB7=通道3（第二个参数控制）

// -------------------------- 函数声明（保留双参数核心接口）--------------------------
void mcp_init();                                   // 初始化MCP23S17（自动使用上面的引脚）
void mux_set_porta(const uint8_t mux0, const uint8_t mux2);  // PORTA：mux0=低4位MUX，mux2=高4位MUX
void mux_set_portb(const uint8_t mux1, const uint8_t mux3);  // PORTB：mux1=低4位MUX，mux3=高4位MUX
uint8_t mcp_read_olat(const uint8_t port);         // 读输出锁存器（0=PORTA，1=PORTB）

#endif