/*
作业 3 (ex03)
功能：使用 millis() 实现 LED 发送 SOS 求救信号
SOS 规则：3次短闪 → 3次长闪 → 3次短闪 → 长停顿
新增：串口输出当前执行步骤，便于调试
*/
const int ONBOARD_LED_PIN = 2; // ESP32板载LED引脚

// 时间参数（单位：毫秒）
const int DOT_DURATION = 200;    // 短闪亮灯时间
const int DASH_DURATION = 600;   // 长闪亮灯时间
const int GAP_DURATION = 200;    // 字符间熄灭时间
const int END_GAP_DURATION = 2000; // 完整SOS结束后的长停顿

// SOS 信号序列：0=短闪，1=长闪，2=结束停顿
const int sosSequence[] = {0, 0, 0, 1, 1, 1, 0, 0, 0, 2};
int currentStep = 0;
unsigned long previousMillis = 0;

void setup() {
  pinMode(ONBOARD_LED_PIN, OUTPUT);
  
  // 新增：初始化串口（波特率 115200）
  Serial.begin(115200);
  Serial.println("==================================");
  Serial.println("ex03: SOS 求救信号程序启动");
  Serial.println("SOS 规则：3次短闪 → 3次长闪 → 3次短闪 → 长停顿");
  Serial.println("==================================");
}

void loop() {
  unsigned long currentMillis = millis();
  long waitTime;
  String stepType; // 用于串口打印的步骤类型

  // 根据当前步骤确定等待时间 & 步骤类型
  if (sosSequence[currentStep] == 0) {
    waitTime = DOT_DURATION;
    stepType = "短闪 (DOT)";
  } else if (sosSequence[currentStep] == 1) {
    waitTime = DASH_DURATION;
    stepType = "长闪 (DASH)";
  } else {
    waitTime = END_GAP_DURATION;
    stepType = "结束停顿 (END GAP)";
  }

  if (currentMillis - previousMillis >= waitTime) {
    previousMillis = currentMillis;
    
    // 控制LED亮灭
    if (sosSequence[currentStep] != 2) {
      digitalWrite(ONBOARD_LED_PIN, !digitalRead(ONBOARD_LED_PIN));
    } else {
      digitalWrite(ONBOARD_LED_PIN, LOW); // 结束时保持熄灭
    }

    // 新增：串口打印当前步骤信息
    Serial.print("[Step ");
    Serial.print(currentStep);
    Serial.print("] ");
    Serial.print(stepType);
    Serial.print(" | 持续时间: ");
    Serial.print(waitTime);
    Serial.println("ms");

    // 前进到下一步，循环
    currentStep = (currentStep + 1) % (sizeof(sosSequence)/sizeof(int));

    // 新增：一轮 SOS 结束后打印分隔线
    if (currentStep == 0) {
      Serial.println("----------------------------------");
      Serial.println("一轮 SOS 结束，开始下一轮");
      Serial.println("----------------------------------");
    }
  }
}
