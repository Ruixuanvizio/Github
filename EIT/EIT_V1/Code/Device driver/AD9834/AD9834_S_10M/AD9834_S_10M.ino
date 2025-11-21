// 1. 引脚定义（仅保留SPI相关引脚，RST/FS/PS通过软件控制，🔶1-76、🔶1-81）
#define AD9834_FSYNC  9   // FSYNC引脚（帧同步，低有效）
#define AD9834_SCK    10  // SCLK引脚（串行时钟，下降沿采样）
#define AD9834_SDA    11  // SDATA引脚（串行数据输入）

// 2. AD9834 寄存器与控制位宏定义（基于🔶1-223、🔶1-225、🔶1-232、🔶1-254、🔶1-267）
// 寄存器地址
#define AD9834_CTRL_REG    0x0000  // 控制寄存器：DB15=0, DB14=0
#define AD9834_FREQ0_REG   0x4000  // FREQ0寄存器：DB15=0, DB14=1
#define AD9834_FREQ1_REG   0x8000  // FREQ1寄存器：DB15=1, DB14=0
#define AD9834_PHASE0_REG  0xC000  // PHASE0寄存器：DB15=1, DB14=1, DB13=0
#define AD9834_PHASE1_REG  0xE000  // PHASE1寄存器：DB15=1, DB14=1, DB13=1

// 控制寄存器位（含软件替代硬件引脚关键位，🔶1-232、🔶1-242、🔶1-269）
#define AD9834_B28         0x2000  // 28位频率寄存器模式（1=使能，🔶1-255）
#define AD9834_HLB         0x1000  // 高/低字节选择
#define AD9834_FSEL        0x0800  // 软件选频率寄存器：0=FREQ0，1=FREQ1（🔶1-242）
#define AD9834_PSEL        0x0400  // 软件选相位寄存器：0=PHASE0，1=PHASE1（🔶1-242）
#define AD9834_PIN_SW      0x0200  // PIN/SW位位置：1=硬件控制，0=软件控制
#define AD9834_PIN_SW_0    0x0000  // PIN/SW=0（启用软件控制，与AD9834_PIN_SW互补）
#define AD9834_RESET       0x0100  // 软件复位位：1=复位内部寄存器（🔶1-269、🔶1-271）
#define AD9834_SLEEP1      0x0080  // 禁用内部时钟
#define AD9834_SLEEP12     0x0040  // 关断DAC
#define AD9834_OPBITEN     0x0020  // 使能SIGN BIT OUT引脚
#define AD9834_SIGN_PIB    0x0010  // 输出选择：1=比较器输出，0=DAC MSB
#define AD9834_DIV2        0x0008  // DAC MSB分频：1=不分频，0=2分频
#define AD9834_MODE        0x0002  // 波形模式：1=三角波，0=正弦波（🔶1-300、🔶1-301）
#define AD9834_MODE_0      0x0000  // 波形模式：正弦波（MODE=0）
#define AD9834_RESERVED    0x0001  // 保留位（必须为0，🔶1-232）

// 3. MCLK配置（10MHz，符合B/C级器件规格，🔶1-51、🔶1-420）
const uint64_t AD9834_MCLK = 10000000;  // MCLK=10MHz
const uint64_t AD9834_FREQ_BIT = 268435456;  // 2^28（28位频率寄存器，🔶1-23）

// 4. 发送16位数据到AD9834（SCLK下降沿采样，符合时序🔶1-59、🔶1-212）
void AD9834_Send16Bit(uint16_t data) {
  digitalWrite(AD9834_FSYNC, LOW);
  delayMicroseconds(1);  // 满足FSYNC到SCLK建立时间t7≥5ns
  
  // 从DB15到DB0传输（16位串行数据，🔶1-209）
  for (int i = 15; i >= 0; i--) {
    digitalWrite(AD9834_SDA, (data >> i) & 0x01);
    delayMicroseconds(1);
    
    digitalWrite(AD9834_SCK, LOW);  // SCLK下降沿采样数据
    delayMicroseconds(1);
    digitalWrite(AD9834_SCK, HIGH); // SCLK恢复高电平（空闲状态，🔶1-213）
    delayMicroseconds(1);
  }

  digitalWrite(AD9834_FSYNC, HIGH);
  delayMicroseconds(1);  // 满足FSYNC保持时间t8≥10ns
}

// 5. 软件复位（替代硬件RST引脚，效果一致，🔶1-269、🔶1-271）
void AD9834_SoftReset() {
  // 复位命令：PIN/SW=0（软件控制）、RESET=1（复位内部寄存器）
  uint16_t reset_cmd = AD9834_B28 | AD9834_MODE_0 | AD9834_PIN_SW_0 | AD9834_RESET;
  AD9834_Send16Bit(AD9834_CTRL_REG | reset_cmd);
  delayMicroseconds(100);  // 保持复位稳定
  
  // 退出复位：RESET=0（恢复正常工作）
  reset_cmd &= ~AD9834_RESET;
  AD9834_Send16Bit(AD9834_CTRL_REG | reset_cmd);
}

// 6. 软件选择频率寄存器（替代硬件FSELECT引脚，🔶1-242）
// freq_reg：0=FREQ0，1=FREQ1
void AD9834_SoftSelectFreqReg(uint8_t freq_reg) {
  uint16_t ctrl_cmd = AD9834_B28 | AD9834_MODE_0 | AD9834_PIN_SW_0;
  if (freq_reg == 1) {
    ctrl_cmd |= AD9834_FSEL;  // FSEL=1→选择FREQ1
  } else {
    ctrl_cmd &= ~AD9834_FSEL; // FSEL=0→选择FREQ0
  }
  AD9834_Send16Bit(AD9834_CTRL_REG | ctrl_cmd);
  delayMicroseconds(10);  // 满足寄存器切换延迟（8~9个MCLK周期，🔶1-218）
}

// 7. 软件选择相位寄存器（替代硬件PSELECT引脚，🔶1-242）
// phase_reg：0=PHASE0，1=PHASE1
void AD9834_SoftSelectPhaseReg(uint8_t phase_reg) {
  uint16_t ctrl_cmd = AD9834_B28 | AD9834_MODE_0 | AD9834_PIN_SW_0;
  if (phase_reg == 1) {
    ctrl_cmd |= AD9834_PSEL;  // PSEL=1→选择PHASE1
  } else {
    ctrl_cmd &= ~AD9834_PSEL; // PSEL=0→选择PHASE0
  }
  AD9834_Send16Bit(AD9834_CTRL_REG | ctrl_cmd);
  delayMicroseconds(10);  // 满足寄存器切换延迟（🔶1-218）
}

// 8. 初始化AD9834（全软件控制，无硬件引脚依赖，🔶1-215）
void AD9834_Init() {
  // 配置SPI相关引脚为输出模式
  pinMode(AD9834_FSYNC, OUTPUT);
  pinMode(AD9834_SCK, OUTPUT);
  pinMode(AD9834_SDA, OUTPUT);

  // 步骤1：软件复位（避免初始化杂散输出）
  AD9834_SoftReset();

  // 步骤2：默认选择FREQ0、PHASE0寄存器
  AD9834_SoftSelectFreqReg(0);
  AD9834_SoftSelectPhaseReg(0);
}

// 9. 设置输出频率（基于MCLK=10MHz，公式🔶1-240：FREQREG=(f_OUT×2^28)/MCLK）
// f_out：目标频率（0.037Hz~5MHz，MCLK/2），freq_reg：0=FREQ0，1=FREQ1
void AD9834_SetFrequency(float f_out, uint8_t freq_reg) {
  // 频率范围限制（10MHz MCLK下，分辨率≈0.037Hz，上限5MHz，🔶1-6、🔶1-23、🔶1-196）
  if (f_out < 0.037 || f_out > 5000000) {
    Serial.printf("频率超出范围（0.037Hz~5MHz），当前输入：%.2fHz\n", f_out);
    return;
  }

  // 计算28位频率控制字（避免溢出，用64位整数）
  uint64_t freq_ctrl = (uint64_t)(f_out * AD9834_FREQ_BIT) / AD9834_MCLK;
  uint16_t freq_lsb = (uint16_t)(freq_ctrl & 0x3FFF);  // 低14位
  uint16_t freq_msb = (uint16_t)((freq_ctrl >> 14) & 0x3FFF);  // 高14位

  // 选择目标频率寄存器
  uint16_t reg_addr = (freq_reg == 1) ? AD9834_FREQ1_REG : AD9834_FREQ0_REG;

  // 28位模式：先写低14位，再写高14位（🔶1-255）
  AD9834_Send16Bit(reg_addr | freq_lsb);
  AD9834_Send16Bit(reg_addr | freq_msb);
}

// 10. 设置输出相位（公式🔶1-240：PHASEREG=(phase_deg×4096)/360，12位精度）
// phase_deg：目标相位（0~360°），phase_reg：0=PHASE0，1=PHASE1
void AD9834_SetPhase(float phase_deg, uint8_t phase_reg) {
  // 相位范围归一化（0~360°）
  phase_deg = fmod(phase_deg, 360.0f);
  if (phase_deg < 0) phase_deg += 360.0f;

  // 计算12位相位控制字（4096=2^12，🔶1-181）
  uint16_t phase_ctrl = (uint16_t)((phase_deg * 4096.0f) / 360.0f) & 0x0FFF;

  // 选择目标相位寄存器
  uint16_t reg_addr = (phase_reg == 1) ? AD9834_PHASE1_REG : AD9834_PHASE0_REG;

  AD9834_Send16Bit(reg_addr | phase_ctrl);
}

// 11. 切换输出波形（0=正弦波，1=三角波，🔶1-300、🔶1-301）
void AD9834_SetWaveform(uint8_t wave_mode) {
  uint16_t ctrl_cmd = AD9834_B28 | AD9834_PIN_SW_0;  // 保持软件控制+28位频率模式
  if (wave_mode == 1) {
    ctrl_cmd |= AD9834_MODE;       // MODE=1→三角波（旁路SIN ROM）
  } else {
    ctrl_cmd |= AD9834_MODE_0;     // MODE=0→正弦波（使能SIN ROM）
  }
  AD9834_Send16Bit(AD9834_CTRL_REG | ctrl_cmd);
}

// 12. 主函数：初始化与功能测试
void setup() {
  Serial.begin(115200);  // 串口调试（可选）
  AD9834_Init();         // 初始化AD9834（全软件控制，MCLK=10MHz）

  // 示例配置：FREQ0=1MHz（10MHz MCLK下合法）、PHASE0=0°、正弦波
  AD9834_SetFrequency(1000000.0f, 0);  // FREQ0=1MHz，FREQREG=(1e6×268435456)/10e6=26843.5456→26844
  AD9834_SetPhase(0.0f, 0);            // PHASE0=0°，PHASEREG=(0×4096)/360=0
  AD9834_SetWaveform(0);               // 切换为正弦波

  Serial.println("AD9834初始化完成！MCLK=10MHz，当前配置：1MHz 正弦波，0°相位");
}

void loop() {
  // 测试1：软件切换频率（1MHz ↔ 2MHz，均在10MHz MCLK合法范围内）
  delay(5000);
  AD9834_SetFrequency(20000.0f, 0);  // FREQ0更新为2MHz
  Serial.println("当前输出：2MHz 正弦波");
  
  delay(5000);
  AD9834_SetFrequency(10000.0f, 0);  // FREQ0恢复为1MHz
  Serial.println("当前输出：1MHz 正弦波");

  // 测试2：软件切换波形（正弦波 ↔ 三角波，注释可取消启用）
  // delay(5000);
  // AD9834_SetWaveform(1);
  // Serial.println("当前输出：1MHz 三角波");
  // delay(5000);
  // AD9834_SetWaveform(0);
  // Serial.println("当前输出：1MHz 正弦波");

  // 测试3：软件切换频率寄存器（FREQ0 ↔ FREQ1，注释可取消启用）
  // delay(5000);
  // AD9834_SetFrequency(3000000.0f, 1);  // FREQ1=3MHz
  // AD9834_SoftSelectFreqReg(1);         // 软件选择FREQ1
  // Serial.println("当前输出：3MHz 正弦波（FREQ1）");
  // delay(5000);
  // AD9834_SoftSelectFreqReg(0);         // 软件切换回FREQ0
  // Serial.println("当前输出：1MHz 正弦波（FREQ0）");
}