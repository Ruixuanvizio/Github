// 1. 引脚定义（仅保留SPI相关引脚，RST/FS/PS通过软件控制）
#define AD9834_FSYNC  9   // FSYNC引脚（🔶1-76、🔶1-81）
#define AD9834_SCK    10  // SCLK引脚（🔶1-76、🔶1-81）
#define AD9834_SDA    11  // SDATA引脚（🔶1-76、🔶1-81）

// 2. AD9834 寄存器与控制位宏定义（补充AD9834_PIN_SW_0，🔶1-223、🔶1-232、🔶1-242）
// 寄存器地址
#define AD9834_CTRL_REG    0x0000  // 控制寄存器（DB15=0, DB14=0）
#define AD9834_FREQ0_REG   0x4000  // FREQ0寄存器（DB15=0, DB14=1）
#define AD9834_FREQ1_REG   0x8000  // FREQ1寄存器（DB15=1, DB14=0）
#define AD9834_PHASE0_REG  0xC000  // PHASE0寄存器（DB15=1, DB14=1, DB13=0）
#define AD9834_PHASE1_REG  0xE000  // PHASE1寄存器（DB15=1, DB14=1, DB13=1）

// 控制寄存器位（补充AD9834_PIN_SW_0：PIN/SW=0，启用软件控制）
#define AD9834_B28         0x2000  // 28位频率寄存器模式（1=使能，🔶1-255）
#define AD9834_HLB         0x1000  // 高/低字节选择
#define AD9834_FSEL        0x0800  // 软件选择频率寄存器（🔶1-242）
#define AD9834_PSEL        0x0400  // 软件选择相位寄存器（🔶1-242）
#define AD9834_PIN_SW      0x0200  // PIN/SW位位置（DB9，1=硬件控制）
#define AD9834_PIN_SW_0    0x0000  // PIN/SW=0（启用软件控制，与AD9834_PIN_SW互补）
#define AD9834_RESET       0x0100  // 软件复位位（1=复位，🔶1-269、🔶1-271）
#define AD9834_SLEEP1      0x0080  // 禁用内部时钟
#define AD9834_SLEEP12     0x0040  // 关断DAC
#define AD9834_OPBITEN     0x0020  // 使能SIGN BIT OUT引脚
#define AD9834_SIGN_PIB    0x0010  // 输出选择
#define AD9834_DIV2        0x0008  // DAC MSB分频
#define AD9834_MODE        0x0002  // 波形模式（1=三角波，0=正弦波，🔶1-300、🔶1-301）
#define AD9834_MODE_0      0x0000  // 波形模式：正弦波（MODE=0）
#define AD9834_RESERVED    0x0001  // 保留位（必须为0）

// 3. 发送16位数据到AD9834（SCLK下降沿采样，🔶1-59、🔶1-212）
void AD9834_Send16Bit(uint16_t data) {
  digitalWrite(AD9834_FSYNC, LOW);
  delayMicroseconds(1);
  
  for (int i = 15; i >= 0; i--) {
    digitalWrite(AD9834_SDA, (data >> i) & 0x01);
    delayMicroseconds(1);
    
    digitalWrite(AD9834_SCK, LOW);
    delayMicroseconds(1);
    digitalWrite(AD9834_SCK, HIGH);
    delayMicroseconds(1);
  }

  digitalWrite(AD9834_FSYNC, HIGH);
  delayMicroseconds(1);
}

// 4. 软件复位（替代硬件RST引脚，🔶1-269、🔶1-271）
void AD9834_SoftReset() {
  // 复位命令：PIN/SW=0（软件控制）、RESET=1（复位）
  uint16_t reset_cmd = AD9834_B28 | AD9834_MODE_0 | AD9834_PIN_SW_0 | AD9834_RESET;
  AD9834_Send16Bit(AD9834_CTRL_REG | reset_cmd);
  delayMicroseconds(100);
  
  // 退出复位：RESET=0
  reset_cmd &= ~AD9834_RESET;
  AD9834_Send16Bit(AD9834_CTRL_REG | reset_cmd);
}

// 5. 软件选择频率寄存器（替代硬件FS引脚，🔶1-242）
void AD9834_SoftSelectFreqReg(uint8_t freq_reg) {
  uint16_t ctrl_cmd = AD9834_B28 | AD9834_MODE_0 | AD9834_PIN_SW_0;
  if (freq_reg == 1) ctrl_cmd |= AD9834_FSEL;
  else ctrl_cmd &= ~AD9834_FSEL;
  AD9834_Send16Bit(AD9834_CTRL_REG | ctrl_cmd);
  delayMicroseconds(10);
}

// 6. 软件选择相位寄存器（替代硬件PS引脚，🔶1-242）
void AD9834_SoftSelectPhaseReg(uint8_t phase_reg) {
  uint16_t ctrl_cmd = AD9834_B28 | AD9834_MODE_0 | AD9834_PIN_SW_0;
  if (phase_reg == 1) ctrl_cmd |= AD9834_PSEL;
  else ctrl_cmd &= ~AD9834_PSEL;
  AD9834_Send16Bit(AD9834_CTRL_REG | ctrl_cmd);
  delayMicroseconds(10);
}

// 7. 初始化AD9834（全软件控制）
void AD9834_Init() {
  pinMode(AD9834_FSYNC, OUTPUT);
  pinMode(AD9834_SCK, OUTPUT);
  pinMode(AD9834_SDA, OUTPUT);

  AD9834_SoftReset();
  AD9834_SoftSelectFreqReg(0);
  AD9834_SoftSelectPhaseReg(0);
}

// 8. 设置输出频率（公式：FREQREG=(f_out×2^28)/75MHz，🔶1-240）
void AD9834_SetFrequency(float f_out, uint8_t freq_reg) {
  if (f_out < 0.28 || f_out > 37500000) {
    Serial.println("频率超出范围（0.28Hz~37.5MHz）");
    return;
  }

  const uint64_t MCLK = 75000000;
  const uint64_t BIT_28 = 268435456;
  uint64_t freq_ctrl = (uint64_t)(f_out * BIT_28) / MCLK;

  uint16_t freq_lsb = (uint16_t)(freq_ctrl & 0x3FFF);
  uint16_t freq_msb = (uint16_t)((freq_ctrl >> 14) & 0x3FFF);

  uint16_t reg_addr = (freq_reg == 1) ? AD9834_FREQ1_REG : AD9834_FREQ0_REG;
  AD9834_Send16Bit(reg_addr | freq_lsb);
  AD9834_Send16Bit(reg_addr | freq_msb);
}

// 9. 设置输出相位（公式：PHASEREG=(phase_deg×4096)/360，🔶1-240）
void AD9834_SetPhase(float phase_deg, uint8_t phase_reg) {
  phase_deg = fmod(phase_deg, 360.0f);
  if (phase_deg < 0) phase_deg += 360.0f;

  uint16_t phase_ctrl = (uint16_t)((phase_deg * 4096.0f) / 360.0f) & 0x0FFF;
  uint16_t reg_addr = (phase_reg == 1) ? AD9834_PHASE1_REG : AD9834_PHASE0_REG;
  AD9834_Send16Bit(reg_addr | phase_ctrl);
}

// 10. 切换输出波形（🔶1-300、🔶1-301）
void AD9834_SetWaveform(uint8_t wave_mode) {
  uint16_t ctrl_cmd = AD9834_B28 | AD9834_PIN_SW_0;
  if (wave_mode == 1) ctrl_cmd |= AD9834_MODE;
  else ctrl_cmd |= AD9834_MODE_0;
  AD9834_Send16Bit(AD9834_CTRL_REG | ctrl_cmd);
}

// 11. 主函数
void setup() {
  Serial.begin(115200);
  AD9834_Init();

  AD9834_SetFrequency(1000000.0f, 0);
  AD9834_SetPhase(0.0f, 0);
  AD9834_SetWaveform(0);
  Serial.println("AD9834初始化完成！当前配置：1MHz 正弦波，0°相位");
}

void loop() {
  delay(5000);
  AD9834_SetFrequency(20000000.0f, 0);
  Serial.println("当前输出：2MHz 正弦波");
  
  delay(5000);
  AD9834_SetFrequency(10000000.0f, 0);
  Serial.println("当前输出：1MHz 正弦波");
}