#include "AD9240.h"
#include "AD9834.h"
#include "AD5621.h"
#include "MCP23S17.h"
#include "AD5165.h"



void setup() {
  Serial.begin(115200);
  while (!Serial);

  // 初始化辅助芯片（按需保留）
  AD9834_Init();
  AD9834_SetFrequency(80000.0f, 0);
  AD9834_SetWaveform(0);
  AD5621_Init();
  AD5621_SetVoltage(0.5);
  AD5165_Init();
  AD5165_SetRWA(50.0f);
  mcp_init();
  mux_set_porta(0, 1);
  mux_set_portb(2, 3);
  ad9240_get_peak();  // 调用一次，输出一次峰峰值

  // 初始化AD9240
  //ad9240_init();
  Serial.println("就绪：发送 'c' 触发采集");
}

// 主函数仅做两件事：监听串口指令 + 调用采集处理函数
void loop() {
  // 监听串口指令：收到'c'启动采集
  mux_set_porta(0, 1);
  mux_set_portb(2, 3);
 if (Serial.available() > 0) {
    char cmd = Serial.read();
    if (cmd == 'c') {
      Serial.println("\n开始采样...");
      mux_set_porta(0, 1);
      mux_set_portb(2, 3);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(0, 1);
      mux_set_portb(3, 4);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(0, 1);
      mux_set_portb(4, 5);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(0, 1);
      mux_set_portb(5, 6);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(0, 1);
      mux_set_portb(6, 7);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(0, 1);
      mux_set_portb(7, 8);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(0, 1);
      mux_set_portb(8, 9);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(0, 1);
      mux_set_portb(9, 10);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(0, 1);
      mux_set_portb(10, 11);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(0, 1);
      mux_set_portb(11, 12);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(0, 1);
      mux_set_portb(12, 13);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(0, 1);
      mux_set_portb(13, 14);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(0, 1);
      mux_set_portb(14, 15);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值


      
      mux_set_porta(1, 2);
      mux_set_portb(3, 4);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(1, 2);
      mux_set_portb(4, 5);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(1, 2);
      mux_set_portb(5, 6);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(1, 2);
      mux_set_portb(6, 7);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(1, 2);
      mux_set_portb(7, 8);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(1, 2);
      mux_set_portb(8, 9);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(1, 2);
      mux_set_portb(9, 10);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(1, 2);
      mux_set_portb(10, 11);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(1, 2);
      mux_set_portb(11, 12);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(1, 2);
      mux_set_portb(12, 13);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(1, 2);
      mux_set_portb(13, 14);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(1, 2);
      mux_set_portb(14, 15);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(1, 2);
      mux_set_portb(15, 0);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值

      mux_set_porta(2, 3);
      mux_set_portb(4, 5);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(2, 3);
      mux_set_portb(5, 6);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(2, 3);
      mux_set_portb(6, 7);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(2, 3);
      mux_set_portb(7, 8);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(2, 3);
      mux_set_portb(8, 9);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(2, 3);
      mux_set_portb(9, 10);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(2, 3);
      mux_set_portb(10, 11);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(2, 3);
      mux_set_portb(11, 12);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(2, 3);
      mux_set_portb(12, 13);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(2, 3);
      mux_set_portb(13, 14);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(2, 3);
      mux_set_portb(14, 15);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(2, 3);
      mux_set_portb(15, 0);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(2, 3);
      mux_set_portb(0, 1);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值


      mux_set_porta(3, 4);
      mux_set_portb(5, 6);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(3, 4);
      mux_set_portb(6, 7);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(3, 4);
      mux_set_portb(7, 8);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(3, 4);
      mux_set_portb(8, 9);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(3, 4);
      mux_set_portb(9, 10);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(3, 4);
      mux_set_portb(10, 11);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(3, 4);
      mux_set_portb(11, 12);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(3, 4);
      mux_set_portb(12, 13);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(3, 4);
      mux_set_portb(13, 14);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(3, 4);
      mux_set_portb(14, 15);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(3, 4);
      mux_set_portb(15, 0);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(3, 4);
      mux_set_portb(0, 1);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(3, 4);
      mux_set_portb(1, 2);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值


      mux_set_porta(4, 5);
      mux_set_portb(6, 7);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(4, 5);
      mux_set_portb(7, 8);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(4, 5);
      mux_set_portb(8, 9);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(4, 5);
      mux_set_portb(9, 10);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(4, 5);
      mux_set_portb(10, 11);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(4, 5);
      mux_set_portb(11, 12);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(4, 5);
      mux_set_portb(12, 13);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(4, 5);
      mux_set_portb(13, 14);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(4, 5);
      mux_set_portb(14, 15);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(4, 5);
      mux_set_portb(15, 0);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(4, 5);
      mux_set_portb(0, 1);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(4, 5);
      mux_set_portb(1, 2);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(4, 5);
      mux_set_portb(2, 3);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值


      mux_set_porta(5, 6);
      mux_set_portb(7, 8);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(5, 6);
      mux_set_portb(8, 9);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(5, 6);
      mux_set_portb(9, 10);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(5, 6);
      mux_set_portb(10, 11);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(5, 6);
      mux_set_portb(11, 12);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(5, 6);
      mux_set_portb(12, 13);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(5, 6);
      mux_set_portb(13, 14);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(5, 6);
      mux_set_portb(14, 15);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(5, 6);
      mux_set_portb(15, 0);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(5, 6);
      mux_set_portb(0, 1);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(5, 6);
      mux_set_portb(1, 2);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(5, 6);
      mux_set_portb(2, 3);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(5, 6);
      mux_set_portb(3, 4);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值


      mux_set_porta(6, 7);
      mux_set_portb(8, 9);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(6, 7);
      mux_set_portb(9, 10);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(6, 7);
      mux_set_portb(10, 11);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(6, 7);
      mux_set_portb(11, 12);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(6, 7);
      mux_set_portb(12, 13);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(6, 7);
      mux_set_portb(13, 14);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(6, 7);
      mux_set_portb(14, 15);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(6, 7);
      mux_set_portb(15, 0);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(6, 7);
      mux_set_portb(0, 1);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(6, 7);
      mux_set_portb(1, 2);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(6, 7);
      mux_set_portb(2, 3);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(6, 7);
      mux_set_portb(3, 4);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(6, 7);
      mux_set_portb(4, 5);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值


      mux_set_porta(7, 8);
      mux_set_portb(9, 10);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(7, 8);
      mux_set_portb(10, 11);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(7, 8);
      mux_set_portb(11, 12);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(7, 8);
      mux_set_portb(12, 13);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(7, 8);
      mux_set_portb(13, 14);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(7, 8);
      mux_set_portb(14, 15);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(7, 8);
      mux_set_portb(15, 0);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(7, 8);
      mux_set_portb(0, 1);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(7, 8);
      mux_set_portb(1, 2);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(7, 8);
      mux_set_portb(2, 3);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(7, 8);
      mux_set_portb(3, 4);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(7, 8);
      mux_set_portb(4, 5);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(7, 8);
      mux_set_portb(5, 6);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值


      mux_set_porta(8, 9);
      mux_set_portb(10, 11);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(8, 9);
      mux_set_portb(11, 12);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(8, 9);
      mux_set_portb(12, 13);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(8, 9);
      mux_set_portb(13, 14);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(8, 9);
      mux_set_portb(14, 15);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(8, 9);
      mux_set_portb(15, 0);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(8, 9);
      mux_set_portb(0, 1);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(8, 9);
      mux_set_portb(1, 2);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(8, 9);
      mux_set_portb(2, 3);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(8, 9);
      mux_set_portb(3, 4);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(8, 9);
      mux_set_portb(4, 5);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(8, 9);
      mux_set_portb(5, 6);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(8, 9);
      mux_set_portb(6, 7);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值


      mux_set_porta(9, 10);
      mux_set_portb(11, 12);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(9, 10);
      mux_set_portb(12, 13);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(9, 10);
      mux_set_portb(13, 14);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(9, 10);
      mux_set_portb(14, 15);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(9, 10);
      mux_set_portb(15, 0);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(9, 10);
      mux_set_portb(0, 1);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(9, 10);
      mux_set_portb(1, 2);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(9, 10);
      mux_set_portb(2, 3);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(9, 10);
      mux_set_portb(3, 4);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(9, 10);
      mux_set_portb(4, 5);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(9, 10);
      mux_set_portb(5, 6);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(9, 10);
      mux_set_portb(6, 7);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(9, 10);
      mux_set_portb(7, 8);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值


      mux_set_porta(10, 11);
      mux_set_portb(12, 13);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(10, 11);
      mux_set_portb(13, 14);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(10, 11);
      mux_set_portb(14, 15);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(10, 11);
      mux_set_portb(15, 0);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(10, 11);
      mux_set_portb(0, 1);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(10, 11);
      mux_set_portb(1, 2);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(10, 11);
      mux_set_portb(2, 3);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(10, 11);
      mux_set_portb(3, 4);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(10, 11);
      mux_set_portb(4, 5);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(10, 11);
      mux_set_portb(5, 6);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(10, 11);
      mux_set_portb(6, 7);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(10, 11);
      mux_set_portb(7, 8);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(10, 11);
      mux_set_portb(8, 9);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值

      
      mux_set_porta(11, 12);
      mux_set_portb(13, 14);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(11, 12);
      mux_set_portb(14, 15);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(11, 12);
      mux_set_portb(15, 0);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(11, 12);
      mux_set_portb(0, 1);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(11, 12);
      mux_set_portb(1, 2);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(11, 12);
      mux_set_portb(2, 3);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(11, 12);
      mux_set_portb(3, 4);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(11, 12);
      mux_set_portb(4, 5);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(11, 12);
      mux_set_portb(5, 6);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(11, 12);
      mux_set_portb(6, 7);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(11, 12);
      mux_set_portb(7, 8);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(11, 12);
      mux_set_portb(8, 9);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(11, 12);
      mux_set_portb(9, 10);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值


      mux_set_porta(12, 13);
      mux_set_portb(14, 15);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(12, 13);
      mux_set_portb(15, 0);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(12, 13);
      mux_set_portb(0, 1);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(12, 13);
      mux_set_portb(1, 2);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(12, 13);
      mux_set_portb(2, 3);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(12, 13);
      mux_set_portb(3, 4);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(12, 13);
      mux_set_portb(4, 5);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(12, 13);
      mux_set_portb(5, 6);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(12, 13);
      mux_set_portb(6, 7);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(12, 13);
      mux_set_portb(7, 8);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(12, 13);
      mux_set_portb(8, 9);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(12, 13);
      mux_set_portb(9, 10);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(12, 13);
      mux_set_portb(10, 11);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值


      mux_set_porta(13, 14);
      mux_set_portb(15, 0);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(13, 14);
      mux_set_portb(0, 1);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(13, 14);
      mux_set_portb(1, 2);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(13, 14);
      mux_set_portb(2, 3);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(13, 14);
      mux_set_portb(3, 4);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(13, 14);
      mux_set_portb(4, 5);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(13, 14);
      mux_set_portb(5, 6);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(13, 14);
      mux_set_portb(6, 7);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(13, 14);
      mux_set_portb(7, 8);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(13, 14);
      mux_set_portb(8, 9);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(13, 14);
      mux_set_portb(9, 10);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(13, 14);
      mux_set_portb(10, 11);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(13, 14);
      mux_set_portb(11, 12);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值


      mux_set_porta(14, 15);
      mux_set_portb(0, 1);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(14, 15);
      mux_set_portb(1, 2);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(14, 15);
      mux_set_portb(2, 3);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(14, 15);
      mux_set_portb(3, 4);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(14, 15);
      mux_set_portb(4, 5);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(14, 15);
      mux_set_portb(5, 6);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(14, 15);
      mux_set_portb(6, 7);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(14, 15);
      mux_set_portb(7, 8);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(14, 15);
      mux_set_portb(8, 9);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(14, 15);
      mux_set_portb(9, 10);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(14, 15);
      mux_set_portb(10, 11);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(14, 15);
      mux_set_portb(11, 12);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(14, 15);
      mux_set_portb(12, 13);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值


      mux_set_porta(15, 0);
      mux_set_portb(1, 2);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(15, 0);
      mux_set_portb(2, 3);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(15, 0);
      mux_set_portb(3, 4);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(15, 0);
      mux_set_portb(4, 5);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(15, 0);
      mux_set_portb(5, 6);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(15, 0);
      mux_set_portb(6, 7);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(15, 0);
      mux_set_portb(7, 8);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(15, 0);
      mux_set_portb(8, 9);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(15, 0);
      mux_set_portb(9, 10);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(15, 0);
      mux_set_portb(10, 11);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(15, 0);
      mux_set_portb(11, 12);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(15, 0);
      mux_set_portb(12, 13);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      mux_set_porta(15, 0);
      mux_set_portb(13, 14);
      delay(1);
      ad9240_get_peak();  // 调用一次，输出一次峰峰值
      Serial.println("等待下一次指令（发送 'c' 继续）...");
    }
  }
  
}