// 引脚定义（与硬件接线对应）
#define AD9834_FSYNC  9   // FSYNC引脚
#define AD9834_SCK    10  // SCLK引脚
#define AD9834_SDA    11  // SDATA引脚
#define AD9834_RST    12  // RESET引脚
#define AD9834_FS     4   // FSELECT引脚
#define AD9834_PS     5   // PSELECT引脚

// AD9834 寄存器配置宏定义（🔶1-223、🔶1-225、🔶1-254、🔶1-267）
#define AD9834_CTRL_REG    0x0000  // 控制寄存器：DB15=0, DB14=0
#define AD9834_FREQ0_REG   0x4000  // FREQ0寄存器：DB15=0, DB14=1
#define AD9834_FREQ1_REG   0x8000  // FREQ1寄存器：DB15=1, DB14=0
#define AD9834_PHASE0_REG  0xC000  // PHASE0寄存器：DB15=1, DB14=1, DB13=0
#define AD9834_PHASE1_REG  0xE000  // PHASE1寄存器：DB15=1, DB14=1, DB13=1

// 控制寄存器位定义（🔶1-222、🔶1-231、🔶1-232）
#define AD9834_B28         0x2000  // 28位频率寄存器模式（1=使能，🔶1-255）
#define AD9834_HLB         0x1000  // 高/低字节选择
#define AD9834_FSEL        0x0800  // 软件选择频率寄存器（🔶1-242）
#define AD9834_PSEL        0x0400  // 软件选择相位寄存器（🔶1-242）
#define AD9834_RESET       0x0100  // 复位内部寄存器（🔶1-269）
#define AD9834_SLEEP1      0x0080  // 禁用内部时钟（🔶1-276）
#define AD9834_SLEEP12     0x0040  // 关断DAC（🔶1-276）
#define AD9834_OPBITEN     0x0020  // 使能SIGN BIT OUT引脚（🔶1-286）
#define AD9834_SIGN_PIB    0x0010  // 输出选择（🔶1-288）
#define AD9834_DIV2        0x0008  // DAC MSB分频（🔶1-288）
#define AD9834_MODE        0x0002  // 波形模式（1=三角波，0=正弦波，🔶1-300、🔶1-301）
#define AD9834_MODE_0      0x0000  // 波形模式：正弦波（MODE=0）
#define AD9834_RESERVED    0x0001  // 保留位（必须为0）

// 发送16位数据到AD9834（SCLK下降沿采样，🔶1-59、🔶1-212）
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

// 初始化AD9834（🔶1-215、🔶1-269、🔶1-270）
void AD9834_Init() {
  pinMode(AD9834_FSYNC, OUTPUT);
  pinMode(AD9834_SCK, OUTPUT);
  pinMode(AD9834_SDA, OUTPUT);
  pinMode(AD9834_RST, OUTPUT);
  pinMode(AD9834_FS, OUTPUT);
  pinMode(AD9834_PS, OUTPUT);

  // 硬件复位
  digitalWrite(AD9834_RST, HIGH);
  delayMicroseconds(100);
  digitalWrite(AD9834_RST, LOW);
  delayMicroseconds(100);

  // 初始化控制寄存器
  uint16_t ctrl_init = AD9834_B28 | AD9834_MODE_0;
  AD9834_Send16Bit(AD9834_CTRL_REG | ctrl_init);

  // 默认选择FREQ0和PHASE0
  digitalWrite(AD9834_FS, LOW);
  digitalWrite(AD9834_PS, LOW);
}

// 设置输出频率（公式：FREQREG = (f_out * 2^28) / f_MCLK，🔶1-240；f_MCLK=75MHz）
void AD9834_SetFrequency(float f_out, uint8_t freq_reg) {
  if (f_out < 0.28 || f_out > 37500000) {
    Serial.println("频率超出AD9834范围（0.28Hz~37.5MHz）");
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

// 设置输出相位（公式：PHASEREG = (phase_deg * 4096) / 360，🔶1-240）
void AD9834_SetPhase(float phase_deg, uint8_t phase_reg) {
  phase_deg = fmod(phase_deg, 360.0f);
  if (phase_deg < 0) phase_deg += 360.0f;

  uint16_t phase_ctrl = (uint16_t)((phase_deg * 4096.0f) / 360.0f) & 0x0FFF;

  uint16_t reg_addr = (phase_reg == 1) ? AD9834_PHASE1_REG : AD9834_PHASE0_REG;

  AD9834_Send16Bit(reg_addr | phase_ctrl);
}

// 切换输出波形（0=正弦波，1=三角波，🔶1-300、🔶1-301）
void AD9834_SetWaveform(uint8_t wave_mode) {
  uint16_t ctrl_val = AD9834_B28;
  if (wave_mode == 1) {
    ctrl_val |= AD9834_MODE;
  } else {
    ctrl_val |= AD9834_MODE_0;
  }
  AD9834_Send16Bit(AD9834_CTRL_REG | ctrl_val);
}

// 硬件选择频率寄存器（🔶1-242）
void AD9834_SelectFreqReg(uint8_t freq_reg) {
  digitalWrite(AD9834_FS, freq_reg ? HIGH : LOW);
  delayMicroseconds(10);
}

// 硬件选择相位寄存器（🔶1-242）
void AD9834_SelectPhaseReg(uint8_t phase_reg) {
  digitalWrite(AD9834_PS, phase_reg ? HIGH : LOW);
  delayMicroseconds(10);
}

// 主函数：初始化与测试
void setup() {
  Serial.begin(115200);
  AD9834_Init();

  // 示例配置：FREQ0=1MHz，PHASE0=0度，正弦波
  AD9834_SetFrequency(1000000.0f, 0);
  AD9834_SetPhase(0.0f, 0);
  AD9834_SetWaveform(0);
  AD9834_SelectFreqReg(0);
  AD9834_SelectPhaseReg(0);

  Serial.println("AD9834初始化完成！当前配置：1MHz 正弦波，0度相位");
}

void loop() {
  // 切换频率：1MHz ↔ 2MHz
  delay(5000);
  AD9834_SetFrequency(2000000.0f, 0);
  Serial.println("当前输出：2MHz 正弦波");
  
  delay(5000);
  AD9834_SetFrequency(1000000.0f, 0);
  Serial.println("当前输出：1MHz 正弦波");

  // 切换波形（注释可取消启用）
  // delay(5000);
  // AD9834_SetWaveform(1);
  // Serial.println("当前输出：1MHz 三角波");
  // delay(5000);
  // AD9834_SetWaveform(0);
  // Serial.println("当前输出：1MHz 正弦波");
}