#include "AD9834.h"
#include "AD5621.h"
void setup() {
    Serial.begin(115200);
    AD9834_Init();  // 初始化AD9834
    AD5621_Init(); // 初始化AD5621（必须调用）
    AD5621_SetVoltage(0.01);
    // 示例：设置5MHz正弦波（26MHz MCLK下合法，参考🔶1-196）
    AD9834_SetFrequency(50000.0f, 0);  // FREQ0=5MHz
    AD9834_SetPhase(0.0f, 0);            // PHASE0=0°
    AD9834_SetWaveform(1);               // 输出正弦波
    Serial.println("AD9834初始化完成：MCLK=26MHz，5MHz正弦波");
}

void loop() {
    // 循环切换频率：5MHz ↔ 8MHz（均在AD9834_MAX_FREQ=13MHz范围内）
    delay(5000);
    AD9834_SetFrequency(80000.0f, 0);
    Serial.println("当前输出：8MHz正弦波");
    
    delay(5000);
    AD9834_SetFrequency(50000.0f, 0);
    Serial.println("当前输出：5MHz正弦波");
}