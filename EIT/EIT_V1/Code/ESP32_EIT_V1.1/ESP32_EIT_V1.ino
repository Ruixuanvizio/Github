#include "AD5621.h"
#include "AD9834.h"
#include "MCP23S17.h"
#include "AD5165.h"
#include "AD9240.h"


void setup() {
    Serial.begin(115200);
    AD9834_Init(); // 初始化AD9834
    AD9834_SetFrequency(80000.0f, 0);    // FREQ0=5MHz
    AD9834_SetPhase(0.0f, 0);            // PHASE0=0°
    AD9834_SetWaveform(0);               // 输出正弦波

    AD5621_Init(); // 初始化AD5621
    AD5621_SetVoltage(0.1);

    mcp_init(); // 初始化MCP23S17
    mux_set_porta(7, 8); // MUX07，MUX2=12
    mux_set_portb(5, 10); // MUX1=7，MUX3=12

    AD5165_Init();  // 初始化（内部已设置默认电阻）
    AD5165_SetRWA(50);  // 大电阻，对应小增益

    AD9240_Init(); // 初始化AD9240
    AD9240_SetPeakWindow(10);  // 设置1秒窗口（可选，默认就是1秒）

}

void loop() {
 AD9240_ClockLoop();  // 维持时钟信号

  // 高频更新峰峰值计算（建议比采样率高）
  AD9240_UpdatePeakToPeak();

  // 每500ms打印一次当前状态
  static unsigned long last_print = 0;
  if (millis() - last_print > 50) {
    Serial.print(" 窗口峰峰值: ");
    Serial.println(AD9240_GetLastPeakToPeak());
    Serial.print(" RAW: ");
    Serial.println(AD9240_Read());
    last_print = millis();
    
  }
}
