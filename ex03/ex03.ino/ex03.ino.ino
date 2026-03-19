/*
作业 3 (ex03)
功能：使用 millis() 实现 LED 发送 SOS 求救信号
SOS 规则：3次短闪 → 3次长闪 → 3次短闪 → 长停顿
*/
const int ONBOARD_LED_PIN = 2; // ESP32板载LED引脚

// 时间参数（单位：毫秒）
const int DOT_DURATION = 250;    // 短闪亮灯时间
const int DASH_DURATION = 600;   // 长闪亮灯时间
const int GAP_DURATION = 200;    // 字符间熄灭时间
const int END_GAP_DURATION = 2000; // 完整SOS结束后的长停顿

// SOS 信号序列：0=短闪，1=长闪，2=结束停顿
const int sosSequence[] = {0, 0, 0, 1, 1, 1, 0, 0, 0, 2};
int currentStep = 0;
unsigned long previousMillis = 0;

void setup() {
  pinMode(ONBOARD_LED_PIN, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  long waitTime;

  // 根据当前步骤确定等待时间
  if (sosSequence[currentStep] == 0) {
    waitTime = DOT_DURATION;
  } else if (sosSequence[currentStep] == 1) {
    waitTime = DASH_DURATION;
  } else {
    waitTime = END_GAP_DURATION;
  }

  if (currentMillis - previousMillis >= waitTime) {
    previousMillis = currentMillis;
    
    // 控制LED亮灭
    if (sosSequence[currentStep] != 2) {
      digitalWrite(ONBOARD_LED_PIN, !digitalRead(ONBOARD_LED_PIN));
    } else {
      digitalWrite(ONBOARD_LED_PIN, LOW); // 结束时保持熄灭
    }

    // 前进到下一步，循环
    currentStep = (currentStep + 1) % (sizeof(sosSequence)/sizeof(int));
  }
}