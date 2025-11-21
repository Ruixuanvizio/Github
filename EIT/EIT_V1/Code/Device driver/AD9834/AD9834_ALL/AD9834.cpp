#include "AD9834.h"  // 包含头文件，关联宏定义与函数原型

// 1. 向AD9834发送16位数据（遵循SCLK下降沿采样时序，参考🔶1-59、🔶1-212）
void AD9834_Send16Bit(uint16_t data) {
    digitalWrite(AD9834_FSYNC, LOW);  // 拉低FSYNC，启动数据传输（满足t7≥5ns建立时间）
    delayMicroseconds(1);              // 确保时序合规，避免数据采样错误
    
    // 从最高位（DB15）到最低位（DB0）传输16位数据（参考🔶1-209）
    for (int i = 15; i >= 0; i--) {
        digitalWrite(AD9834_SDA, (data >> i) & 0x01);  // 输出当前位数据
        delayMicroseconds(1);
        
        digitalWrite(AD9834_SCK, LOW);  // SCLK下降沿，AD9834采样数据（参考🔶1-212）
        delayMicroseconds(1);
        digitalWrite(AD9834_SCK, HIGH); // SCLK恢复高电平（空闲状态，参考🔶1-213）
        delayMicroseconds(1);
    }

    digitalWrite(AD9834_FSYNC, HIGH); // 拉高FSYNC，结束数据传输（满足t8≥10ns保持时间）
    delayMicroseconds(1);
}

// 2. 软件复位（替代硬件RST引脚，复位内部寄存器至0，参考🔶1-269、🔶1-271）
void AD9834_SoftReset(void) {
    // 复位命令：28位频率模式+正弦波+软件控制+复位使能
    uint16_t reset_cmd = AD9834_B28 | AD9834_MODE_0 | AD9834_PIN_SW_0 | AD9834_RESET;
    AD9834_Send16Bit(AD9834_CTRL_REG | reset_cmd);
    delayMicroseconds(100);  // 保持复位状态，确保内部寄存器稳定复位
    
    reset_cmd &= ~AD9834_RESET;  // 清除复位位，退出复位状态
    AD9834_Send16Bit(AD9834_CTRL_REG | reset_cmd);
}

// 3. 软件选择频率寄存器（替代硬件FSELECT引脚，参考🔶1-242）
// freq_reg：0=选择FREQ0，1=选择FREQ1
void AD9834_SelectFreqReg(uint8_t freq_reg) {
    uint16_t ctrl_cmd = AD9834_B28 | AD9834_MODE_0 | AD9834_PIN_SW_0;
    // 根据freq_reg设置FSEL位：1=选择FREQ1，0=选择FREQ0
    ctrl_cmd = (freq_reg == 1) ? (ctrl_cmd | AD9834_FSEL) : (ctrl_cmd & ~AD9834_FSEL);
    AD9834_Send16Bit(AD9834_CTRL_REG | ctrl_cmd);
    delayMicroseconds(10);  // 满足寄存器切换延迟（8~9个MCLK周期，参考🔶1-218）
}

// 4. 软件选择相位寄存器（替代硬件PSELECT引脚，参考🔶1-242）
// phase_reg：0=选择PHASE0，1=选择PHASE1
void AD9834_SelectPhaseReg(uint8_t phase_reg) {
    uint16_t ctrl_cmd = AD9834_B28 | AD9834_MODE_0 | AD9834_PIN_SW_0;
    // 根据phase_reg设置PSEL位：1=选择PHASE1，0=选择PHASE0
    ctrl_cmd = (phase_reg == 1) ? (ctrl_cmd | AD9834_PSEL) : (ctrl_cmd & ~AD9834_PSEL);
    AD9834_Send16Bit(AD9834_CTRL_REG | ctrl_cmd);
    delayMicroseconds(10);  // 满足寄存器切换延迟（参考🔶1-218）
}

// 5. AD9834初始化（全软件控制，无需依赖硬件RST/FS/PS引脚，参考🔶1-215）
void AD9834_Init(void) {
    // 配置SPI相关引脚为输出模式
    pinMode(AD9834_FSYNC, OUTPUT);
    pinMode(AD9834_SCK, OUTPUT);
    pinMode(AD9834_SDA, OUTPUT);

    // 步骤1：软件复位（避免初始化时产生杂散输出，参考🔶1-269）
    AD9834_SoftReset();
    // 步骤2：默认选择FREQ0和PHASE0寄存器（初始状态配置）
    AD9834_SelectFreqReg(0);
    AD9834_SelectPhaseReg(0);
}

// 6. 设置输出频率（基于DDS频率公式：FREQREG=(f_out×2^28)/MCLK，参考🔶1-240）
// f_out：目标输出频率（需满足AD9834_MIN_FREQ ≤ f_out ≤ AD9834_MAX_FREQ）
// freq_reg：0=写入FREQ0，1=写入FREQ1
void AD9834_SetFrequency(float f_out, uint8_t freq_reg) {
    // 频率范围校验（超出范围则打印错误信息，参考🔶1-6、🔶1-196）
    if (f_out < AD9834_MIN_FREQ || f_out > AD9834_MAX_FREQ) {
        Serial.printf("AD9834频率超出范围！应在%.3fHz~%luHz，当前输入：%.2fHz\n", 
                      AD9834_MIN_FREQ, AD9834_MAX_FREQ, f_out);
        return;
    }

    // 计算28位频率控制字（64位整数避免溢出，参考🔶1-23、🔶1-240）
    uint64_t freq_ctrl = (uint64_t)(f_out * AD9834_FREQ_BIT) / AD9834_MCLK;
    uint16_t freq_lsb = (uint16_t)(freq_ctrl & 0x3FFF);  // 频率控制字低14位
    uint16_t freq_msb = (uint16_t)((freq_ctrl >> 14) & 0x3FFF);  // 频率控制字高14位

    // 选择目标频率寄存器并写入数据（28位模式：先写LSB，再写MSB，参考🔶1-255）
    uint16_t reg_addr = (freq_reg == 1) ? AD9834_FREQ1_REG : AD9834_FREQ0_REG;
    AD9834_Send16Bit(reg_addr | freq_lsb);
    AD9834_Send16Bit(reg_addr | freq_msb);
}

// 7. 设置输出相位（基于相位公式：PHASEREG=(phase_deg×4096)/360，12位精度，参考🔶1-240）
// phase_deg：目标相位（0~360°，超出范围自动归一化）
// phase_reg：0=写入PHASE0，1=写入PHASE1
void AD9834_SetPhase(float phase_deg, uint8_t phase_reg) {
    // 相位范围归一化（确保在0~360°，参考🔶1-181）
    phase_deg = fmod(phase_deg, 360.0f);
    if (phase_deg < 0) {
        phase_deg += 360.0f;
    }

    // 计算12位相位控制字（4096=2^12，参考🔶1-181）
    uint16_t phase_ctrl = (uint16_t)((phase_deg * 4096.0f) / 360.0f) & 0x0FFF;
    // 选择目标相位寄存器并写入数据（参考🔶1-267）
    uint16_t reg_addr = (phase_reg == 1) ? AD9834_PHASE1_REG : AD9834_PHASE0_REG;
    AD9834_Send16Bit(reg_addr | phase_ctrl);
}

// 8. 切换输出波形（0=正弦波，1=三角波，参考🔶1-300、🔶1-301）
void AD9834_SetWaveform(uint8_t wave_mode) {
    uint16_t ctrl_cmd = AD9834_B28 | AD9834_PIN_SW_0;
    // 根据wave_mode设置MODE位：1=三角波（旁路SIN ROM），0=正弦波（使能SIN ROM）
    ctrl_cmd = (wave_mode == 1) ? (ctrl_cmd | AD9834_MODE) : (ctrl_cmd | AD9834_MODE_0);
    AD9834_Send16Bit(AD9834_CTRL_REG | ctrl_cmd);
}