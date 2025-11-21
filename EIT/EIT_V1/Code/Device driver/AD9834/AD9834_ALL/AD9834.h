#ifndef AD9834_H
#define AD9834_H

#include <Arduino.h>  // 适配ESP32S3 Arduino环境，提供引脚控制等基础函数

// 1. SPI引脚定义（与硬件接线对应，参考🔶1-76、🔶1-81）
#define AD9834_FSYNC  9   // 帧同步引脚（低有效，控制数据传输开始/结束）
#define AD9834_SCK    10  // 串行时钟引脚（下降沿采样数据，参考🔶1-212）
#define AD9834_SDA    11  // 串行数据引脚（向AD9834写入数据）

// 2. 寄存器地址宏定义（严格遵循文档寄存器地址规则，参考🔶1-223、🔶1-254、🔶1-267）
#define AD9834_CTRL_REG    0x0000  // 控制寄存器：DB15=0、DB14=0
#define AD9834_FREQ0_REG   0x4000  // FREQ0寄存器：DB15=0、DB14=1（存储输出频率控制字）
#define AD9834_FREQ1_REG   0x8000  // FREQ1寄存器：DB15=1、DB14=0（备用频率寄存器）
#define AD9834_PHASE0_REG  0xC000  // PHASE0寄存器：DB15=1、DB14=1、DB13=0（存储相位控制字）
#define AD9834_PHASE1_REG  0xE000  // PHASE1寄存器：DB15=1、DB14=1、DB13=1（备用相位寄存器）

// 3. 控制寄存器关键位宏定义（参考🔶1-232、🔶1-242、🔶1-269、🔶1-300）
#define AD9834_B28         0x2000  // 28位频率寄存器模式使能（1=启用，参考🔶1-255）
#define AD9834_FSEL        0x0800  // 软件选择频率寄存器：0=FREQ0、1=FREQ1（替代FSELECT引脚，参考🔶1-242）
#define AD9834_PSEL        0x0400  // 软件选择相位寄存器：0=PHASE0、1=PHASE1（替代PSELECT引脚，参考🔶1-242）
#define AD9834_PIN_SW_0    0x0000  // PIN/SW=0（启用软件控制，禁用硬件引脚控制，参考🔶1-232）
#define AD9834_RESET       0x0100  // 软件复位位：1=复位内部寄存器（替代RST引脚，参考🔶1-269）
#define AD9834_MODE        0x0002  // 波形模式：1=三角波（旁路SIN ROM）、0=正弦波（使能SIN ROM，参考🔶1-300）
#define AD9834_MODE_0      0x0000  // 波形模式：正弦波（MODE=0的显式定义）

// 4. 核心常量定义（参考🔶1-23、🔶1-51、🔶1-420）
#define AD9834_MCLK        10000000UL  // 主时钟频率：26MHz（B/C级器件均支持，参考🔶1-51）
#define AD9834_FREQ_BIT    268435456ULL // 2^28（28位频率寄存器位数，参考🔶1-23）
#define AD9834_MAX_FREQ    (AD9834_MCLK / 2UL)  // 最大输出频率：MCLK/2（奈奎斯特频率，参考🔶1-196）
#define AD9834_MIN_FREQ    ((float)AD9834_MCLK / AD9834_FREQ_BIT)  // 最小频率：MCLK/2^28≈0.096Hz（参考🔶1-23）

// 5. 函数原型声明（所有驱动功能的外部调用接口）
void AD9834_Send16Bit(uint16_t data);          // 向AD9834发送16位数据（内部基础功能，参考🔶1-212）
void AD9834_SoftReset(void);                   // 软件复位（替代硬件RST引脚，参考🔶1-269）
void AD9834_SelectFreqReg(uint8_t freq_reg);   // 软件选择频率寄存器（替代FSELECT引脚，参考🔶1-242）
void AD9834_SelectPhaseReg(uint8_t phase_reg); // 软件选择相位寄存器（替代PSELECT引脚，参考🔶1-242）
void AD9834_Init(void);                        // AD9834初始化（全软件控制，参考🔶1-215）
void AD9834_SetFrequency(float f_out, uint8_t freq_reg);  // 设置输出频率（参考🔶1-240）
void AD9834_SetPhase(float phase_deg, uint8_t phase_reg);  // 设置输出相位（参考🔶1-240）
void AD9834_SetWaveform(uint8_t wave_mode);    // 切换输出波形（正弦波/三角波，参考🔶1-300）

#endif  // AD9834_H