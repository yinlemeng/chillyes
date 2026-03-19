/*
 作业 2 (ex02)
 功能：使用 millis() 实现 LED 以 1Hz 频率稳定闪烁
 频率说明：1Hz = 亮 500ms + 灭 500ms
*/
const int ONBOARD_LED_PIN =2; // 使用板载LED引脚

unsigned long previousMillis = 0; // 记录上一次闪烁的时间
const long interval = 1000;       // 闪烁间隔 (1000毫秒)
bool ledState = false;           // LED当前状态

void setup() {
  pinMode(ONBOARD_LED_PIN, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis(); // 获取当前系统运行时间

  // 判断是否到达闪烁时间点
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;   // 记录当前时间
    ledState = !ledState;             // 翻转状态
    digitalWrite(ONBOARD_LED_PIN, ledState);   // 控制LED
  }
}